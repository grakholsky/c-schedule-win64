#include "AFCore.h"

void AFServe(int clientSocket /*, list routes*/)
{
	AFHttpRequest* httpRequest = malloc(sizeof(AFHttpRequest));
	int code = AFAcceptRequest(clientSocket, httpRequest);
	if (code > 0) { 
		closesocket(clientSocket);
		free(httpRequest);	
		return; 
	}
	AFHttpResponse* httpResponse = malloc(sizeof(AFHttpResponse));
	AFFile* fileDescriptor = malloc(sizeof(AFFile));
	char filename[AFUrlStringSize];
	
	/* Routes */
	if (strcmp(httpRequest->Method, "GET") == 0 && strcmp(httpRequest->Url, "/") == 0) {		
		String* string = malloc(sizeof(String));
		string->Size = 100000;
		string->Data = malloc(string->Size);
		string->Data[0] = '\0';
		string->Length = null;

		AFTemplateSimple("index.chtml", "Welcome", string);
		httpResponse->Data = string->Data;
		httpResponse->dataLength = string->Length;
		AFPrepareHeaders("200 OK", httpResponse);
		AFSendString(clientSocket, httpResponse);
		/* free data 'after send' */
		free(string->Data);
		free(string);
	}
	else if (strcmp(httpRequest->Method, "GET") == 0 && strcmp(httpRequest->Url, "/journal") == 0) {
		String* string = malloc(sizeof(String));
		string->Size = 100000;
		string->Data = malloc(string->Size);
		string->Data[0] = '\0';
		string->Length = null;

		AFTemplate("layout.chtml", "journal.chtml", "Journal", string);
		httpResponse->Data = string->Data;
		httpResponse->dataLength = string->Length;
		AFPrepareHeaders("200 OK", httpResponse);
		AFSendString(clientSocket, httpResponse);
		/* free data 'after send' */
		free(string->Data);
		free(string);
	}
	else if (strcmp(httpRequest->Method, "GET") == 0 && strcmp(httpRequest->Url, "/schedule") == 0) {
		sprintf(filename, "schedule.chtml");
		int fileCode = AFLoadFile(filename, fileDescriptor);

		/* if success read */
		if (fileCode == 0) {
			AFPrepareHeadersFile(fileDescriptor, httpRequest, httpResponse);
			AFSendFile(clientSocket, fileDescriptor, httpResponse);
			AFClearFile(fileDescriptor);
		} /* if file not read or found */
		else {
			httpResponse->Data = "Sorry, page not found";
			httpResponse->dataLength = strlen(httpResponse->Data);
			AFPrepareHeaders("404 Not Found", httpResponse);
			AFSendString(clientSocket, httpResponse);
		}
	}
	else if (strcmp(httpRequest->Method, "POST") == 0 && strcmp(httpRequest->Url, "/schedule") == 0) {
		sprintf(filename, "groups.json");
		int fileCode = AFLoadFile(filename, fileDescriptor);

		/* if success read */
		if (fileCode == 0) {
			AFPrepareHeadersFile(fileDescriptor, httpRequest, httpResponse);
			AFSendFile(clientSocket, fileDescriptor, httpResponse);
			AFClearFile(fileDescriptor);
		} /* if file not read or found */
		else {
			httpResponse->Data = "Sorry, page not found";
			httpResponse->dataLength = strlen(httpResponse->Data);
			AFPrepareHeaders("404 Not Found", httpResponse);
			AFSendString(clientSocket, httpResponse);
		}
	}
	else if (strcmp(httpRequest->Method, "GET") == 0 && strcmp(httpRequest->Url, "/testing") == 0) {
		String* string = malloc(sizeof(String));
		string->Size = 100000;
		string->Data = malloc(string->Size);
		string->Data[0] = '\0';
		string->Length = null;

		AFTemplate("layout.chtml", "testing.chtml", "Testing", string);
		httpResponse->Data = string->Data;
		httpResponse->dataLength = string->Length;
		AFPrepareHeaders("200 OK", httpResponse);
		AFSendString(clientSocket, httpResponse);
		/* free data 'after send' */
		free(string->Data);
		free(string);
	}
	else {
#ifdef _WIN32		
		size_t i = 1, j = 0;
		for (; (i < strlen(httpRequest->Url)) && (j < AFUrlStringSize); i++, j++) {
			if (httpRequest->Url[i] == '/') {
				filename[j] = '\\';
				continue;
			}
			filename[j] = httpRequest->Url[i];
		}
		filename[j] = '\0';
#endif		
		int fileCode = AFLoadFile(filename, fileDescriptor);

		/* if success read */
		if (fileCode == 0 && strcmp(fileDescriptor->Extension, "chtml") != 0) {
			AFPrepareHeadersFile(fileDescriptor, httpRequest, httpResponse);
			AFSendFile(clientSocket, fileDescriptor, httpResponse);
			AFClearFile(fileDescriptor);
		} /* if file not read or found */
		else {
			httpResponse->Data = "Sorry, page not found";
			httpResponse->dataLength = strlen(httpResponse->Data);
			AFPrepareHeaders("404 Not Found", httpResponse);
			AFSendString(clientSocket, httpResponse);
		}
	}

	/* close connection with client */
	closesocket(clientSocket);

	/* Clear fields in structures */
	AFClearHttpRequest(httpRequest);
	AFClearHttpResponse(httpResponse);
	
	/* Free definitions */
	free(httpRequest);
	free(httpResponse);
	free(fileDescriptor);
}

void AFStartupHttp()
{
	/* load mime-types string into memory */
	AFLoadMimetypes();
	/* start-load listen socket */
	int serverSocket = AFLoadListenSocket(HttpPort);
	/**/
	struct sockaddr_in clientName;
	int clientNameLength = sizeof(clientName);
	printf("running on port %d\n", HttpPort);
	/* main loop */
	while (1) {
		/* client socket */
		int clientSocket = -1;
		/* establish connection with incoming client */
		clientSocket = accept(serverSocket, (struct sockaddr*)&clientName, &clientNameLength);
		if (clientSocket == -1) AFErrorDie("accept");

		/* One thread */
		AFServe(clientSocket);

		/* Multi thread */
		/*unsigned id;
		_beginthreadex(NULL, NULL, &acceptrequest, client_sock, NULL, &id);*/

		/*if (pthread_create(&newthread, NULL, acceptrequest, client_sock) != 0)
		perror("pthread_create");*/		
	}

	AFUnloadMimetypes();
	/* unload server socket */
	closesocket(serverSocket);
}