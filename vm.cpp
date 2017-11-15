#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
using namespace std;

#define PAGE_TABLE_LINE_NUM 32
#define RAM_PAGE_NUM 4

ifstream fin;
ofstream fout;

class PageTableLine {

public:
	unsigned long address;
	int pageNumber;
	int present;
};

class RAMTableEntry {

public:
	unsigned long address;
	int use;
};

int lookup(unsigned long address, RAMTableEntry ram[]){
	for (int i = 0; i < RAM_PAGE_NUM; i++){
		if (ram[i].address == address){
			return i;
		}
	}

	return -1;
}

void printRAM(RAMTableEntry ram[]){
	for (int i = 0; i < RAM_PAGE_NUM; i++){
		if (ram[i].address != 0){
			if (i == 0){
				fout << hex << ram[i].address;
			} else {
				fout << " " <<  hex << ram[i].address;
			}
		}
	}
	fout << endl;
}

int main(int argc, char* args[]){

	fin.open(args[1]);
	fout.open("vm-out.txt");

	// store first 32 addresses
	PageTableLine pt[PAGE_TABLE_LINE_NUM];
	unsigned long address;
	for (int i = 0; i < PAGE_TABLE_LINE_NUM; i++){
		fin >> hex >> address;
		pt[i].address = address;
	}

	// do clock replacement algorithm
	RAMTableEntry ram[RAM_PAGE_NUM];
	int star = 0;
	while(fin >> hex >> address){
		address = (address >> 10) << 10;

		int index = lookup(address, ram);

		if (index == -1){ // not in RAM
			if (ram[star].use == 0){ // use bit 0, replace number there
				ram[star].address = address;
				ram[star].use = 1;
				star = (star+1) % RAM_PAGE_NUM;
			} else { // use bit not 0, turn other use bits to 0
				int temploc = star;
				while (1){
					ram[temploc].use = 0;
					temploc = (temploc+1) % RAM_PAGE_NUM;

					if (ram[temploc].use == 0){
						ram[temploc].address = address;
						ram[temploc].use = 1;
						break;
					}
				}
				star = (temploc+1) % RAM_PAGE_NUM;
			}

		} else { // in RAM
			ram[index].use = 1;
		}

		printRAM(ram);
	}

	fin.close();
	fout.close();
	return 0;
}



