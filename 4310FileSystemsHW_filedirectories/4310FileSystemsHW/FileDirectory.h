#pragma once
class FileDirectory
{
public:
	const int sizeoffilename = 11;
	int N = 4;
	FileDirectory();
	bool create(char filename[], unsigned int numberBytes);
	bool deleter(char filename[]);
	int read(char filename[], char data1[]);
	bool write(bool canifuckingwriteornot, char filename[], unsigned int numberBytes, unsigned char data1[], unsigned char date[], unsigned char time[]);
	bool copy(bool canwecopy, char filename[], char filename1[], unsigned char date[], unsigned char time[]);
	bool merge( char filename1[], char filename2[], char filename[]);
	void printclusters(char filename[]);
	void printDirectory();
	void printData(char filename[]);
	~FileDirectory();
private:
	unsigned char(*filedirectory)[32] = new unsigned char [N][32];

		/*
		file name[7:0] : 8 - byte
		file name extension[8:10] : 3 - byte
		date[25:24] and 
		time[23:22] of file creating or modification
		16 - bit first sector address[27:26] // upper byte is 26, lower byte is 27
		32 - bit file size[31:28], for a file size up to 4 gbytes
		*/

	unsigned short int FAT16[256];

	unsigned char data[1024];

	
};

