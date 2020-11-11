/* 컴퓨터구조 2015111730 조예찬 */

#include <iostream>
#include <vector>
#include <fstream>
#include "simulator.h"

using namespace std;

int main() {

	string filename;
	cout << "파일이름 입력 : ";
	cin >> filename;

	ifstream ifs;
	ifs.open(filename);

	if (!ifs.is_open()) {
		cout << "잘못된 파일이름" << endl;
		return 0;
	}

	one_level_sim(ifs);  // 1-level cache 실행
	ifs.close();

	ifs.open(filename);
	two_level_sim(ifs); // 성능향상을 위한 2-level cache 실행 
	ifs.close();
}