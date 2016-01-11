#define _POSIX_SOURCE
#include "Commandes_Internes.h"
#include "Shell.h"
#include "Affichage.h"
#include "Evaluation.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>



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

void cd(char **s){
  chdir(s[0]);
}

  
void pwd(){
  char pwd[1024];
  getcwd(pwd, sizeof(pwd));
  printf("%s\n", pwd);
}

void kill2(char** pid){
  int i=0;
  int signal;
  signal = SIGTERM;
  while(pid[i] != NULL){
    if (strcmp(pid[i], "-l") == 0){
      printf(" Seulement SIGINT est implémenté, si aucun signal -> SIGTERM est envoyé\n");
      i++;
    }else if(strcmp(pid[i], "-SIGINT") == 0){
      signal = SIGINT;
      i++;
    }else{     
    kill(atoi(pid[i]), signal);
    i++;
    }
  }
}

void exit2(){
  exit(0);
}

void ls(){
	struct dirent *p;
	DIR *rep;
	rep = opendir(".");
	while((p = readdir(rep))){
		printf("%s \n", p->d_name);
	}
	closedir(rep);
}

void history(){
	
	char*line_read = (char *)NULL;
	
	line_read = readline ("");
	if (line_read && *line_read){
		add_history (line_read);}
	printf("%s\n",line_read);
	

}
