#include "AFTemplater.h"

void WriteTemplate(String* response, const char* source) {
	char* res = &response->Data[response->Length];
	uint len = null;
	while ((*res++ = *source++) != null) len++;
	response->Length += len;
}

void Partial(String* responce, const char* partial) {
	char* buffer = AFReadFileToString(partial);
	WriteTemplate(responce, buffer);
	free(buffer);
}

/*
 * Lexical analyzer
 */
void Lexer(const char* tBuffer, Token* tTokens)
{
	Token* tokens = tTokens;
	const char* buffer = tBuffer;
	const char* bufhtml = tBuffer;
	/*description = realloc(description, 100 * sizeof(char));*/
	uint countLine = 1;
	do {
		/* Counting lines */
		if (*buffer == '\n') countLine++;
		/* if characters compare with '<' and '%' */
		if (*buffer == 0x3c && *(buffer + 1) == 0x25) {
			tokens->Name = "html";
			tokens->Value = malloc((buffer - bufhtml) + 1);
			tokens->NumberLine = 0;
			char* temphtml = tokens->Value;
			while (*bufhtml) {
				if (*bufhtml == '<' && *(bufhtml + 1) == '%') {
					break;
				}
				*temphtml++ = *bufhtml++;
			}
			*temphtml = '\0';
			tokens++;

			char* tokStart = malloc(3);
			tokStart[0] = '<';
			tokStart[1] = '%';
			tokStart[2] = '\0';
			tokens->Name = tokStart;
			tokens->Value = tokStart;
			tokens->NumberLine = countLine;
			tokens++;
			buffer += 2; /* '<' + '%' */
			do {
				/* Counting lines */
				if (*buffer == '\n') countLine++;
				if (*buffer == 0x25 && *(buffer + 1) == 0x3e)
					break;
				/* Is punctuation character? */
				if (*buffer != '"' && ispunct(*buffer)) {
					if (*buffer == '*' || *buffer == '/' || *buffer == '-' || *buffer == '+')
					{
						char* op = malloc(2);
						op[0] = *buffer;
						op[1] = '\0';
						tokens->Name = "operator";
						tokens->Value = op;
						tokens->NumberLine = countLine;
						tokens++;
					}
					else if ((*buffer == '=' && *(buffer + 1) == '=') || (*buffer == '>' && *(buffer + 1) == '=') ||
						(*buffer == '<' && *(buffer + 1) == '=') || (*buffer == '!' && *(buffer + 1) == '=') ||
						(*buffer == '&' && *(buffer + 1) == '&') || (*buffer == '|' && *(buffer + 1) == '|'))
					{
						tokens->Name = "compare";
						char* op = malloc(3);
						op[0] = *buffer;
						op[1] = *(buffer + 1);
						op[2] = '\0';
						tokens->Value = op;
						tokens->NumberLine = countLine;
						buffer++;
						tokens++;
					}
					else if (*buffer == '>' || *buffer == '<') {
						char* op = malloc(2);
						op[0] = *buffer;
						op[1] = '\0';
						tokens->Name = "compare";
						tokens->Value = op;
						tokens->NumberLine = countLine;
						tokens++;
					}
					else {
						char* op = malloc(2);
						op[0] = *buffer;
						op[1] = '\0';
						tokens->Name = op;
						tokens->Value = op;
						tokens->NumberLine = countLine;
						tokens++;
					}
				}
				else if (isalnum(*buffer)) {
					const char* buf = buffer;
					for (; isalnum(*buf) || *buf == '.'; buf++);
					char* alnum = malloc((buf - buffer) + 1);
					char* talnum = alnum;
					for (; isalnum(*buffer) || *buffer == '.'; buffer++) {
						*talnum++ = *buffer;
					}
					*talnum = '\0';
					buf = alnum;
					int temp = 0;
					for (;*buf; buf++) {
						if (isalpha(*buf)) {
							temp = -1;
							break;
						}
					}
					if (temp < 0) {
						/* Filling lexem */
						if (strcmp(alnum, "if") == 0 || strcmp(alnum, "else") == 0 ||
							strcmp(alnum, "for") == 0 || strcmp(alnum, "foreach") == 0 ||
							strcmp(alnum, "in") == 0 || strcmp(alnum, "end") == 0)
						{
							tokens->Name = alnum;
							tokens->Value = alnum;
							tokens->NumberLine = countLine;
						}
						else if (strcmp(alnum, "string") == 0 || strcmp(alnum, "int") == 0 ||
							strcmp(alnum, "float") == 0 || strcmp(alnum, "StringList") == 0)
						{
							tokens->Name = "type";
							tokens->Value = alnum;
							tokens->NumberLine = countLine;
						}
						else if (*buffer == '(') {
							tokens->Name = "function";
							tokens->Value = alnum;
							tokens->NumberLine = countLine;
						}
						/* Filling variable */
						else {
							tokens->Name = "variable";
							tokens->Value = alnum;
							tokens->NumberLine = countLine;
						}
					}
					else {
						tokens->Name = "number";
						tokens->Value = alnum;
						tokens->NumberLine = countLine;
					}
					tokens++;
					/* Continue loop without counting countCharBuf */
					continue;
				}
				else if (*buffer == '"') {
					buffer++;
					const char* buf = buffer;
					for (; *buf != '"'; buf++);
					char* liter = malloc((buf - buffer) + 1);
					char* tliter = liter;
					for (; *buffer != '"'; buffer++) {
						*tliter++ = *buffer;
					}
					*tliter = '\0';
					/* Filling token data */
					tokens->Name = "literal";
					tokens->Value = liter;
					tokens->NumberLine = countLine;
					tokens++;
				}
				/*printf("H: %x D: %d\n", buffer, buffer);*/
				buffer++;
			} while (*buffer);

			char* tokEnd = malloc(3);
			tokEnd[0] = '%';
			tokEnd[1] = '>';
			tokEnd[2] = '\0';

			tokens->Name = tokEnd;
			tokens->Value = tokEnd;
			tokens->NumberLine = countLine;
			tokens++;
			buffer += 2;
			bufhtml = buffer;
		}
		buffer++;
	} while (*buffer);

	tokens->Name = "html";
	tokens->Value = malloc((buffer - bufhtml) + 1);
	tokens->NumberLine = 0;
	char* temphtml = tokens->Value;
	while (*bufhtml) {
		if (*bufhtml == '<' && *(bufhtml + 1) == '%') {
			break;
		}
		*temphtml++ = *bufhtml++;
	}
	*temphtml = '\0';
	tokens++;

	tokens->Name = malloc(1);
	tokens->Name[0] = '\0';

	/* Temp view */
	/*Token* t = tTokens;	
	while (*t->Name) {
		if (t->Name == "html") 
			printf("Tokens html val: %s\n", t->Value);
		else
			printf("\nTokens: %s\nvalue: %s line %i\n", t->Name, t->Value, t->NumberLine);
		t++;
	}*/
}
/*
 *
 */
