#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "utility.h"
#include "eval.h"

int main(int argc, char** argv) {
	char* expr = argumentManager(argc, argv);
	LISTE_TOKEN* tokens = buildTokens(expr);
	if(!checkValidity(tokens)){
		perror("L'expression n'est pas valide");
		exit(-1);
	}
	LISTE_TOKEN* postfix = InfixToPostfix(tokens);
	ARBRE_TOKEN* a = postfixToTree(postfix);
	printf("result = %g\n", evaluateTree(a));
	freeTokens(tokens);
	freeTokens(postfix);
	freeTree(a);
	free(expr);
	return 0;
}
