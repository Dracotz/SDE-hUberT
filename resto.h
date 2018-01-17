#ifndef _RESTO_H
#define _RESTO_H

#define TIMEOUT_STDIN 500
#define SIZE_TEXTBUFF 50

#define KEY_MEM 690
#define KEY_MUTEX 9090
#define PLAT_SUPP_MAX 3

#include "shared.h"
#include "struct.h"
#include <time.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

int pid, id;

int travail = 1;


void envoyerMenu(MENU menu, int tailleMenu);
int commande(MENU menu, int numero, int quantite);
int rechargeStock(MENU menu, int mutex, int tailleMenu);
void initialisationMenu(MENU menu, int tailleMenu);
void afficherMenu(MENU menu, int tailleMenu);

int aleatoire(int max);

void viderTampon();
void nettoyageSegment();
void signal_handle();
void signal_end();

int filePrivee;
int filePublic;
int connected;

void deco();
int disconnect(int file, int fileDeco);
int getCmd(char *text);

void envoyerMenu(MENU menu, int tailleMenu) {
  afficherMenu(menu, tailleMenu);
}

int commande(MENU menu, int numero, int quantite) {
  if(menu[numero].quantite < quantite) {
    menu[numero].preparation = quantite - menu[numero].quantite + aleatoire(PLAT_SUPP_MAX);
    kill(pid, SIGUSR1);
    printf("On ne peut pas envoyer %d %s ! Veuillez attendre et redemander plus tard.\n", quantite, menu[numero].nom);
  } else {
    printf("On peut envoyer %d %s !\n", quantite, menu[numero].nom);
    menu[numero].quantite = menu[numero].quantite - quantite;
  }
	return 0;
}

int rechargeStock(MENU menu, int mutex, int tailleMenu) {
  int i;
  int aRefaire;
  for(i=0; i<tailleMenu; i++) {
    if(menu[i].preparation != 0) {
      aRefaire = i;
    }
  }
  for(i=0; i<menu[aRefaire].preparation; i++) {
    sleep(5);
    down(mutex);
    menu[aRefaire].quantite++;
    up(mutex);
  }
	return 0;
}

void initialisationMenu(MENU menu, int tailleMenu) {
  int i;
  for(i=0; i<tailleMenu; i++) {
    menu[i].quantite = 0;
    menu[i].preparation = 0;
    strcat(menu[i].nom,"");
  }
}

void afficherMenu(MENU menu, int tailleMenu) {
  int i;
  printf("\n*****MENU*****\n");
  for(i=0; i<tailleMenu; i++) {
    printf("%s : %d;\n", menu[i].nom, menu[i].quantite);
  }
  printf("**************\n\n");
}




int aleatoire(int max) {
  srand(time(NULL));
  return rand()%max+1;
}

void viderTampon() {
  int c;
  while ((c = getchar ()) != '\n' && c != EOF);
}

void nettoyageSegment() {
  shmctl(shmget(KEY_MEM, sizeof(repas), 0660), IPC_RMID, NULL);
}

void signal_handle() {}

void signal_end() {
  if(pid == 0) { //si je suis le fils
    printf("\nMerci pour tes succulents plats et à bientôt, Cuisine ! ;)\n*************************DECONNEXION*************************\n");

  } else {      //si je suis le pere
    printf("\nC'est une fin de tournage pour le restaurant :D\n*************************DECONNEXION*************************\n");

  }
  travail = 0;
}

#endif
