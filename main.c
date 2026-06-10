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
        char *argv[MAX_ARGS];
        int argc=0;

        char *input_file=NULL;
        char *output_file=NULL;
        int append=0;

        int parse_error=0;

        char *saveptr;
        char *token=strtok_r(line," ",&saveptr);
        
        while(token!=NULL){
            if(strcmp(token,">")==0){
                token=strtok_r(NULL," ",&saveptr);

                if(token==NULL){
                    printf("Syntax error: expected filename after >\n");
                    parse_error=1;
                    break;
                }

                // > means output redirection (TRUNC)
                output_file=token;
                append=0; // 0 means TRUNC
            }
            else if(strcmp(token,">>")==0){
                token=strtok_r(NULL," ",&saveptr);

                if(token==NULL){
                    printf("Syntax error: expected filename after >>\n");
                    parse_error=1;
                    break;
                }
                
                // >> means output redirection again (APPEND)
                output_file=token;
                append=1; //1 means APPEND
            }
            else if(strcmp(token,"<")==0){
                token=strtok_r(NULL," ",&saveptr);

                
                if(token==NULL){
                    printf("Syntax error: expected filename after <\n");
                    parse_error=1;
                    break;
                }
                //< input redirection
                input_file=token;
            }
            else{
                if(argc<MAX_ARGS-1){
                    //we don't pass redirection symbols in the argument array
                    argv[argc++]=token;
                }
            }
            token=strtok_r(NULL," ",&saveptr);
        }
        argv[argc]=NULL; //making sure that the last agrument is NULL (for execvp)
        
        if(parse_error){
            free(line);
            continue;
        }

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
            //apply redirections before execution
            if(input_file!=NULL){
                int fd=open(input_file,O_RDONLY);
                if(fd<0){
                    perror(input_file);
                    exit(1);
                }
                //manipulating the flow of data
                dup2(fd,STDIN_FILENO);
                close(fd);
            }
            if(output_file!=NULL){
                int fd;
                if(append){
                    //this will be >> APPEND redirection
                    fd=open(output_file,O_WRONLY | O_CREAT | O_APPEND, 0644);
                }
                else{
                    //this will be > TRUNC redirection
                    fd=open(output_file,O_WRONLY | O_CREAT | O_TRUNC, 0644);
                }
                if(fd<0){
                    perror(output_file);
                    exit(1);
                }
                dup2(fd,STDOUT_FILENO);
                close(fd);
            }
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