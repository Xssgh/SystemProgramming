##7.1 파일 잠금 (flock)
- **설명**: 파일 전체를 잠궈 동시 접근 충돌 방지
- **함수**: `int flock(int fd, int operation)`
- **상수**:
  - `LOCK_SH`: 공유 잠금 (읽기)
  - `LOCK_EX`: 배타 잠금 (쓰기)
  - `LOCK_UN`: 잠금 해제
  - `LOCK_NB`: 논블로킹 모드

```c
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int fd = open(argv[1], O_WRONLY|O_CREAT, 0600);
    if (flock(fd, LOCK_EX) != 0) { perror("flock"); exit(1); }
    for (int i = 0; i < 5; i++) {
        printf("PID %d: 작업 %d/5\n", getpid(), i+1);
        sleep(1);
    }
    flock(fd, LOCK_UN);
    close(fd);
    return 0;
}
```

##7.2 레코드 잠금 (fcntl)
- **설명**: 파일의 일부(레코드)만 잠궈 세밀한 동시 접근 제어
- **함수**: `int fcntl(int fd, int cmd, struct flock *lock)`
- **주요 명령**:
  - `F_RDLCK`: 공유 읽기 잠금
  - `F_WRLCK`: 배타 쓰기 잠금
  - `F_UNLCK`: 잠금 해제
  - `F_SETLK` / `F_SETLKW`: 논블로킹/블로킹 잠금 설정
  - `F_GETLK`: 잠금 상태 검사

```c
/* rdlock.c 예제: 읽기 잠금 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "student.h"

int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDONLY), id;
    struct flock lock;
    struct student rec;

    printf("조회할 학번 입력: ");
    while (scanf("%d", &id) == 1) {
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (id-START_ID)*sizeof(rec);
        lock.l_len = sizeof(rec);
        fcntl(fd, F_SETLKW, &lock);

        lseek(fd, lock.l_start, SEEK_SET);
        read(fd, &rec, sizeof(rec));
        printf("이름:%s, 학번:%d, 점수:%d\n", rec.name, rec.id, rec.score);

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        printf("조회할 학번 입력: ");
    }
    close(fd);
    return 0;
}

/* wrlock.c 예제: 쓰기 잠금 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "student.h"

int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDWR), id;
    struct flock lock;
    struct student rec;

    printf("수정할 학번 입력: ");
    while (scanf("%d", &id) == 1) {
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (id-START_ID)*sizeof(rec);
        lock.l_len = sizeof(rec);
        fcntl(fd, F_SETLKW, &lock);

        lseek(fd, lock.l_start, SEEK_SET);
        read(fd, &rec, sizeof(rec));
        printf("이름:%s, 학번:%d, 점수:%d\n", rec.name, rec.id, rec.score);

        printf("새 점수 입력: ");
        scanf("%d", &rec.score);
        lseek(fd, -sizeof(rec), SEEK_CUR);
        write(fd, &rec, sizeof(rec));

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        printf("수정할 학번 입력: ");
    }
    close(fd);
    return 0;
}
```

##7.3 간단한 잠금 함수 (lockf)
- **설명**: 간단한 파일 영역 잠금/해제
- **함수**: `int lockf(int fd, int cmd, off_t len)`
- **명령**:
  - `F_LOCK`: 블로킹 쓰기 잠금
  - `F_TLOCK`: 논블로킹 잠금
  - `F_TEST`: 잠금 상태 검사
  - `F_ULOCK`: 잠금 해제

```c
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

/* lockf 예제 */
int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDWR);
    if (lockf(fd, F_LOCK, 0) != 0) { perror("lockf"); exit(1); }
    // 파일 작업 수행
    lockf(fd, F_ULOCK, 0);
    close(fd);
    return 0;
}
```

##7.4 권고 잠금 vs 강제 잠금
- **권고 잠금**: 프로세스 자발적 준수 (읽기·쓰기 가능)
- **강제 잠금**: 커널 차원 강제 (읽기·쓰기 금지)

```bash
chmod 2644 mandatory.txt
ls -l mandatory.txt
# -rw-r-Sr-- ... (set-group-ID + 그룹 실행 비트 해제)
```

##8.1 쉘과 프로세스
- **전면 처리**: 명령 실행→완료까지 대기  
- **후면 처리**: 백그라운드 실행

```bash
date; who; pwd
find . -name "*.c" > out.txt &
jobs
fg %1
```

##8.2 프로그램 실행 및 환경 변수
- **명령줄 인수 & 환경 변수**: `main(argc, argv)`

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    extern char **environ;
    for (int i = 0; i < argc; i++)
        printf("argv[%d]: %s\n", i, argv[i]);
    printf("HOME=%s\n", getenv("HOME"));
    return 0;
}
```

##8.3 프로세스 종료와 exit 처리
- **정상 종료**: `exit()` → 스트림 정리 → exit handler 호출  
- **비정상 종료**: `abort()`, 시그널  
- **exit 처리기 등록**: `atexit()`

```c
#include <stdio.h>
#include <stdlib.h>

void handler1(void) { printf("첫 번째 처리기\n"); }
void handler2(void) { printf("두 번째 처리기\n"); }

int main(void) {
    atexit(handler1);
    atexit(handler2);
    printf("main 끝\n");
    exit(0);
}
```

##8.4 PID와 사용자 ID
```c
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>

int main() {
    printf("PID = %d\n", getpid());
    printf("PPID= %d\n", getppid());
    printf("UID = %d(%s)\n", getuid(), getpwuid(getuid())->pw_name);
    printf("EUID= %d(%s)\n", geteuid(), getpwuid(geteuid())->pw_name);
    return 0;
}
```

##8.5 프로세스 메모리 이미지
- **구성**:  
  - 텍스트(text): 실행 코드  
  - 데이터(data): 전역/정적 변수  
  - 힙(heap): 동적 메모리  
  - 스택(stack): 함수 호출 레코드  
  - U-영역(user-area): 프로세스 상태 정보  

