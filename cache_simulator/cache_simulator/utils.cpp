#include "utils.h"

unsigned int getL1_Offset(unsigned int num) { // L1캐시의 경우 block의 크기를 L2의 반으로 줄임

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