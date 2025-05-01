

# 6장 프로세스

| 01 | 프로세스             |
| :--- | :------------------- |
| 02 | 작업 제어            |
| 03 | 프로세스 제어          |
| 04 | 프로세스와 사용자 ID |
| 05 | 시그널과 프로세스     |


## 6.1 프로세스

프로세스(process)는 실행 중인 프로그램을 말합니다.

* 각 프로세스는 유일한 프로세스 번호 PID를 갖습니다.
* 각 프로세스는 부모 프로세스에 의해 생성됩니다.

프로세스의 종류로는 시스템 프로세스와 사용자 프로세스가 있습니다.

* 시스템 프로세스: 시스템 운영에 필요한 기능을 수행하는 프로세스이며, 대표적인 예로 부팅 시 생성되는 데몬 프로세스가 있습니다.
* 사용자 프로세스: 사용자의 명령 또는 프로그램을 실행하기 위해 생성된 프로세스입니다.

### 프로세스 상태 보기: ps (process status)

* 사용법:

    ```bash
    $ ps [-옵션]
    ```

    현재 시스템 내에 존재하는 프로세스들의 실행 상태를 요약해서 출력합니다.
* 사용 예:

    ```bash
    $ ps
    ```

    | PID  | TTY   | TIME     | CMD  |
    | :--- | :---- | :------- | :--- |
    | 1519 | pts/3 | 00:00:00 | bash |
    | 1551 | pts/3 | 00:00:00 | ps   |
   

* `$ ps -f`

    | UID    | PID  | PPID | C    | STIME | TTY   | TIME     | CMD        |
    | :----- | :--- | :--- | :--- | :---- | :---- | :------- | :--------- |
    | chang  | 1519 | 1518 | 0    | 17:40 | pts/3 | 00:00:00 | /bin/bash  |
    | chang  | 1551 | 1519 | 0    | 17:40 | pts/3 | 00:00:00 | ps -f     |
   

* `$ ps -ef`

    | UID    | PID  | PPID | C    | STIME    | TTY   | TIME     | CMD                       |
    | :----- | :--- | :--- | :--- | :------- | :---- | :------- | :------------------------ |
    | root   | 1    | 0    | 0    | 9월 30   | ?     | 00:00:23 | /sbin/init auto noprompt  |
    | root   | 2    | 0    | 0    | 9월 30   | ?     | 00:00:00 | \[kthreadd]               |
    | root   | 3    | 2    | 0    | 9월 30   | ?     | 00:00:00 | \[rcu\_gp]                |
    | root   | 4    | 2    | 0    | 9월 30   | ?     | 00:00:00 | \[rcu\_par\_gp]            |
    | root   | 6    | 2    | 0    | 9월 30   | ?     | 00:00:00 | \[kworker/0:0H-events\_highpri] |
    | root   | 9    | 2    | 0    | 9월 30   | ?     | 00:00:00 | \[mm\_percpu\_wq]          |
    | root   | 10   | 2    | 0    | 9월 30   | ?     | 00:00:00 | \[rcu\_tasks\_rude_]       |
    | root   | 11   | 2    | 0    | 9월 30   | ?     | 00:00:00 | \[rcu\_tasks\_trace]      |
    | root   | 12   | 2    | 0    | 9월 30   | ?     | 00:00:03 | \[ksoftirqd/0]            |
    | root   | 13   | 2    | 0    | 9월 30   | ?     | 00:01:07 | \[rcu\_sched]            |
    | root   | 14   | 2    | 0    | 9월 30   | ?     | 00:00:02 | \[migration/0]            |
    | root   | 24   | 2    | 0    | 9월 30   | ?     | 00:00:00 | \[netns]                  |
    | root   | 25   | 2    | 0    | 9월 30   | ?     | 00:00:00 | \[inet\_frag\_wq]          |
    | root   | 26   | 2    | 0    | 9월 30   | ?     | 00:00:00 | \[kauditd]                |
    | --More-- |      |      |      |          |       |          |                           |
   

| 항목   | 의미                               |
| :----- | :--------------------------------- |
| UID    | 프로세스를 실행시킨 사용자 ID          |
| PID    | 프로세스 번호                          |
| PPID   | 부모 프로세스 번호                      |
| C      | 프로세스의 우선순위                      |
| STIME  | 프로세스의 시작 시간                     |
| TTY    | 명령어가 시작된 터미널                   |
| TIME   | 프로세스에 사용된 CPU 시간               |
| CMD    | 실행되고 있는 명령어 (프로그램) 이름 |
   

### 특정 프로세스 리스트: pgrep

* 특정 프로세스만 리스트:

    ```bash
    $ ps -ef | grep -w sshd
    ```
   

