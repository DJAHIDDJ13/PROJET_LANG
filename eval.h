#ifndef EVAL_H
#define EVAL_H
#include "struct.h"
// pour creer la liste des tokens
LISTE_TOKEN* buildTokens(char* expr);
// pour tester si un liste des tokens est valide
_Bool checkValidity(LISTE_TOKEN* tokens);
// pour transformer un liste de tokens en forme infix a la forme postfix
LISTE_TOKEN* InfixToPostfix(LISTE_TOKEN* t);
// pour creer l'arbre
ARBRE_TOKEN* postfixToTree(LISTE_TOKEN* p);
// pour calculer le resultat d'arbre
double evaluateTree(ARBRE_TOKEN* a);
#endif
