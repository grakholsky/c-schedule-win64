#include "AFNetwork.h"

int AFGetLine(int sock, char *buf, int size)
{
	int i = 0;
	char c = '\0';
	int n;

	while ((i < size - 1) && (c != '\n'))
	{
		n = recv(sock, &c, 1, 0);
		/* DEBUG printf("%02X\n", c); */
		if (n > 0)
		{
			if (c == '\r')
			{
				n = recv(sock, &c, 1, MSG_PEEK);
				/* DEBUG printf("%02X\n", c); */
				if ((n > 0) && (c == '\n'))
					recv(sock, &c, 1, 0);
				else
					c = '\n';
			}
			buf[i] = c;
			i++;
		}
		else
			c = '\n';
	}
	buf[i] = '\0';

	return i;
}

void AFSendString(int clientSocket, AFHttpResponse* httpResponse)
{
	char buffer[60];
	sprintf(buffer, "%s %s", httpResponse->Protocol, httpResponse->Phrase);
	send(clientSocket, buffer, strlen(buffer), 0);
	for (int count = 0; count < httpResponse->countHeaders; count++) {
		send(clientSocket, httpResponse->Headers[count], strlen(httpResponse->Headers[count]), 0);
	}
	sprintf(buffer, "\n");
	send(clientSocket, buffer, strlen(buffer), 0);
	send(clientSocket, httpResponse->Data, httpResponse->dataLength, 0);
}

void AFPrepareHeaders(char* codePhrase, AFHttpResponse* httpResponse)
{
	sprintf(httpResponse->Protocol, "HTTP/1.1");
	/* Headers: Date, Content-Type, Content-Length */
	httpResponse->countHeaders = 1;
	httpResponse->Headers = malloc(httpResponse->countHeaders * sizeof(char*));
	for (int counter = 0; counter < httpResponse->countHeaders; counter++){
		httpResponse->Headers[counter] = malloc(sizeof(char*) * 100);
	}
	sprintf(httpResponse->Headers[0], "Content-Type: text/html\r\n");
	sprintf(httpResponse->Phrase, "%s\r\n", codePhrase);
}

void AFPrepareHeadersFile(AFFile* file, AFHttpRequest* httpRequest, AFHttpResponse* httpResponse)
{
	sprintf(httpResponse->Protocol, "HTTP/1.1");
	/* Headers: Date, Last-Modified, Content-Type, Content-Length */
	httpResponse->countHeaders = 2;
	httpResponse->Headers = malloc(httpResponse->countHeaders * sizeof(char*));
	for (int counter = 0; counter < httpResponse->countHeaders; counter++){
		httpResponse->Headers[counter] = malloc(sizeof(char*) * 100);
	}
	sprintf(httpResponse->Headers[0], "Last-Modified: %s\r\n", file->LastModified);
	sprintf(httpResponse->Headers[1], "Content-Type: %s\r\n", file->MimeType);

	for (int count = 0; count < httpRequest->countHeaders; count++) {
		if (strcmp(httpRequest->Headers[count][0], "If-Modified-Since") == 0) {
			if (strcmp(httpRequest->Headers[count][1], file->LastModified) == 0) {
				sprintf(httpResponse->Phrase, "304 Not Modified");
				return;
			}
		}
	}
	sprintf(httpResponse->Phrase, "200 OK");
}

void AFSendFile(int clientSocket, AFFile* file, AFHttpResponse* httpResponse)
{
	char buffer[60];
	sprintf(buffer, "%s %s\r\n", httpResponse->Protocol, httpResponse->Phrase);
	send(clientSocket, buffer, strlen(buffer), 0);
	for (int count = 0; count < httpResponse->countHeaders; count++) {
		send(clientSocket, httpResponse->Headers[count], strlen(httpResponse->Headers[count]), 0);
	}
	sprintf(buffer, "\n");
	send(clientSocket, buffer, strlen(buffer), 0);
	send(clientSocket, file->Data, file->FileSize, 0);

	/* Streaming */
	/*
	size_t sizeBuffer = 1024;
	char buf[sizeBuffer];
	fgets(buf, sizeBuffer, file->Stream);
	while (!feof(file->Stream)) {
	send(client, buf, strlen(buf), 0);
	fgets(buf, sizeBuffer, file->Stream);
	}
	*/
}

