/* ��ǻ�ͱ��� 2015111730 ������ */
#include <iostream>
#include <vector>
#include <stdio.h>
#include <fstream>

using namespace std;

int main() {

	string filename;
	cout << "�����̸� �Է� : ";
	cin >> filename;

	ifstream ifs;
	ifs.open(filename);

	if (!ifs.is_open()) {

		cout << "�߸��� �����̸�" << endl;
		return 0;
	}


}