#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "struct.h"
#include "utility.h"

LISTE_TOKEN* buildTokens(char* expr){
	LISTE_TOKEN* t = NULL;
	for(int i=0; i<strlen(expr); i++){
		TOKEN tmp;
		if(isOperator(expr[i])){
			tmp.type = 0;
			tmp.val = expr[i];
		} else if(expr[i] == '(' || expr[i] == ')') {
			tmp.type = 1;
			tmp.val = expr[i];
		} else if(isdigit(expr[i])){
			tmp.type = 2;
			tmp.val = atof(expr + i);
			int cmptVirg = 0;
			while(isdigit(expr[i]) || expr[i] == '.'){
				if(expr[i] == '.')
					cmptVirg ++;
				i++;
			}
			if(cmptVirg > 1 || expr[--i] == '.'){
				perror("Invalid expression!\n");
				exit(-1);
			}
		} else {
			perror("Invalid expression!\n");
			exit(-1);
		}
		t = pushToken(t, tmp);
	}
	// pour gerer les nombres negatifs
	LISTE_TOKEN* orig = t;
	if(t){
		if(t->suiv){

			if(t->token.type == 0 && t->token.val == '-' && t->suiv->token.type == 2){
				orig = t->suiv;
				t->suiv->token.val *= -1;
				free(t);
				t = orig;
			}
			while(1){
				if(!t->suiv)
					break;
				if(!t->suiv->suiv)
					break;
				if(t->token.type < 2 && t->token.val != ')' && t->suiv->token.type == 0 &&
				   t->suiv->token.val == '-' && t->suiv->suiv->token.type == 2){
					LISTE_TOKEN* toFree = t->suiv;
					t->suiv = t->suiv->suiv;
					t->suiv->token.val *= -1;
					free(toFree);
				}
				t = t->suiv;
			}
		}
	}
	return orig;
}

_Bool checkValidity(LISTE_TOKEN* tokens){
	if(!tokens)
		return 0;
	if(tokens->suiv == NULL)
		if(tokens->token.type == 2)
			return 1;
	// test de premier token  && (tokens->token.val == '*' || tokens->token.val == '/')
	if(tokens->token.type == 0)
		return 0;
	// test des cases suivantes {'(*', '*)', '()', '**', ')1', '1('}
	LISTE_TOKEN* tmp = tokens;
	while(tmp->suiv){
		if(tmp->token.type == 1){
			if(tmp->token.val == '('){
				if(tmp->suiv->token.type == 0)
					return 0;
				if(tmp->suiv->token.type == 1 && tmp->suiv->token.val == ')')
					return 0;
			}
			if(tmp->token.val == ')' && tmp->suiv->token.type == 2)
				return 0;
			
		} else if(tmp->token.type == 0) {
			if(tmp->suiv->token.type == 1){
				if(tmp->suiv->token.val == ')')
					return 0;
			} else if(tmp->suiv->token.type == 0) {
				return 0;
			}
		} else {
			if(tmp->suiv->token.type == 1 && tmp->suiv->token.val == '(')
				return 0;
		}
		tmp = tmp->suiv;
	}
	// test de dernier token
	if(tmp->token.type == 0)
		return 0;
	// test des parentheses
	int balance = 0;
	tmp = tokens;
	while(tmp){
		if(tmp->token.type == 1){
			if(tmp->token.val == '('){
				balance ++;
			} else if(tmp->token.val == ')'){
				balance --;
			}
			if(balance < 0)
				return 0;
		}
		tmp = tmp->suiv;
	}
	if(balance != 0)
		return 0;
	return 1;
}

LISTE_TOKEN* InfixToPostfix(LISTE_TOKEN* t){
	LISTE_TOKEN* res = NULL;
	LISTE_TOKEN* pile = NULL;
	while(t){
		if(t->token.type == 2){
			res = pushToken(res, t->token);
		} else if(t->token.type == 1){
			if(t->token.val == '('){
				pile = pushToken(pile, t->token);
			} else {
				TOKEN tok;
				pile = popToken(pile, &tok);
				while (tok.type != 1 || tok.val != '('){
					res = pushToken(res, tok);
					pile = popToken(pile, &tok);
				}
			}
		} else {
			TOKEN tok;
			pile = popToken(pile, &tok);
			while(priority(tok) >= priority(t->token)){
				if(tok.type == 1){
					break;
				}
				res = pushToken(res, tok);
				pile = popToken(pile, &tok);		
			}
			pile = pushToken(pile, tok);
			pile = pushToken(pile, t->token);
		}
		t = t->suiv;
	}
	while(pile){
		TOKEN tmp;
		pile = popToken(pile, &tmp);
		if(tmp.type != -1)
			res = pushToken(res, tmp);
	}
    return res;
}

ARBRE_TOKEN* postfixToTree(LISTE_TOKEN* p){
	LISTE_ARBRE *pile = NULL;
	int i=0;
	while(p){
		if(p->token.type == 0){
			ARBRE_TOKEN* ptr1 = NULL, *ptr2 = NULL;
			pile = popArbre(pile, &ptr1, &ptr2);
			ARBRE_TOKEN* n = malloc(sizeof(ARBRE_TOKEN));
			n->token = p->token;
			n->g = ptr1;
			n->d = ptr2;
			pile = pushArbre(pile, n);
		} else {
			ARBRE_TOKEN* n = malloc(sizeof(ARBRE_TOKEN));
			n->token = p->token;
			n->g = NULL;
			n->d = NULL;
			pile = pushArbre(pile, n);
		}
		p = p->suiv;
		i++;
	}
	ARBRE_TOKEN* res = pile->abr;
	free(pile);
	return res;
}

double evaluateTree(ARBRE_TOKEN* a){
	if(a->g == NULL || a->d == NULL){
		return a->token.val;
	}
	double x = evaluateTree(a->g);
	double y = evaluateTree(a->d);
	switch((int)a->token.val){
		case '+': return x+y;
		case '-': return x-y;
		case '*': return x*y;
		case '/':
		if(y == 0){
			perror("div by 0");
			exit(-1);
		}
		return x/y;
	}
	return 0;
}

