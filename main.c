#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "struct.h"
_Bool isOperator(char c){
	return c=='+' || c=='-' || c=='*' || c=='/';
}
LISTE_TOKEN* pushToken(LISTE_TOKEN* source, TOKEN a){
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
LISTE_TOKEN* popToken(LISTE_TOKEN* source, TOKEN* a){
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
			tmp.val = 0;
			while(isdigit(expr[i])){
				tmp.val *= 10;
				tmp.val += expr[i] - '0';
				i++;
			}
			i--;
		} else {
			perror("Invalid expression!\n");
			exit(-1);
		}
		t = pushToken(t, tmp);
	}
	return t;
}
void printList(LISTE_TOKEN* tokens){
	if(!tokens)
		printf("empty");
	while(tokens){
		switch(tokens->token.type){
			case 0:
			case 1: printf("%c ", (int)tokens->token.val); break;
			case 2:printf("%g ", tokens->token.val);
		}
		tokens = tokens->suiv;
	}
	printf("\n");
}

_Bool checkValidity(LISTE_TOKEN* tokens){
	// check if there are empty operands
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

LISTE_TOKEN* toPostfix(LISTE_TOKEN* t){
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

void printArbre(ARBRE_TOKEN* a){
	if(!a)
		return;
	switch(a->token.type) {
		case 0:
		case 1: printf("%c ", (int)a->token.val); break;
		case 2: printf("%g ", a->token.val);
	}
	printArbre(a->g);
	printArbre(a->d);
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
int evaluateTree(ARBRE_TOKEN* a){
	if(a->g == NULL || a->d == NULL){
		return a->token.val;
	}
	int x = evaluateTree(a->g);
	int y = evaluateTree(a->d);
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
void freeArbre(ARBRE_TOKEN* a){
	if(a){
		freeArbre(a->g);
		freeArbre(a->d);
		free(a);
	}
}
void freeTokens(LISTE_TOKEN* l){
	while(l){
		TOKEN tmp;
		l = popToken(l, &tmp);
	}
}
int main(int argc, char** argv) {
	if(argc != 2){
		return -1;
	}
	char* expr = argv[1];
	LISTE_TOKEN* tokens = buildTokens(expr);
	if(!checkValidity(tokens)){
		perror("L'expression n'est pas valide");
		exit(-1);
	}
	LISTE_TOKEN* postfix = toPostfix(tokens);
	ARBRE_TOKEN* a = postfixToTree(postfix);
	printf("%d\n", evaluateTree(a));
	freeTokens(tokens);
	freeTokens(postfix);
	freeArbre(a);	
	return 0;
}
