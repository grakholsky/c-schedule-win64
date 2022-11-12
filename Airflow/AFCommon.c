/* File AFCommon.c */
#include "AFCommon.h"

int AFFind(char* source, char* find)
{
	int i = 0;
	int count = 0;
	int strln = strlen(find);
	while (i < strlen(source)) {
		if (source[i] == find[0]) {
			int b = i;
			int j = 0;
			while (j < strln){
				if (source[b] != find[j]){
					break;
				}
				b++;
				j++;
			}
			if (j == strln)
			{
				count++;
			}
		}
		i++;
	}
	return count;
}

int AFFindFirst(char* source, char* find)
{
	int i = 0;
	int strln = strlen(find);
	while (i < strlen(source)) {
		if (source[i] == find[0]) {
			int b = i;
			int j = 0;
			while (j < strln){
				if (source[b] != find[j]){
					break;
				}
				b++;
				j++;
			}
			if (j == strln)
			{
				return i + 1;
			}
		}
		i++;
	}
	return -1;
}

char** AFSplitAll(char* source, char* separator)
{
	int found;
	/* get count of separated string in source string */
	if ((found = AFFind(source, separator)) != 0) {
		/* alloc memory for result array */
		char** str = malloc(sizeof(char*)*(found + 1));
		/* count separated strings */
		int pos = 0;
		/* count source string */
		int i = 0;
		/* position symbol of last separated string */
		int lastpos = 0;
		/* length of separator string */
		int strln = strlen(separator);
		/* loop */
		while (i < strlen(source))
		{
			/* if symbols with index 'i' equal first symbol of separate string */
			if (source[i] == separator[0])
			{
				/* loop full match of sep string */
				int b = i;
				int j = 0;
				while (j < strln) {
					if (source[b] != separator[j]){
						break;
					}
					b++;
					j++;
				}
				/* if count of matches equal length of separated string */
				if (j == strln)
				{
					str[pos] = malloc(sizeof(char)*(i - lastpos) + 1);
					b = 0;
					while (lastpos < i){
						str[pos][b] = source[lastpos];
						lastpos++;
						b++;
					}
					str[pos][b] = '\0';
					//printf("%s\n", str[pos]);
					lastpos = j + i;
					/* increment of found */
					pos++;
				}
			}
			i++;
		}
		/* get last string separated */
		/* get length of last string */
		int b = i - lastpos;
		/* len > 0 : last string exist */
		if (b > 0) {
			str[pos] = malloc(sizeof(char)*b + 1);
			b = 0;
			while (lastpos < i) {
				str[pos][b] = source[lastpos];
				lastpos++;
				b++;
			}
			str[pos][b] = '\0';
		}
		else {
			/* str[pos] = malloc(sizeof(char)*1 + 1) */
			str[pos] = malloc(2);
			str[pos][0] = ' ';
			str[pos][1] = '\0';
		}
		return str;
	}
	else {
		return null;
	}
}

char** AFSplitFirst(char* source, char* separator)
{
	int found;
	/* get count of separated string in source string */
	if ((found = AFFind(source, separator)) != 0) {
		/* alloc memory for result array */
		/* first string and other */
		char** str = malloc(sizeof(char*) * 2);
		/* count separated strings */
		int pos = 0;
		/* count source string */
		int i = 0;
		/* position symbol of last separated string */
		int lastpos = 0;
		/* length of separator string */
		int strln = strlen(separator);
		/* length of source string */
		int strlns = strlen(source);
		/* loop */
		while (i < strlns)
		{
			/* if symbols with index 'i' equal first symbol of separate string */
			if (source[i] == separator[0])
			{
				/* loop full match of sep string */
				int b = i;
				int j = 0;
				while (j < strln) {
					if (source[b] != separator[j]){
						break;
					}
					b++;
					j++;
				}
				/* if count of matches equal length of separated string */
				if (j == strln)
				{
					str[pos] = malloc(sizeof(char)*(i - lastpos) + 1);
					b = 0;
					while (lastpos < i){
						str[pos][b] = source[lastpos];
						lastpos++;
						b++;
					}
					str[pos][b] = '\0';
					/* increment of found */
					pos++;
					lastpos = j + i;
					str[pos] = malloc(sizeof(char)*(strlns - lastpos) + 1);
					b = 0;
					while (lastpos < strlns){
						str[pos][b] = source[lastpos];
						lastpos++;
						b++;
					}
					str[pos][b] = '\0';
					return str;
				}
			}
			i++;
		}
	}
	else {
		return null;
	}
}

