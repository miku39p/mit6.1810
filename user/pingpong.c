#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]){
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);
    char buf[5];
    int pid = fork();
    if(pid == 0){
        close(p1[1]);
        read(p1[0], buf, 5);
        fprintf(1, "%d: received %s\n", getpid(), buf);
        close(p1[0]);
        close(p2[0]);
        write(p2[1],"pong", 5);
        close(p2[1]);
    }
    else{
        close(p1[0]);
        write(p1[1],"ping", 5);
        close(p1[1]);
        close(p2[1]);
        read(p2[0], buf, 5);
        fprintf(1, "%d: received %s\n", getpid(), buf);
        close(p2[0]);
    }
    exit(0);
}