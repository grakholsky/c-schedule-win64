#pragma once
#include "AFCommon.h"
/* TOKENS
* nl - number line
*			  <assign, nl> -> =
*			<operator, nl> -> + - / *
*				 <cbl, nl> -> (
*				 <cbr, nl> -> )
*				 <fbl, nl> -> {
*				 <fbr, nl> -> }
*				 <sbl, nl> -> <%
*				 <sbr, nl> -> %>
*			   <comma, nl> -> ,
*         <semicolon, nl> -> ;
*			   <colon, nl> -> :
*               <dot, nl> -> .
*	  <compare, value, nl> -> == != >= <=
*	 <variable, value, nl> -> variable message text
*	   <number, value, nl> -> 3,14 4 85
*	  <literal, value, nl> -> "Hello Leaf"
*			    <type, nl> -> string, int
*			<function, nl> -> Partial
*				  <if, nl> -> if
*				<else, nl> -> else
*/

/*
* Token struct
*/
typedef struct
{
	char* Name;
	char* Value;
	uint NumberLine;
}Token;

typedef struct
{
	char* Name;
	char* Type;
	char* Value;
}ViewBag;

typedef struct {
	char* Type;
}Params;

typedef struct
{
	char* Name;
	Params* Param;
}Functions;

typedef struct
{
	char* Data;
	uint Length;
	uint Size;
}String;

void WriteTemplate(String* response, const char* source);

void Lexer(const char* tBuffer, Token* tTokens);

Token* RunFunc(Token* tokens, ViewBag* tBag, Params* tParams, String* response);

uint ParseError(char* TokenName, uint NumberLine);

Token* ParseSequence(Token* tokens, char* endToken, char* separator);

Token* ParseIF(Token* tokens, char* endToken);

Token* ParseExpressions(Token* tokens, uint firstSemi);

Token* Parser(Token* tTokens, ViewBag* viewBag, Functions* viewFunc, String* response);

void AFTemplateSimple(char* ViewName, char* Title, String* string);

void AFTemplate(char* Layout, char* ViewName, char* Title, String* string);