#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void xargs_run(int argc, char* argv[]){
    //get cmd;
    char *cmd = argv[1];    
    // malloc actual args
    char* actual_args[MAXARG];
    for (int i = 0; i < MAXARG; i++){
        actual_args[i] = (char*)malloc(sizeof(char)*MAXARGSIZE);
    }
    //copy original args
    for (int i = 1; i<argc; i++){
        memcpy(actual_args[i-1],argv[i],strlen(argv[i]));
    }
    //record the last index in actual_args
    int last_index = argc-1;
    int cur_index = last_index;
    // use for restore an address that to be set zero in actual_args
    char *restore_address;
    // read args from stdoutput
    //declare arg
    char arg[MAXARGSIZE];
    char *p = arg;
    while (read(0, p, 1))
    {
        // it is end of one arg
        if((*p) == ' '){
            // first change the last char to end 0
            *p = 0;
            // second copy it to copy 0 we should use strlen(arg)+1
            memcpy(actual_args[cur_index], arg, strlen(arg)+1);
            //third reset pointer
            p = arg;
            cur_index++;
            continue;
        }
        //it is end of one command
        if((*p) == '\n'){
            //first change the last char to end 0 and copy it
            *p = 0;
            //to copy 0 we should use strlen(arg)+1
            memcpy(actual_args[cur_index], arg, strlen(arg)+1);
            cur_index++;
            //save origin pointer
            restore_address = actual_args[cur_index];
            //set the last 0 to indicate this array end;
            actual_args[cur_index] = 0;
            // exectue
            int pid = fork();
            //add last 0
            if (pid == 0){
                exec(cmd, actual_args);
                exit(0);
            }
            else{
                wait(0);
            }
            // reset
            actual_args[cur_index] = restore_address;
            cur_index = last_index;
            p = arg;
            continue;
        }
        p++;
    }
    // free actual args
    for(int i = 0; i < MAXARG; i++) {
        free(actual_args[i]);
    }
    return;
}

int main(int argc, char* argv[]){
    if(argc == 1){
        fprintf(2,"arg error\n");
        exit(1);
    }
    xargs_run(argc, argv);
    exit(0);
}