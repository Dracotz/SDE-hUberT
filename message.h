#ifndef _MESSAGE_H
#define _MESSAGE_H

#define KEY_PUBLIC_USER 150
#define KEY_PUBLIC_RESTO 200
#define DISCONNECT 1
#define CONNECT 2
#define LEN_STR_TXT 50

typedef struct MsgConnexion {
  
  long type;
  int id; // id si type = 0 ou  id a détruire si type =1


} MsgConnexion;


typedef struct MsgCmd {
  long type; // type de message
  int length; // taille du message qui va suivre
}MsgCmd;

typedef struct MsgText{
  long type;
  char cmd[LEN_STR_TXT]; 
}MsgText;

typedef struct Commande{
  long type;
  int *menu;

}Commande;

int getFilePublic(int key);
int connectToFilePrivate(int*connected,int key);
int fileEmpty(int idFile);
int createFilePublicUser();
int createFilePublicResto();
int sendNumber(int idFile);

#endif
