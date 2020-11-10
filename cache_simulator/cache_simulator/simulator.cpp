#include <iostream>
#include <vector>
#include <stdio.h>
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

	int		L2_read_hit = 0;
	int		L2_write_hit = 0;
	int		read_num = 0;
	int		write_num = 0;
	int		L2_currentWriteIndex = 0; // 최근에 캐시에 데이터를 작성한 인덱스, FIFO 구현에 필요
	char	instruction[13];
	char	readwrite;
	uint	address;
	bool	ishit;

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