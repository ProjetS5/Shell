#include "Commandes_Internes.h"
#include "Shell.h"
#include "Affichage.h"
#include "Evaluation.h"

#include <stdbool.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>



void echo(char** l){
int i = 0;
while(l[i] !=NULL){
	fprintf(stdout,"%s",l[i]);
	i++;
}
}
