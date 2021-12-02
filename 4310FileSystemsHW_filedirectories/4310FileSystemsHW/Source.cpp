#include <iostream>
#include "FileDirectory.h"
//SHOULD BE C#
using namespace std;
int main()
{
	
	FileDirectory myfiledirectory;
	const int FILENAMEMAX = 11;
	char firstfile[11] = { 'f', 'i', 'l','e','1','.','c','p','p',' ', ' ' };
	char secondfile[11] = { 'f', 'i', 'l','e','2','.','c','p','p',' ', ' ' };
	char thirdfile[11] = { 'f', 'i', 'l','e','3','.','a','s','m',' ', ' ' };
	char fourthfile[11] = { 'f', 'i', 'l','e','4','.','o','b','j',' ', ' ' };


	
	unsigned char data_file1[200];
	for (int i = 0; i < 200; i++)
	{
		data_file1[i] = 0xFF;
	}
	unsigned char data_file3[400];
	for (int i = 0; i < 400; i++)
	{
		data_file3[i] = 0xFF;
	}
	// goes in order of upper byte then lower byte
	unsigned char datefile1[2] = { 0x23, 0x33 };
	unsigned char timefile1[2] = { 0x6E, 0xFD };

	unsigned char datefile2[2] = { 0x24, 0x35 };
	unsigned char timefile2[2] = { 0x6F, 0xFE };


	
	myfiledirectory.write(myfiledirectory.create(firstfile, 200), firstfile, 200, data_file1, datefile1, timefile1);
	myfiledirectory.printDirectory();
	myfiledirectory.printclusters(firstfile);
	myfiledirectory.copy(myfiledirectory.create(firstfile, 200), firstfile, secondfile, datefile2, timefile2);
	cout << endl;
	myfiledirectory.printDirectory();
	myfiledirectory.printclusters(secondfile);


	myfiledirectory.write(myfiledirectory.create(thirdfile, 400), thirdfile, 400, data_file3, datefile1, timefile2);

	myfiledirectory.printDirectory();
	myfiledirectory.printclusters(thirdfile);
	cout << endl << "Attempting to merge files 1 and 3\n" << endl;
	bool diditmerge = myfiledirectory.merge(firstfile, thirdfile, fourthfile);
	
	myfiledirectory.printDirectory();
	myfiledirectory.printclusters(fourthfile);
	
	bool diditdelete = myfiledirectory.deleter(secondfile); // of course it did i spent 20 hours on this project whattajokester

	myfiledirectory.printDirectory();
	cout << endl << "Attempting to merge files 1 and 3\n" << endl;
	myfiledirectory.merge(firstfile, thirdfile, fourthfile);


}
