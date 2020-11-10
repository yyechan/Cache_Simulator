#include "utils.h"

unsigned int getL1_Offset(unsigned int num) { // L1ĳ���� ��� block�� ũ�⸦ L2�� ������ ����

	return num & 15;
}

unsigned int getL1_Tag(unsigned int num) {

	return num >> 4;
}

unsigned int getL2_Offset(unsigned int num) {

	return num & 31;
}

unsigned int getL2_Tag(unsigned int num) {

	return num >> 5;
}