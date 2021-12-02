#include "FileDirectory.h"
#include <iostream>

using namespace std;
 FileDirectory::FileDirectory()
{

	this->N = 4; // max file directories
	 for (int i = 0; i < N; i++) {
		 for (int j = 0; j < 32; j++)
		 {
			 filedirectory[i][j] = 0;
		 }
	 }
	
	for (int i = 0; i < 256; i++)
	{
		this->FAT16[i] = 0;
	}

	for (int i = 0; i < 1024; i++)
	{
		this->data[i] = 0;
	}

}
bool FileDirectory::create(char filename[], unsigned int numberBytes)
{
	unsigned int clusteramount = 0;
	bool fileavailableflag = false;
	bool clusterflagavailable = false;
	for (int i = 0; i < N; i++) {
		if ((filedirectory[i][0]) == 0)
		{
			fileavailableflag = true; // Set Available Directory flag for if statement to catch
			break;
		}
	}
	
	if (fileavailableflag == true) 
	{
		for (int i = 0; i < 256; i++)
		{
			if (FAT16[i] == 0 || FAT16[i] == 1)
			{
				clusteramount = 4 + clusteramount; // for comparing to bytes needed for new files
			}
			if (clusteramount >= numberBytes)
			{
				clusterflagavailable = true;
				break;
			}
		}
		 
	}
	if (fileavailableflag == true && clusterflagavailable == true)
	{
		

		fileavailableflag = false;
		clusterflagavailable = false;
		return true;
	}
	else
	{
	return false;
	}
}
bool FileDirectory::deleter(char filename[])

{
	// Purpose : to delete a file with the specified file name from the Directory and FAT
	unsigned int fatfinder = 0;
	unsigned int fatvaluesave = 0;
	bool badclusterfound = false;
	bool IsFileNamePresentFlag = false;
	unsigned int counter = 0; // count number of same characters, if length of filename then we have our file!

	for (int i = 0; i < N; i++) // search through file directory
	{
		for (int j = 0; j < sizeoffilename; j++) // length of filename array in bytes
		{
			if (filename[j] == filedirectory[i][j]) // if first char in each array ==
			{
				counter++;
				
			}
			if (sizeoffilename == counter) //THIS IS THE MAIN DELETE
				{
					filedirectory[i][0] = 0;

					fatfinder = filedirectory[i][26] << 8 | filedirectory[i][27];


					while ((FAT16[fatfinder]) != (0xFFF7 | 0xFFF8 | 0xFFF9 | 0xFFFA | 0xFFFB | 0xFFFC | 0xFFFd | 0xFFFE | 0xFFFF)) // needs fatfinder to be first cluster address of file in question, verified file in directory
					{
				
						
							fatvaluesave = FAT16[fatfinder]; // rewrite old clusters as usable (overwriteable)
							FAT16[fatfinder] = 1;
							fatfinder = fatvaluesave;
						
					}


					return true;

				}
			
				
				
		}
			counter = 0;
	}
	
	return false;
}


