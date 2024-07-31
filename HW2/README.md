# 64bit 초과 입력에 대한 덧셈, 뺄셈 구현

### 전체코드

```c
#include <stdio.h>
#include <ctype.h> // isdigit, isspace
#include <string.h>

#define MAX_DIGITS 1000

void clearBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

// 숫자 문자열의 유효성 검증
int isValidNumber(char str[]) {
	int len = strlen(str);

	if (str[0] != '-' && !isdigit(str[0])) return 0; // '-'가 아니면서 ('-'는 허용) 정수가 아닌경우 유효하지 않다. 즉, '-' 또는 정수값(0~9)은 유효하다.

	for (int i = 1; i < len; i++) {
		if (!isdigit(str[i])) {
			return 0; // 숫자나 음수 기호가 아닌 문자가 포함된 경우
		}
	}
	return 1;
}

// 숫자를 입력받는 함수
void getNum(char num[]) {
	while (1) {
		// sizeof(num)은 포인터 크기로 인식돼서 8바이트로 잡히기 때문에 직접 값을 명시해준다. 
		if (scanf_s("%s", num, MAX_DIGITS + 2) != 1) {
			clearBuffer(); // 정해긴 길이보다 더 긴 입력이 들어왔을 경우, 이후의 입력에 끼치는 영향이 없도록, 버퍼를 비워준다.
			printf("잘못된 입력입니다. 다시 입력해주세요.\n: ");
			continue;
		}
		if (isValidNumber(num)) break;
		else printf("잘못된 입력입니다. 다시 입력해주세요.\n: ");
	}
}

// 두 문자열의 절대값 비교
int absCompare(const char* num1, const char* num2) {
	if (num1[0] == '-') num1++;
	if (num2[0] == '-') num2++;

	int len1 = strlen(num1);
	int len2 = strlen(num2);

	if (len1 > len2) return 1;
	if (len1 < len2) return -1;

	return strcmp(num1, num2);
}

// 문자열 덧셈 함수
void add(char num1[], char num2[], char result[]);

// 문자열 뺄셈 함수
void subtract(char num1[], char num2[], char result[]);

void add(char num1[], char num2[], char result[]) {
	// 1) 양수 더하기 양수: 그냥 하면 됨
	// 2) 음수 + 음수: 절댓값끼리 더하고, 마지막에 음수 부호만 붙이면 된다.
	// 3) 음수 + 양수: -2 + 5 or -5 + 2 // 부호를 제외하고, 절대값이 큰 수에서 절대값이 작은 수를 뺀 뒤에, 절대값이 큰 쪽의 부호를 선택한다. (strncmp로 비교)
	// 4) 양수 + 음수: 5 + -2 or 2 + -5 // 3과 마찬가지.

	int len1 = strlen(num1);
	int len2 = strlen(num2);
	int isNegative = 0; // 결과가 음수인지 양수인지 

	char temp[MAX_DIGITS + 2] = { 0 }; // 덧셈 결과를 역순으로 저장. 

	if (num1[0] == '-' && num2[0] == '-') {
		// 음수 + 음수: 절댓값끼리 더한 후 결과에 음수 부호를 붙임
		isNegative = 1;
	}
	else if (num1[0] == '-') {
		// 음수 + 양수: 부호를 제외하고 절대값이 큰 수에서 절대값이 작은 수를 뺀 뒤 절대값이 큰 쪽의 부호를 선택
		int comp = absCompare(num1, num2);
		if (comp > 0) {
			result[0] = '-';
			subtract(num1 + 1, num2, result + 1);
			return;
		}
		else if (comp < 0) {
			subtract(num2, num1 + 1, result);
			return;
		}
		else {
			result[0] = '0';
			result[1] = '\0';
			return;
		}
	}
	else if (num2[0] == '-') {
		// 양수 + 음수: 부호를 제외하고 절대값이 큰 수에서 절대값이 작은 수를 뺀 뒤 절대값이 큰 쪽의 부호를 선택
		subtract(num1, num2 + 1, result); // num2의 절대값을 빼기
		return;
	}

	int i = len1 - 1, j = len2 - 1;
	int sum = 0, carry = 0;
	int digit1 = 0, digit2 = 0;
	int idx = 0;
	
	while (i >= 0 || j >= 0 || carry > 0) {
		if (i >=0 && num1[i] != '-') {
			digit1 = num1[i] - '0';
		}
		else {
			digit1 = 0;
		}

		if (j >= 0 && num2[j] != '-') {
			digit2 = num2[j] - '0';
		}
		else {
			digit2 = 0;
		}

		sum = (digit1 + digit2 + carry); 
		temp[idx++] = (sum % 10) + '0'; // 문자로 저장 13
		carry = (sum) / 10;

		i--;
		j--;
		
	}
	if (num1[0] == '-' && num2[0] == '-') {
		temp[idx - 1] = '\0'; // 위 while문에서 의미없는 0 값이 더해져서, 삭제
	}

	idx = 0;
	if (isNegative) {
		result[0] = '-';
		idx++;
	}

	for (int i = strlen(temp) - 1; i >= 0; i--) {
		result[idx++] = temp[i]; 
	}
	
	result[idx] = '\0';
}

// 실제 연산은 절대값이 큰 쪽에서 절대값이 작은 쪽을 빼게 수행
void subtract(char num1[], char num2[], char result[]) {
	// 1) 양수 빼기 양수: 절대값이 큰 수에서 절대값이 작은 수를 뺀 뒤에, 우항이 더 클때만 - 부호를 붙인다. (크기 비교는 strncmp)
	// 2) 음수 - 음수: (-2) - (-5) = 3 or (-5) - (-2) // 절대값이 큰 수에서 절대값이 작은 수를 뺀 뒤에, 좌항의 절대값이 더 클 때만, - 부호를 붙인다.
	// 3) 음수 - 양수: 절대값끼리 덧셈 연산 후, - 부호를 붙인다. 
	// 4) 양수 - 음수: 절대값끼리 덧셈 연산 수행.
	int len1 = strlen(num1);
	int len2 = strlen(num2);
	int isNegative = 0; // 결과에 음수가 붙는지, 안붙는지

	if (num1[0] == '-' && num2[0] == '-') {
		// 음수 - 음수: 절대값이 큰 수에서 작은 수를 뺀 후 부호를 결정

		if (absCompare(num1, num2) < 0) {
			// Swap num1 and num2
			const char* temp = num1;
			num1 = num2;
			num2 = temp;
			isNegative = 0; // 큰 쪽의 부호를 따라가기 때문에, 음수 - 음수는 결과적으로 양수가 된다.
		}
		else {
			isNegative = 1;
		}
	}
	else if (num1[0] == '-') {
		// 음수 - 양수: 절댓값끼리 더한 후 결과에 음수 부호를 붙임
		result[0] = '-';
		add(num1 + 1, num2, result+1); // num1의 절대값과 더하기
		return;
	}
	else if (num2[0] == '-') {
		// 양수 - 음수: 절댓값끼리 더함
		add(num1, num2 + 1, result); // num2의 절대값과 더하기
		return;
	}
	else {
		// 양수 - 양수: 절대값이 큰 수에서 절대값이 작은 수를 뺀 후 부호를 결정
		if (absCompare(num1, num2) < 0) {
			// Swap num1 and num2
			const char* temp = num1;
			num1 = num2;
			num2 = temp;
			isNegative = 1;
		}
	}

	int i = strlen(num1) - 1, j = strlen(num2) - 1;
	int diff = 0, borrow = 0;
	int digit1 = 0, digit2 = 0;
	int idx = 0;

	char temp[MAX_DIGITS + 2] = { 0 }; // 뺄셈 결과를 역순으로 저장

	while (i >= 0 || j >= 0 || borrow > 0) {
		if (i >= 0 && num1[i] != '-') {
			digit1 = num1[i] - '0';
		}
		else {
			digit1 = 0;
		}

		if (j >= 0 && num2[j] != '-') {
			digit2 = num2[j] - '0';
		}
		else {
			digit2 = 0;
		}

		diff = digit1 - digit2 - borrow; // 125 - 68 -> 750 -> 57
		if (diff < 0) {
			diff += 10;
			borrow = 1;
		}
		else {
			borrow = 0;
		}

		temp[idx++] = diff + '0'; // 문자로 저장

		i--;
		j--;
	}

	if (num1[0] == '-' && num2[0] == '-') {
		temp[idx - 1] = '\0'; // 위 while문에서 의미없는 0 값이 더해져서, 삭제
	}

	idx = 0;
	if (isNegative) {
		result[idx++] = '-';
	}

	i = strlen(temp) - 1;
	while (i >= 0 && temp[i] == '0') {
		i--;
	}
	if (i == -1) result[idx++] = '0';
	for (i; i >= 0; i--) {
		result[idx++] = temp[i];
	}

	result[idx] = '\0';

}

int main(void) {

	char num1[MAX_DIGITS + 2];
	char num2[MAX_DIGITS + 2]; // 음수 포함 최대 천 자릿수 까지 입력 가능 ('-' 기호 + 천자리숫자 + '\0')
	char result[MAX_DIGITS + 3] = { 0 };

	while (1) {
		printf("덧셈: [1], 뺄셈: [2], 종료: [0]\n:");
		int op;
		scanf_s("%d", &op);

		switch (op) {
		case 1:
			printf("첫번째 수를 입력하세요: ");
			getNum(num1);
			printf("두번째 수를 입력하세요: ");
			getNum(num2);
			add(num1, num2, result);
			printf("덧셈 결과: %s\n\n", result);
			break;
		case 2:
			printf("첫번째 수를 입력하세요: ");
			getNum(num1);
			printf("두번째 수를 입력하세요: ");
			getNum(num2);
			subtract(num1, num2, result);
			printf("뺄셈 결과: %s\n\n", result);
			break;
		case 0:
			printf("프로그램을 종료합니다.\n");
			return 0;
			break;
		default:
			printf("잘못된 입력입니다.\n\n");
		}

		clearBuffer(); 
	}

	return 0;
}
```
