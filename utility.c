#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

_Bool isOperator(char c){
	return c=='+' || c=='-' || c=='*' || c=='/';
}
LISTE_TOKEN* pushToken(LISTE_TOKEN* source, TOKEN a) {
	if(!source){
		LISTE_TOKEN* tmp = malloc(sizeof(LISTE_TOKEN));
		tmp->token = a;
		tmp->suiv = NULL;
		return tmp;
	}
	LISTE_TOKEN* orig = source;
	while(source->suiv != NULL)
		source = source->suiv;
	source->suiv = malloc(sizeof(LISTE_TOKEN));
	source->suiv->token = a;
	source->suiv->suiv = NULL;
	return orig;
}
LISTE_TOKEN* popToken(LISTE_TOKEN* source, TOKEN* a) {
	if(source == NULL){
		a->type = -1;
		a->val = 0;
		return NULL;
	}
	if(source->suiv == NULL){
		*a = source->token;
		free(source);
		return NULL;
	}
	LISTE_TOKEN* orig = source;
	while(source->suiv->suiv){
		source = source->suiv;
	}
	*a = source->suiv->token;
	free(source->suiv);
	source->suiv = NULL;
	return orig;
}

LISTE_ARBRE* pushArbre(LISTE_ARBRE* a, ARBRE_TOKEN* node){
	LISTE_ARBRE* tmp = malloc(sizeof(LISTE_ARBRE));
	tmp->abr = node;
	tmp->suiv = NULL;
	if(!a)
		return tmp;
	LISTE_ARBRE *orig = a;
	while(a->suiv)
		a = a->suiv;
	a->suiv = tmp;
	return orig;
}

int priority(TOKEN t){
    char x = t.val;
    if(x == '+' || x == '-')
        return 1;
    if(x == '*' || x == '/')
        return 2;
    if(t.type == 1)
		return 3;
    return -1;
}

LISTE_ARBRE* popArbre(LISTE_ARBRE* p, ARBRE_TOKEN** p1, ARBRE_TOKEN** p2) {
	if(!p){
		*p1 = NULL;
		*p2 = NULL;
		return NULL;
	}
	if(!p->suiv){
		*p1 = p->abr;
		free(p);
		*p2 = NULL;
		return NULL;
	}
	if(!p->suiv->suiv){
		*p1 = p->abr;
		*p2 = p->suiv->abr;
		free(p->suiv);
		free(p);
		return NULL;
	}
	LISTE_ARBRE *orig = p;
	while(p->suiv->suiv->suiv)
		p = p->suiv;
	
	*p1 = p->suiv->abr;
	*p2 = p->suiv->suiv->abr;
	free(p->suiv->suiv);
	p->suiv->suiv = NULL;
	free(p->suiv);
	p->suiv = NULL;
	return orig;
}
void freeTree(ARBRE_TOKEN* a){
	if(a){
		freeTree(a->g);
		freeTree(a->d);
		free(a);
	}
}
void freeTokens(LISTE_TOKEN* l){
	while(l){
		TOKEN tmp;
		l = popToken(l, &tmp);
	}
}
char* argumentManager(int argc, char ** argv){
	if(argc < 1){
		perror("argument error");
		exit(-1);
	}
	int len = 0;
	for(int i=1; i<argc; i++){
		len += strlen(argv[i]);
	}
	char* res = calloc(len+1, sizeof(char));
	for(int i=1; i<argc; i++){
		strcat(res, argv[i]);
	}
	return res;
}
