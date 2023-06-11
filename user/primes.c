#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime(int fd_read)
{

    int value;
    int *v = &value;
    //try to read first value
    int has_read = read(fd_read, v, 4);
    //if no value in pipe return
    if (!has_read)
    {
        close(fd_read);
        return;
    }
    //create new pipe
    int new_p[2];
    pipe(new_p);
    //firs value is prime so print it
    printf("prime %d\n", *v);
    int p = *v;
    //read all the remaining value
    while (read(fd_read, v, 4))
    {
        // if can not be divded
        if ((*v % p) != 0)
        {
            //write to the pipe
            write(new_p[1], v, 4);
        }
    }
    //close write pipe and read pipe
    close(new_p[1]);
    close(fd_read);
    //recursively execute in child process
    int pid = fork();
    if (pid == 0)
    {
        prime(new_p[0]);
        exit(0);
        return;
    }
    else{
        wait(0);
        return;
    }
}

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    //write all number in first pipe
    for (int i = 2; i <= 35; i++)
    {
        write(p[1], &i, 4);
    }
    //cloase first pipe
    close(p[1]);
    //exec read pipe
    prime(p[0]);
    exit(0);
}