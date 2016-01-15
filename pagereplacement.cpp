#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<stdio.h>
#include<chrono>
using namespace std;
int total_pff, total_vsws;
const int f = 2;                                                             //Value F for PFF
const int m = 4, l = 5, q = 1;                                               //Value of M, L and Q for VSWS

string input();
void initial_resident_set(vector<string> page_ref, vector<string> page_in_memory, vector<string> &resident_set, int len_mem);
void resident_set(vector<string> page_ref, vector<string> page_in_memory, vector<string> resident_set, int len_mem);
void num_of_pages_occ(string data, int len, string &num_of_pages);
void page_references(string num_of_pages, string data, vector<string> &page_ref);
void num_of_pages_in_memory(vector<string> page_ref, vector<string> &page_in_memory);
void pff(string num_of_pages, vector<string> page_ref, vector<string> page_in_memory);
void vsws(string num_of_pages, vector<string> page_ref, vector<string> page_in_memory);

void main()
{
	string num_of_pages;
	vector<string> page_ref, page_in_memory;
	string data = input();                                                   //Accept input file and store the data 
	//Our current input file is 10 1 2 3 4 5 6 7 8 2 2 2 2 2 4 4 4 5 6 8 2 4 5 
	if (!data.empty())
	{
		int len = data.size();
		num_of_pages_occ(data, len, num_of_pages);                           //Number of pages the process occupies
		page_references(num_of_pages, data, page_ref);                       //Page References, the program makes 
		num_of_pages_in_memory(page_ref, page_in_memory);                    //Pages in Memory
		cout << "\nThe process occupies " << num_of_pages << " pages.";
		cout << "\nPFF will be applied first, followed by VSWS.\n";

		//Perform PFF
		cout << "\n\n1. Page Fault Frequency (PFF)\n";
		cout << "\nThe value of F chosen is " << f << "\n";
		pff(num_of_pages, page_ref, page_in_memory);

		//Perform VSWS
		cout << "\n\n2. Variable Interval Sampled Working Set (VSWS)\n";
		cout << "\nThe value of M = " << m << ", L = " << l << " and Q = " << q << "\n";
		vsws(num_of_pages, page_ref, page_in_memory);

		//Performance Evaluation 
		cout << "\n\nMemory usage depends on our input, the frame size and the size of resident set for both algorithms.";
		//PFF Performance Evaluation 
		cout << "\n\nPFF Performance Evaluation:";
		cout << "\nFor PFF, the number of page faults reduce on increasing the value of F. ";
		cout << "After increasing the value of F upto a particular limit, the number of page faults become unaffected.";
		cout << "\n\nTotal page faults occured in PFF for F = " << f << " for our input are ";
		cout << total_pff << ".\n";     //Number of page faults in PFF
		//VSWS Performance Evaluation
		cout << "\n\nVSWS Performance Evaluation:";
		cout << "\nFor VSWS, the number of page faults reduce as the value of (L-M) and Q increases.";
		cout << "After increasing the value of (L-M) upto a particular limit, the number of page faults become unaffected.";
		cout << "\n\nTotal page faults occured in VSWS for M = " << m << ", L = " << l << " and Q =" << q << " for our input are ";
		cout<< total_vsws <<".\n";   //Number of page faults in VSWS
		cin.get();
	}
	cin.get();
}

//Take input file for the program on the command line 
string input()                                              
{
	ifstream inputFile;
	string fileName;
	string data, strcpy;
	cout << "Enter the file name: ";
	getline(cin, fileName);
	inputFile.open(fileName);

	if (!inputFile.is_open())                                   //When file does not exist
	{
		cerr << "Error Opening File" << endl;
	}

	else {
		do
		{
			inputFile >> data;                                  //Collect data from the file
			strcpy.append(data);
			strcpy.append("\n");
		} while (!inputFile.eof());
	}

	inputFile.close();
	return strcpy;
}

//Initial resident set will be empty
void initial_resident_set(vector<string> page_ref, vector<string> page_in_memory, vector<string> &residentset, int len_mem)
{
	cout << "\nNo page faults occur in the beginning, so the values for working set is 0 for all page references.";
	for (int i = 0; i < len_mem; i++)
	{
		residentset.push_back("0");                                      //Resident Set will be empty initially
	}
	resident_set(page_ref, page_in_memory, residentset, len_mem);        //Display current resident set
}

//Display current resident set on each page demand
void resident_set(vector<string> page_ref, vector<string> page_in_memory, vector<string> residentset, int len_mem)
{
	cout << "\nCurrent working set: \n";
	for (int i = 0; i < len_mem; i++)
	{
		cout << page_in_memory[i] << ":" << residentset[i] << " ";
	}
	cout << endl;
}

//Number of pages the process occupies
void num_of_pages_occ(string data, int len, string &num_of_pages)
{
	string num_of_pages_temp;
	for (int i = 0; i < len; i++)
	{
		if (isspace(data[i]))
			continue;
		else
		{
			num_of_pages_temp = data[i];
			num_of_pages.append(num_of_pages_temp);
			if (isspace(data[i + 1]))
				break;
		}
	}
}