int FileDirectory::read(char filename[], char data1[])
{
	// Purpose : to retrieve the data from the data[] array from the file with specified name
	unsigned short int saveaddresses[256];
	for (int i = 0; i < 256; i++)
	{
		saveaddresses[i] = 0;
	}// 2 bytes per int
	unsigned int data_array_value = 0;
	unsigned int fatvaluesave = 0;
	unsigned int fatfinder = 0;
	unsigned int counter = 0; // count to see if the file name is in the file directory
	bool filereadflag = false;
	bool dataendflag = false;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < sizeoffilename; j++)
		{
			if (filename[j] == filedirectory[i][j])
			{
				counter++;
			}
			if (counter == sizeoffilename)
				{
					unsigned int address = 0;
					fatfinder = filedirectory[i][26] << 8 | filedirectory[i][27];
					while ((FAT16[fatfinder] & 0xFFFF) <= 0xFFF6) // needs fatfinder to be first cluster address of file in question, verified file in directory
					{
						// SHOULD ADD IF BAD CLUSTER CASE

						saveaddresses[address] = fatfinder; // save the addresses of current FAT cluster into array

						fatfinder = FAT16[fatfinder]; // get next fat cluster address
						address++;
					}
					// Use the addresses saved to read data from file and input into data1[]
					for (int k = 0; k < (sizeof(saveaddresses) / 4); k++) // only transverse byte amounts
					{
						for (int i = 0; i < 4; i++)
						{
							if (data[(saveaddresses[k] * 4) + i == 0])
							{
								dataendflag = true; // end of data in clusters has been found
								break;
							}
							data1[data_array_value] = data[(saveaddresses[k] * 4) + i];

							data_array_value++;
						}
						if (dataendflag == true) // check to see if we are leaving previous break statement for data reading
						{
							dataendflag = false;
							break;
						}
					}
					filereadflag = true;
				}
			else {
				counter = 0;
			}
		}
	}


	if (filereadflag == false)
	{
		// put what happens if the file is not present and must 
		return 0;
	}
	else
	{
		filereadflag = false;
		return data_array_value + 1; // returns the number of bytes read from filename[] and saved into data1[] // could be wrong so change the 1 to 0 
	}

}
bool FileDirectory::write(bool canifuckingwriteornot, char filename[], unsigned int numberBytes,unsigned char data1[], unsigned char date[], unsigned char time[])
{
	unsigned int datachars = 0;
	unsigned short int arr[256]; // array for storing the FAT16 new addresses
	for (int i = 0; i < 256; i++)
	{
		arr[i] = 0;
	}
	unsigned short int previousaddress = 0;
	unsigned short int usemeforaddresses = 0x0002;
	unsigned int fat16clusterstaken = 0;
	if (canifuckingwriteornot) // CAN WE WRITE OR NOT? dont start writing to stuff if we cant!
	{
		for (int i = 2; i < 256; i++)
		{

			if ((FAT16[i] == 0 || FAT16[i] == 1) && (numberBytes > (fat16clusterstaken*4))) // as long as #bytes greater than the cluster bytes we plan to use for data
			{
				arr[fat16clusterstaken] = usemeforaddresses; // array saving the new addresses
				fat16clusterstaken++;
				previousaddress = usemeforaddresses;
			}
			if (numberBytes <= (fat16clusterstaken * 4))
			{
				arr[fat16clusterstaken] = 0xFFFF;
				break;
			}
			usemeforaddresses = usemeforaddresses + 0x0001;
		}
	

		// For putting the values of our new FAT16 address data
		for ( int i = 0; i < 256; i++)
		{
		
			if (arr[i] == (0xFFF7 | 0xFFF8 | 0xFFF9 | 0xFFFA | 0xFFFB | 0xFFFC | 0xFFFd | 0xFFFE | 0xFFFF))
			{
				break;
			}
			FAT16[arr[i]] = arr[i + 1];
			for (int j = 0; j < 4; j++)
			{
				data[(arr[i]*4) + j] = data1[datachars];
				datachars++;
			}
			
		}

		// there was definitely an easier way to do this, inputing values into directory
		unsigned int datewriter = 0;
		unsigned int timewriter = 0;
		for (int i = 0; i < N; i++) {
			if ((filedirectory[i][0] & 0xFF) == 0)
			{
				for (int filenameWriter = 0; filenameWriter < 11; filenameWriter++)
				{
					filedirectory[i][filenameWriter] = filename[filenameWriter];
				}
				for (int DateandTimeWriter = 22; DateandTimeWriter < 26; DateandTimeWriter++)
				{
					if (DateandTimeWriter <= 23)
					{
						filedirectory[i][DateandTimeWriter] = time[timewriter];
						timewriter++;
					}
					else {
						filedirectory[i][DateandTimeWriter] = date[datewriter];
						datewriter++;
					}

				}
				filedirectory[i][26] = arr[0] >> 8; //upper byte of first sector address
				filedirectory[i][27] = (arr[0] & 0x00ff); // lower byte of first sector address

				filedirectory[i][28] = numberBytes >> 24; // highest byte 
				filedirectory[i][29] = numberBytes >> 16;
				filedirectory[i][30] = numberBytes >> 8;
				filedirectory[i][31] = numberBytes & 0xff;// lowest byte

				break;
			}
		}
	
	
	}

	else // cant write, return false
	{
		cout << "can't write bro" << endl;
		return false;
	}
}

