#include "kernel/types.h"
#include "kernel/param.h" // 一些参数宏定义
#include "user/user.h"

int main(int argc, char *argv[]){
    char buf[512];
    char *params[MAXARG];
    int i;
    if(argc < 2){
        fprintf(2, "usage: xargs your_command\n");
        exit(1);
    }
  
    if(argc + 1 > MAXARG) {
      fprintf(2, "too many args\n");
      exit(1);
    }
    for(i = 1; i < argc; i++){
        params[i - 1] = argv[i];
    }
    params[argc] = 0;

    while(1){
        i = 0;
        while(1){
            if(read(0, &buf[i], 1) == 0 || buf[i] == '\n') break;
            i++;
        }
        if(i == 0) break;
        buf[i] = 0;
        params[argc - 1] = buf;
        if(fork() == 0){
            exec(params[0],params);
            exit(0);
        }else{
            wait(0);
        }
    }

    exit(0);
}