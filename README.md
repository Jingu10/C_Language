### 요구사항

- 비트연산자를 이용한 사칙연산기
- 입력/출력은 모두 정수
- 입력예: 5/2
- 출력예: 몫: 2, 나머지: 1

### 헤더파일 포함

```c
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
```

**stdio.h** : 표준 입출력 함수를 사용하기 위함. printf(), fgets(), getchar()

**string.h**: 문자열 조작 함수 사용을 위함. strncpy_s(), strcpy_s(), strlen(), strcmp(), strpbrk()

**limits.h**: 데이터 타입 한계 값 사용을 위함. INT_MIN, INT_MAX 

**stdlib.h**: 일반 유틸리티 함수 사용을 위함. atoll(), strtoll()

### 함수 선언

```c
void print_intro(); 
void remove_spaces(char*);  
int parse_input(const char*, char*, char*, char*); 
int is_valid_integer(const char*);
void clear_input_buffer();

long long add(long long, long long);
long long subtract(long long, long long);
long long multiply(long long, long long);
void divide(long long, long long, long long*, long long*);
```

**void print_intro();**

프로그램 실행 시 안내 문구 출력.

**void remove_spaces(char*);**

사용자 입력에 대한 공백을 없앰. 숫자와 연산자 사이의 공백 유무와 관계없이 정상 작동 하기 위함. 

**int parse_input(const char*, char*, char*, char*);**

공백을 없앤 사용자 입력에 대해 두 피연산자(숫자)에 대한 문자열 배열과 연산자로 나누어 주는 함수. 성공하면 1, 실패 시 0 반환.

**int is_valid_integer(const char*);**

입력으로부터 분리된 숫자가 int 범위 내에 있는지 검사 하는 함수. 피연산자를 int 범위 내로 설정한 이유는 int의 최대값 끼리 곱셈을 할 경우, 표현할 수 있는 범위가 최대 64비트 이기 때문. 

**void clear_input_buffer();**

입력 버퍼에 남아있는 문자를 지우는 함수. 

**long long add(long long, long long);
long long subtract(long long, long long);
long long multiply(long long, long long);
void divide(long long, long long, long long*, long long*);**

두 피연산자에 대해 비트 연산을 수행하는 함수. 연산의 결과는 int 범위를 초과할 수 있으므로 long long 타입으로 설정. 피연산자 매개변수도 long long 타입으로 설정한 이유는 각 함수의 전체 코드에서 설명. 

### main함수

### 변수 선언

```c
	print_intro();

	char input[30]; // 사용자의 입력을 담기 위한 배열.
	char num1_str[12], num2_str[12];  
	char operator;
	long long num1, num2;
	long long result, quotient, remainder;
```

**char num1_str[12]**

사용자의 입력으로부터 분리한 피연산자를 담는 배열. 피연산자는 INT 범위 안 즉, 최대 10자리 숫자이고 음수일 경우, 앞에 ‘-’ 가 붙을 것과 마지막에 들어갈 ‘\0’ 자리를 위해 크기를 12로 설정.

**long long num1**

유효한 num1_str에 대해 long long 타입 정수로 변환한 값. 

long long 타입으로 선언한 이유는 함수 내에서 비트 연산(shift)의 용이성을 위함. 

### while 이벤트 루프

### 입력 받기 및 너무 긴 입력에 대한 예외처리

```c
		if (fgets(input, sizeof(input), stdin) == NULL) {
			printf("Error reading input.\n");
			continue;
		}

		if (input[strlen(input) - 1] != '\n') {
			clear_input_buffer();
			printf("Input is too long. Please enter a shorter expression considering input Limit.\n");
			continue;
		}
		else {
			input[strlen(input) - 1] = '\0';
		}
```

**fgets(input, sizeof(input), stdin)**

gets_s를 사용할 경우, input의 크기를 초과한 입력에 대해서는 에러창이 출력된다. 잘못된 입력에도 다시 입력할 기회를 주기 위해 fgets 함수를 사용했다. fgets는 파일 입력을 위해 사용되는 함수이지만, 파일포인터 매개변수로 표준입출력 파일인 stdin을 주면 사용자 입력 또한 받을 수 있다. 

**clear_input_buffer();** 