Token* RunFunc(Token* tokens, ViewBag* tBag, Params* tParams, String* response)
{
	Params* params = tParams;
	tokens++;
	for (;*params->Type; tokens += 2, params++) /* +=2 miss separator */
	{
		if (strcmp(tokens->Name, "variable") != null) {
			if (strcmp(tokens->Name, "literal") == 0) {
				if (strcmp("string", params->Type) == 0) {
					Partial(response, tokens->Value);
				}
			}
			else {
				if (strcmp("int", params->Type) == 0) {

				}
			}
		}
		else {
			ViewBag* bag = tBag;
			for (;*bag->Name;bag++) {
				if (strcmp(bag->Name, tokens->Name) == 0 && strcmp(bag->Type, params->Type) == 0) {
					Partial(response, tokens->Value);
				}
			}
		}
	}
	return tokens;
}
/*
 * Syntax analyzer
 */
uint ParseError(char* TokenName, uint NumberLine)
{
	printf("Error line:%i token:%s\n", NumberLine, TokenName);
	system("pause");
	return 1;
}

Token* ParseSequence(Token* tokens, char* endToken, char* separator)
{
	tokens++;
	for (;strcmp(tokens->Name, "literal") == null || strcmp(tokens->Name, "number") == null || strcmp(tokens->Name, "variable") == null; tokens++)
	{
		tokens++;
		if (strcmp(tokens->Name, endToken) == null) {
			tokens++;
			break;
		}
		else if (strcmp(tokens->Name, separator) == null) {
			if (strcmp((tokens + 1)->Name, endToken) == null) {
				ParseError(tokens->Name, tokens->NumberLine);
			}
			continue;
		}
		else {
			ParseError(tokens->Name, tokens->NumberLine);
		}
	}
	return tokens;
}

