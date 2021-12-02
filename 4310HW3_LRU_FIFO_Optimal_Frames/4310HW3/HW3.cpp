#include <iostream>
#include <algorithm>
using namespace std;
int LRU(int n, int references[], int nFrames);
int FIFO(int n, int references[], int nFrames);
int Optimal(int n, int references[], int nFrames);
int main()
{

	const int n = 20; // number of refernece addresses

	int references[n] = { 1, 2, 3, 4, 2, 1,5,6, 2,1,2,3, 7,6,3,2, 1,2,3,6 }; // array that stores teh reference addr., all non zeros

	int nFrames = 7; // number of frames in the phys memory from 1 to 7
	cout << "LRU \t FIFO \t Optimal" << endl;
	for (int i = 1; i <= nFrames; i++) {
		cout << LRU(n, references, i);
		cout << "\t" << FIFO(n, references, i);
		//cout << "\t" << Optimal(n, references, i);
		cout << endl;
	}

}

// Write main fxn to display numbers of the page faults from 3 algorithms for number of frames 1 to 7



int LRU(int n, int references[], int nFrames)
{
	bool hitflag = false;
	int hitsave = 0;
	int countermax = 0;
	int countermaxpos = 0;
	int numberPageFaults = 0;
	int* pageTable = new int[nFrames];
	int* counter = new int[nFrames];


	for (int i = 0; i < nFrames; i++)
	{
		pageTable[i] = 0; counter[i] = 0;

	}

	for (int i = 0; i < n; i++) // all 20 reference check if in table for faults
	{
		for (int pageFrames = 0; pageFrames < nFrames; pageFrames++) // check the page table for refernce
		{
			if (pageTable[pageFrames] == references[i])
			{
				counter[pageFrames] = 1;
				hitsave = pageFrames;
				hitflag = true;
				break;
				
			}
			else
			{
				if (pageFrames == nFrames-1) // check to see if we are on last page, increment if no refernces in page table are match
				{
					numberPageFaults++;

					for (int checkcounter = 0; checkcounter < nFrames; checkcounter++)
					{
						if (counter[checkcounter] == 0) // if = 0 replace pageTable with our new reference
						{
							pageTable[checkcounter] = references[i]; // replaces page table

							counter[checkcounter]++; // increment counter space to 1
							break;
						}

					}

					//check counter to look for max count, replace the max count

					countermax = counter[0];
					countermaxpos = 0;
					// gets position
					if (i > nFrames) {
						for (int checkmax = 0; checkmax < nFrames; checkmax++) // get max and position of it
						{
							if (counter[checkmax] > countermax)
							{
								countermax = counter[checkmax];
								countermaxpos = checkmax;
							}
						}

						pageTable[countermaxpos] = references[i]; // puts the reference into page table
						counter[countermaxpos] = 1;
					}

				}
			}

		}
		// counter increment
		if ((i > nFrames)) {
			for (int inccounter = 0; inccounter < nFrames; inccounter++)
			{
				if (counter[inccounter])
				{
					counter[inccounter]++;
				}
			}// counter increment

			if (hitflag == true) {
				counter[hitsave] = 1;
			}
		}

		hitflag = false;
	}

	return numberPageFaults;
}

int FIFO(int n, int references[], int nFrames)
{

	int numberPageFaults = 0;
	int* pageTable = new int[nFrames];
	int fifocounter = 0;

	for (int i = 0; i < nFrames; i++)
	{
		pageTable[i] = 0; 

	}

	for (int i = 0; i < n; i++)
	{
		for (int checkpagetable = 0; checkpagetable < nFrames; checkpagetable++)
		{
			if (pageTable[checkpagetable] == references[i])
			{
				break;
			}

			else if(checkpagetable == nFrames-1) 
			{
				numberPageFaults++;
				if (fifocounter == nFrames) {
					fifocounter = 0;
				}

				pageTable[fifocounter] = references[i];

				fifocounter++;
			}

		}


	}

	return numberPageFaults;
}

int Optimal(int n, int references[], int nFrames)
{
	int numberPageFaults = 0;
	int pagenumber = 0;
	bool pagebreakflag = false;
	int* pageTable = new int[nFrames];
	int* pageReferenceFlag = new int[nFrames];
	int pagerefcounter = 0;

	for (int i = 0; i < nFrames; i++)
	{
		pageTable[i] = 0;

	}
	for (int i = 0; i < n; i++) // all 20 reference check if in table for faults
	{
		for (int pageFrames = 0; pageFrames < nFrames; pageFrames++) // check the page table for refernce
		{
			if (pageTable[pageFrames] == references[i])
			{
			
				break;

			}
			else
			{
				if (pageFrames == nFrames - 1) // check to see if we are on last page, increment if no refernces in page table are match
				{
					numberPageFaults++;

					for (int checktable = 0; checktable < nFrames; checktable++)
					{
						if (pageTable[checktable] == 0) // if = 0 replace pageTable with our new reference
						{
							pageTable[checktable] = references[i]; // replaces page table

							pageTable[checktable]++; // increment counter space to 1
							break;
						}

					}
					for (int checkref = n - i; checkref < n; checkref) // check every value in reference past i, one we get all hits for values except for one of them, break and put page value into that portion of table
					{
						if (pagerefcounter == nFrames - 1)
						{
							for (int k = 0; k < nFrames; k++)
							{
								if (pageTable[k] == 0)
								{
									pagebreakflag == true;
									pagenumber = k;
									break; 
									
								}
							}
						}
						if (pagebreakflag == true) { pagebreakflag = false;  break; }
						for (int tabletrans = 0; tabletrans < nFrames; tabletrans++) //transverse table and look for a match to ref i in future
						{
							if (pageTable[tabletrans] == references[checkref])
							{
								
								pageReferenceFlag[tabletrans] = 1;
								pagerefcounter++;
								break;
							}
						}
					}

					pageTable[pagenumber] = references[i];
				}




			}
		}
	}

	return numberPageFaults;
}