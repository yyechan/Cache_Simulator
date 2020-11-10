#include <fstream>
using namespace std;

typedef unsigned int uint;

typedef struct {
	int tag;
	int offset;
}AddrInfo;

void one_level_sim(ifstream&);
void two_level_sim(ifstream&);

