#ifndef _USER_H
#define _USER_H

#define TIMEOUT_STDIN 500
#define SIZE_TEXTBUFF 50


int filePrivee;
int filePublic;
int connected;

void deco();
int disconnect(int file, int fileDeco);
int getCmd(char *text);


#endif