//Page References
void page_references(string num_of_pages, string data, vector<string> &page_ref)
{
	size_t pos = data.find(num_of_pages);
	pos = pos + num_of_pages.length();
	string data_temp = data.substr(pos);
	int len_temp = data_temp.size();
	for (int i = 0; i < len_temp; i++)
	{
		size_t pos1 = data_temp.find('\n');
		pos1 = pos1 + 1;
		data_temp = data_temp.substr(pos1);
		size_t pos2 = data_temp.find('\n');
		string temp = data_temp.substr(0, pos2);
		if (temp != "")
			page_ref.push_back(temp);                                      //Page References from input file
		pos1 = 0;
		pos2 = 0;
	}
}

//Pages in Memory
void num_of_pages_in_memory(vector<string> page_ref, vector<string> &page_in_memory)
{
	int len_ref = page_ref.size();
	for (int i = 0; i < len_ref; i++)
	{
		int len_mem = page_in_memory.size();
		bool flag = false;
		for (int j = 0; j < len_mem; j++)
		{
			flag = true;
			if (page_in_memory[j] != page_ref[i])              //No repetitions of page values in memory
			{
				flag = false;
				continue;
			}
			else
			{
				flag = true;
				break;
			}
		}
		if (flag == false)
			page_in_memory.push_back(page_ref[i]);                           //All pages in memory

	}
}

//Page Fault Frequency
void pff(string num_of_pages, vector<string> page_ref, vector<string> page_in_memory)
{	
	int t_last = 0, t_current = 0;                                              //Last Time and Current Time of Page Fault occurance
	int len = page_ref.size();
	int len_mem = page_in_memory.size();
	vector<string> residentset;
	initial_resident_set(page_ref, page_in_memory, residentset, len_mem);           //Initial working set will be empty
	for (int i = 0; i < len; i++)
	{
		++t_current;                                                            //Increase Current Time for each page demand
		cout << "\nPage Reference is " << page_ref[i];                           
		vector<string> page_temp;
		vector<int> page_num;
		bool invalid_ref = false;
		for each(string mem in page_in_memory)
		{
			if (mem == page_ref[i])
			{
				int value = t_current - t_last;
				int val = 0;
				if (stoi(page_ref[i]) > stoi(num_of_pages))        //When Page Reference is greater than the number of pages the process occupies
				{
					val = 1;
					invalid_ref = true;                            //Set invalid flag as true
				}
				else
					//Checking frame size
				{
					if ((t_current - t_last) <= f)                 
						val = 1;
					else if ((t_current - t_last) > f)
						val = 2;
				}
				switch (val)
				{					    
				case 1: //When (current time - last time) is less than or equal to F 
					if (invalid_ref == false)                     //If Page Reference is valid
					{
						cout << "\nDifference between current time and Last time is less than or equal to F.";
						cout << "\nChecking if page fault occurs....";
						for (int j = 0; j < len_mem; j++)
						{
							if (residentset[j] == "F")			 //When page fault occured in the last working set		
								residentset[j] = '1';
							if (mem == page_in_memory[j])    //Pick the index of page from memory and modify it's corresponding working set
							{
								if (residentset[j] == "0")      //When page fault occurs in the current working set
								{
									residentset[j] = 'F';
									cout << "\nPage Fault occurs for " << page_ref[i] << ".";
									t_last = t_current;
									++total_pff;
								}
								else                          //Page Fault does not occur
								{
									cout << "\nPage already in memory. No page fault occurs for " << page_ref[i] << ".";
								}
							}
						}
					}
					else                                      //If Page Reference is invalid
					{
						cout << "\nIt is invalid.";
						for (int j = 0; j < len_mem; j++)
						{
							if (mem == page_in_memory[j])
								residentset[j] = 'X';            //Give invalid value as X
						}
					}
					break;
					    
				case 2: //When (current time - last time) is greater than F 
					cout << "\nDifference between current time and Last time is greater than F.";
					cout << "\nThe pages not referenced within the time range will be removed, if page fault occurs.";
					cout << "\nChecking if page fault occurs....\n";
					for (int j = 0; j < len_mem; j++)
					{
						if (residentset[j] == "F")                            //When page fault occured in the last working set
							residentset[j] = '1';
						if (mem == page_in_memory[j])                      //Pick the index of page from memory and modify it's corresponding working set
						{
							if (residentset[j] == "F" || residentset[j] == "1")  //Page Fault does not occur
							{
								cout << "\nNo page fault occurs. Therefore, no pages will be removed.";
								break;
							}
							cout << "\nPage Fault occurs. Therefore, pages not referenced within the time range will be removed.";
							if (residentset[j] == "0")                         //When page fault occurs in the current working set
							{
								residentset[j] = 'F';
								++total_pff;
							}
							for (int k = (t_current - 1); k >= t_last; k--)  //Temporarily store the page references, lying within the time range
							{
								page_temp.push_back(page_ref[k - 1]);       
							}
							t_last = t_current;
							for each(string temp in page_temp)               //Temporarily store the index of page references, lying within the time range
							{
								for (int l = 0; l < len_mem; l++)
								{
									if (temp == page_in_memory[l])
									{
										page_num.push_back(l);
										break;
									}
									else
										continue;
								}
							}
							int len_page = page_num.size();
							for (int l = 0; l < len_mem; l++)                 //Removing pages not referenced within the time range from the working set
							{
								bool flag = false;
								for (int n = 0; n < len_page; n++)
								{
									if (l == page_num[n])
									{
										flag = true;
										break;
									}
								}
								if (flag == false && residentset[l] != "F" && residentset[l] != "X")
									residentset[l] = '0';
							}
							break;
						}
					}
					break;
				}
			}		
			else
				continue;
		}
		resident_set(page_ref, page_in_memory, residentset, len_mem);        //Current working set
	}
}

