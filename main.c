#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>

#define MAX_ARGS 64
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
        
        //step 1: Tokenization
        char *argv[MAX_ARGS];
        int argc=0;
        char *saveptr;
        char *token=strtok_r(line," ",&saveptr);
        
        while(token!=NULL && argc<MAX_ARGS-1){
            argv[argc++]=token;
            token=strtok_r(NULL," ",&saveptr);
        }
        argv[argc]=NULL; //making sure that the last agrument is NULL (for execvp)
        
        //if the entered command is only spaces
        if(argc==0){
            free(line);
            continue;
        }
        //step 2: fork
        pid_t pid=fork();
        if(pid<0){
            perror("fork");
            free(line);
            continue;
        }

        //child process (its program will be replaced by execvp())
        if(pid==0){
            execvp(argv[0],argv);
            //this line will only be reached if execvp() fails
            perror("execvp");
            //127 means command not found
            exit(127);
        }

        //parent process (should wait for the child process to finish)
        int status;
        if(waitpid(pid,&status,0)==-1){
            perror("waitpid");
        }
        //check if the child terminated normally
        else if(WIFEXITED(status)){
            //collect exit code
            printf("Exit code: %d\n",WEXITSTATUS(status));
        }
        free(line);
    }
    return 0;
}