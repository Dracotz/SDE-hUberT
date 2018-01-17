
#include <stdio.h>
#include <stdlib.h>
#include "message.h"
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <signal.h>
#include "hubert.h"
#include <time.h>
#include <string.h>


int main(){

  int i ;
  for(i=0;i<N_USERS_MAX;i++){
    usersId[i]=0;
  }

  for(i=0;i<N_RESTOS_MAX;i++){
    restosId[i]=0;
    strcpy(nomsRestos[i],"");
  }

  nUsersConnected=0;
  nRestosConnected=0;

  connected = 1;

  idFileUser = createFilePublicUser();
  idFileResto = createFilePublicResto();


  MsgText msgText;
  msgText.type = 1;

  signal(2,disconnectAll);  

  while(connected!=0){

    carePublicResto(idFileResto);
    carePublicUser(idFileUser);




    for(i=0;i<N_USERS_MAX;i++){
      if(usersId[i]!=0){
        if(fileEmpty(usersId[i]) != 1){
          printf("Message repéré par hubert dans file priv user %d\n",i);
          msgrcv(usersId[i],&msgText,sizeof(msgText.cmd),0,0);
          printf("Message recu, type=%ld, cmd = %s\n",msgText.type,msgText.cmd);
        }
      }
    }

    for(i=0;i<N_RESTOS_MAX;i++){
      if(restosId[i]!=0){
        if(fileEmpty(restosId[i]) != 1){
          printf("Message repéré par hubert dans file priv resto %d\n",i);
          msgrcv(restosId[i],&msgText,sizeof(msgText.cmd),0,0);
          printf("Message recu, type=%ld, cmd = %s\n",msgText.type,msgText.cmd);
        }
      }
    }



  }

  return 0;
}
