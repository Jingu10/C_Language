#include <stdio.h>
#include <string.h>
#include <limits.h>

void printIntro();
void remove_spaces(char*);
int parse_input(const char*, char*, char*, char*);
int is_valid_integer(const char*);


int main() {

	printIntro();

	char input[30];
	char num1_str[11], num2_str[11];
	char operator;
	int num1, num2;

	while (1) {
		printf("\nEnter the express. If you want to quit, press 'q'.\n: ");
		
		if (gets_s(input, sizeof(input)) == NULL) {
			printf("Error reading input.\n");
			continue;
		}

		if (strcmp(input, "q") == 0) {
			printf("You terminated calculator.\n");
			break;
		}

		remove_spaces(input);


		if (!parse_input(input, num1_str, &operator, num2_str)) {
			printf("Check Your Input.\n");
			continue;
		}

		if (!is_valid_integer(num1_str) || !is_valid_integer(num2_str)) {
			printf("Numbers must be integers within the range of -2,147,483,648 and 2,147,483,647.\n");
			continue;
		}

		num1 = atoi(num1_str);
		num2 = atoi(num2_str);

		printf("You entered: %d %c %d\n", num1, operator, num2);
		
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
	const char* op_pos = strpbrk(input, operators);

	if (op_pos == NULL) {
		return 0; // 연산자가 없음
	}

	*operator = *op_pos; 

	if (op_pos - input > 10) {
		return 0;
	}

	strncpy_s(num1_str, sizeof(num1_str), input, op_pos - input);
	num1_str[op_pos - input] = '\0';

	// 연산자 뒤의 숫자 복사
	strcpy_s(num2_str, sizeof(num2_str), op_pos + 1);

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