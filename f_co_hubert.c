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

void carePublicUser(int id){


  if(fileEmpty(idFileUser) != 1){
    MsgConnexion message;
    message.type = 10;
    message.id = 10;	
    //printf("Message repéré par hubert dans file pub user\n");
    msgrcv(idFileUser,&message,sizeof(int),-5,IPC_NOWAIT);

    if(message.type==CONNECT){

      printf("Message recu user public, type= CONNECT , id= %d\n",message.id);
      connectUser(message.id);    

    }else if(message.type==DISCONNECT){

      printf("Message recu user public, type= DISCONNECT , id=%d\n",message.id);
      if(userDisconnect(message.id)!=0){
        printf("Error cant disconnect user cause no user has id %d\n",message.id);
      } 

    }
  }

}

void carePublicResto(int id){
  if(fileEmpty(idFileResto) != 1){
    MsgConnexion message ;
    message.type = 10;
    message.id = 10;
    //printf("Message repéré par hubert dans file pub resto\n");
    msgrcv(idFileResto,&message,sizeof(int),-5,IPC_NOWAIT);

    if(message.type==CONNECT){

      printf("Message recu resto public, type= CONNECT , id= %d\n",message.id);
      connectResto(message.id);    

    }else if(message.type==DISCONNECT){

      printf("Message recu resto public, type= DISCONNECT , id=%d\n",message.id);
      if(restoDisconnect(message.id)!=0){
        printf("Error cant disconnect resto cause no resto has id %d\n",message.id);
      } 

    }
  }

}

void disconnectAll(){
  int i;
  time_t start = time(NULL);

  printf("\n\nDISCONNECT ALL USERS AND RESTAURANTS\n");

  for(i=0;i<N_USERS_MAX;i++){
    if(usersId[i]!=0){
      printf("UsersId[%d] = %d\n",i,usersId[i]);
      disconnectUser(usersId[i],i);
    }
  }

  for(i=0;i<N_RESTOS_MAX;i++){
    if(restosId[i]!=0){
      printf("RestosId[%d] = %d\n",i,restosId[i]);
      disconnectResto(restosId[i],i);
    }
  }

  MsgConnexion msg;

  while((fileEmpty(idFileResto) != 1 || fileEmpty(idFileUser) != 1) && (time(NULL)<(start+TIMEOUT_DECO_S))){
    //    printf("waiting\n");
    if(fileEmpty(idFileResto) != 1 ) msgrcv(idFileResto,&msg,sizeof(msg.id),-5,IPC_NOWAIT);
    if(fileEmpty(idFileUser) != 1)  msgrcv(idFileUser,&msg,sizeof(msg.id),-5,IPC_NOWAIT);  
  }

  if((msgctl(idFileUser,IPC_RMID,NULL))!=0){
    printf("Error disconnectAll cant close public file user from hubert\n");
    perror("msgctl");
    exit(-1);
  }

  if((msgctl(idFileResto,IPC_RMID,NULL))!=0){
    printf("Error disconnectAll cant close public file resto from hubert\n");
    perror("msgctl");
    exit(-1);
  }

  printf("\nHubert is going to sleep... See you !\n");
  connected =0;
  exit(0); 


}

int disconnectUser(int id,int pos){
  MsgConnexion message;
  message.type=id;
  message.id=id;


  printf("Message deco send, id a deco :%d\n",id);

  if((msgsnd(idFileUser,&message,sizeof(message.id),0))!=0){
    printf("Error disconnectUser cant send message\n");
    perror("msgrcv");
    return -1;
  }


  if((msgctl(message.id,IPC_RMID,NULL)) != 0){
    printf("Error disconnectUser cant close file %d\n",message.id);
    perror("msgctl");
    return -1;
  }

  printf("User file no: %d disconnected and file closed\n",message.id);

  if(pos>0){
    usersId[pos]=0;
    nUsersConnected--;
  }


  return 0;
}

int disconnectResto(int id,int pos){
  MsgConnexion message;
  message.type=id;
  message.id=id;


  printf("Message deco send, id a deco :%d\n",id);

  if((msgsnd(idFileResto,&message,sizeof(message.id),0))!=0){
    printf("Error disconnectResto cant send message\n");
    perror("msgrcv");
    return -1;
  }


  if((msgctl(message.id,IPC_RMID,NULL)) != 0){
    printf("Error disconnectResto cant close file %d\n",message.id);
    perror("msgctl");
    return -1;
  }

  printf("Resto file no: %d disconnected and file closed\n",message.id);

  if(pos>0){
    restosId[pos]=0;
    nRestosConnected--;
    strcpy(nomsRestos[pos],"");
  }


  return 0;
}


int userDisconnect(int id){
  printf("User n°%d request to be disconnected\n",id);
  int i;
  for(i=0;i<N_USERS_MAX;i++){
    if(usersId[i]==id){
      usersId[i]=0;
      nUsersConnected--;
      return 0;
    }
  }

  return -1;
}

int restoDisconnect(int id){
  printf("Resto n°%d request to be disconnected\n",id);
  int i;
  for(i=0;i<N_USERS_MAX;i++){
    if(restosId[i]==id){
      restosId[i]=0;
      strcpy(nomsRestos[i],"");
      nRestosConnected--;
      return 0;
    }
  }

  return -1;
}



int connectUser(int id){

  if(nUsersConnected == N_USERS_MAX){
    printf("Error too much clients n=%d connected to hubert\n",nUsersConnected);
    disconnectUser(id,-1); //avec -1 on n'enleve pas l'utilisateur de notre liste, puisqu'il n'y était aps 

    return -1;
  }

  int i;

  for(i=0;i<N_USERS_MAX;i++){
    if(usersId[i]==0){
      usersId[i]=id;
      nUsersConnected++;
      return 1;
    }
  }

  return 0;

}


int connectResto(int id){

  if(nRestosConnected == N_RESTOS_MAX){
    printf("Error too much retaurants n=%d connected to hubert\n",nRestosConnected);
    disconnectResto(id,-1); //avec -1 on n'enleve pas le resto de notre liste, puisqu'il n'y était aps 

    return -1;
  }

  int i;

  MsgText msgNom;
  while(fileEmpty(id)!=1){}
  msgrcv(id,&msgNom,LEN_STR_TXT,CONNECT,0);
  for(i=0;i<N_RESTOS_MAX;i++){
    if(restosId[i]!=0){
      if(strcmp(nomsRestos[i],msgNom.cmd)==0){
        printf("Error nom de restaurant %s déjà pris !\n",msgNom.cmd);
        disconnectResto(id,-1);
        return -1;
      }
    }
  }

  for(i=0;i<N_RESTOS_MAX;i++){
    if(restosId[i]==0){
      restosId[i]=id;
      nRestosConnected++;
      strcpy(nomsRestos[i],msgNom.cmd);
      printf("New Restaurant Connected named %s, his private id is %d, welcome !\n",nomsRestos[i],id);
      return 0;
    }
  }

  return 0;

}
