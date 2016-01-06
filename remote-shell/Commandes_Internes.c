#include "Commandes_Internes.h"
#include "Shell.h"
#include "Affichage.h"
#include "Evaluation.h"
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/utsname.h>



void echo(char** l){
  int i = 0;
  while(l[i] !=NULL){
    fprintf(stdout,"%s ",l[i]);
    i++;
  }
  printf("\n");
}

void date(){
  
  struct tm date;                 
  char format[128];
  time_t tps;
 
 //on remplit la structure avec l'heure
  time(&tps);
  date = *localtime(&tps);       

  //on remplit la chaine de caractères
  strftime(format, 128, "%A, %B %d %Y\n%X\n%Z", &date);  

  //on affiche la date
  puts(format);

}

void hostname(){
  struct utsname host;
  uname(&host);
  
  printf("%s\n", host.nodename);
}
