
## 시스템프로그래밍

```c
// 시스템 호출의 예
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int open(const char *path, int oflag, [mode_t mode]);
int close(int fd);
ssize_t read(int fd, void *buf, size_t nbytes);
ssize_t write(int fd, void *buf, size_t nbytes);
off_t lseek(int fd, off_t offset, int whence);
int creat(const char *path, mode_t mode);
int dup(int oldfd);
int dup2(int oldfd, int newfd);
```

```c
// open 사용 예
int fd;
fd = open("account", O_RDONLY);
fd = open(argv[1], O_RDWR);
fd = open(argv[1], O_RDWR | O_CREAT, 0600);
fd = open("tmpfile", O_WRONLY | O_CREAT | O_TRUNC, 0600);
fd = open("/sys/log", O_WRONLY | O_APPEND | O_CREAT, 0600);
if ((fd = open("tmpfile", O_WRONLY | O_CREAT | O_EXCL, 0666)) == -1)
    perror("파일 열기 실패");
```

```c
// fopen.c
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int fd;
    if ((fd = open(argv[1], O_RDWR)) == -1)
        printf("파일 열기 오류\n");
    else
        printf("파일 %s 열기 성공 : %d\n", argv[1], fd);
    close(fd);
    exit(0);
}
```

```c
// creat 사용 예
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int creat(const char *path, mode_t mode);
// open(path, O_WRONLY | O_CREAT | O_TRUNC, mode); 와 동일
```

```c
// read & write 예
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t nbytes);
ssize_t write(int fd, void *buf, size_t nbytes);
```

```c
// fsize.c: 파일 크기 측정
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#define BUFSIZE 512

int main(int argc, char *argv[]) {
    char buffer[BUFSIZE];
    int fd;
    ssize_t nread;
    long total = 0;

    if ((fd = open(argv[1], O_RDONLY)) == -1) 
        perror(argv[1]);

    while ((nread = read(fd, buffer, BUFSIZE)) > 0)
        total += nread;

    close(fd);
    printf("%s 파일 크기 : %ld 바이트 \n", argv[1], total);
    exit(0);
}
```

```c
// copy.c: 파일 복사
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) { 
    int fd1, fd2, n;
    char buf[BUFSIZ];

    if (argc != 3) { 
        fprintf(stderr, "사용법: %s file1 file2\n", argv[0]); 
        exit(1); 
    }

    if ((fd1 = open(argv[1], O_RDONLY)) == -1) {
        perror(argv[1]);
        exit(2); 
    }

    if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
        perror(argv[2]);
        exit(3); 
    }

    while ((n = read(fd1, buf, BUFSIZ)) > 0)
        write(fd2, buf, n);

    exit(0); 
}
```

```c
// dup.c: 파일 디스크립터 복제
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    int fd1, fd2;
    if ((fd1 = creat("myfile", 0600)) == -1)
        perror("myfile");

    write(fd1, "Hello! Linux", 12);
    fd2 = dup(fd1);
    write(fd2, "Bye! Linux", 10);
    exit(0); 
}
```

```c
// lseek 사용 예
lseek(fd, 0L, SEEK_SET);    // 파일 처음으로
lseek(fd, 100L, SEEK_SET);  // 100바이트 지점
lseek(fd, 0L, SEEK_END);    // 파일 끝으로
lseek(fd, sizeof(record), SEEK_CUR);  // 다음 레코드
```

```c
// student.h
#define MAX 24
#define START_ID 1401001

struct student {
    char name[MAX];
    int id;
    int score;
};
```

```c
// dbcreate.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "student.h"

int main(int argc, char *argv[]) {
    int fd;
    struct student record;

    if (argc < 2) {
        fprintf(stderr, "사용법 : %s file\n", argv[0]);
        exit(1);
    }

    if ((fd = open(argv[1], O_WRONLY|O_CREAT|O_EXCL, 0640)) == -1) {
        perror(argv[1]);
        exit(2);
    }

    printf("%-9s %-8s %-4s\n", "학번", "이름", "점수");
    while (scanf("%d %s %d", &record.id, record.name, &record.score) == 3) {
        lseek(fd, (record.id - START_ID) * sizeof(record), SEEK_SET);
        write(fd, (char *) &record, sizeof(record));
    }
    close(fd);
    exit(0);
}
```

```c
// dbquery.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "student.h"

int main(int argc, char *argv[]) {
    int fd, id;
    char c;
    struct student record;

    if (argc < 2) {
        fprintf(stderr, "사용법 : %s file\n", argv[0]);
        exit(1);
    }

    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        perror(argv[1]);
        exit(2);
    }

    do {
        printf("\n검색할 학생의 학번 입력:");
        if (scanf("%d", &id) == 1) {
            lseek(fd, (id - START_ID) * sizeof(record), SEEK_SET);
            if ((read(fd, (char *)&record, sizeof(record)) > 0) && (record.id != 0))
                printf("이름:%s\t 학번:%d\t 점수:%d\n", record.name, record.id, record.score);
            else
                printf("레코드 %d 없음\n", id);
        } else printf("입력 오류\n");

        printf("계속하겠습니까?(Y/N)");
        scanf(" %c", &c);
    } while (c == 'Y');

    close(fd);
    exit(0);
}
```

```c
// dbupdate.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "student.h"

int main(int argc, char *argv[]) {
    int fd, id;
    char c;
    struct student record;

    if (argc < 2) {
        fprintf(stderr, "사용법 : %s file\n", argv[0]);
        exit(1);
    }

    if ((fd = open(argv[1], O_RDWR)) == -1) {
        perror(argv[1]);
        exit(2);
    }

    do {
        printf("수정할 학생의 학번 입력: ");
        if (scanf("%d", &id) == 1) {
            lseek(fd, (long)(id - START_ID) * sizeof(record), SEEK_SET);
            if ((read(fd, (char *)&record, sizeof(record)) > 0) && (record.id != 0)) {
                printf("학번:%8d\t 이름:%4s\t 점수:%4d\n", record.id, record.name, record.score);
                printf("새로운 점수: ");
                scanf("%d", &record.score);
                lseek(fd, (long)-sizeof(record), SEEK_CUR);
                write(fd, (char *)&record, sizeof(record));
            } else printf("레코드 %d 없음\n", id);
        } else printf("입력오류\n");
        printf("계속하겠습니까?(Y/N)");
        scanf(" %c", &c);
    } while (c == 'Y');

    close(fd);
    exit(0);
}
