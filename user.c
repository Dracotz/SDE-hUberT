
#include <stdio.h>
#include <stdlib.h>
#include "message.h"
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include "user.h"
#include <signal.h>


int getCmd(char *text){

  struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };

  if(poll(&mypoll,1,TIMEOUT_STDIN)){
    fgets(text,SIZE_TEXTBUFF,stdin);
    return 1;
  }
  // printf("Stdin empty\n");
  // char *p = strchr(text, '\n');

  // if(p) *p = 0;

  return 0;


}






int main(){

  filePrivee = connectToFilePrivate(&connected,KEY_PUBLIC_USER);
  filePublic = getFilePublic(KEY_PUBLIC_USER);

  if(filePublic<0) connected = 0;
  else connected =1; 

  MsgConnexion msgCo;
  msgCo.type= 2;
  msgCo.id=0;

  signal(2,deco);

  MsgText messageTxt;
  messageTxt.type  = 1;
  char text[SIZE_TEXTBUFF];
  // strcpy(messageTxt.cmd,text);
  //  printf("Size of cmd : %ld && size of text: %ld\n",sizeof(messageTxt.cmd),sizeof(text));


  while(connected == 1){

    if(fileEmpty(filePublic)!=1){
      printf("ID attendu %d\n",filePrivee);
      msgrcv(filePublic,&msgCo,sizeof(msgCo.id),filePrivee,IPC_NOWAIT);
      if(msgCo.id==filePrivee){
        connected =0;
        printf("Client deconnected by hubert\n");
        exit(0);
      }

    }    

    if(getCmd(text)==1){ //return 1 si text sinon 0

      if(strcmp(text,"EXIT\n")==0){
        printf("EXIT DEMANDE\n");
        disconnect(filePrivee,filePublic);           
      }      
      else{
        strcpy(messageTxt.cmd,text);
        if( msgsnd(filePrivee,&messageTxt,sizeof(messageTxt.cmd),1)!=0){
          printf("Error sending message file : %d",filePrivee);
          perror("msgsnd"); 
        }
      }

    }  

  }




  return 0;
}
