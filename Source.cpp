//Name :Abayjeet Singh


#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>
#include<map>
#include<unordered_map>
#include<algorithm>
using namespace std;

/*
Reads a binary File and stores it in memory.
*/
char *readbinaryFile(char *, streampos size, ifstream &file );

/*
Returns longest strand of bytes that is identical between two or more files.
This method also  stores the offsets and file names in the passed in paramteres.
*/
int LongestByteStrand(int , map<int, vector<int>> *m, unordered_map<int, vector<string>> &file_names,
	unordered_map<string, vector<int>> &offsets, vector<int> *bytes, string);

int main()
{

	const string FILENAME = "sample.";
	streampos size;
	char * memblock;
	

	int num_files = 0;
	cout << "Enter no of Files:  ";

	cin >> num_files;
    map<int, vector<int>> *m = new map<int, vector<int>>[num_files];


	unordered_map<int, vector<string>> file_names;
	unordered_map<string, vector<int>> offsets;


	vector<int> *bytes  =  new vector<int>[num_files];


	for (int x = 1; x <=num_files; x++)
	{
		ifstream file(FILENAME + to_string(x), ios::in | ios::binary | ios::ate);
		if (file.is_open())
		{
			size = file.tellg();
			memblock = new char[size];
			readbinaryFile(memblock, size, file);
		

			for (int i = 0; i < size; i++)
			{
			
				m[x - 1][memblock[i]].push_back(i);
				bytes[x-1].push_back(memblock[i]);
	
			}

		}
		else
		{
			cout << "Unable to open file";
		}
	}
	cout << "Finding Longest strand of bytes....(should takes < 18 seconds for 10 files)" << endl;


	
	int longest_strand_len = 0;

	longest_strand_len = LongestByteStrand(num_files, m, file_names, offsets, bytes, FILENAME);
	
	cout << endl;
	sort(file_names[longest_strand_len].begin(), file_names[longest_strand_len].begin()+file_names[longest_strand_len].size());

	auto j =unique(file_names[longest_strand_len].begin(), file_names[longest_strand_len].begin() + file_names[longest_strand_len].size());

	file_names[longest_strand_len].resize(distance(file_names[longest_strand_len].begin(), j)); 
	 //Resizing to remove undefined terms after duplicates are removed

	cout << "Length of longest strand of bytes :  " << longest_strand_len << endl;
	cout << "Files which contain the longest strand are :" << endl;
	for (auto j = file_names[longest_strand_len].begin(); j != file_names[longest_strand_len].end(); ++j)
	{
		cout << *j << " " << "at offset  "<<offsets[*j].back()<< endl;
	}
	
	m->clear();
	bytes->clear();

	

	return 0;

 
}

int LongestByteStrand(int num_files, map<int, vector<int>> *m, unordered_map<int, vector<string>> &file_names,
	unordered_map<string, vector<int>> &offsets, vector<int> *bytes, string FILENAME)
{

	int longest_strand_len = 0;
	int result1 = 0;
	int result2 = 0;
	for (int a = 0; a < num_files - 1; a++)
	{
		for (int b = a + 1; b < num_files; b++)
		{

			for (auto i = m[a].begin(); i != m[a].end(); i++)
			{
				int len = 0;

				auto r = m[b].find(i->first);
				if (r != m[b].end())
				{
					for (auto j = i->second.begin(); j != i->second.end(); ++j)
					{
						for (auto k = r->second.begin(); k != r->second.end(); ++k)
						{
							if (longest_strand_len > bytes[b].size() || longest_strand_len > bytes[a].size())
							{
								continue;
							}

							int index = (*j);
							int index2 = (*k);

							int offset = index;
							int offset2 = index2;

							while (!(bytes[a].at(index) ^ bytes[b].at(index2)))  //A==B if A XOR B =0
							{
								len++;



								if (index + 1 < bytes[a].size() && index2 + 1 < bytes[b].size())
								{
									index = index + 1;
									index2 = index2 + 1;

								}
								else
								{
									break;
								}

							}
							if (len >= longest_strand_len)
							{
								longest_strand_len = len;
								len = 0;
								result1 = offset;
								result2 = offset2;

								file_names[longest_strand_len].push_back(FILENAME + to_string(a + 1));
								file_names[longest_strand_len].push_back(FILENAME + to_string(b + 1));

								offsets[FILENAME + to_string(a + 1)].push_back(result1);
								offsets[FILENAME + to_string(b + 1)].push_back(result2);


							}
							len = 0;

						}

					}

				}


			}
		}
	}
	cout << "__________________________________________________________________________________________" << endl;





	return longest_strand_len;

}


char *readbinaryFile(char *memblock, streampos size, ifstream &file)
{

	
	file.seekg(0, ios::beg);
	file.read(memblock, size);
	file.close();
	

	return memblock;
}



