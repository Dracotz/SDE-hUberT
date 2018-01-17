
#include <stdio.h>
#include <stdlib.h>
#include "message.h"
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include "user.h"
#include <signal.h>

void deco(){
  printf("Signal deconnect received\n");
  disconnect(filePrivee,filePublic);
  printf("User deconnected by signal\n");
  exit(0);
}


int disconnect( int file, int fileDeco){
  MsgConnexion msg;
  msg.type=DISCONNECT;//type le plus important : déconnexion
  msg.id = file;

  if((msgsnd(fileDeco,&msg,sizeof(msg.id),0))!=0){
    printf("Error not disconnected fileid : %d\n",msg.id);
    perror("msgsnd");
    return -1;
  }

  else{
    connected=0;
    printf("DISCONNECTED\n");
    return 0;

  }
}