Token* ParseIF(Token* tokens, char* endToken)
{
	while (*tokens->Name && strcmp(tokens->Name, "%>") != null)
	{
		tokens++;
		if (strcmp(tokens->Name, "(") == null) {
			tokens = ParseIF(tokens, ")");
		}
		else if (strcmp(tokens->Name, "compare") == null) {
			if (strcmp((tokens + 1)->Name, "%>") == null) {
				ParseError(tokens->Name, tokens->NumberLine);
			}
		}
		else if (strcmp(tokens->Name, ")") == null) {
			if (strcmp((tokens + 1)->Name, "%>") == null) {
				break;
			}
			continue;
		}
		else {
			uint countParams = 0;
			while (*tokens->Name)
			{
				if (strcmp(tokens->Name, "variable") == null || strcmp(tokens->Name, "literal") == null || strcmp(tokens->Name, "number") == null)
				{
					tokens++;
					countParams++;
					if (strcmp(tokens->Name, "compare") == null) {
						if (strcmp((tokens + 1)->Name, endToken) == null) {
							ParseError(tokens->Name, tokens->NumberLine);
						}
						/*if (strcmp(tokens[countTokens].Name, "literal") == 0 ||
						strcmp(tokens[countTokens].Value, "str") == 0) {
						}*/
					}
					else if (strcmp(tokens->Name, endToken) == null) {
						if (countParams >= 2) {
							break;
						}
					}
					else {
						ParseError(tokens->Name, tokens->NumberLine);
					}
				}
				else if (strcmp(tokens->Name, "function") == null) {
					tokens++;
					if (strcmp(tokens->Name, "(") == null) {
						tokens = ParseSequence(tokens, ")", ",");
					}
					else {
						ParseError(tokens->Name, tokens->NumberLine);
					}
					countParams++;
					if (strcmp(tokens->Name, "compare") == null) {
						if (strcmp((tokens + 1)->Name, endToken) == null) {
							ParseError(tokens->Name, tokens->NumberLine);
						}
					}
					else if (strcmp(tokens->Name, endToken) == null) {
						if (countParams >= 2) {
							break;
						}
					}
					else {
						ParseError(tokens->Name, tokens->NumberLine);
					}
				}
				else {
					ParseError(tokens->Name, tokens->NumberLine);
				}
				tokens++;
			}
			return tokens;
		}
	}
	return tokens;
}

Token* ParseExpressions(Token* tokens, uint firstSemi)
{
	while (*tokens->Name && strcmp(tokens->Name, "%>") != null)
	{
		if (strcmp(tokens->Name, "type") == null)
		{
			tokens++;
			/*char* tempType = tokens->Value;*/
			if (strcmp(tokens->Name, "variable") == null)
			{
				tokens++;
				if (strcmp(tokens->Name, ";") == null)
				{
					tokens++;
					if (firstSemi == null) continue;
					else {
						ParseError(tokens->Name, tokens->NumberLine);
					}
				}
				else if (strcmp(tokens->Name, "=") == null)
				{
					if (strcmp((tokens + 1)->Name, "{") == null)
					{
						tokens++;
						tokens = ParseSequence(tokens, "}", ",");
						if (strcmp(tokens->Name, ";") == null)
						{
							tokens++;
							if (firstSemi == null) continue;
							else {
								ParseError(tokens->Name, tokens->NumberLine);
							}
						}
						else {
							ParseError(tokens->Name, tokens->NumberLine);
						}
					}
					else {
						tokens = ParseSequence(tokens, ";", "operator");
					}
					if (firstSemi == null) continue;
					else break;
				}
				else {
					ParseError(tokens->Name, tokens->NumberLine);
				}
			}
		}
		else if (strcmp(tokens->Name, "variable") == null) {
			tokens++;
			if (strcmp(tokens->Name, "=") == null)
			{
				tokens = ParseSequence(tokens, ";", "operator");
			}
			else if (strcmp(tokens->Name, "operator") == null)
			{
				tokens++;
				if (strcmp(tokens->Name, "operator") != null && strcmp((tokens + 1)->Name, ";") != null)
				{
					ParseError(tokens->Name, tokens->NumberLine);
				}
			}
			else {
				ParseError(tokens->Name, tokens->NumberLine);
			}
		}
		else {
			ParseError(tokens->Name, tokens->NumberLine);
		}
	}
	return tokens;
}

