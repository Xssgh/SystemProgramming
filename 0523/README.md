
# 제9장 프로세스 제어

## 9.1 프로세스 생성 (fork)

설명: 부모 프로세스가 자신을 복제해 자식 프로세스를 생성  
함수: `pid_t fork(void)`  
특징:  
- 부모 프로세스: 자식의 PID 반환  
- 자식 프로세스: 0 반환  
- 호출 후 부모·자식이 병렬로 실행됨  

### 예제: fork1.c
```c
#include <stdio.h>
#include <unistd.h>

int main() {
    int pid;
    printf("[%d] 프로세스 시작\n", getpid());
    pid = fork();
    printf("[%d] 리턴값 %d\n", getpid(), pid);
    return 0;
}
```

### 예제: fork2.c
```c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int pid = fork();
    if (pid == 0) {
        printf("[Child] Hello, world! pid=%d\n", getpid());
    } else {
        printf("[Parent] Hello, world! pid=%d\n", getpid());
    }
    return 0;
}
```

### 예제: fork3.c
```c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int pid1 = fork();
    if (pid1 == 0) {
        printf("[Child 1] Hello, world! pid=%d\n", getpid());
        exit(0);
    }
    int pid2 = fork();
    if (pid2 == 0) {
        printf("[Child 2] Hello, world! pid=%d\n", getpid());
        exit(0);
    }
    printf("[Parent] Hello, world! pid=%d\n", getpid());
    return 0;
}
```

---

## 9.2 프로그램 실행 (exec)

설명: 자식 프로세스가 fork() 후 exec()로 새 프로그램 실행  
함수: 
- `execl`, `execv`, `execlp`, `execvp`  
특징: 성공 시 절대 리턴하지 않음, 실패 시 -1 반환  

### 예제: execute1.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("부모 프로세스 시작\n");
    if (fork() == 0) {
        execl("/bin/echo", "echo", "hello", NULL);
        fprintf(stderr, "첫 번째 실패\n");
        exit(1);
    }
    printf("부모 프로세스 끝\n");
    return 0;
}
```

### 예제: execute2.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("부모 프로세스 시작\n");

    if (fork() == 0) {
        execl("/bin/echo", "echo", "hello", NULL);
        exit(1);
    }
    if (fork() == 0) {
        execl("/bin/date", "date", NULL);
        exit(2);
    }
    if (fork() == 0) {
        execl("/bin/ls", "ls", "-l", NULL);
        exit(3);
    }

    printf("부모 프로세스 끝\n");
    return 0;
}
```

### 예제: execute3.c
```c
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int pid = fork(), child, status;
    if (pid == 0) {
        execvp(argv[1], &argv[1]);
        fprintf(stderr, "%s: 실행 불가\n", argv[1]);
    } else {
        child = wait(&status);
        printf("[%d] 자식 프로세스 %d 종료\n", getpid(), child);
        printf("종료 코드 %d\n", status >> 8);
    }
    return 0;
}
```

---

## 9.3 입출력 재지정

설명: 명령어 표준출력을 파일로 저장  
함수: `dup2(int oldfd, int newfd)`  

### 예제: redirect1.c
```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0600);
    dup2(fd, 1);  // 표준출력을 fd로 복제
    close(fd);
    printf("Hello stdout!\n");
    fprintf(stderr, "Hello stderr!\n");
    return 0;
}
```

### 예제: redirect2.c
```c
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int pid = fork(), fd, status;
    if (pid == 0) {
        fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0600);
        dup2(fd, 1);
        close(fd);
        execvp(argv[2], &argv[2]);
        fprintf(stderr, "%s: 실행 불가\n", argv[2]);
        exit(1);
    } else {
        wait(&status);
        printf("[%d] 자식 프로세스 종료\n", getpid());
    }
    return 0;
}
```

---

## 9.4 프로세스 그룹

설명: 관련 프로세스를 그룹으로 묶어 신호 전달에 활용  
함수:
- `pid_t getpgrp(void)` : 현재 프로세스 그룹 ID 반환  
- `int setpgid(pid_t pid, pid_t pgid)` : 프로세스 그룹 설정  

### 예제: pgrp1.c
```c
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("PARENT: PID=%d GID=%d\n", getpid(), getpgrp());
    if (fork() == 0) {
        printf("CHILD: PID=%d GID=%d\n", getpid(), getpgrp());
    }
    return 0;
}
```

### 예제: pgrp2.c
```c
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("PARENT: PID=%d GID=%d\n", getpid(), getpgrp());
    if (fork() == 0) {
        setpgid(0, 0);  // 자식이 새로운 그룹 리더 됨
        printf("CHILD: PID=%d GID=%d\n", getpid(), getpgrp());
    }
    return 0;
}
```

- **신호 보내기 예시**  
  - `kill -9 pid` : 특정 프로세스  
  - `kill -9 -pid` : 그룹 전체  

---

## 9.5 시스템 부팅

설명: 커널이 fork/exec를 통해 여러 초기 프로세스 생성  

1. **swapper** (스케줄러 프로세스)  
2. **init** : `/etc/inittab`에 따라 시스템 초기화  
3. **getty** : 로그인 프롬프트 제공  
4. **login** : 사용자 인증  
5. **shell** : 셸 시작 및 명령어 대기  

---

**핵심 개념 요약**
- `fork()`: 프로세스 복제  
- `exec()`: 프로그램 대치 실행  
- `wait()`: 자식 종료 대기  
- `dup2()`: 파일 디스크립터 복제  
- `setpgid()`: 프로세스 그룹 관리  
- 시스템 부팅 절차도 fork/exec로 구현  
