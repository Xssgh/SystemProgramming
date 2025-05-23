#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>  // fork(), getpid() 사용을 위해 필요

int main()
{
    int pid;
    pid = fork();

    if (pid == 0) { // 자식 프로세스
        printf("[Child] : Hello, world pid=%d\n", getpid());
    }
    else { // 부모 프로세스
        printf("[Parent] : Hello, world pid=%d\n", getpid());
    }

    return 0;
}