Token* Parser(Token* tTokens, ViewBag* viewBag, Functions* viewFunc, String* response)
{
	Token* tokens = tTokens;
	do {
		if (strcmp(tokens->Name, "<%") == null) {
			tokens++;
			if (strcmp(tokens->Name, "end") == null) {
				tokens++;
				if (strcmp(tokens->Name, "%>") != null) {
					ParseError(tokens->Name, tokens->NumberLine);
				}
				break;
			}
			else if (strcmp(tokens->Name, "if") == null) {
				//printf("\nIF Branch\n");
				tokens++;
				if (strcmp(tokens->Name, "(") == null)
				{
					tokens = ParseIF(tokens, ")");
					tokens++;
					if (strcmp(tokens->Name, "%>") != null) {
						ParseError(tokens->Name, tokens->NumberLine);
					}
					//printf("\nIF inner\n");
					tokens = Parser(tokens, viewBag, viewFunc, response);
				}
				else ParseError(tokens->Name, tokens->NumberLine);
			}
			else if (strcmp(tokens->Name, "else") == null) {
				//printf("\nELSE\n");
				tokens++;
				if (strcmp(tokens->Name, "%>") != null) {
					ParseError(tokens->Name, tokens->NumberLine);
				}
				tokens++;
				tokens = Parser(tokens, viewBag, viewFunc, response);
				break; /*close IF condition */
			}
			else if (strcmp(tokens->Name, "for") == null)
			{
				//printf("\nFOR\n");
				tokens++;
				if (strcmp(tokens->Name, "(") == null) {
					tokens++;
					tokens = ParseExpressions(tokens, 1);
					tokens--;
					tokens = ParseIF(tokens, ";");
					tokens++;
					if (strcmp(tokens->Name, "variable") == null) {
						tokens++;
						if (strcmp(tokens->Name, "=") == null) {
							tokens = ParseSequence(tokens, ")", "operator");
						}
						else if (strcmp(tokens->Name, "operator") == null) {
							tokens++;
							if (strcmp(tokens->Name, "operator") != null && strcmp((tokens + 1)->Name, ")") != null) {
								ParseError(tokens->Name, tokens->NumberLine);
							}
						}
						else ParseError(tokens->Name, tokens->NumberLine);
					}
					else ParseError(tokens->Name, tokens->NumberLine);
				}
				else ParseError(tokens->Name, tokens->NumberLine);
				//printf("\nFOR inner\n");
				tokens = Parser(tokens, viewBag, viewFunc, response);
			}
			else if (strcmp(tokens->Name, "foreach") == null)
			{
				//printf("\nFOREACH\n");
				tokens++;
				if (strcmp(tokens->Name, "(") == null) {
					tokens++;
					if (strcmp(tokens->Name, "type") == null) {
						tokens++;
						if (strcmp(tokens->Name, "variable") == null) {
							tokens++;
							if (strcmp(tokens->Name, "in") == null) {
								tokens++;
								if (strcmp(tokens->Name, "variable") == null) {
									tokens++;
									if (strcmp(tokens->Name, ")") != null)
										ParseError(tokens->Name, tokens->NumberLine);
								}
								else ParseError(tokens->Name, tokens->NumberLine);
							}
							else ParseError(tokens->Name, tokens->NumberLine);
						}
						else ParseError(tokens->Name, tokens->NumberLine);
					}
					else ParseError(tokens->Name, tokens->NumberLine);
				}
				else ParseError(tokens->Name, tokens->NumberLine);
				//printf("\nFOREACH inner\n");
				tokens = Parser(tokens, viewBag, viewFunc, response);
			}
			else if (strcmp(tokens->Name, "=") == null) {
				tokens++;
				if (strcmp(tokens->Name, "variable") == null) {
					tokens++;
					if (strcmp(tokens->Name, "%>") == null) {
						ViewBag* vBag = viewBag;
						for (;*vBag->Name;vBag++) {
							if (strcmp(vBag->Name, (tokens - 1)->Value) == null) {
								WriteTemplate(response, vBag->Value);
							}
						}
						continue;
					}
					else ParseError(tokens->Name, tokens->NumberLine);
				}
				else if (strcmp(tokens->Name, "function") == null) {
					if (strcmp(tokens->Value, viewFunc->Name) == 0) {
						tokens++;
						if (strcmp(tokens->Name, "(") == null) {
							Token* tok = tokens;
							tok = ParseSequence(tok, ")", ",");
							tokens = RunFunc(tokens, viewBag, viewFunc->Param, response);
						}
						else ParseError(tokens->Name, tokens->NumberLine);
						if (strcmp(tokens->Name, "%>") != null) {
							ParseError(tokens->Name, tokens->NumberLine);
						}
					}
					else ParseError(tokens->Value, tokens->NumberLine);
				}
				else ParseError(tokens->Name, tokens->NumberLine);
			}
			else tokens = ParseExpressions(tokens, null);
		}
		else if (strcmp(tokens->Name, "html") == null)
		{
			//printf("Tokens html RootLev\n"),
			WriteTemplate(response, tokens->Value);
		}
		tokens++;
	} while (*tokens->Name);
	return tokens;
}

