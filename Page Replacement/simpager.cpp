//********************************************************************
//
// Jeff Mitchell
// Operating Systems
// Programming Project #6: Simulation of Page Replacement Strategies
// November 9, 2022
// Instructor: Dr. Siming Liu
//
//********************************************************************
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <cstdlib>

using namespace std;
//********************************************************************
//
// FIFO Page Replacement Function
//
// This function goes through a page reference string and using the first in first out
// page replacement algorithm determines the number of page faults. the page that has been in
// the frame the longest is replaced with the new page
//
// Return Value
// ------------
// pageFaults		int		number of page faults using fifo method                        
//
// Value Parameters
// ----------------
// pages		vector<int>		page reference string
// frames	    int	         	number of frames to be used for page replacement
//
// 
//
//*******************************************************************
int FIFO(vector<int> pages, int frames)
{
 	unordered_set<int> s;
	queue<int> indexes;
	int pageFaults = 0;
	for (int i=0;i<pages.size(); i++)
	{
		if(s.size() < frames)
		{
			if(s.find(pages.at(i))==s.end())
					{
					s.insert(pages.at(i));
					pageFaults++;
					indexes.push(pages.at(i));
					}
		}
		else
		{
			if(s.find(pages.at(i))==s.end())
			{
				int val = indexes.front();
				indexes.pop();
				s.erase(val);
				s.insert(pages.at(i));
				indexes.push(pages[i]);
				pageFaults++;
			}
		}

	}
	return pageFaults;
}
//********************************************************************
//
// LRU Page Replacement Function
//
// This function goes through a page reference string and determins the number of page faults
// using the Least Recently Used page replacement Algorithm. This algorithm replaces the frame
// that was least recently used with the new one
// 
//
// Return Value
// ------------
// pageFaults		int		number of page faults deteceted by this algorithm                         
//
// Value Parameters
// ----------------
// pages		vector<int>		page reference string
// frames	    int	        	size of page frame
//
// 
//
//*******************************************************************
int LRU(vector<int> pages, int frames)
{
	unordered_set<int> s;
	unordered_map<int,int> indexes;
	int pageFaults=0;
	for(int i=0;i<pages.size();i++)
	{
		if(s.size() < frames)
		{
			if(s.find(pages.at(i))==s.end())
			{
				s.insert(pages.at(i));
				pageFaults++;
			}
			indexes[pages.at(i)] = i;
		}
		else
		{
			if(s.find(pages.at(i))==s.end())
			{
				int lru=10000;
				int val;
				for(auto it=s.begin();it!=s.end();it++)
				{
					if (indexes[*it]<lru)
					{
						lru = indexes[*it];
						val=*it;
					}
				}
				s.erase(val);
				s.insert(pages.at(i));
				pageFaults++;
			}
			indexes[pages.at(i)]=i;
		}
	}
	return pageFaults;
}


//********************************************************************
//
// OPT Page Replacement Function
//
// This function goes through a page reference string and determins the number of page faults
// using the Optimal page replacement Algorithm. This algorithm replaces the frame
// that will not be used for the longest time
// 
//
// Return Value
// ------------
// pageFaults		int		number of page faults deteceted by this algorithm                         
//
// Value Parameters
// ----------------
// pages		vector<int>		page reference string
// frames	    int	        	size of page frame
//
// 
//
//*******************************************************************
int OPT(vector<int> pages, int frames)
{
	//frame vector
	vector<int> pFrames;
	int pageFaults=0;
	//iterate through vector determine which frame will not be used in the near future
	for (int i=0;i<pages.size();i++)
	{
		int k;
		for (k=0;k<pFrames.size();k++)
			if(pages.at(i) == pFrames.at(k))
				break;
			if (k==pFrames.size())
			{
				if(pFrames.size() < frames)
				{
					pFrames.push_back(pages.at(i));
				}
				else{
					int index = i+1;
					int res=-1;
					int farthest = index;
					for (int l=0;l<pFrames.size();l++)
					{
						int j;
						for(j=index;j<pages.size();j++)
						{
							if(pFrames.at(l)==pages.at(j))
							{
								if(j > farthest)
								{
									farthest = j;
									res = l;
								}
								break;
							}
						}
						if(j==pages.size())
						{
							res=l;
							break;
						}
					}
					pFrames.at(res) = pages.at(i);
				}
				pageFaults++;
			}
	}
	return pageFaults;
}


//********************************************************************
//
// RAND Page Replacement Function
//
// This function goes through a page reference string and determins the number of page faults
// using the Random page replacement Algorithm. This algorithm replaces the frame
// randomly
// 
//
// Return Value
// ------------
// pageFaults		int		number of page faults deteceted by this algorithm                         
//
// Value Parameters
// ----------------
// pages		vector<int>		page reference string
// frames	    int	        	size of page frame
//
// 
//
//*******************************************************************
int RAND(vector<int> pages, int frames)
{
	//create pageframe vector and number of page fault variables
	vector<int> pframes;
	int pageFaults=0;
	//go through reference string add pages untill frame vector full then start replacing randomly
	for (int i=0;i<pages.size();i++)
	{
		if (i<frames)
		{
			pframes.push_back(pages.at(i));
			pageFaults++;
		}
		else
		{
			for(int j=0;j<pframes.size();j++)
			{
				if(pframes.at(j)==pages.at(i))
				{
					break;
				}
				else{
					srand((unsigned) time(NULL));
					int randomIndex = 0+(rand()%frames);
					pframes.at(randomIndex) = pages.at(i);
					pageFaults++;
				}
			}
		}
	}
	return pageFaults;
}


int main(int argc, char *argv[])
{
//declare variables
 int numFrames;
 vector<int> pageRef;
 //parse input file
 string line;
 ifstream inFile(argv[1]);
 vector<string> inputVector;
 if(inFile.is_open()){
 while (getline(inFile,line))
	 {
	inputVector.push_back(line);
	 }
 }
 else
 {
   cout << "Could Not open file! Please provide valid file path."<< '\n';
   return 0;
 }
 //convert string at first element of input vector to its own vector of type int
stringstream stringStream(inputVector.at(0));
int num;
while (stringStream >> num)
{
 pageRef.push_back(num);
}
numFrames = stoi(inputVector.at(1));

//echo input
cout << "Page Reference String:\n";
for (int i=0;i<pageRef.size();i++)
{
	cout << pageRef.at(i) << " ";
}
cout << "\nNumber of Frames: " << numFrames << "\n";

//determine which algorithm(s) to use and perform operations
for (int i=2;i<inputVector.size();i++)
{
	if(inputVector.at(i) == "FIFO")
	{
		cout << "FIFO: "<< FIFO(pageRef,numFrames)<< '\n';
	}
	else if(inputVector.at(i) == "LRU")
	{
		cout << "LRU: " << LRU(pageRef,numFrames) << '\n';
	}
	else if(inputVector.at(i) == "OPT")
	{
		cout << "OPT: " << OPT(pageRef,numFrames) << '\n';
	}
	else if(inputVector.at(i) == "RAND")
	{
		cout << "RAND: " << RAND(pageRef,numFrames) << '\n';
	}
}

 return 0;
}
