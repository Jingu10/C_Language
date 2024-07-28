#include <stdio.h>
#include <ctype.h> // isdigit, isspace
#include <string.h>

#define MAX_DIGITS 1000

void clearBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

// ���� ���ڿ��� ��ȿ�� ����
int isValidNumber(char str[]) {
	int len = strlen(str);

	if (str[0] != '-' && !isdigit(str[0])) return 0; // '-'�� �ƴϸ鼭 ('-'�� ���) ������ �ƴѰ�� ��ȿ���� �ʴ�. ��, '-' �Ǵ� ������(0~9)�� ��ȿ�ϴ�.

	for (int i = 1; i < len; i++) {
		if (!isdigit(str[i])) {
			return 0; // ���ڳ� ���� ��ȣ�� �ƴ� ���ڰ� ���Ե� ���
		}
	}
	return 1;
}

// ���ڸ� �Է¹޴� �Լ�
void getNum(char num[]) {
	while (1) {
		// sizeof(num)�� ������ ũ��� �νĵż� 8����Ʈ�� ������ ������ ���� ���� ������ش�. 
		if (scanf_s("%s", num, MAX_DIGITS + 2) != 1) {
			clearBuffer(); // ���ر� ���̺��� �� �� �Է��� ������ ���, ������ �Է¿� ��ġ�� ������ ������, ���۸� ����ش�.
			printf("�߸��� �Է��Դϴ�. �ٽ� �Է����ּ���.\n: ");
			continue;
		}
		if (isValidNumber(num)) break;
		else printf("�߸��� �Է��Դϴ�. �ٽ� �Է����ּ���.\n: ");
	}
}

// �� ���ڿ��� ���밪 ��
int absCompare(const char* num1, const char* num2) {
	if (num1[0] == '-') num1++;
	if (num2[0] == '-') num2++;

	int len1 = strlen(num1);
	int len2 = strlen(num2);

	if (len1 > len2) return 1;
	if (len1 < len2) return -1;

	return strcmp(num1, num2);
}

// ���ڿ� ���� �Լ�
void add(char num1[], char num2[], char result[]);

// ���ڿ� ���� �Լ�
void subtract(char num1[], char num2[], char result[]);

