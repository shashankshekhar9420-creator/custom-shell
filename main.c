#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<string.h>
int main(){
    char *line;
    while(1){
        line=readline("mysh> ");
        if(line==NULL){
            printf("\n");
            break;
        }
        if(strlen(line)==0){
            free(line);
            continue;
        }
        if(strcmp(line,"exit")==0){
            free(line);
            break;
        }
        printf("%s\n",line);
        free(line);
    }
    return 0;
}