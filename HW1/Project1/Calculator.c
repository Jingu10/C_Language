#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

void printIntro(); 
void remove_spaces(char*);  
int parse_input(const char*, char*, char*, char*); 
int is_valid_integer(const char*);
void clear_input_buffer();

long long add(long long, long long);
long long subtract(long long, long long);
long long multiply(long long, long long);
void divide(long long, long long, long long*, long long*);


int main() {

	printIntro();

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
				printf("Result: %lld", result);
				break;
			case '-':
				result = subtract(num1, num2);
				printf("Result: %lld", result);
				break;
			case '*':
				result = multiply(num1, num2);
				printf("Result: %lld", result);
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

void printIntro() {
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
	long num = strtol(str, &endptr, 11);

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

	for (int i = 63; i >= 0; i--) {
		*remainder <<= 1;
		*remainder |= (ua >> i) & 1;
		if (*remainder >= ub) {
			*remainder = subtract(*remainder, ub);
			*quotient |= 1ULL << i;
		}
	}

	*quotient *= sign_q;
	*remainder *= sign_r;
}