#include <iostream>
#include <vector>
#include <fstream>
#include "simulator.h"
#include "utils.h"

using namespace std;

vector<unsigned int> L1_cache(512); // L1캐시의 경우 Line의 개수를 L2의 반으로 줄임 
vector<unsigned int> L2_cache(1024);

//read cycle 
int L1_read_hit_cycle = 5;
int L2_read_hit_cycle = 20;
int read_miss_cycle = 125;
//write cycle
int L1_write_hit_cycle = 105;
int L2_write_hit_cycle = 125;
int write_miss_cycle = 200;

void one_level_sim(ifstream& file) { // 1-level 캐시의 경우 L2 캐시 하나만으로 동작

	int L2_read_hit = 0;
	int	L2_write_hit = 0;
	int	read_num = 0;
	int	write_num = 0;
	int	L2_currentWriteIndex = 0; // 최근에 캐시에 데이터를 작성한 인덱스, FIFO 구현에 필요
	char instruction[13];
	char readwrite;
	bool ishit;
	uint address;
	AddrInfo curInfo;

	while (!file.eof()) {

		file.getline(instruction, 13);

		readwrite = instruction[0]; // load 인지 store인지 
		address = strtoul(instruction + 2, NULL, 16);
		curInfo.tag = getL2_Tag(address);
		curInfo.offset = getL2_Offset(address);

		ishit = false;

		for (int i = 0; i < L2_cache.size(); i++) {
			if (L2_cache[i] == curInfo.tag) {
				ishit = true;
				break;
			}
		}

		if (ishit) {   // l 은 load s 는 store  

			if (readwrite == 'l') { // 읽기
				L2_read_hit++;
				read_num++;
			}
			else {  // 쓰기 write through 방식 가정
				L2_write_hit++;
				write_num++;
			}
		}
		else { // write through

			if (readwrite == 'l') { // 읽기
				read_num++;
			}
			else {  // 쓰기 write through 방식 가정
				write_num++;
			}

			L2_cache[L2_currentWriteIndex % L2_cache.size()] = curInfo.tag;
			L2_currentWriteIndex++;
		}
	}
	double read_miss_rate = (1 - (double)L2_read_hit / read_num);
	double write_miss_rate = (1 - (double)L2_write_hit / write_num);

	cout << "One cache read miss rate : " << read_miss_rate * 100 << endl;
	cout << "One cache write miss rate : " << write_miss_rate * 100 << endl;
	cout << "Average read cycle : " << L2_read_hit_cycle + read_miss_rate * read_miss_cycle << endl;
	cout << "Average write cycle : " << L2_write_hit_cycle + write_miss_rate * write_miss_cycle << endl;
	cout << "Read hit rate : " << (1 - read_miss_rate) * 100 << endl;
	cout << "Write hit rate : " << (1 - write_miss_rate) * 100 << endl;

	return;

}


void two_level_sim(ifstream& file) {

	int L1_read_hit = 0;
	int	L1_write_hit = 0;

	int	L2_read_hit = 0;
	int	L2_write_hit = 0;

	int	read_num = 0;
	int	write_num = 0;

	char instruction[13];
	char readwrite;
	int	L1_currentWriteIndex = 0;
	int	L2_currentWriteIndex = 0;

	uint address;
	AddrInfo L1_curInfo;
	AddrInfo L2_curInfo;

	for (auto& iter : L2_cache) {
		iter = 0;
	}

	while (!file.eof()) {

		file.getline(instruction, 13);

		readwrite = instruction[0];
		address = strtoul(instruction + 2, NULL, 16);

		L1_curInfo.tag = getL1_Tag(address);
		L1_curInfo.offset = getL1_Offset(address);

		L2_curInfo.tag = getL2_Tag(address);
		L2_curInfo.offset = getL2_Offset(address);

		bool isL1_hit = false;
		bool isL2_hit = false;

		for (int i = 0; i < L1_cache.size(); i++) { // L1에서 먼저 찾기 

			if (L1_cache[i] == L1_curInfo.tag) {
				isL1_hit = true;
				break;
			}

		}

		if (isL1_hit) {

			if (readwrite == 'l') { // 읽기
				L1_read_hit++;
				read_num++;
			}
			else {  // 쓰기
				L1_write_hit++;
				write_num++;
			}

		}
		else {  // L1에서 miss가 났으면 그 다음 L2 에서 찾기 

			for (int i = 0; i < L2_cache.size(); i++) {
				if (L2_cache[i] == L2_curInfo.tag) {
					isL2_hit = true;
					break;
				}
			}

			if (isL2_hit) {
				if (readwrite == 'l') { // 읽기
					L2_read_hit++;
					read_num++;
				}
				else {  // 쓰기
					L2_write_hit++;
					write_num++;
				}

				// L1에 해당 정보 업데이트 
				L1_cache[L1_currentWriteIndex % L1_cache.size()] = L1_curInfo.tag;
				L1_currentWriteIndex++;
			}
			else {

				if (readwrite == 'l') { // 읽기
					read_num++;
				}
				else {  // 쓰기
					write_num++;
				}

				//L1,L2에 해당 정보 업데이트 

				L1_cache[L1_currentWriteIndex % L1_cache.size()] = L1_curInfo.tag;
				L1_currentWriteIndex++;

				L2_cache[L2_currentWriteIndex % L2_cache.size()] = L2_curInfo.tag;
				L2_currentWriteIndex++;
			}
		}
	}
	cout << "\n\n\n";

	double L1_read_miss_rate = (1 - (double)L1_read_hit / read_num);
	double L1_write_miss_rate = (1 - (double)L1_write_hit / write_num);

	double L2_read_miss_rate = (1 - L2_read_hit / ((double)read_num - L1_read_hit));
	double L2_write_miss_rate = (1 - L2_write_hit / ((double)write_num - L1_write_hit));

	cout << "L1 cache read miss rate : " << L1_read_miss_rate * 100 << endl;
	cout << "L1 cache write miss rate : " << L1_write_miss_rate * 100 << endl;
	cout << "\n";
	cout << "L2 cache read miss rate : " << L2_read_miss_rate * 100 << endl;
	cout << "L2 cache write miss rate : " << L2_write_miss_rate * 100 << endl;

	cout << "average read cycle : " << L1_read_hit_cycle + L1_read_miss_rate * (L2_read_hit_cycle + L2_read_miss_rate * read_miss_cycle) << endl;
	cout << "average write cycle : " << L1_write_hit_cycle + L1_write_miss_rate * (L2_write_hit_cycle + L2_write_miss_rate * write_miss_cycle) << endl;
	cout << "average read hit rate : " << ((double)L1_read_hit + L2_read_hit) / read_num * 100 << endl;
	cout << "average write hit rate : " << ((double)L1_write_hit + L2_write_hit) / write_num * 100 << endl;

}