fgets로 입력을 받을 때, 매개변수로 전달된 크기보다 큰 입력이 들어올 경우, 전달된 크기만큼의 값만 저장한다. 하지만, 잘려진 입력은 여전히 버퍼에 남아있기 때문에 이로 인해 발생할 문제를 예방하기 위해 버퍼를 비워줘야 한다.

**input[strlen(input) - 1] = ‘\0’;**

만약, 적절한 길이의 입력의 경우에 fgets는 입력의 끝에 개행문자를 추가한다. 우리는 이 개행문자를 문자열의 끝을 나타내는 ‘\0’ 로 바꿔줘야 한다.

### 종료조건 확인

```c
		if (strcmp(input, "q") == 0) {
			printf("You terminated calculator.\n");
			break;
		}
```

### 공백 제거

```c
remove_spaces(input);
```

```c

void remove_spaces(char* str) {
	char* i = str;
	char* j = str;
	while (*j != 0) {
		*i = *j++;
		if (*i != ' ') {
			i++;
		}
	}
	*i = 0;
}
```

이 함수는 두 개의 포인터 i와 j를 사용하여 주어진 문자열을 순회한다. j포인터는 문자열을 한 문자씩 이동하며, i 포인터는 공백 문자가 아닌 문자만을 복사한다. 공백 문자는 건너뛰고, 최종적으로 문자열의 끝에 널 문자를 추가하여 공백이 제거된 문자열을 생성한다.

### 피연산자, 연산자 분리

```c
		
		
		if (!parse_input(input, num1_str, &operator, num2_str)) {
			printf("Check Your Input.\n"); // 연산자가 없거나, 숫자가 10자리를 초과.
			continue;
		}

```

 

```c

int parse_input(const char* input, char* num1_str, char* operator, char* num2_str) {
	const char* operators = "+-*/";
	const char* op_pos;
	// 음수 처리: 첫 번째 문자가 '-'일 경우 무시
	if (input[0] == '-') {
		op_pos = strpbrk(input + 1, operators);
	}
	else {
		op_pos = strpbrk(input, operators);
	}

	if (op_pos == NULL) {
		return 0; // 연산자가 없음
	}

	*operator = *op_pos; 

	if (op_pos - input > 11) {
		return 0;
	}

	// sizeof(num1_str)은 오류 발생. 함수의 매개변수로 들어간 배열은 포인터로 인식되고, 포인터의 크기인 8바이트로 인식된다.
	strncpy_s(num1_str, 12, input, op_pos - input);
	num1_str[op_pos - input] = '\0';

	// 연산자 뒤의 숫자 복사
	
	strcpy_s(num2_str, 12, op_pos + 1); // 우항은 이 과정에서 '\0' 이 복사된다.

	return 1;
}
```

**if (op_pos == NULL)**

만약, 입력값에 연산자가 포함되지 않다면 strpbrk 함수의 반환값으로 op_pos 에 NULL 이 저장된다. 유효한 입력이 아니었으므로 0을 반환 후, main 함수 에서 사용자에게 오류 문구를 보여준다.

**if (op_pos - input > 11)** 

주소 연잔자끼리의 뺄셈 연산은, 인덱스 차이를 구하는 과정이다. 

이는 연산자의 좌항이 11자(10자리까지 가능하지만 음수 부호 포함 11자)를 초과할 경우, 이미 int 범위를 넘어가므로 불필요한 연산을 줄이기 위해 return 한다.

**strncpy(num1_str, 12, input, op_pos - input);**

**strcpy_s(num2_str, 12, op_pos + 1);**

연산자 위치를 기준으로, 좌항과 우항을 각각 num1_str과 num2_str에 나눠 담는다.

**트러블슈팅1** 

음수에 붙은 ‘-’ 기호를 고려하지 못하여, 연산식의 좌항이 음수일 경우 음수부호가 연산자로 설정되고, 숫자가 제대로 토큰화 되지 못하는 문제가 있었음. 
→ 첫번째 문자가 ‘-’인 경우를 무시하여 해결. 

**트러블슈팅2**

strncpy_s(num1_str, 12, input, op_pos - input); 

에서 두번째 인자를 sizeof(num1_str) 로 설정하여 오류 발생. 

