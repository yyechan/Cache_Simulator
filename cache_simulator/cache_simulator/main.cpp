/* 컴퓨터구조 2015111730 조예찬 */
#include <iostream>
#include <vector>
#include <stdio.h>
#include <fstream>

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

		
}