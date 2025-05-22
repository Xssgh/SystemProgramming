#📘 Unix 시스템 프로그래밍 요약 (Chapter 7 & Chapter 8)

##7.1 파일 잠금 (flock)
```c
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int fd = open(argv[1], O_WRONLY | O_CREAT, 0600);
    if (flock(fd, LOCK_EX) != 0) {
        printf("flock error\n");
        exit(1);
    }
    for (int i = 0; i < 5; i++) {
        printf("file lock %d : %d\n", getpid(), i);
        sleep(1);
    }
    flock(fd, LOCK_UN);
    close(fd);
}
```

##7.2 레코드 잠금 (fcntl)
**rdlock.c**
```c
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "student.h"

int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDONLY);
    struct flock lock;
    struct student record;
    int id;

    printf("검색할 학생의 학번 입력: ");
    while (scanf("%d", &id) == 1) {
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (id - START_ID) * sizeof(record);
        lock.l_len = sizeof(record);
        fcntl(fd, F_SETLKW, &lock);

        lseek(fd, lock.l_start, SEEK_SET);
        read(fd, &record, sizeof(record));
        printf("이름:%s 학번:%d 점수:%d\n", record.name, record.id, record.score);

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        printf("검색할 학생의 학번 입력: ");
    }
    close(fd);
    return 0;
}
```

**wrlock.c**
```c
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "student.h"

int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDWR);
    struct flock lock;
    struct student record;
    int id;

    printf("수정할 학생의 학번 입력: ");
    while (scanf("%d", &id) == 1) {
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (id - START_ID) * sizeof(record);
        lock.l_len = sizeof(record);
        fcntl(fd, F_SETLKW, &lock);

        lseek(fd, lock.l_start, SEEK_SET);
        read(fd, &record, sizeof(record));
        printf("이름:%s 학번:%d 점수:%d\n", record.name, record.id, record.score);

        printf("새로운 점수: ");
        scanf("%d", &record.score);
        lseek(fd, -sizeof(record), SEEK_CUR);
        write(fd, &record, sizeof(record));

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        printf("수정할 학생의 학번 입력: ");
    }
    close(fd);
    return 0;
}
```

##7.3 간단한 잠금 함수 (lockf)
```c
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDWR);
    if (lockf(fd, F_LOCK, 0) == -1) {
        perror("lockf error");
        exit(1);
    }
    // 파일 수정 작업
    lockf(fd, F_ULOCK, 0);
    close(fd);
    return 0;
}
```

##8.1 쉘과 프로세스
```bash
# 전면 처리
$ date; who; pwd

# 후면 처리
$ find . -name "*.c" > out.txt &

# 작업 확인
$ jobs
$ fg %1

# 프로세스 상태 확인
$ ps -aux
$ ps -ef | more
```

##8.2 프로그램 실행 (exec)
```c
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    extern char **environ;
    for (int i = 0; i < argc; i++)
        printf("argv[%d]: %s\n", i, argv[i]);
    char *ptr = getenv("HOME");
    printf("HOME = %s\n", ptr);
    return 0;
}
```

##8.3 프로그램 종료 (exit)
```c
#include <stdlib.h>
void atexit(void (*func)(void));

static void handler1(void) {
    printf("첫 번째 exit 처리기\n");
}

static void handler2(void) {
    printf("두 번째 exit 처리기\n");
}

int main(void) {
    atexit(handler1);
    atexit(handler2);
    printf("main 끝\n");
    exit(0);
}
```

##8.4 프로세스 ID와 사용자 ID
```c
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

int main() {
    printf("PID: %d\n", getpid());
    printf("PPID: %d\n", getppid());
    printf("UID: %d(%s)\n", getuid(), getpwuid(getuid())->pw_name);
    printf("EUID: %d(%s)\n", geteuid(), getpwuid(geteuid())->pw_name);
    return 0;
}
```

##8.5 프로세스 이미지
```txt
프로세스 메모리 구성:
- 텍스트 (코드)
- 데이터 (전역/정적 변수)
- 힙 (동적 할당)
- 스택 (함수 호출 실행 스택)
- U-영역 (프로세스 제어 정보)
```

