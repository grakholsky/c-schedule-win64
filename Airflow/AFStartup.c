#include "AFCore.h"

int main()
{
	/* load mime-types string into memory */
	//AFLoadMimetypes();
	//for (int i = 0; i < 1000000; i++) {
	//	AFFile* fileDescriptor = malloc(sizeof(AFFile));
	//	int fileCode = AFLoadFile("layout.html", fileDescriptor);
	//	AFClearFile(fileDescriptor);
	//	free(fileDescriptor);


	//	char* bufferPost = "djlsadlk=aklsdjlaks&ghjhkhkj=iouioio";
	//	char* temp = bufferPost;
	//	uint countPostParams = 0;
	//	while (*temp != NULL)
	//	{
	//		if (*temp == '=')
	//		{
	//			countPostParams++;
	//		}
	//		*temp++;
	//	}
	//	char* decodedData = AFUrlDecode(bufferPost);
	//	char*** Data = malloc(countPostParams * sizeof(char**));
	//	char** tempPost = AFSplitAll(decodedData, "&");
	//	if (tempPost != NULL)
	//	{
	//		uint countParams = 0;
	//		while (countParams < countPostParams)
	//		{
	//			Data[countParams] = AFSplitFirst(tempPost[countParams], "=");
	//			countParams++;
	//		}
	//		/* free temp post */
	//		for (int counter = 0; counter < countParams; counter++) // key: value
	//		{
	//			free(tempPost[counter]);
	//		}
	//		free(tempPost);
	//	}
	//	else {
	//		size_t countParams = 0;
	//		while (countParams < countPostParams)
	//		{
	//			Data[countParams] = AFSplitFirst(decodedData, "=");
	//			countParams++;
	//		}
	//	}
	//	free(decodedData);
	//	for (int i = 0; i < countPostParams; i++)
	//	{
	//		for (int j = 0; j < 2; j++) // key: value
	//		{
	//			free(Data[i][j]);
	//		}
	//		free(Data[i]);
	//	}
	//	free(Data);
	//}

	char* splitly = AFFindReplaceAll("Testing: jakd#asu#ldja jak#asu##asu#dlsd", "#asu#" ,"0000");
	printf("%s\n",splitly);
	//printf(splitly[1]);
	//for (int c = 0; c < 2; c++) {
	//	free(splitly[c]);
	//}
	free(splitly);
	AFStartupHttp();
	return 0;
}