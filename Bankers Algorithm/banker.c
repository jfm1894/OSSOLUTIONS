//********************************************************************
//
// Jeff Mitchell
// Operating Systems
// Programming Project #5: Bankers Algorithm
// October 23 2022
// Instructor: Dr. Siming Liu
//
//********************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//function headers
void makeLabels(int labelSize, int typeFlag);
//void printMatrix(int oMatrix[n][m],int rows, int columns);
void print1D(int oArray[], int size);


int main(int argc, char *argv[])
{
//declare variables
//get file path from command line
FILE* input;
int num;
input = fopen(argv[1], "r");
int n,m;
int inputArr[256];
if (input == NULL)
{
    printf("File could not be opened!\n");
    return 0;
}
//get length of file and store contents to an array for further parsing
int fileLen=0;
while (fscanf(input, "%d%*c", &num)==1)
{
	inputArr[fileLen]=num;
        fileLen = fileLen + 1;
}

//relevent variables
n = inputArr[0];//set n equal to first element of input array which is # processes
m = inputArr[1];//set m equal to secone element of input array which is # resources
int allocationMatrix[n][m];
int maxMatrix[n][m];
int needMatrix[n][m];
int available[m];
int request[m];
int process;

//index that will be used to keep track of input array starting at 2
int in = 2;
//create allocation matrix
for (int i=0;i<n;i++)
{
	for(int j=0;j<m;j++)
	{
		allocationMatrix[i][j] = inputArr[in];
		in = in+1;
	}
}

//create max matrix
for (int i=0;i<n;i++)
{
	for(int j=0;j<m;j++)
	{
		maxMatrix[i][j] = inputArr[in];
		in = in+1;
	}
}
//create need matrix
for (int i=0;i<n;i++)
{
	for(int j=0;j<m;j++)
	{
	needMatrix[i][j] = maxMatrix[i][j]-allocationMatrix[i][j];
	}
}

//create available  vector
for (int i=0;i<m;i++)
{
	available[i]=inputArr[in];
	in = in+1;
}
//define process number
process = inputArr[in];
in = in+1;
//create request vector
for(int i=0;i<m;i++)
{
request[i] = inputArr[in];
in = in+1;
}
//start printing output
//print number of processes and resource types
printf("There are %d processes in the system.\n\n",n);
printf("There are %d resource types.\n\n",m);
//print allocation matrix
printf("The Allocation Matrix is...\n");
makeLabels(m,0);
for (int i=0;i<n;i++)
{
        printf("%d: ",i);
        for(int j=0;j<m;j++)
        {
                printf("%d ", allocationMatrix[i][j]);
        }
       printf("\n");
}
//print max matrix
printf("\nThe Max Matrix is...\n");
makeLabels(m,0);
for (int i=0;i<n;i++)
{
        printf("%d: ",i);
        for(int j=0;j<m;j++)
        {
                printf("%d ", maxMatrix[i][j]);
        }
       printf("\n");
}
//print need matrix
printf("\nThe Need Matrix is...\n");
makeLabels(m,0);
for (int i=0;i<n;i++)
{
        printf("%d: ",i);
        for(int j=0;j<m;j++)
        {
                printf("%d ", needMatrix[i][j]);
        }
       printf("\n");
}
//print available vector
printf("\nThe Available Vector is...\n");
makeLabels(m,1);
print1D(available,m);

//determine system safety
int work[m];
int finish[n];
int unSafe=1;
for (int i=0;i<n;i++)
{
	finish[i]=0;
}
int rowGood=0;
//create new available vector initially set to available
for(int i=0;i<m;i++)
{
        work[i] = available[i];
}
//iterate need matrix to determine if system is in a safe state
for(int i=0;i<n;i++)
{
        for(int j=0;j<m;j++)
        {
		rowGood=0;
                if(needMatrix[i][j]<=work[j])
                {
                      rowGood=1;
                }
		else
		{
			break;
		}
        }
        //if the row met the condition add row from allocation matrix to work array
        if(rowGood == 1)
        {
        for(int l=0;l<m;l++)
                {
                        work[l] = work[l]+allocationMatrix[i][l];
                }
        finish[i] = 1;       
        }

}
//check to see if all rows passed the test if a row did not recheck with new available matrix
int unsafe;
for(int i=0;i<n;i++)
{
if(finish[i] == 0)
{
	unsafe = i;
}
}
for (int i=0;i<m;i++)
{
	if(needMatrix[unsafe][i]<=work[i])
	{
		finish[unsafe] = 1;
	}
	else
	{
		break;
	}

}

for(int i=0;i<n;i++)
{
 if(finish[i] == i)
 {
	 unSafe=0;
 }
}

if( unSafe == 0)
{
	printf("\nTHE SYSTEM IS IN A SAFE STATE!\n");
}
else
{
	printf("\nTHE SYSTEM IS NOT SAFE!\n");
}


//print request vector
printf("\nThe Request Vector is...\n");
makeLabels(m,2);
printf("%d:",process);
print1D(request,m);
//compute if request can be granted
int canGrant = 1;
for (int i=0;i<m;i++)
{
        if(available[i]-request[i]<0)
        {
                canGrant = 0;
        }
}
if(canGrant==1)
{
        printf("\nTHE REQUEST CAN BE GRANTED!\n");
}
else
{
        printf("\nTHE REQUEST CANNOT BE GRANTED!\n");
}

//compute new available vector
if(canGrant == 1)
{
for (int i=0;i<m;i++)
{
        available[i] = available[i]-request[i];
}
printf("\nThe Available Vector is...\n");
makeLabels(m,1);
print1D(available,m);
}
return 0;
}

//********************************************************************
//
// Make Labels Function
//
// This function prints out the column labels of the matrixes and arrays
// does not return anything
// 
//
// Return Value
// ------------
// none                         
//
// Value Parameters
// ----------------
// labelSize		int		number of columns that need labels
// typeFlag	        int	        type of data structure we are printing labels for
//
// 
//
//*******************************************************************
void makeLabels(int labelSize, int typeFlag)
{
int asciiStart = 65;
if(typeFlag ==0)
{
        printf("   ");
}
if (typeFlag == 2)
{
   printf("  ");
}
for(int i=0;i<labelSize;i++)
{
  char label = asciiStart + i;
  printf("%c ",label);
}
printf("\n");

}

//void printMatrix(int oMatrix[n][m], int rows, int columns)
//{
//for (int i=0;i<rows;i++)
//{
//        printf("%d: ",i);
//        for(int j=0;j<columns;j++)
//        {
//                printf("%d ", oMatrix[rows][columns]);
//        }
//        printf("\n");
//}
//}

//********************************************************************
//
// print 1D array function
//
// This function prints out the a 1 dimensional array
// does not return anything
// 
//
// Return Value
// ------------
// none                         
//
// Value Parameters
// ----------------
// oArray[]		int		the array which we want to print
// size	                int	        the size of the array that we are printing
//
// 
//
//*******************************************************************
void print1D(int oArray[], int size)
{
 for (int i=0;i<size;i++)
 {
	 printf("%d ",oArray[i]);
 }
 printf("\n");
}


