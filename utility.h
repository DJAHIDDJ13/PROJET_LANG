#ifndef UTIL_H
#define UTIL_H
#include "struct.h"
// pour tester si un caractere est un operateur
_Bool isOperator(char c);
// pour empiler dans la liste des tokens
LISTE_TOKEN* pushToken(LISTE_TOKEN* source, TOKEN a);
// pour depiler la liste des tokens (la valeur depile est mis dans a)
LISTE_TOKEN* popToken(LISTE_TOKEN* source, TOKEN* a);
// pour empiler dans la liste des arbres
LISTE_ARBRE* pushArbre(LISTE_ARBRE* a, ARBRE_TOKEN* node);
// pour depiler la liste des arbres (les valeurs depile sont mises dans p1 et p2)
LISTE_ARBRE* popArbre(LISTE_ARBRE* p, ARBRE_TOKEN** p1, ARBRE_TOKEN** p2);
// retourne la priorite d'un token
int priority(TOKEN t);
// pour liberer la memoire d'un arbre binaire
void freeTree(ARBRE_TOKEN* a);
// pour liberer la memoire d'un pile des tokes
void freeTokens(LISTE_TOKEN* l);
// pour gerer les argument de terminal
char* argumentManager(int argc, char ** argv);
#endif
