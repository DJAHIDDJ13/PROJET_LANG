#ifndef STRUCT_H
#define STRUCT_H
typedef struct {
	int type;
	double val;
} TOKEN;

typedef struct liste_token {
	TOKEN token;
	struct liste_token* suiv;
} LISTE_TOKEN;
typedef struct arbre {
	TOKEN token;
	struct arbre* g;
	struct arbre* d;
} ARBRE_TOKEN;
typedef struct liste_arbre {
	ARBRE_TOKEN* abr;
	struct liste_arbre* suiv;
} LISTE_ARBRE;
#endif