// SINCE WE COPY THE NAME AS WELL, WE RELY ON THE TIME CREATED IN ORDER TO DIFFER WHAT FILE IS ORIGINAL, IDEALLY, unfortunately that isnt whats happening here, instead you just pick one
bool FileDirectory::copy(bool canwecopyornot, char filename[], char filename1[], unsigned char date[], unsigned char time[])
{
	// purpose: to copy our data from one file to another file, excluding addresses (create new addresses)
	bool arrayfinishedflag = false; // used to mark when we are done copying data from arrays
	unsigned short int copiedaddressamount = 0;
	unsigned short int numberofaddressesfromCopy = 0;
	unsigned short int arr[256]; // holds the cluster addresses of the file to be copied over
	unsigned short int arrforcopy[256]; // holds the cluster addresses of the new file copied
	unsigned short int usemeforaddresses = 2;
	for (int i = 0; i < 256; i++)
	{
		arr[i] = 0;
		arrforcopy[i] = 0;
	}
	unsigned int fileavailableNumber = 0;
	unsigned int counter = 0;
	if (canwecopyornot)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < sizeoffilename; j++)
			{
				if (filename[j] == filedirectory[i][j])
				{
					counter++;
					
				}
				
			}
			if (sizeoffilename == counter) //THIS IS IF WE FOUND THE FILE IN DIRECTORY
					{
						// Now we need to copy the contents of this file into the directory, as well as the data but not the addresses. We must also make new cluster data.
						// first we must find the open space in directory to copy old file into
						for (int k = 0; k < N; k++) {
							if ((filedirectory[k][0] & 0xFF) == 0)
							{
								fileavailableNumber = k; // Find avialable space in the file directory
								break;
							}
						}
						// First get the contents of filename in the directory 
						for (int filecopyname = 0; filecopyname < 11; filecopyname++)
						{

							filedirectory[fileavailableNumber][filecopyname] = filename1[filecopyname];
						}

						filedirectory[fileavailableNumber][22] = time[0];
						filedirectory[fileavailableNumber][23] = time[1]; // gets our NEW time of creation

						filedirectory[fileavailableNumber][24] = date[0]; // NEW date of creation
						filedirectory[fileavailableNumber][25] = date[1];

						filedirectory[fileavailableNumber][28] = filedirectory[i][28];
						filedirectory[fileavailableNumber][29] = filedirectory[i][29]; //saves the size of the file into the directory
						filedirectory[fileavailableNumber][30] = filedirectory[i][30];
						filedirectory[fileavailableNumber][31] = filedirectory[i][31];

						// now we must get our first sector address, find the addresses of clusters with the data, then save that data into new clusters 

						unsigned short int firstsectoraddress = filedirectory[i][26] << 8 | filedirectory[i][27];
						for (int h = 0; h < 256; h++)
						{
							if (firstsectoraddress == (0xFFF7 | 0xFFF8 | 0xFFF9 | 0xFFFA | 0xFFFB | 0xFFFC | 0xFFFd | 0xFFFE | 0xFFFF))
							{
								numberofaddressesfromCopy++;
								arr[h] = firstsectoraddress; // copy last address into last space of h
								break;
							}
							arr[h] = firstsectoraddress;
							numberofaddressesfromCopy++;
							firstsectoraddress = FAT16[firstsectoraddress];
							// copies the addresses we need to use to find data into array of short ints
						}


						for (int v = 2; v < 256; v++)
						{
							if ((FAT16[v] == 0 || FAT16[v] == 1) && copiedaddressamount <= numberofaddressesfromCopy) // as long as #bytes greater than the cluster bytes we plan to use for data
							{
								if (arr[copiedaddressamount] == (0xFFF7 | 0xFFF8 | 0xFFF9 | 0xFFFA | 0xFFFB | 0xFFFC | 0xFFFd | 0xFFFE | 0xFFFF))
								{
									arrforcopy[copiedaddressamount] = 0xFFFF;
									
									break;
								}

								arrforcopy[copiedaddressamount] = usemeforaddresses;
								// get the addresses for our new clusters of FAT16
								copiedaddressamount++;
							}
							if (copiedaddressamount > numberofaddressesfromCopy)
							{
								break;
							}
							usemeforaddresses++;
						}

						for (int k = 0; k < copiedaddressamount; k++)
						{
							if (arrforcopy[k] == (0xFFF7 | 0xFFF8 | 0xFFF9 | 0xFFFA | 0xFFFB | 0xFFFC | 0xFFFd | 0xFFFE | 0xFFFF))
							{
								break;
							}
							FAT16[arrforcopy[k]] = arrforcopy[k + 1];
						}
						// SAVES the first sector address into filedirectory chars 26 and 27
						filedirectory[fileavailableNumber][26] = arrforcopy[0] >> 8; // upper byte
						filedirectory[fileavailableNumber][27] = arrforcopy[0] & 0xff; // lower byte

						 // now we must use the addresses of the file we are copying from to get the data, and copy it into our arrforcopy addresses data spaces
						for (int p = 0; p < copiedaddressamount; p++)
						{
							for (int z = 0; z < 4; z++)
							{
								if ((arrforcopy[p] || arr[p]) == (0xFFF7 | 0xFFF8 | 0xFFF9 | 0xFFFA | 0xFFFB | 0xFFFC | 0xFFFd | 0xFFFE | 0xFFFF))
								{
									arrayfinishedflag = true;
									break;
								}
								data[(arrforcopy[p]*4) + z] = data[(arr[p]*4) + z];
							}
							if (arrayfinishedflag)
							{
								break;
							}
						}

						// to conclude, write the date, time, and file space #bytes (we can use addess amount * 4) because each address is a cluster of 4 bytes

						filedirectory[fileavailableNumber][22] = time[0];
						filedirectory[fileavailableNumber][23] = time[1];
						filedirectory[fileavailableNumber][24] = date[0];
						filedirectory[fileavailableNumber][25] = date[1];

						// copy the byte amount
						
						filedirectory[fileavailableNumber][28] = filedirectory[i][28];
						filedirectory[fileavailableNumber][29] = filedirectory[i][29];
						filedirectory[fileavailableNumber][30] = filedirectory[i][30];
						filedirectory[fileavailableNumber][31] = filedirectory[i][31];

						

					}
				else
				{
					counter = 0;
				}
		}
	}
}
bool FileDirectory::merge( char filename1[], char filename2[], char filename[])
{
	// Purpose : to copy data of files above into filename[] 
	unsigned char copied_data[1024];
	unsigned short int addressesfromfile1[256];
	unsigned short int addressesfromfile2[256];
	unsigned short int counter = 0;
	unsigned short int counter2 = 0;

	unsigned int getfirstfilebytesize = 0;
	unsigned int getsecondfilebytesize = 0;

	unsigned int markfirstfilewithsamename = 0;
	bool filesaresamename = false;
	unsigned short int firstaddressfile1 = 0;
	unsigned short int firstaddressfile2 = 0;
	// initialize
		for (int i = 0; i < 1024; i++)
		{
			if (i > 255)
			{
				data[i] = 0;
			}
			else {
				addressesfromfile1[i] = 0;
				addressesfromfile2[i] = 0;
				data[i] = 0;
			}
		}
		if (true) // check to see if our two merging files maybe have the same name, most of whats below is kinda useless 
		{
			for (int i = 0; i < sizeoffilename; i++)
			{
				if (filename1[i] == filename2[i])
				{
					counter++;
				}
			}
			if (counter == sizeoffilename)
			{
				filesaresamename == true;
				counter = 0;
			}
			else {
				counter = 0;
			}

		}
		for (int i = 0; i < N; i++) // check the first file
		{
			for (int j = 0; j < sizeoffilename; j++)
			{
				if (filename1[j] == filedirectory[i][j])
				{
					counter++;
				}

			}

			if (counter == sizeoffilename && filesaresamename) // not likely to happen but it can
			{
				counter = 0;

				markfirstfilewithsamename = i; // saves the space that we have our first file in.
				getfirstfilebytesize = filedirectory[i][28] << 24 | filedirectory[i][29] << 16 | filedirectory[i][30] << 8 | (filedirectory[i][31]);
				firstaddressfile1 = filedirectory[i][26] << 8 | filedirectory[i][27];
				break;
			}
			else if (counter == sizeoffilename && !filesaresamename) // typical case, get first files F.C. address and byte syze
			{
				counter = 0;
				markfirstfilewithsamename = 0;
				firstaddressfile1 = filedirectory[i][26] << 8 | filedirectory[i][27];
				getfirstfilebytesize = filedirectory[i][28] << 24 | filedirectory[i][29] << 16 | filedirectory[i][30] << 8 | (filedirectory[i][31]);
				break;
			}
			else {
				counter = 0;
			}
		}
		

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < sizeoffilename; j++)
			{
				if (filename2[j] == filedirectory[i][j])
				{
					counter++;
				}
			}
			if (counter == sizeoffilename && filesaresamename && i == markfirstfilewithsamename)
			{
				counter = 0;
			}
			else if (counter == sizeoffilename && filesaresamename && i != markfirstfilewithsamename) // get first cluster address and byte size if files are same name but not same directory entry
			{
				getsecondfilebytesize = filedirectory[i][28] << 24 | filedirectory[i][29] << 16 | filedirectory[i][30] << 8 | (filedirectory[i][31]);
				firstaddressfile2 = filedirectory[i][26] << 8 | filedirectory[i][27];
				counter = 0;
				break;
			}
			else if (counter == sizeoffilename && !filesaresamename) // if theyre not the same we get to do the samet thing
			{
				getsecondfilebytesize = filedirectory[i][28] << 24 | filedirectory[i][29] << 16 | filedirectory[i][30] << 8 | (filedirectory[i][31]);
				firstaddressfile2 = filedirectory[i][26] << 8 | filedirectory[i][27];
				counter = 0;
				break;
			}
			else {
				counter = 0;
			}
		}

		counter = 0;

		// store addresses , there seems to be some sort of memory leak in this area that I can't fix? Ive used the exact same syntax in every function, have to run program an extra 2 times before things are reset
		for (int i = 0; i < 256; i++)
		{
			if (firstaddressfile1 >= 0xFFF7)
			{
				addressesfromfile1[i] = firstaddressfile1;
				counter++;
				break;
			}
			addressesfromfile1[i] = firstaddressfile1;

			firstaddressfile1 = FAT16[firstaddressfile1];
			counter++;
		}
		// store addresses
		for (int i = 0; i < 256; i++)
		{
			if (firstaddressfile2 >= 0xFFF7)
			{
				addressesfromfile2[i] = firstaddressfile2;
				counter2++;
				break;
			}
			addressesfromfile2[i] = firstaddressfile2;

			firstaddressfile2 = FAT16[firstaddressfile2];
			counter2++;
		}

		//now we store all of the data from the addresses, as well as get byte amount used

		for (int i = 0; i < (counter + counter2); i++)
		{
			if (i < counter)
			{
				for (int j = 0; j < 4; j++)
				{
					copied_data[i] = data[addressesfromfile1[i] * 4 + j];
				}
			}
			else
			{
				for (int j = 0; j < 4; j++)
				{
					copied_data[i] = data[addressesfromfile2[i] * 4 + j];
				}
			}
		}

		// now we create a fake date that will be used for all merge files
		unsigned char date[] = { 0x83,0x03 };
		unsigned char time[] = { 0x03,0x83 };

		unsigned int newbytes = getfirstfilebytesize + getsecondfilebytesize;



		if (create(filename, newbytes))
		{
			cout << endl << "Merge successful" << endl;
			write(true, filename, newbytes, copied_data, date, time);
			return true;
		}
		else
		{
			cout << "We cannot merge these files, it exceeds storage" << endl;
			return false;
		}
}
void FileDirectory::printclusters(char filename[])
{
	unsigned int counter = 0;
	unsigned short int firstaddressfile1 = 0;
	unsigned int filewithsamenameamount = 0;
	unsigned int savefilenumber[4] = { 0 , 0 , 0 , 0 };
	for (int i = 0; i < N; i++) // search through file directory
	{
		for (int j = 0; j < sizeoffilename; j++) // length of filename array in bytes
		{
			if (filename[j] == filedirectory[i][j]) // if first char in each array ==
			{
				counter++;
			}
		}
		if (counter == sizeoffilename)
		{
			savefilenumber[filewithsamenameamount] = i;
			filewithsamenameamount++;
			counter = 0;
			
		}
		else
		{
			counter = 0;
		}
	}
		int a = 0;
	if (filewithsamenameamount) // if we find any hits on our file name it will sift through a shift case
	{
		switch (filewithsamenameamount) {
		case 0: 
			cout << "We have not found your file, sorry try again\n\n";
			break;

		case 1:
			cout << "We have found the file you request \n\n";
			firstaddressfile1 = filedirectory[savefilenumber[0]][26] << 8 | filedirectory[savefilenumber[0]][27];
			break;
		case 2:
			cout << "We have found multiple files with the same name, would you like to look at the (1) first (made first) or second (2)(made second, likely a copy)? \n\n\n\n";
			
			cin >> a;
			if (a == 1)
			{
				firstaddressfile1 = filedirectory[savefilenumber[a - 1]][26] << 8 | filedirectory[savefilenumber[a - 1]][27];
			}
			else if (a == 2)
			{
				firstaddressfile1 = filedirectory[savefilenumber[a-1]][26] << 8 | filedirectory[savefilenumber[a-1]][27];
			}
			else {
				firstaddressfile1 = filedirectory[savefilenumber[0]][26] << 8 | filedirectory[savefilenumber[0]][27];
			}
			break;
		case 3:
			cout << "We have found multiple files with the same name, would you like to look at the (1) first (made first) or second (2)(made second, likely a copy) or (3) why do you even have this many copies man? \n\n\n\n";
			
			cin >> a;
			if (a == 1)
			{
				firstaddressfile1 = filedirectory[savefilenumber[a - 1]][26] << 8 | filedirectory[savefilenumber[a - 1]][27];
			}
			else if (a == 2)
			{
				firstaddressfile1 = filedirectory[savefilenumber[a-1]][26] << 8 | filedirectory[savefilenumber[a-1]][27];
			}
			else if (a == 3) {
				firstaddressfile1 = filedirectory[savefilenumber[a-1]][26] << 8 | filedirectory[savefilenumber[a - 1]][27];
			}
			else {

			}
			break;
		case 4:
			cout << "We have found multiple files with the same name, would you like to look at the (1) first (made first) or second (2)(made second, likely a copy) or (3) why do you even have this many copies man or (4) i dont even know what to say? \n\n\n\n";
			
			cin >> a;
			if (a == 1)
			{
				firstaddressfile1 = filedirectory[savefilenumber[a - 1]][26] << 8 | filedirectory[savefilenumber[a - 1]][27];
			}
			else if (a == 2)
			{
				firstaddressfile1 = filedirectory[savefilenumber[a - 1]][26] << 8 | filedirectory[savefilenumber[a - 1]][27];
			}
			else if (a == 3) {
				firstaddressfile1 = filedirectory[savefilenumber[a - 1]][26] << 8 | filedirectory[savefilenumber[a - 1]][27];
			}
			else if (a == 4)
			{
				firstaddressfile1 = filedirectory[savefilenumber[a - 1]][26] << 8 | filedirectory[savefilenumber[a - 1]][27];
			}
			else {
				firstaddressfile1 = filedirectory[savefilenumber[0]][26] << 8 | filedirectory[savefilenumber[0]][27];
			}
			break;
		default:break;


		}
		// Print out the addresses of our clusters in FAT16
		cout << "Clusters Printing : \t";
		for (int i = 0; i < 256; i++)
		{
			if (firstaddressfile1 == (0xFFF7 | 0xFFF8 | 0xFFF9 | 0xFFFA | 0xFFFB| 0xFFFC | 0xFFFd | 0xFFFE | 0xFFFF))
			{
				
				cout << firstaddressfile1 << " EOF " << endl;
					
				break;
			}
			
			cout << firstaddressfile1 << " - > ";
			firstaddressfile1 = FAT16[firstaddressfile1];
		}
	}

	else
	{
		cout << " Sorry your file is not present, try again \n\n";
	}
}
void FileDirectory::printDirectory()
{
	for (int i = 0; i < N; i++)
	{
		if (filedirectory[i][0] != 0)
		{
			for (int j = 0; j < 11; j++)
			{
				cout << filedirectory[i][j];
			}
			cout << "\t";
			// PRINT OUT THE DIRECTORY VALUES
			unsigned short int time = filedirectory[i][22] << 8 | filedirectory[i][23];
			unsigned short int date = filedirectory[i][24] << 8 | filedirectory[i][25];
			cout << (date & 0x000f) << "/" << ((date >> 4)&0x000f) << "/" << (((date >> 8) &0x007F) + 1980) << "\t" ;
			cout << (time >> 11) << ":" << ((time >> 5) & 0x003f) << ":" << ((time & 0x001F));
			unsigned int filesize = (filedirectory[i][28] << 24) | (filedirectory[i][29] << 16) | (filedirectory[i][30] << 8) | (filedirectory[i][31]);
			cout << "  " << filesize << " ";
			unsigned short int firstaddress = filedirectory[i][26] << 8 | filedirectory[i][27];
			cout << firstaddress << endl;
			
		}
		else
		{
			
		}
	}
}
void FileDirectory::printData(char filename[])
{
	unsigned int counter = 0;
	unsigned short int firstaddressfile1 = 0;
	unsigned short int arr[256]; // array for holding the cluster addresses
	for (int i = 0; i < 256; i++)
	{
		arr[i] = 0;
	}
	unsigned int filewithsamenameamount = 0;
	unsigned int savefilenumber[4] = { 0 , 0 , 0 , 0 };
	for (int i = 0; i < N; i++) // search through file directory
	{
		for (int j = 0; j < sizeoffilename; j++) // length of filename array in bytes
		{
			if (filename[j] == filedirectory[i][j]) // if first char in each array ==
			{
				counter++;
			}
		}
		if (counter == sizeoffilename)
		{
			savefilenumber[filewithsamenameamount] = i;
			filewithsamenameamount++;


		}
	}
	int a = 0;
	if (filewithsamenameamount) // if we find any hits on our file name it will sift through a shift case
	{
		switch (filewithsamenameamount) {
		case 0:
			cout << "We have not found your file, sorry try again\n\n";
			break;

		case 1:
			cout << "We have found the file you request \n\n";
			firstaddressfile1 = filedirectory[savefilenumber[0]][26] << 8 | filedirectory[savefilenumber[0]][27];
			break;
		case 2:
			cout << "We have found multiple files with the same name, would you like to look at the (1) first (possibly first) or second (2)(likely made second, likely a copy)? \n\n\n\n";
			
			cin >> a;
			if (a == 1)
			{
				firstaddressfile1 = filedirectory[savefilenumber[a - 1]][26] << 8 | filedirectory[savefilenumber[a - 1]][27];
			}
			else if (a == 2)
			{
				firstaddressfile1 = filedirectory[savefilenumber[a]][26] << 8 | filedirectory[savefilenumber[a]][27];
			}
			else {
				firstaddressfile1 = filedirectory[savefilenumber[0]][26] << 8 | filedirectory[savefilenumber[0]][27];
			}
			break;
		case 3:
			cout << "We have found multiple files with the same name, would you like to look at the (1) first (made first) or second (2)(made second, likely a copy) or (3) why do you even have this many copies man? \n\n\n\n";
		
			cin >> a;
			if (a == 1)
			{
				firstaddressfile1 = filedirectory[savefilenumber[a - 1]][26] << 8 | filedirectory[savefilenumber[a - 1]][27];
			}
			else if (a == 2)
			{
				firstaddressfile1 = filedirectory[savefilenumber[a - 1]][26] << 8 | filedirectory[savefilenumber[a - 1]][27];
			}
			else if (a == 3) {
				firstaddressfile1 = filedirectory[savefilenumber[a - 1]][26] << 8 | filedirectory[savefilenumber[a - 1]][27];
			}
			else {

			}
			break;
		case 4:
			cout << "We have found multiple files with the same name, would you like to look at the (1) first (made first) or second (2)(made second, likely a copy) or (3) why do you even have this many copies man or (4) i dont even know what to say? \n\n\n\n";
		
			cin >> a;
			if (a == 1)
			{
				firstaddressfile1 = filedirectory[savefilenumber[a - 1]][26] << 8 | filedirectory[savefilenumber[a - 1]][27];
			}
			else if (a == 2)
			{
				firstaddressfile1 = filedirectory[savefilenumber[a - 1]][26] << 8 | filedirectory[savefilenumber[a - 1]][27];
			}
			else if (a == 3) {
				firstaddressfile1 = filedirectory[savefilenumber[a - 1]][26] << 8 | filedirectory[savefilenumber[a - 1]][27];
			}
			else if (a == 4)
			{
				firstaddressfile1 = filedirectory[savefilenumber[a - 1]][26] << 8 | filedirectory[savefilenumber[a - 1]][27];
			}
			else {
				firstaddressfile1 = filedirectory[savefilenumber[0]][26] << 8 | filedirectory[savefilenumber[0]][27];
			}
			break;
		default:break;


		}
	}
	
	for (int i = 0; i < 256; i++) // save the addresses into our arr
	{
		if (firstaddressfile1 >= 0xFFF7)
		{
			arr[i] = firstaddressfile1;

		}
		arr[i] = firstaddressfile1;
		firstaddressfile1 = FAT16[firstaddressfile1];
	}
	cout << "Data bout to be printed boi " << endl;
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << data[(arr[i] * 4) + j] << " ";
		}
	}
	cout << endl;
}
FileDirectory::~FileDirectory()
{
	delete[]filedirectory;
} 