char* AFFindReplaceFirst(char* source, char* find, char* replace)
{
	int found;
	/* get first */
	if ((found = AFFindFirst(source, find)) != -1) {
		int strlensource = strlen(source);
		char* result = malloc(strlen(replace) + strlensource + 1);
		int g = 0;
		/* write string before replace */
		while (g < found - 1) {
			result[g] = source[g];
			g++;
		}
		/* write replace */
		int j = 0;
		while (j < strlen(replace)) {
			result[g] = replace[j];
			g++;
			j++;
		}
		/* write other string */
		found += strlen(find) - 1;
		while (found < strlensource) {
			result[g] = source[found];
			g++;
			found++;
		}
		result[g] = '\0';
		return result;
	}
	else {
		return null;
	}
}

char* AFFindReplaceAll(char* source, char* findstr, char* replace)
{
	int found;
	/* get first */
	if ((found = AFFind(source, findstr)) != 0) {
		int strlensource = strlen(source);
		int strlenfind = strlen(findstr);
		int len = strlensource - found*strlenfind + found*strlen(replace);
		char* str = malloc(len + 1);
		/* count source string */
		int i = 0;
		/* position symbol of last found findstr ex: hdjas jk, sep - 'jas', lastpos = 5 */
		int lastpos = 0;
		/* position symbol of last written */
		int lastposnew = 0;
		/* loop */
		while (i < strlensource)
		{
			/* if symbols with index 'i' equal first symbol of separate string */
			if (source[i] == findstr[0])
			{
				/* loop full match of sep string */
				int b = i;
				int j = 0;
				while (j < strlenfind) {
					if (source[b] != findstr[j]){
						break;
					}
					b++;
					j++;
				}
				/* if count of matches equal length of separated string */
				if (j == strlenfind)
				{
					while (lastpos < i){
						str[lastposnew] = source[lastpos];
						lastpos++;
						lastposnew++;
					}
					b = 0;
					while (b < strlen(replace)) {
						str[lastposnew] = replace[b];
						b++;
						lastposnew++;
					}
					str[lastposnew] = '\0';
					/*printf("%s\n", str[pos]);*/
					lastpos = j + i;
				}
			}
			i++;
		}
		/* get last string */
		/* get length of last string */
		int b = i - lastpos;
		/* len > 0 : last string exist */
		if (b > 0) {
			while (lastpos < i){
				str[lastposnew] = source[lastpos];
				lastpos++;
				lastposnew++;
			}
			str[lastposnew] = '\0';
		}
		return str;
	}
	else {
		return null;
	}
}

void StringCopy(char* dest, const char* source) {
	char* res = &dest[strlen(dest)];
	while ((*res++ = *source++) != null);
}

int StringCompare(const char* str0, const char* str1) {
	for (;*str0 == *str1; str0++, str1++) {
		if (*str0 == '\0') {
			return null;
		}
	}
	return -1;
}

