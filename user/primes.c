#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void childProcess(int *fd){
    int i;
    int prime;
    int pipefd[2];

    if(read(fd[0], &prime, sizeof(prime)) == 0){
        close(fd[0]);
        exit(0);
    }
    fprintf(1, "prime %d\n", prime);

    pipe(pipefd);//创建管道


    if(fork() == 0){//child
        close(pipefd[1]);
        close(fd[0]);
        childProcess(pipefd);
    }else{//father
        close(pipefd[0]);
        while(1){
            if(read(fd[0], &i, sizeof(i)) == 0){
                break;
            }
            if(i % prime != 0){
                write(pipefd[1], &i, sizeof(i));
            }
        }
        close(pipefd[1]);
        close(fd[0]);
        wait(0);
    }

    exit(0);

}

int main(int argc, char *argv[]){
    int i;
    int fd[2];
    pipe(fd);

    if(argc > 1){
        fprintf(2, "usage: primes\n");
        exit(1);
    }

    if(fork() == 0){//child
        close(fd[1]);
        childProcess(fd);

    }else{//father
        close(fd[0]);

        for(i = 2; i <= 35; i++){
            write(fd[1], &i, sizeof(i));
        }
        close(fd[1]);
        wait(0);
    }

    exit(0);
}