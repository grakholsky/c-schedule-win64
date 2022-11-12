/* File AFCommon.h */
#pragma once 
#ifndef AFCOMMON_H
#define AFCOMMON_H
#define AFMimeCount 16
#define AFMimeTypesFile "mimes.conf"
#define AFHttpCodesCount 6
#define AFHttpCodesFile "httpcodes.txt"
#define null 0
typedef unsigned int uint;          // unsigned 32 bits
typedef unsigned long long	ubig;   // unsigned 64 bits

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

/* MimeTypes list: extension-mimetype  */
char*** AFMimeTypes;

typedef struct {
	FILE* Stream;
	char* Data;
	char LastModified[60];
	char* Path;
	char* MimeType;
	char* Extension;
	ubig FileSize;
} AFFile;

typedef struct {
	char*** Data;
	uint countParams;
} AFParams;

/**********************************************************************/
/* Return count of found strings */
/**********************************************************************/
int AFFind(char* source, char* find);

/**********************************************************************/
/* Return first position of character from find string*/
/**********************************************************************/
int AFFindFirst(char* source, char* find);

/**********************************************************************/
/* Split all string with separator
 * IMPORTANT: be sure to free() the returned string after use */
/**********************************************************************/
char** AFSplitAll(char* source, char* separator);

/**********************************************************************/
/* Split one string with separator 
 * IMPORTANT: be sure to free() the returned string after use */
/**********************************************************************/
char** AFSplitFirst(char* source, char* separator);

/**********************************************************************/
/* Replace one found string
 * IMPORTANT: be sure to free() the returned string after use */
/**********************************************************************/
char* AFFindReplaceFirst(char* source, char* find, char* replace);

/**********************************************************************/
/* Replace all found string
 * IMPORTANT: be sure to free() the returned string after use * /
/**********************************************************************/
char* AFFindReplaceAll(char* source, char* findstr, char* replace);

/**********************************************************************/
/* Copy two strings */
/**********************************************************************/
void StringCopy(char* dest, const char* source);

/**********************************************************************/
/* Compare two strings */
/**********************************************************************/
int StringCompare(const char* str0, const char* str1);

/**********************************************************************/
/* Converts a hex character to its integer value */
/**********************************************************************/
char AFFromHex(char ch);

/**********************************************************************/
/* Converts an integer value to its hex character*/
/**********************************************************************/
char AFToHex(char code);

/**********************************************************************/
/* Returns a url-encoded version of str 
 * IMPORTANT: be sure to free() the returned string after use */
/**********************************************************************/
char* AFUrlEncode(char *str);

/**********************************************************************/
/* Returns a url-decoded version of str 
 * IMPORTANT: be sure to free() the returned string after use */
/**********************************************************************/
char* AFUrlDecode(char *str);

/**********************************************************************/
/* Remove last \r\n symbols from string */
/**********************************************************************/
void AFClearLastCRCF(char* content);

/**********************************************************************/
/* Load Mime-Types 
 * This uses for http headers 
 * IMPORTANT: be sure to AFUnloadMimetypes() the returned string after use */
/**********************************************************************/
void AFLoadMimetypes();

/**********************************************************************/
/* Unload Mime-Types */
/**********************************************************************/
void AFUnloadMimetypes();

/**********************************************************************/
/* Read content from file into ASCII string. Return NULL if file is not
 * readed of exists.
 * Parameters: full path to file 
 * IMPORTANT: be sure to free() the returned string after use */ 
/**********************************************************************/
char* AFReadFileToString(char* path);

/**********************************************************************/
/* Get datetime in GMT format now */
/*********************************************************************
unsigned short* AFGetCurrentDateTime();*/

/**********************************************************************/
/* Load structure AFFile 
 * Returns 0 if file exists, readed
 * Returns 1 if file exists, error reading 
 * Returns 2 if file not exists */
/**********************************************************************/
int AFLoadFile(char* path, AFFile* file);

/**********************************************************************/
/* Free file data and close file stream */
/**********************************************************************/
void AFClearFile(AFFile* file);

/**********************************************************************/
/**/
/**********************************************************************/
char* AFView(char* title, char* viewName, AFParams* params, void (*viewTemplate)());

#endif /* AFCOMMON_H */