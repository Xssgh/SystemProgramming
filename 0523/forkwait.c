#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* 부모 프로세스가 자식 프로세스를 생성하고 끝나기를 기다린다. */
int main()
{
    int pid, child, status;

    printf("[%d] 부모 프로세스 시작 \n", getpid());

    pid = fork();

    if (pid < 0)
    {
        perror("fork 실패");
        exit(1);
    }

    if (pid == 0)
    {
        // 자식 프로세스
        printf("[%d] 자식 프로세스 시작 \n", getpid());
        exit(1); // 종료 코드 1로 종료
    }

    // 부모 프로세스
    child = wait(&status); // 자식이 끝날 때까지 대기
    printf("[%d] 자식 프로세스 %d 종료 \n", getpid(), child);

    if (WIFEXITED(status))
    {
        printf("\t정상 종료, 종료 코드 = %d\n", WEXITSTATUS(status));
    }
    else
    {
        printf("\t비정상 종료\n");
    }

    return 0;
}
