# 생성형 AI

챗GPT로 대표되는 생성형 AI는 **'인터넷' 또는 '모바일 혁명'**에 비견될 혁신적인 기술입니다.  
개인에게 높은 침투율을 보이며, 산업 전반에도 빠르게 확산 중입니다.

---

## 인공지능의 역사

- **인공지능(AI)** 용어 첫 등장  
- **머신러닝(Machine Learning)** 발전  
- **딥러닝(Deep Learning)** 기술 등장  
- 다양한 인공지능 기초 개념 연구  
- **대규모 언어처리 모델(LLM)**의 발전  

---

## 주의사항

⚠️ **개인정보, 기업정보, 기밀정보를 입력하지 마세요.**

---

## GPT가 문장을 만드는 방법

- 빈칸을 채우는 방식  
- 다음 단어를 예측하는 방식  

이러한 방식을 통해 AI는 자연스러운 문장을 생성합니다.

---

## 효율적인 활용법

- 초안 작성에 활용하기  
- 반드시 검토하고 수정하기  
- 주제별로 채팅 세션을 구분해서 사용하기  
- 중간에 답변이 끊긴 경우 **'계속 작성'** 요청하기  

---

## 프롬프트란?

프롬프트는 AI가 특정 작업을 수행하도록 돕는 **지시문(명령어)** 입니다.

- 명확하고 간결하게 작성할수록 원하는 결과를 얻기 쉽습니다.  
- 육하원칙(누가, 언제, 어디서, 무엇을, 어떻게, 왜)을 활용해 구체적으로 작성하세요.


----
# 🧮 수업 과제 제출물 : 8비트 이진수 변환 프로그램 (C)

이 프로그램은 사용자로부터 `0`부터 `255` 사이의 정수를 입력받아,  
- 해당 수의 **8비트 이진수 표현**을 출력하고  
- **1의 개수**를 세며  
- **상위 4비트**를 출력합니다.

---

## 📄 코드

```c
#include <stdio.h>

int main() {
    int num, i;
    int count = 0;
    int binary[8];
    
    // Input validation
    do {
        printf("Enter a number between 0 and 255: ");
        scanf("%d", &num);
        
        if(num < 0 || num > 255) {
            printf("Invalid input! Please enter a number between 0 and 255.\n");
        }
    } while(num < 0 || num > 255);
    
    // Convert to binary
    for(i = 7; i >= 0; i--) {
        binary[i] = num % 2;
        num = num / 2;
    }
    
    // Display binary representation
    printf("Binary representation: ");
    for(i = 0; i < 8; i++) {
        printf("%d", binary[i]);
        
        // Count number of 1's
        if(binary[i] == 1) {
            count++;
        }
    }
    printf("\n");
    
    // Display count of 1's
    printf("Number of 1's: %d\n", count);
    
    // Display upper 4 bits
    printf("Upper 4 bits: ");
    for(i = 0; i < 4; i++) {
        printf("%d", binary[i]);
    }
    printf("\n");
    
    return 0;
}


