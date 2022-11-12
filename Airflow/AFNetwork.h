/* File AFNetwork.h */
#pragma once
#ifndef AFNETWORK_H
#define AFNETWORK_H
#ifdef _WIN32
#include <WinSock2.h>
#pragma comment (lib, "Ws2_32.lib")
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#define AFMaxConnections 1024
#define AFMaxPostAccept 1024
#define AFMaxCountHeaders 15
#define AFMaxBufferAcceptSize 1024
#define AFMethodStringSize 10
#define AFUrlStringSize 255

#include <time.h>
#include "AFCommon.h"
#include "AFError.h"

/**********************************************************************/
/* Http request structure
 * Fields: char* string Url
 *		   char* string Method 
 *         char*** three dimensional string Heades 
 *		   char*** three dimensional string Data (Only POST Request)
 *         size_t count post Params
 *         size_t count headers */
/**********************************************************************/
typedef struct AFHttpRequestStruct {
	/* Uniform Resource Locator ex: /techsolustion.com/schedule */
	char* Url;
	/* HTTP method ex: GET, POST, PUT, DELETE*/
	char Method[AFMethodStringSize];
	/* HTTP headers ex: Content-Type: application/x-www-form-urlencoded */
	char*** Headers;
	/* Data from request (usually POST) ex: name=Vladymyr&lastname=Pavlov */
	char*** Data;
	size_t countPostParams;
	size_t countHeaders;
} AFHttpRequest;

/**********************************************************************/
/* Http response structure
 * Fields: char* string Data 
 *         char** two dimensional string Headers
 *         size_t count headers */
/**********************************************************************/
typedef struct AFHttpResponseStruct {
	char Protocol[9];
	char Phrase[60];
	char* Data;
	char** Headers;
	size_t dataLength;
	size_t countHeaders;
} AFHttpResponse;

/**********************************************************************/
/* Get a line from a socket, whether the line ends in a newline,
 * carriage return, or a CRLF combination.  Terminates the string read
 * with a null character.  If no newline indicator is found before the
 * end of the buffer, the string is terminated with a null.  If any of
 * the above three line terminators is read, the last character of the
 * string will be a linefeed and the string will be terminated with a
 * null character.
 * Parameters: the socket descriptor
 *             the buffer to save the data in
 *             the size of the buffer
 * Returns: the number of bytes stored (excluding null)  */
/**********************************************************************/
int AFGetLine(int sock, char* buf, int size);

/**********************************************************************/
/* Prepare the informational HTTP headers about a plain/text contents.
* Parameters: pointer to AFFile structure object
*             pointer to AFHttpResponse structure object */
/**********************************************************************/
void AFPrepareHeaders(char* codePhrase, AFHttpResponse* httpResponse);

/**********************************************************************/
/* Prepare the informational HTTP headers about a file.
* Parameters: pointer to AFFile structure object
*			   pointer to AFHttpRequest structure object
*             pointer to AFHttpResponse structure object */
/**********************************************************************/
void AFPrepareHeadersFile(AFFile* file, AFHttpRequest* httpRequest, AFHttpResponse* httpResponse);

/**********************************************************************/
/* Send a string data content to the client.
 * Parameters: socket client descriptor
 *             pointer to AFHttpResponse structure object */
/**********************************************************************/
void AFSendString(int clientSocket, AFHttpResponse* httpResponse);

/**********************************************************************/
/* Send a regular file to the client. Streaming (not set).
 * Parameters: socket client descriptor
 *			   pointer to AFFile structure object
 *             pointer to AFHttpResponse structure object */
/**********************************************************************/
void AFSendFile(int clientSocket, AFFile* file, AFHttpResponse* httpResponse);

/**********************************************************************/
/* A request has caused a call to accept() on the server port to
 * return.  Process the request appropriately.
 * Parameters: the socket connected to the client 
 *             pointer to AFHttpRequest structure object*/
/**********************************************************************/
int AFAcceptRequest(int clientSocket, AFHttpRequest* httpRequest);

/**********************************************************************/
/* Free memory in HttpRequest struct */
/**********************************************************************/
void AFClearHttpRequest(AFHttpRequest* httpRequest);

/**********************************************************************/
/* Free memory in HttpRequest struct */
/**********************************************************************/
void AFClearHttpResponse(AFHttpResponse* httpResponse);

/**********************************************************************/
/* This function starts the process of listening for web connections
* on a specified port.  If the port is 0, then dynamically allocate a
* port and modify the original port variable to reflect the actual port.
* Parameters: pointer to variable containing the port to connect on
* Returns: the socket */
/**********************************************************************/
int AFLoadListenSocket(unsigned short port);

#endif /* AFNETWORK_H */