int AFAcceptRequest(int clientSocket, AFHttpRequest* httpRequest)
{
	char bufferAccept[AFMaxBufferAcceptSize];
	char bufferUrl[AFUrlStringSize];
	int numchars;
	size_t i = 0;
	size_t j = 0;
	httpRequest->countPostParams = 0;
	httpRequest->countHeaders = 0;

	numchars = AFGetLine(clientSocket, bufferAccept, AFMaxBufferAcceptSize);
	if (numchars == 0) {
		AFHttpResponse* httpResponse = malloc(sizeof(AFHttpResponse));
		httpResponse->Data = "Sorry, error request";
		httpResponse->dataLength = strlen(httpResponse->Data);
		AFPrepareHeaders("400 Bad Request", httpResponse);
		AFSendString(clientSocket, httpResponse);
		free(httpResponse);
		return 1;
	}
	printf("%s\n", bufferAccept);

	while (!isspace((int)bufferAccept[j]) && (i < AFMethodStringSize - 1)) {
		httpRequest->Method[i] = bufferAccept[j];
		i++;
		j++;
	}
	httpRequest->Method[i] = '\0';

	if (strcmp(httpRequest->Method, "GET") && strcmp(httpRequest->Method, "POST")) {
		AFHttpResponse* httpResponse = malloc(sizeof(AFHttpResponse));
		httpResponse->Data = "Sorry, method not found";
		httpResponse->dataLength = strlen(httpResponse->Data);
		AFPrepareHeaders("501 Method is not implemented", httpResponse);
		AFSendString(clientSocket, httpResponse);
		free(httpResponse);
		return 1;
	}

	i = 0;
	while (isspace((int)bufferAccept[j]) && (j < AFMaxBufferAcceptSize)) {
		j++;
	}
	while (!isspace((int)bufferAccept[j]) && (i < AFUrlStringSize - 1) && (j < AFMaxBufferAcceptSize)) {
		bufferUrl[i] = bufferAccept[j];
		i++;
		j++;
	}
	bufferUrl[i] = '\0';
	httpRequest->Url = AFUrlDecode(bufferUrl);

	/* Parse headers */
	int countHeaders = 0;
	numchars = AFGetLine(clientSocket, bufferAccept, AFMaxBufferAcceptSize);
	httpRequest->Headers = malloc(AFMaxCountHeaders * sizeof(char**));
	while ((numchars > 0) && strcmp("\n", bufferAccept) && (countHeaders < AFMaxCountHeaders))
	{
		AFClearLastCRCF(bufferAccept);
		httpRequest->Headers[countHeaders] = AFSplitFirst(bufferAccept, ": ");
		numchars = AFGetLine(clientSocket, bufferAccept, AFMaxBufferAcceptSize);
		countHeaders++;
	}
	httpRequest->countHeaders = countHeaders;

	if (strcmp(httpRequest->Method, "POST") == 0)
	{
		char bufferPost[AFMaxPostAccept];
		numchars = recv(clientSocket, bufferPost, AFMaxPostAccept, 0);
		bufferPost[numchars] = '\0';
		char* temp = bufferPost;
		while (*temp != NULL)
		{
			if (*temp == '=')
			{
				httpRequest->countPostParams++;
			}
			*temp++;
		}
		char* decodedData = AFUrlDecode(bufferPost);
		httpRequest->Data = malloc(httpRequest->countPostParams * sizeof(char**));
		char** tempPost = AFSplitAll(decodedData, "&");
		if (tempPost != NULL)
		{
			size_t countParams = 0;
			while (countParams < httpRequest->countPostParams)
			{
				httpRequest->Data[countParams] = AFSplitFirst(tempPost[countParams], "=");
				countParams++;
			}
			/* free temp post */
			for (int counter = 0; counter < countParams; counter++) // key: value
			{
				free(tempPost[counter]);
			}
			free(tempPost);
		} else {
			size_t countParams = 0;
			while (countParams < httpRequest->countPostParams)
			{
				httpRequest->Data[countParams] = AFSplitFirst(decodedData, "=");
				countParams++;
			}
		}
		free(decodedData);
	}
	return 0;
}

int AFLoadListenSocket(unsigned short port)
{
	int socketListen = 0;
	struct sockaddr_in name;

	/* load WinSock2 */
#ifdef _WIN32
	WSADATA info;

	if (WSAStartup(MAKEWORD(2, 0), &info)) {
		AFErrorDie("Could not start WinSock2");
	}
#endif

	socketListen = socket(PF_INET, SOCK_STREAM, 0);
	if (socketListen < 0) {
		AFErrorDie("Listen socket");
	}
	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_port = htons(port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(socketListen, (struct sockaddr *)&name, sizeof(name)) < 0) {
		AFErrorDie("Bind listen socket");
	}

	if (listen(socketListen, AFMaxConnections) < 0) {
		AFErrorDie("Start listen socket");
	}
	return socketListen;
}

void AFClearHttpRequest(AFHttpRequest* httpRequest)
{
	/* Free headers */
	for (int i = 0; i < httpRequest->countHeaders; i++)
	{
		for (int j = 0; j < 2; j++) // key: value
		{
			free(httpRequest->Headers[i][j]);
		}
		free(httpRequest->Headers[i]);
	}
	free(httpRequest->Headers);
	free(httpRequest->Url);
	/* Free post */
	if (strcmp(httpRequest->Method, "POST") == 0) {
		for (int i = 0; i < httpRequest->countPostParams; i++)
		{
			for (int j = 0; j < 2; j++) // key: value
			{
				free(httpRequest->Data[i][j]);
			}
			free(httpRequest->Data[i]);
		}
		free(httpRequest->Data);
	}
}

void AFClearHttpResponse(AFHttpResponse* httpResponse) 
{
	/* Free headers */
	for (int i = 0; i < httpResponse->countHeaders; i++)
	{
		free(httpResponse->Headers[i]);
	}
	free(httpResponse->Headers);
}
