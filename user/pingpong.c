#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv){
    int fd[2];
    pipe(fd);
    char ch = 'h';
    if(argc > 1){
        fprintf(2, "usage:pingpong\n");
        exit(1);
    }

    if(fork() == 0){    //子进程
        read(fd[0], &ch, 1);
        printf("%d: received ping\n", getpid());
        close(fd[0]);

        write(fd[1], &ch, 1);
        close(fd[1]);


    }else{
        //父进程
        write(fd[1], &ch, 1);
        close(fd[1]);

        wait(0);//防止父进程自己read

        read(fd[0], &ch, 1);
        printf("%d: received pong\n", getpid());
        close(fd[1]);
        
        
    }


    




    
    exit(0);
}