char AFFromHex(char ch) {
	return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

char AFToHex(char code) {
	static char hex[] = "0123456789abcdef";
	return hex[code & 15];
}

char* AFUrlEncode(char *str)
{
	char* pstr = str;
	char* buf = malloc(strlen(str) * 3 + 1);
	char* pbuf = buf;
	while (*pstr)
	{
		if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~')
			*pbuf++ = *pstr;
		else if (*pstr == ' ')
			*pbuf++ = '+';
		else
			*pbuf++ = '%',
			*pbuf++ = AFToHex(*pstr >> 4),
			*pbuf++ = AFToHex(*pstr & 15);
		pstr++;
	}
	*pbuf = '\0';
	return buf;
}

char* AFUrlDecode(char *str)
{
	char* pstr = str;
	char* buf = malloc(strlen(str) + 1);
	char* pbuf = buf;
	while (*pstr)
	{
		if (*pstr == '%') {
			if (pstr[1] && pstr[2]) {
				*pbuf++ = AFFromHex(pstr[1]) << 4 | AFFromHex(pstr[2]);
				pstr += 2;
			}
		}
		else if (*pstr == '+') {
			*pbuf++ = ' ';
		}
		else {
			*pbuf++ = *pstr;
		}
		pstr++;
	}
	*pbuf = '\0';
	return buf;
}

void AFClearLastCRCF(char* content)
{
	int counter = strlen(content);
	while (counter > 0) {
		if (content[counter] == '\n' || content[counter] == '\r') {
			content[counter] = '\0';
		}
		counter--;
	}
}

void AFLoadMimetypes()
{
	struct stat statmimes;
	if (stat(AFMimeTypesFile, &statmimes) == 0)
	{
		char buff[100];
		FILE* resource = NULL;
		errno_t err = fopen_s(&resource, AFMimeTypesFile, "r");
		if (err == 0)
		{
			AFMimeTypes = malloc(AFMimeCount * sizeof(char **));
			for (int count = 0; count < AFMimeCount; count++)
			{
				fgets(buff, sizeof(buff), resource);
				AFClearLastCRCF(buff);
				AFMimeTypes[count] = AFSplitAll(buff, ":");
			}
		}
	}
}

void AFUnloadMimetypes()
{
	for (int i = 0; i < AFMimeCount; i++)
	{
		for (int j = 0; j < 2; j++) // one: extension, two: mime-type
		{
			free(AFMimeTypes[i][j]);
		}
		free(AFMimeTypes[i]);
	}
	free(AFMimeTypes);
}

char* AFReadFileToString(char* path)
{
	struct stat statpath;

	if (stat(path, &statpath) == -1) {
		return null;
	}
	else
	{
		FILE* resource = null;
		int err = fopen_s(&resource, path, "rb");
		if (err == 0) {
			fseek(resource, 0, SEEK_END);
			long filesize = ftell(resource);
			fseek(resource, 0, SEEK_SET);
			char* buffer = malloc(filesize + 1);
 			size_t lenreaded = fread(buffer, 1, filesize, resource);
			buffer[lenreaded] = '\0';
			fclose(resource);
			return buffer;
		}
		else {
			return null;
		}
	}
}

//unsigned short* AFGetCurrentDateTime()
//{
//	unsigned short date[100];
//	time_t now = time(0);
//	struct tm tstruct = *gmtime(&now);
//	strftime(date, 100, "%a, %d %b %Y %H:%M:%S GMT", &tstruct);
//	return date;
//}

int AFLoadFile(char* path, AFFile* file)
{
	struct stat statpath;
	if (stat(path, &statpath) == -1) {
		return 2;
	}
	else {		
		int err = fopen_s(&(file->Stream), path, "rb");
		if (err == 0) {
			fseek(file->Stream, 0, SEEK_END);
			long filesize = ftell(file->Stream);
			fseek(file->Stream, 0, SEEK_SET);
			file->Data = malloc(filesize + 1); /* unsigned short */
			file->FileSize = fread(file->Data, 1, filesize, file->Stream);
			file->Data[file->FileSize] = '\0';
			file->Path = path;
			char* extension = strrchr(path, '.');
			extension++;
			file->Extension = extension;
			int count = 0;
			while (count < AFMimeCount) {
				if (strcmp(extension, AFMimeTypes[count][0]) == 0) {
					break;
				}
				count++;
			}
			file->MimeType = AFMimeTypes[count][1];
			struct tm* tstruct = gmtime(&statpath.st_mtime);
			strftime(file->LastModified, sizeof(file->LastModified), "%a, %d %b %Y %H:%M:%S GMT", tstruct);
			return 0;			
		}
		else {
			return 1;
		}
	}
}

void AFClearFile(AFFile* file)
{
	free(file->Data);
	fclose(file->Stream);
}

char* AFView(char* title, char* viewName, AFParams* params, void(*viewTemplate)())
{
	char* buffer = AFReadFileToString(viewName);
	(*viewTemplate)();
	int i = 0;
	while (i < params->countParams){
		char* reple = AFFindReplaceAll(buffer, params->Data[i][0], params->Data[i][1]);
		free(buffer);
		buffer = reple;
		i++;
	}
	return buffer;
}