함수의 매개변수로 들어간 배열은 포인터로 인식되고, 메모리 추적을 통해 포인터의 크기인 8바이트로 인식됨을 발견. 값 12 를 명시해줌으로써 해결. 

### 입력 유효성 검증

```c
		if (!is_valid_integer(num1_str) || !is_valid_integer(num2_str)) {
			printf("Check Your Input.\n"); // 숫자가 없거나, int 범위 내의 숫자가 아님. 
			continue;
		}
```

```c
int is_valid_integer(const char* str) {
	char* endptr;
	long num = strtol(str, &endptr, 10);

	if (*endptr != '\0' || endptr == str) {
		return 0; // 변환할 수 없는 문자가 있거나 변환할 수 있는 문자가 없음
	}

	if (num < INT_MIN || num > INT_MAX) {
		return 0; // 숫자가 int 범위를 벗어남
	}

	return 1; // 유효한 정수
}
```

**long num = strtol(str, &endptr, 10);**

계산할 수 없는 문자열을 발견하면 두 번째 입력 인자로 받은 endptr이 가리키는 곳에 설정.

10은 10진수를 의미. 

### 피연산자 숫자 형변환

```c
		num1 = atoll(num1_str);
		num2 = atoll(num2_str);
```

유효성 검증을 마쳤으므로, atoll 함수를 이용하여 정수형태의 문자열을 long long 타입 정수로 변환한다.

### 비트연산

나누기 연산을 하기 전, num2가 0인 경우만 함수 호출 전 미리 검사하여 예외처리를 해준다.

### 덧셈

```c
long long add(long long a, long long b) {
	while (b != 0) {
		long long carry = a & b;
		a = a ^ b;
		b = carry << 1;
	}
	return a;
}
```

a: 1010

b: 0011

**carry = a & b;**

carry: 0010

**a = a^b;**

carry가 발생하지 않는다고 가정하고, 덧셈 수행.

a: 1001

**b = carry << 1;**

carry를 다음 자릿수에 더하기 위해, shift 연산을 사용.

a: 1001

b: 0100

carry: 0000

a: 1101

더 이상 carry가 발생되지 않을 때의 a의 값이 덧셈의 결과이다.

### 뺄셈

```c
long long subtract(long long a, long long b) {
	b = ~b + 1;
	return add(a, b);
}
```

뺄셈 연산은 빼는 수에 1의 보수를 취하고, 1을 더한 값과 덧셈 연산을 수행하면 된다.

### 곱셈

```c
long long multiply(long long a, long long b) {
	long long result = 0;
	while (b != 0) {
		if (b & 1) {
			result += a;
		}
		a <<= 1;
		b >>= 1;
	}
	return result;
}
```

a: 1010

b: 0011

을 곱한다고 하면

    1010 (10)

*  0011 (3)

---

    1010 (10)

+1010  (20)

---

=11110 (30)

b의 최하위 비트가 0이라면, 덧셈 연산을 수행하지 않고, 1일 때만 덧셈연산을 수행하면 된다.

- b를 오른쪽으로 1비트 시프트하여 다음 비트를 검사합니다.
- a를 왼쪽으로 1비트 시프트하여 자릿수를 올립니다.

### 나눗셈

```c
void divide(long long a, long long b, long long* quotient, long long* remainder) {
	int sign_q = ((a < 0) ^ (b < 0)) ? -1 : 1;
	int sign_r = (a < 0) ? -1 : 1;

	// 절대값으로 처리
	unsigned long long ua = (a < 0) ? -a : a;
	unsigned long long ub = (b < 0) ? -b : b;

	*quotient = 0;
	*remainder = 0;

  for (int i = 63; i >= 0; i--) {
      if ((ua >> i) >= ub) {
          ua = subtract(ua, ub << i);
          *quotient = add(*quotient, 1LL << i);
      }
  }

	*remainder = ua;

	*quotient *= sign_q;
	*remainder *= sign_r;
}
```

**int sign_q = ((a < 0) ^ (b < 0)) ? -1 : 1;
int sign_r = (a < 0) ? -1 : 1;**

몫의 부호는 두 연산자의 부호가 같을 땐 1, 다르면 -1

나머지값의 부호는 좌항의 부호를 따라간다.

***quotient = 0;
*remainder = 0;**

두 값을 반환할 수 없으므로 callByReference 방식으로 구현

