

#include <iostream>
using namespace std;
int main()
{
    std::cout << "Hello World!\n";
}

void dataRecovery(int N, int n, int badDisk, unsigned char diskData[][N + 1], unsigned char fileData[])
{
    unsigned char parity = 0;
    for (int i = 0; i < n; i++)
    {
        parity = diskData[i % 512 + 1 / (512 * N + 1) * 512][(i / 512) % N + 1]; // get the parity bit data
        for (int j = 0; j < N; j++) // we need this to run through all instances of the disk data, but to skip the bad disk for parity
        {
            if (j == badDisk)
            {

            }
            else {
                parity = parity ^ diskData[i % 512 + 1 / (512 * j) * 512][(i / 512) % j]; // XOR previous parity data with good disk data until no more good disks are available
            }
        }
        fileData[i] = parity; // store our recovered data back into fileData
    }

}

int minimumDistance(int n, int coordinate[][2])
{
    int disMarks[n]; // make markers for our previously used distances ( so we dont re calculate a distance )
    disMarks[0] = 0;
    for (int i = 1; i < n; i++)
    {
        disMarks[i] = -1;
    }
    int totaldistance = 0; // total dist
    int distance = 0; // current distance checker
    int prevmindistance = 0; // previous minimum dist, starts with first two entries for comparability
    bool wasfirstentriesflag = false; // to check if our first entries were the minimum
    bool previously_used_coordinate = false;
    prevmindistance = abs(coordinate[0][0] - coordinate[1][0]) + abs(coordinate[1][1] - coordinate[1][1]); // give algorithm a starting distance to compare against. but we will still start with our first coordinate
    
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n-1; j++)
        {
            for (int k = 0; k < i; k++)
            {
                if (disMarks[k] = j)
                {
                    previously_used_coordinate = true;
                    break;
                }
            }
            if (previously_used_coordinate) {}
            // enter next instance of loop

            // else 

            else {
                distance = abs(coordinate[j][0] - coordinate[j + 1][0]) + abs(coordinate[j][1] - coordinate[i + 1][1]);
                if (distance < prevmindistance)
                {
                    disMarks[i] = j;
                    prevmindistance = distance;
                    totaldistance += prevmindistance;
                }
            }
        }
        if (wasfirstentriesflag = false)
        {
            totaldistance = prevmindistance;
            wasfirstentries = true;
        }
    }
    
    return totaldistance;

}


int SCAN(int n, int coordinate[][2])
{
    // we will transverse through each street X from 0 to N and ensure we go to all Y's during this scan
    // this can be done as each street Y from 0 to N as well, as done on my paper calculation
    int totaldistance = 0;
    int savei = 0;
    int saveprevx = 0;
    int saveprevy = 0;
    savei = 0;
    saveprevx = 0;
    saveprevy = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (coordinate[i][0] == i)
            {
                if (coordinate[i][1] == j)
                {
                    totaldistance += abs(coordinate[savei][saveprevx] - coordinate[i][0]) + abs(coordinate[i][saveprevy] - coordinate[i][1]);
                    savei = i;
                    saveprevx = coordinate[i][0];
                    saveprevy = coordinate[i][1];
                }


            }
        }

    }
    return totaldistance;

}