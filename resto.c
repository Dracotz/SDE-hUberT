
#include <stdio.h>
#include <stdlib.h>
#include "message.h"
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include "resto.h"
#include <signal.h>

#include "struct.h"
#include "shared.h"
#include <time.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>



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
	
 signal(SIGUSR1, signal_handle);
  signal(SIGINT, signal_end);
  int i, tailleMenu;
  MENU menu;

	
  //INITIALISATION MUTEX
  int mutex = semaphore_create(KEY_MUTEX);
  semaphore_init(mutex, 1);


 filePrivee = connectToFilePrivate(&connected,KEY_PUBLIC_RESTO);
  filePublic = getFilePublic(KEY_PUBLIC_RESTO);

  if(filePublic<0) connected = 0;
  else connected =1; 

  MsgConnexion msgCo;
  msgCo.type= 2;
  msgCo.id=0;

  signal(2,deco);

  MsgText messageTxt;
  messageTxt.type  = 1;
  char text[SIZE_TEXTBUFF];
 printf("%s , combien de plats voulez-vous dans votre Menu ?\n", NomRestaurant);

  tailleMenu = -1;
  while(tailleMenu < 1) {
    scanf("%d", &tailleMenu);
    viderTampon();
  }

  /*****************************************************************/
  menu = (MENU)malloc(tailleMenu*sizeof(repas));
  initialisationMenu(menu, tailleMenu);

  //INITIALISATION MEMOIRE PARTAGEE
  nettoyageSegment();
  id=shmget(KEY_MEM, tailleMenu*sizeof(repas), 0660 | IPC_CREAT | IPC_EXCL);//creation de la memoire
  menu = (MENU) shmat(id, 0, 0);      //attachement

  pid=fork();


  if (pid==0) {						//FILS
    while(travail) {
      pause();
      printf("La cuisine se met au boulot pour préparer ce qu'il faut.\n");
      rechargeStock(menu, mutex, tailleMenu);
    }






    //FIN FILS
  } else {
								//PERE
    down(mutex);
    for(i=0; i<tailleMenu; i++) {
      printf("Nom du plat : ");
      scanf("%32s",menu[i].nom);
      viderTampon();
      printf("Quantite : ");
      scanf("%d", &menu[i].quantite);
      viderTampon();
    }
    up(mutex);
    /*******************Preparation Connexion avec hUberT*************************/

 


  while(connected == 1){

    if(fileEmpty(filePublic)!=1){
      printf("ID attendu %d\n",filePrivee);
      msgrcv(filePublic,&msgCo,sizeof(msgCo.id),filePrivee,IPC_NOWAIT);
      if(msgCo.id==filePrivee){
        connected =0;
        printf("Client deconnected by hubert\n");
        
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


}
               //FIN PERE   
shmdt(menu);;
  shmctl(id, IPC_RMID, 0); // destruction du segment
  semaphore_remove(mutex);
  //if(pid!=0) {free(menu);}
  printf("\n\n\n\n***THE END***\n");
  exit(0);
  


  
}
