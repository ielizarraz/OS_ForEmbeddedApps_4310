#include <iostream>
//THIS PROJECT SHOULDVE BEEN IMPLEMENTED WITH JAVA OR C#, which i learned afterwards, dynamic arrays are not friendly in C++
using namespace std;
unsigned const int N = 4;
unsigned char Data[2048][N + 1];


void byte_striping(unsigned char fileData[], unsigned char diskData[][N + 1], int n);
void block_striping(unsigned char fileData[], unsigned char diskData[][N + 1], int n);
bool byte_Destriping(unsigned char fileData[], unsigned char diskData[][N + 1], int n);
bool block_Destriping(unsigned char fileData[], unsigned char diskData[][N + 1], int n);
int main()
{
	// create our data arrays that we will be inputting
	unsigned char firstdata[] = "hmm";

	unsigned char seconddate[] = "dang";

	cout << "Striping data" << endl;



	byte_striping(firstdata, Data, sizeof(firstdata));
	if (byte_Destriping(firstdata, Data, sizeof(firstdata))) 
	{
		cout << "Read enabled \n";
		for (int i = 0; i < sizeof(firstdata); i++)
		{
			
			cout << firstdata[i];
		}
		cout << endl;
	}
	else
	{
		cout << "read failed!\n";
	}

	block_striping(seconddate, Data, sizeof(seconddate));
	if (block_Destriping(seconddate, Data, sizeof(seconddate)))
	{
		cout << "Read enabled \n";
		for (int i = 0; i < sizeof(seconddate); i++)
		{
			
			cout << seconddate[i];
		}
	}


}
bool byte_Destriping(unsigned char fileData[], unsigned char diskData[][N + 1], int n) {
	
	
	unsigned int parity = 0;
	// read data from disk per byte
	for (int i = 0; i < n; i++) 
	{ 
		fileData[i] = diskData[i / N][i % N];
	
	}
		
	// check for match with parity bit


	for (int row = 0; row < n / N; row++) {
		parity = 0;
		for (int disk = 0; disk < N; disk++) {
			parity = (parity ^ diskData[row][disk]);
		}
		// return false of the parity of any return false
		if (parity != diskData[row][N])
		{
				return false;

		}
			
	}
	//return true if the parities are all good
	return true;
}

bool block_Destriping(unsigned char fileData[], unsigned char diskData[][N + 1], int n) {
	unsigned int parity = 0;
	// read from disk data for every 512 bytes
	for (int i = 0; i < n; i++)
	{

		fileData[i] = diskData[i % 512 + i / (512 * N) * 512][(i / 512) % N];

	}
		
	// check for match with parity bit


	for (int row = 0; row < n / N; row++) {
		parity = 0;
		for (int disk = 0; disk < N; disk++) 
		{
			parity = (parity ^ diskData[row][disk]);
		}
		// return false if parities do not match
		if (parity != diskData[row][N])
		{
			return false;
		}
			
	}
	//return true if all parities match
	return true;
}

void byte_striping(unsigned char fileData[], unsigned char diskData[][N + 1], int n) {
	//to implement Level ? RAID with N data disks and dedicated parity disk. 
	//input: n-byte file data in the array dataFile[] 
	//output: diskData[][] with striped data in Disk 0 to Disk (N-1)and parity in Disk N.  

	for (int i = 0; i < n; i++)
		diskData[i / n][i % n] = fileData[i];	//byte-level  
	//calculate byte parity and store in disk N. 
	for (int row = 0; row < N / n; row++) {
		diskData[row][n] = 0;
		for (int disk = 0; disk < n; disk++) diskData[row][n] = diskData[row][n] ^ diskData[row][disk];
	}
}

void block_striping(unsigned char fileData[], unsigned char diskData[][N + 1], int n)
{
	//to implement Level 4 RAID with N data disks and dedicated parity disk
	//input: n-byte file data in the array dataFile[]
	//output: diskData with striped block data in Disk 0 to (N-1)and parity
	//in Disk N.
	for (int i = 0; i < n; i++) diskData[i % 512 + i / (512 * N) * 512][
		(i / 512) % N] = fileData[i];
		//calculate block parity (same as byte parity) and store in disk N.
		for (int row = 0; row < n / N; row++)
		{
			diskData[row][N] = 0;
			for (int disk = 0; disk < N; disk++) diskData[row][N] =
				diskData[row][N] ^ diskData[row][disk];
		}
}
