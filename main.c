#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>

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
        char *argv1[MAX_ARGS];
        int argc1=0;
        char *argv2[MAX_ARGS];
        int argc2=0;

        int pipe_found=0;

        char *saveptr;
        char *token=strtok_r(line," ",&saveptr);
        
        while(token!=NULL){
            //found pipe symbol
            if(strcmp(token,"|")==0){
                if(pipe_found){
                    printf("This version supports only one pipe.\n");
                    break;
                }
                pipe_found=1;
            }

            else{
                //before finding the pipe symbol
                //that means we are dealing with the first argument
                if(pipe_found==0){
                    if(argc1<MAX_ARGS-1)
                    argv1[argc1++]=token;
                    else
                    printf("Too many arguments\n");
                }
                //after finding the pipe symbol
                //that means we are dealing with the second argument
                else{
                    if(argc2<MAX_ARGS-1)
                    argv2[argc2++]=token;
                    else
                    printf("Too many arguments\n");
                }
            }
            token=strtok_r(NULL," ",&saveptr);
        }
        argv1[argc1]=NULL; //making sure that the last agrument is NULL (for execvp)
        argv2[argc2]=NULL;

        //check for invalid pipe
        if(pipe_found && (argc1==0 || argc2==0)){
            printf("Syntax error near |\n");
            free(line);
            continue;
        }

        //normal command- without pipe
        if(pipe_found==0){
            //create the child process
            pid_t pid=fork();
            if(pid<0){
                perror("fork");
                free(line);
                continue;
            }

            //child process
            if(pid==0){
                execvp(argv1[0],argv1);
                //will reach here only if exec() fails
                perror("execvp");
                exit(127);
            }

            //parent process
            waitpid(pid,NULL,0);
            free(line);
            continue;
        }

        //with pipe
        //creating pipe object before fork() so that it is inherited by the child processes
        int pipefd[2];
        //pipefd[0] is the read end- child 2 reads from it
        //pipefd[1] is the write end- child 1 writes into it
        if(pipe(pipefd)==-1){
            perror("pipe");
            free(line);
            continue;
        }
        //create the first child process
        pid_t pid1=fork();
        if(pid1<0){
            perror("fork");
            free(line);
            continue;
        }
        //inside the first child process
        if(pid1==0){
            //rewire the first child
            dup2(pipefd[1],STDOUT_FILENO);
            close(pipefd[1]);
            close(pipefd[0]);

            //execute command
            execvp(argv1[0],argv1);

            perror("execvp");
            free(line);
            exit(127);
        }

        //create the second child process
        pid_t pid2=fork();
        if(pid2<0){
            perror("fork");
            free(line);
            continue;
        }
        //inside the second child process
        if(pid2==0){
            //rewire the second child
            dup2(pipefd[0],STDIN_FILENO);
            close(pipefd[0]);
            close(pipefd[1]);

            //execute command
            execvp(argv2[0],argv2);

            perror("execvp");
            free(line);
            exit(127);
        }

        //inside the parent process
        close(pipefd[0]);
        close(pipefd[1]);

        waitpid(pid1,NULL,0);
        waitpid(pid2,NULL,0);
        free(line);
    }
    return 0;
}