void add(char num1[], char num2[], char result[]) {
	// 1) ��� ���ϱ� ���: �׳� �ϸ� ��
	// 2) ���� + ����: ���񰪳��� ���ϰ�, �������� ���� ��ȣ�� ���̸� �ȴ�.
	// 3) ���� + ���: -2 + 5 or -5 + 2 // ��ȣ�� �����ϰ�, ���밪�� ū ������ ���밪�� ���� ���� �� �ڿ�, ���밪�� ū ���� ��ȣ�� �����Ѵ�. (strncmp�� ��)
	// 4) ��� + ����: 5 + -2 or 2 + -5 // 3�� ��������.

	int len1 = strlen(num1);
	int len2 = strlen(num2);
	int isNegative = 0; // ����� �������� ������� 

	char temp[MAX_DIGITS + 2] = { 0 }; // ���� ����� �������� ����. 

	if (num1[0] == '-' && num2[0] == '-') {
		// ���� + ����: ���񰪳��� ���� �� ����� ���� ��ȣ�� ����
		isNegative = 1;
	}
	else if (num1[0] == '-') {
		// ���� + ���: ��ȣ�� �����ϰ� ���밪�� ū ������ ���밪�� ���� ���� �� �� ���밪�� ū ���� ��ȣ�� ����
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
		// ��� + ����: ��ȣ�� �����ϰ� ���밪�� ū ������ ���밪�� ���� ���� �� �� ���밪�� ū ���� ��ȣ�� ����
		subtract(num1, num2 + 1, result); // num2�� ���밪�� ����
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
		temp[idx++] = (sum % 10) + '0'; // ���ڷ� ����
		carry = (sum) / 10;


		i--;
		j--;
		
	}
	if (num1[0] == '-' && num2[0] == '-') {
		temp[idx - 1] = '\0'; // �� while������ �ǹ̾��� 0 ���� ��������, ����
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

// ���� ������ ���밪�� ū �ʿ��� ���밪�� ���� ���� ���� ����
void subtract(char num1[], char num2[], char result[]) {
	// 1) ��� ���� ���: ���밪�� ū ������ ���밪�� ���� ���� �� �ڿ�, ������ �� Ŭ���� - ��ȣ�� ���δ�. (ũ�� �񱳴� strncmp)
	// 2) ���� - ����: (-2) - (-5) = 3 or (-5) - (-2) // ���밪�� ū ������ ���밪�� ���� ���� �� �ڿ�, ������ ���밪�� �� Ŭ ����, - ��ȣ�� ���δ�.
	// 3) ���� - ���: ���밪���� ���� ���� ��, - ��ȣ�� ���δ�.
	// 4) ��� - ����: ���밪���� ���� ���� ����.
	int len1 = strlen(num1);
	int len2 = strlen(num2);
	int isNegative = 0; // ����� ������ �ٴ���, �Ⱥٴ���

	if (num1[0] == '-' && num2[0] == '-') {
		// ���� - ����: ���밪�� ū ������ ���� ���� �� �� ��ȣ�� ����

		if (absCompare(num1, num2) < 0) {
			// Swap num1 and num2
			const char* temp = num1;
			num1 = num2;
			num2 = temp;
			isNegative = 0; // ū ���� ��ȣ�� ���󰡱� ������, ���� - ������ ��������� ����� �ȴ�.
		}
		else {
			isNegative = 1;
		}
	}
	else if (num1[0] == '-') {
		// ���� - ���: ���񰪳��� ���� �� ����� ���� ��ȣ�� ����
		result[0] = '-';
		add(num1 + 1, num2, result+1); // num1�� ���밪�� ���ϱ�
		return;
	}
	else if (num2[0] == '-') {
		// ��� - ����: ���񰪳��� ����
		add(num1, num2 + 1, result); // num2�� ���밪�� ���ϱ�
		return;
	}
	else {
		// ��� - ���: ���밪�� ū ������ ���밪�� ���� ���� �� �� ��ȣ�� ����
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

	char temp[MAX_DIGITS + 2] = { 0 }; // ���� ����� �������� ����

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

		diff = digit1 - digit2 - borrow;
		if (diff < 0) {
			diff += 10;
			borrow = 1;
		}
		else {
			borrow = 0;
		}

		temp[idx++] = diff + '0'; // ���ڷ� ����

		i--;
		j--;
	}

	if (num1[0] == '-' && num2[0] == '-') {
		temp[idx - 1] = '\0'; // �� while������ �ǹ̾��� 0 ���� ��������, ����
	}

	idx = 0;
	if (isNegative) {
		result[idx++] = '-';
	}

	i = strlen(temp) - 1;
	while (i >= 0 && temp[i] == '0') {
		i--; // ex) temp�� 7000 �� ����� ���, 0�� �ƴ� ���� ó�� ���� ������ 0�� ���ش�.
	}
	if (i == -1) result[idx++] = '0'; // ��� ���� 0�� ���� ���, 0�� ���ܾ� �Ѵ�.
	for (i; i >= 0; i--) {
		result[idx++] = temp[i]; // ex) 1237000 �̾��� ���, 7321 ������ result�� ����. 
	}

	result[idx] = '\0';


}


int main(void) {

	char num1[MAX_DIGITS + 2];
	char num2[MAX_DIGITS + 2]; // ���� ���� �ִ� õ �ڸ��� ���� �Է� ���� ('-' ��ȣ + õ�ڸ����� + '\0')
	char result[MAX_DIGITS + 3] = { 0 };


	while (1) {
		printf("����: [1], ����: [2], ����: [0]\n:");
		int op;
		scanf_s("%d", &op);


		switch (op) {
		case 1:
			printf("ù��° ���� �Է��ϼ���: ");
			getNum(num1);
			printf("�ι�° ���� �Է��ϼ���: ");
			getNum(num2);
			add(num1, num2, result);
			printf("���� ���: %s\n\n", result);
			break;
		case 2:
			printf("ù��° ���� �Է��ϼ���: ");
			getNum(num1);
			printf("�ι�° ���� �Է��ϼ���: ");
			getNum(num2);
			subtract(num1, num2, result);
			printf("���� ���: %s\n\n", result);
			break;
		case 0:
			printf("���α׷��� �����մϴ�.\n");
			return 0;
			break;
		default:
			printf("�߸��� �Է��Դϴ�.\n\n");
		}

		clearBuffer(); 
	}

	return 0;
}