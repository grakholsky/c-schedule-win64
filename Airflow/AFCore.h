/* File AFCore.h */
#pragma once 
#ifndef AFCORE_H
#define AFCORE_H
#include "AFDatabase.h"
#include "AFNetwork.h"
#include "AFTemplater.h"

#define HttpPort 80 /* port for start http server (http-defaut: 80) */

/**/
void AFServe(int clientSocket);
/**/
void AFStartupHttp();

#endif /* AFCORE_H */