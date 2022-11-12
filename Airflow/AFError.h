#pragma once
#ifndef AFERROR_H
#define AFRRROR_H
#include "AFCommon.h"

/**********************************************************************/
/* Print out an error message with perror() (for system errors; based
* on value of errno, which indicates system call errors) and exit the
* program indicating an error. */
/**********************************************************************/
void AFErrorDie(char* message);

#endif /* AFERRROR_H */