#ifndef _HUBERT_H
#define _HUBERT_H

#define N_USERS_MAX 2
#define TIMEOUT_DECO_S 60
#define N_RESTOS_MAX 2
#define LEN_NAME 50


int usersId[N_USERS_MAX];
int nUsersConnected;
int idFileUser;


int connected;

int restosId[N_RESTOS_MAX];
char nomsRestos[N_RESTOS_MAX][LEN_NAME];
int nRestosConnected;
int idFileResto;


void disconnectAll();

int disconnectResto(int id, int pos);
int connectResto(int id);
int restoDisconnect(int id);
void carePublicResto(int id);

int disconnectUser(int id,int pos);
int connectUser(int id);
int userDisconnect(int id);
void carePublicUser(int id);

#endif
