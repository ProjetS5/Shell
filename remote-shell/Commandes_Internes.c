#define _POSIX_SOURCE
#include "Commandes_Internes.h"
#include "Shell.h"
#include "Affichage.h"
#include "Evaluation.h"
#include <time.h>
#include <stdio.h>
#include <stdlib>
#include <sys/utsname.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>




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

void cd(char *s){

  chdir(s);
}

  
void pwd(){
  
  char pwd[1024];
  getcwd(pwd, sizeof(pwd));
  printf("%s\n", pwd);

}

/*int kill(pid_t** pid){
  int i=0;
  while(pid[i] != NULL){           //Probleme pid_t ???
    kill(pid[i], SIGTERM);
    i++;
  }
}
*/

void exit2(){
  exit(0);
}