**unsigned long long ua = (a < 0) ? -a : a;
unsigned long long ub = (b < 0) ? -b : b;**

***quotient *= sign_q;
*remainder *= sign_r;**

절댓값으로 처리 후, 계산 결과에 부호를 붙이는 방식

**속도 최적화**

```c

	while (ua >= ub) {
		ua = subtract(ua, ub);
		(*quotient)++;
	}
	
	*remainder = ua;
```

나눗셈은 뺄셈의 반복이라는 아이디어에서 처음엔 위와 같이 코드를 작성하였지만, 좌항과 우항의 크기 차이가 클 때, 계산이 오래 걸리는 것을 확인. 극단적으로 ua가 10억이고, ub가 1일 경우 10억 번의 연산을 수행해야 함. 

**변경된 로직**

```c
	long long temp_ub = ub;
	long long multiple = 1;

	// ub를 가능한 크게 만들어줌
	while ((temp_ub << 1) <= ua) {
		temp_ub <<= 1;
		multiple <<= 1;
	}

	while (ua >= ub) {
		if (ua >= temp_ub) {
			ua = subtract(ua, temp_ub);
			*quotient = add(*quotient, multiple);
		}
		temp_ub >>= 1; // 작아진 ua이하의 값이 될 때까지, ub를 계속 낮춰간다. 
		multiple >>= 1;
	}
	
	*remainder = ua;
```

100 / 3을 한다고 가정해보자. 100에서 3을 더 이상 뺄 수 없을 때 까지 반복적으로 빼면서, 횟수를 1씩 늘려가고 더 이상 뺄 수 없을 때의 횟수가 몫이 되는 방식이 기존 로직이었다. 

3을 늘려가며 ( << shift 방식 ) 100보다 작지만, 최대한 큰 3의 배수를 찾고, 그 수를 빼면서 배수를 몫으로 더해나가는 방식을 생각했다.

3 * (1 << 5) = 3 * (2^5) = 3 * 32 = 96

100에서 96을 뺀 뒤, 몫은 32, 나머지는 4

그리고 4를 다시 나눠지는 수로 두고, 위 로직을 반복하면 된다. (좌항이 우항보다 작아질 때까지)

 

### 전체코드