//Variable Interval Sampled Working Set
void vsws(string num_of_pages, vector<string> page_ref, vector<string> page_in_memory)
{
	int q_temp = 0, t_current = 0, t_count = 0;
	int len = page_ref.size();
	int len_mem = page_in_memory.size();
	vector<string> residentset;
	initial_resident_set(page_ref, page_in_memory, residentset, len_mem);   //Initial working set will be empty
	for (int i = 0; i < len; i++)
	{
		vector<string> page_temp;
		vector<int> page_num;
		++t_current;                                                    //Increase Current Time for each page demand
		++t_count;
		cout << "\nPage Reference is " << page_ref[i];
		bool invalid_ref = false;
		for each(string mem in page_in_memory)
		{
			if (mem == page_ref[i])
			{
				if (stoi(page_ref[i]) > stoi(num_of_pages))             //When Page Reference is greater than the number of pages the process occupies
				{
					invalid_ref = true;                                 //Set invalid flag as true
				}
					if (invalid_ref == false)                           //If Page Reference is valid
					{
						cout << "\nChecking if page fault occurs....";
						for (int j = 0; j < len_mem; j++)
						{
							if (residentset[j] == "F")			           //When page fault occured in the last working set		
								residentset[j] = '1';
							if (mem == page_in_memory[j])              //Pick the index of page from memory and modify it's corresponding working set
							{
								if (residentset[j] == "0")                //When page fault occurs in the current working set
								{
									residentset[j] = 'F';
									cout << "\nPage Fault occurs for " << page_ref[i] << ".";
									++q_temp;
									++total_vsws;
								}
								else                                       //Page Fault does not occur
								{
									cout << "\nPage already in memory. No page fault occurs for " << page_ref[i] << ".";
								}
							}
						}
					}
					else if (invalid_ref == true)                            //If Page Reference is invalid
					{
						cout << "\nIt is invalid.";
						for (int j = 0; j < len_mem; j++)
						{
							if (mem == page_in_memory[j])
								residentset[j] = 'X';                           //Give invalid value as X
						}
					}
				}
			else
				continue;
			
				if (t_count == m)
				{
					if (q_temp >= q)
					{
						cout << "\nQ Page Faults occured, within M time. Therefore, page references outside the time range will be removed.";
						for (int k = (t_current - 1); k >= (t_current - m); k--)  //Temporarily store the page references, lying within the time range
						{
							if (k > 0)
								page_temp.push_back(page_ref[k - 1]);
						}
					}
				}
				else if (t_count >= l)
				{
					cout << "\nVirtual time since last sampling reaches time L. Therefore, page references outside the time range will be removed.";
					for (int k = (t_current - 1); k >= (t_current - l); k--)  //Temporarily store the page references, lying within the time range
					{
						if (k > 0)
						page_temp.push_back(page_ref[k - 1]);
					}
				}
				if(!page_temp.empty())
				{
					for each(string temp in page_temp)                        //Temporarily store the index of page references, lying within the time range
					{
						for (int l = 0; l < len_mem; l++)
						{
							if (temp == page_in_memory[l])
							{
								page_num.push_back(l);
								break;
							}
							else
								continue;
						}
					}
					int len_page = page_num.size();
					for (int l = 0; l < len_mem; l++)                          //Removing pages not referenced within the time range from the working set
					{
						bool flag = false;
						for (int n = 0; n < len_page; n++)
						{
							if (l == page_num[n])
							{
								flag = true;
								break;
							}
						}
						if (flag == false && residentset[l] != "F" && residentset[l] != "X")
							residentset[l] = '0';
					}
					q_temp = 0;
					t_count = 0;
					//working_set(page_ref, page_in_memory, work_set, len_mem);         //Current working set
				}
			break;
		}
		resident_set(page_ref, page_in_memory, residentset, len_mem);         //Current working set
	}
}