void AFTemplateSimple(char* ViewName, char* Title, String* string) 
{
	FILE* resource = null;
	Token* tokens = null;
	/* Default count tokens */
	tokens = malloc(200 * sizeof(Token));
	uint numberBags = 3;
	ViewBag* viewBag = malloc(numberBags * sizeof(ViewBag));
	ViewBag* tViewBag = viewBag;
	tViewBag->Name = "Title";
	tViewBag->Type = "string";
	tViewBag->Value = Title;
	tViewBag++;
	tViewBag->Name = "ViewName";
	tViewBag->Type = "string";
	tViewBag->Value = ViewName;
	tViewBag++;
	tViewBag->Name = "";

	Functions* viewFunc = malloc(sizeof(Functions));
	viewFunc->Name = "Partial";
	viewFunc->Param = malloc(2 * sizeof(Params));
	Params* par = viewFunc->Param;
	par->Type = "string";
	par++;
	par->Type = "";

	/* opening file for reading */
	int err = fopen_s(&resource, ViewName, "rb");
	if (err == null)
	{
		/* Read all file content */
		fseek(resource, null, SEEK_END);
		long filesize = ftell(resource);
		fseek(resource, null, SEEK_SET);
		char* buffer = malloc(filesize + 1);
		ubig lenreaded = fread(buffer, 1, filesize, resource);
		buffer[lenreaded] = '\0';
		fclose(resource);
		/* Call lexer */
		Lexer(buffer, tokens);
		/* Free after analyzing */
		free(buffer);
		/* Call parser */
		Token* t = Parser(tokens, viewBag, viewFunc, string);
	}

	/* free tokens */
	Token* t = tokens;
	for (;*t->Name;t++) {
		free(t->Value);
	}
	free(t->Name);
	free(tokens);

	free(viewBag);
	free(viewFunc->Param);
	free(viewFunc);
}

void AFTemplate(char* Layout, char* ViewName, char* Title, String* string)
{
	FILE* resource = null;
	Token* tokens = null;
	/* Default count tokens */
	tokens = malloc(200 * sizeof(Token));
	char* ViewBuffer = AFReadFileToString(ViewName);
	uint numberBags = 4;
	ViewBag* viewBag = malloc(numberBags * sizeof(ViewBag));
	ViewBag* tViewBag = viewBag;
	tViewBag->Name = "Title";
	tViewBag->Type = "string";
	tViewBag->Value = Title;
	tViewBag++;
	tViewBag->Name = "ViewName";
	tViewBag->Type = "string";
	tViewBag->Value = ViewName;
	tViewBag++;
	tViewBag->Name = "Content";
	tViewBag->Type = "String";
	tViewBag->Value = ViewBuffer;
	tViewBag++;
	tViewBag->Name = "";

	Functions* viewFunc = malloc(sizeof(Functions));
	viewFunc->Name = "Partial";
	viewFunc->Param = malloc(2 * sizeof(Params));
	Params* par = viewFunc->Param;
	par->Type = "string";
	par++;
	par->Type = "";

	/* opening file for reading */
	int err = fopen_s(&resource, Layout, "rb");
	if (err == null)
	{
		/* Read all file content */
		fseek(resource, null, SEEK_END);
		long filesize = ftell(resource);
		fseek(resource, null, SEEK_SET);
		char* buffer = malloc(filesize + 1);
		ubig lenreaded = fread(buffer, 1, filesize, resource);
		buffer[lenreaded] = '\0';
		fclose(resource);
		/* Call lexer */
		Lexer(buffer, tokens);
		/* Free after analyzing */
		free(buffer);
		/* Call parser */
		Token* t = Parser(tokens, viewBag, viewFunc, string);
	}

	/* free tokens */
	Token* t = tokens;
	for (;*t->Name;t++) {
		free(t->Value);
	}
	free(t->Name);
	free(tokens);

	free(viewBag);
	free(ViewBuffer);
	free(viewFunc->Param);
	free(viewFunc);
}