```c
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

void print_intro(); 
void remove_spaces(char*);  
int parse_input(const char*, char*, char*, char*); 
int is_valid_integer(const char*);
void clear_input_buffer();

long long add(long long, long long);
long long subtract(long long, long long);
long long multiply(long long, long long);
void divide(long long, long long, long long*, long long*);

int main() {

	print_intro();

	char input[30]; 
	char num1_str[12], num2_str[12]; // 음수일 수도 있음 
	char operator;
	long long num1, num2;
	long long result, quotient, remainder;

	while (1) {
		printf("\nEnter the express. If you want to quit, press 'q'.\n: ");
		
		/* gets_s 사용 시 버퍼 길이 보다 초과된 입력에 대해 직접 처리할 수 없어서 fgets를 대신 사용.
		if (gets_s(input, sizeof(input)) == NULL) {
			printf("Error reading input.\n");
			continue;
		}
		*/

		if (fgets(input, sizeof(input), stdin) == NULL) {
			printf("Error reading input.\n");
			continue;
		}

		if (input[strlen(input) - 1] != '\n') {
			clear_input_buffer();
			printf("Input is too long. Please enter a shorter expression considering input Limit.\n");
			continue;
		}
		else {
			input[strlen(input) - 1] = '\0';
		}

		if (strcmp(input, "q") == 0) {
			printf("You terminated calculator.\n");
			break;
		}

		remove_spaces(input);

		if (!parse_input(input, num1_str, &operator, num2_str)) {
			printf("Check Your Input.\n"); // 연산자가 없거나, 숫자가 10자리를 초과.
			continue;
		}

		if (!is_valid_integer(num1_str) || !is_valid_integer(num2_str)) {
			printf("Check Your Input.\n"); // 숫자가 없거나, int 범위 내의 숫자가 아님. 
			continue;
		}

		num1 = atoll(num1_str);
		num2 = atoll(num2_str);

		switch (operator) {

			case '+':
				result = add(num1, num2);
				printf("Result: %lld\n", result);
				break;
			case '-':
				result = subtract(num1, num2);
				printf("Result: %lld\n", result);
				break;
			case '*':
				result = multiply(num1, num2);
				printf("Result: %lld\n", result);
				break;
			case '/':
				if (num2 == 0) {
					printf("AnyNumber can not be divided by 0.\n");
					continue;
				}
				divide(num1, num2, &quotient, &remainder);
				printf("Quotient: %lld, Remainder: %lld\n", quotient, remainder);
				break;
			default:
				printf("It is not operator.\n");
				break;
		} 

	}

	return 0;
}

void print_intro() {
	printf("This calculator only accepts integer numbers between -2,147,483,648 ~ 2,147,483,647 as input.\n");
	printf("Please follow the input format below.\n");
	printf("Format: <Integer><Operation><Integer>\nExample: 4+3\n");
}

void remove_spaces(char* str) {
	char* i = str;
	char* j = str;
	while (*j != 0) {
		*i = *j++;
		if (*i != ' ') {
			i++;
		}
	}
	*i = 0;
}

// 입력 문자열에서 숫자와 연산자를 분리하는 함수
int parse_input(const char* input, char* num1_str, char* operator, char* num2_str) {
	const char* operators = "+-*/";
	const char* op_pos;
	// 음수 처리: 첫 번째 문자가 '-'일 경우 무시
	if (input[0] == '-') {
		op_pos = strpbrk(input + 1, operators);
	}
	else {
		op_pos = strpbrk(input, operators);
	}

	if (op_pos == NULL) {
		return 0; // 연산자가 없음
	}

	*operator = *op_pos; 

	if (op_pos - input > 11) {
		return 0;
	}

	// sizeof(num1_str)은 오류 발생. 함수의 매개변수로 들어간 배열은 포인터로 인식되고, 포인터의 크기인 8바이트로 인식된다.
	strncpy_s(num1_str, 12, input, op_pos - input);
	num1_str[op_pos - input] = '\0';

	// 연산자 뒤의 숫자 복사
	
	strcpy_s(num2_str, 12, op_pos + 1);

	return 1;
}

// 문자열이 유효한 정수인지 확인하는 함수
int is_valid_integer(const char* str) {
	char* endptr;
	long num = strtol(str, &endptr, 10);

	if (*endptr != '\0' || endptr == str) {
		return 0; // 변환할 수 없는 문자가 있거나 변환할 수 있는 문자가 없음
	}

	if (num < INT_MIN || num > INT_MAX) {
		return 0; // 숫자가 int 범위를 벗어남
	}

	return 1; // 유효한 정수
}

// 입력 버퍼를 비우는 함수
void clear_input_buffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {}
}

long long add(long long a, long long b) {
	while (b != 0) {
		long long carry = a & b;
		a = a ^ b;
		b = carry << 1;
	}
	return a;
}
long long subtract(long long a, long long b) {
	b = ~b + 1;
	return add(a, b);
}
long long multiply(long long a, long long b) {
	long long result = 0;
	while (b != 0) {
		if (b & 1) {
			result += a;
		}
		a <<= 1;
		b >>= 1;
	}
	return result;
}

void divide(long long a, long long b, long long* quotient, long long* remainder) {
	int sign_q = ((a < 0) ^ (b < 0)) ? -1 : 1;
	int sign_r = (a < 0) ? -1 : 1;

	// 절대값으로 처리
	unsigned long long ua = (a < 0) ? -a : a;
	unsigned long long ub = (b < 0) ? -b : b;

	*quotient = 0;
	*remainder = 0;

	/* 너무 느린 방식
	while (ua >= ub) {
		ua = subtract(ua, ub);
		(*quotient)++;
	}
	*/

	long long temp_ub = ub;
	long long multiple = 1;

	// ub를 가능한 크게 만들어줌
	while ((temp_ub << 1) <= ua) {
		temp_ub <<= 1;
		multiple <<= 1;
	}

	while (ua >= ub) {
		if (ua >= temp_ub) {
			ua = subtract(ua, temp_ub);
			*quotient = add(*quotient, multiple);
		}
		temp_ub >>= 1;
		multiple >>= 1;
	}

	*remainder = ua;

	*quotient *= sign_q;
	*remainder *= sign_r;
}
```

