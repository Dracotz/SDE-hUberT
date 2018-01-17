
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "message.h"
#include <string.h>

int createFilePublicUser(){
  key_t cle;
  int id;

  if((cle = ftok("message.c",KEY_PUBLIC_USER))!= -1){

    if(( id = msgget(cle,0666  | IPC_CREAT | IPC_EXCL)) != -1){
      printf("New File Publique User avec idFile:%d\n",id);
      return id;
    }
    else {
      printf("EROOR CREATION FILE PUBLIC USER MSGGET \n");
      perror("msgget");
      id=msgget(cle,0666);
      if( (msgctl(id,IPC_RMID,NULL))!=0){
        perror("msgctl");
        exit(-1);
      }

      return createFilePublicUser();

    }
  }
  else{
    printf("ERROR CREATION FILE PUBLIC USER FTOK\n");
    perror("ftok");
    exit(-1);
  }

}

int getFilePublic(int key){ //key est KEY USER ou KEY RESTO
  key_t cle;
  int id;

  if((cle = ftok("message.c",key))!= -1){
    if(( id = msgget(cle,0666)) != -1){
      printf("Got File Publique User avec idFile:%d\n",id);
      return id;
    }
    else {
      printf("EROOR GET FILE PUBLIC USER MSGGET \n");
      perror("msgget");
      return -1;
    }
  }
  else{
    printf("ERROR GET FILE PUBLIC USER FTOK\n");
    perror("ftok");
    exit(0);
    return -1;
  }


}


int createFilePublicResto(){
  key_t cle;
  int id;

  if((cle = ftok("message.c",KEY_PUBLIC_RESTO))!= -1){

    if(( id = msgget(cle,0666  | IPC_CREAT | IPC_EXCL)) != -1){
      printf("New File Publique Resto avec idFile:%d\n",id);
      return id;
    }
    else {
      printf("EROOR CREATION FILE PUBLIC RESTO MSGGET \n");
      perror("msgget");
      id=msgget(cle,0666);
      if( (msgctl(id,IPC_RMID,NULL))!=0){
        perror("msgctl");
        exit(-1);
      }

      return createFilePublicResto();     

    }
  }
  else{
    printf("ERROR CREATION FILE PUBLIC RESTO FTOK\n");
    perror("ftok");
    exit(-1);
  }
}





int connectToFilePrivate(int *connected, int key){

  int idFilePublic;
  int idFilePrivee;

  if((idFilePublic = getFilePublic(key))!=-1){
    printf("Try connect to filePubliqueUser avec idFilePublique :%d\n",idFilePublic);
    if(( idFilePrivee = msgget(IPC_PRIVATE,IPC_CREAT|0666))!=-1){
      MsgConnexion message ;
      message.type=CONNECT;
      message.id = idFilePrivee;

      if( msgsnd(idFilePublic,&message,sizeof(message.id),0) == -1){
        printf("Error msgsnd connectToFilePrivate avec idFile : %d et  message.id : %d\n",idFilePublic,message.id);
        perror("msgsnd");}
      else{
        printf("Message sent with idFilePrivee :%d\n",idFilePrivee);
        *connected = 1;
        printf("CONNECTED\n");
      }

    }else{

      printf("Error msgget clePrive \n");
      perror("msgget");
      return -1;
    }
  }else {
    printf("Error getFilePublic  idFilePublic\n");
    perror("msgget");
    return -1;

  }

  if(key==KEY_PUBLIC_RESTO){

    char nom[LEN_STR_TXT];

    printf("Rentrez le nom de votre restaurant :\n");
    fgets(nom,LEN_STR_TXT,stdin);

    MsgText msgNom;
    msgNom.type = CONNECT;
    strcpy(msgNom.cmd,nom);

    msgsnd(idFilePrivee,&msgNom,sizeof(msgNom.cmd),0);

  }


  return idFilePrivee;

}

int sendNumber(int key){
  MsgConnexion message;
  message.type = 2;
  message.id = 10;
  int idFile = msgget(key,IPC_CREAT | 0666);
  int r = msgsnd(idFile,&message,sizeof(message.id),0);
  perror("msgsnd");
  return r;
}

int fileEmpty(int idFile){
  struct msqid_ds info;
  msgctl(idFile,IPC_STAT,&info);
  //  printf("File %d is %ld\n",idFile,info.msg_qnum);
  if(info.msg_qnum == 0)return 1;
  else return -1;

}


