#ifndef STRUCT_H
#define STRUCT_H

struct elem_repas {
	int quantite;
	int preparation;
	char nom[32];
};

typedef struct elem_repas repas;
typedef repas *MENU;

#endif 