* 사용법:

    ```bash
    $ pgrep [옵션] [패턴]
    ```

    패턴에 해당하는 프로세스들만을 리스트 합니다.

    * `-I`: PID와 함께 프로세스의 이름을 출력합니다.
    * `-f`: 명령어의 경로도 출력합니다.
    * `-n`: 패턴과 일치하는 프로세스들 중에서 가장 최근 프로세스만을 출력합니다.
    * `-x`: 패턴과 정확하게 일치되는 프로세스만 출력합니다.
    * `-a`: 전체 명령어 줄과 PID를 출력합니다.

* 예시:

    ```bash
    $ pgrep sshd
    5032
    ```
   

* `-1` 옵션: 프로세스 번호와 프로세스 이름을 함께 출력합니다.

    ```bash
    $ pgrep -I sshd
    5032 sshd
    ```
   

* `-n` 옵션: 가장 최근 프로세스만 출력합니다.

    ```bash
    $ pgrep -In sshd
    5032 sshd
    ```
   

## 6.2 작업 제어

### 쉘과 프로세스



### 후면 처리

```bash
$ 명령어 &
[1] 프로세스번호
## 7장 인터넷과 서버

### 7.1 네트워크 구성

- **LAN**: 근거리 통신망 (집, 사무실, 학교 등)
- **이더넷**: Xerox PARC에서 개발된 일반적인 LAN 구현 방식
- **라우터**: 네트워크 간 연결 및 데이터 패킷 전달 장치
- **게이트웨이**: LAN과 외부 인터넷을 연결하는 장치
- **WAP (Wireless Access Point)**: 무선 장치를 유선망에 연결

### 7.2 인터넷

- **인터넷**: TCP/IP 프로토콜을 사용하는 공개 통신망
- **IP 주소**: 장치의 고유 주소 (예: 203.252.201.11)
- **TCP**: 데이터 전달 보장 및 순서 유지
- **DNS**: 도메인 이름을 IP 주소로 변환

```bash
$ nslookup cs.sookmyung.ac.kr
$ finger 사용자명
```

### 7.3 서버 설치

- **Apache 웹 서버 설치**
```bash
$ sudo apt install apache2 php mariadb-server
$ sudo systemctl start apache2
$ sudo systemctl enable apache2
$ sudo systemctl status apache2
```

- **방화벽 설정**
```bash
$ sudo firewall-cmd --permanent --add-service=http
$ sudo firewall-cmd --reload
```

### 7.4 파일 전송

- **FTP / SFTP**
```bash
$ ftp 호스트명
$ sftp 호스트명
sftp> get 파일명
sftp> put 파일명
```

### 7.5 원격 접속

```bash
$ ssh 사용자명@호스트명
$ ssh 호스트명 명령어
```

### 7.6 원격 데스크톱 연결

```bash
$ sudo apt install xrdp
$ sudo systemctl start xrdp
$ sudo systemctl enable xrdp
$ sudo ufw allow 3389
```

### 7.7 월드와이드웹(WWW)

- **HTML**: 웹 문서 작성 언어  
- **HTTP**: 클라이언트-서버 간 통신 프로토콜  
- **URL**: 웹 자원의 주소 (예: http://www.mozilla.or.kr)  
- **웹 브라우저**: Chrome, Firefox, Safari 등

---

## 10장 Bash 쉘 스크립트

### 10.1 변수

```bash
$ city="Seoul"
$ echo $city

$ cities=(서울 부산 제주)
$ echo ${cities[1]}
```

### 10.2 조건문

```bash
if [ 조건식 ]; then
  명령어들
fi
```

### 10.3 반복문

```bash
for name in ${list[@]}; do
  echo $name
done

while ((조건)); do
  명령어들
done
```

### 10.4 함수

```bash
myfunc() {
  echo "안녕하세요"
}
myfunc
```

### 10.5 연산

- **산술**
```bash
$ let a=2+3
$ echo $a
```

- **문자열 비교**
```bash
if [ "$x" == "y" ]; then
  echo "같음"
fi
```

- **파일 검사**
```bash
if [ -f "파일명" ]; then
  echo "일반 파일입니다"
fi
```

---

## 11장 프로그래밍 환경

### 11.1 gedit 편집기

```bash
$ gedit hello.c &
```

### 11.2 gcc 컴파일

```bash
$ gcc hello.c
$ ./a.out

$ gcc -o hello hello.c
```

### 11.3 다중 모듈 프로그램

```bash
$ gcc -c main.c
$ gcc -c copy.c
$ gcc -o main main.o copy.o
```

- **Makefile 예시**
```make
main: main.o copy.o
	gcc -o main main.o copy.o

main.o: main.c copy.h
	gcc -c main.c

copy.o: copy.c
	gcc -c copy.c
```

### 11.4 gdb 디버깅

```bash
$ gcc -g -o main main.c
$ gdb main
(gdb) break main
(gdb) run
(gdb) next
(gdb) print 변수
```

### 11.5 vi 에디터

- 저장 및 종료: `:wq`
- 강제 종료: `:q!`
- 줄 번호 표시: `:set number`
