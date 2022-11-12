#include "AFError.h"

void AFErrorDie(char* message)
{
	perror(message);
	exit(EXIT_FAILURE);
}