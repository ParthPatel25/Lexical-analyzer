//Name: Parth G Patel
//Class: CS 280
//Professor: Sean Cusack





#include <iostream> //Jay Swaminarayan
#include <fstream>
#include <sstream>
#include <string>
#include <cstddef>
#include <vector>
#include<iomanip>
using namespace std;
int total = 60;
static vector<char> mdap;



void control(vector<char>& padm, int cou[], int len[])
{
	int space = 0, temp = 0;
	vector<char>::iterator it;
	int p = 1, j = 0, m = 0, l = 0;
	it = padm.begin();


	for (int i = 0; i < padm.size(); i++, j++)
	{
		if (i == cou[m])
		{
			total = len[m];
			m++;
		}
		if (padm[i] == '\n')
		{
			cou[m]++;
			j = 0;
		}
		if (j % total == 0 && j>0)
		{
			temp = 0;
			for (int k = i - 1;; k--)
			{
				if (padm[k] != ' ')
				{
					space = k;
					temp++;
				}
				else
					break;
			}

			if (temp > 0)
			{
				it = padm.begin() + (space);
				for (int k = 0; k < temp; k++)
				{
					padm.insert(it, ' ');
				}
			}
			//padm.insert(padm.begin() + (space), temp, ' ');
		}
	}

	int n = 0;
	int i = 0;
	j = 0;
	total = 60;

	for (n = 0, it = padm.begin(); it < padm.end(); it++, n++, j++)
	{
		if (n == cou[i])
		{
			total = len[i];
			i++;
		}
		if (padm[n] == '\n')
		{
			j = 0;
		}
		if (j % total == 0 && j > 0)
			cout << endl;
		cout << *it;
	}
}


int blankspace(ifstream& out_file, string str)
{
	static int count = 0;
	int j = 0;
	for (int i = 0; i <= int(str.length()); i++)
	{
		if (str[i] != ' ' && str[i] != '\t')
		{
			count++;
			mdap.push_back(str[i]);
			j = 0;
		}
		else
		{
			if ((str[i] == ' ' || str[i] == '\t') && j == 0)
			{
				count++;
				mdap.push_back(' ');
				j++;
			}
		}
	}
	return count++;
}


void blankline(ifstream& in_file, string line)
{
	int len[120], cou[120];
	int length = 0, count = 0;
	int num[3];
	int i = 0, j = 0, k = 0;

	while (getline(in_file, line))
	{
		if (line[0] == '.')
		{
			if (7 >= line.size())
			{
				//mdap.push_back('\n');
				mdap.push_back('\n');
				if (line[1] == 'l' && line[2] == 'l' && line[3] == ' ')
				{
					if (isdigit(line[4]) && isdigit(line[5]) && isdigit(line[6]))
					{
						num[0] = int(line[4]) - 48;
						num[1] = int(line[5]) - 48;
						num[2] = int(line[6]) - 48;
						length = num[0] * 100 + num[1] * 10 + num[2];

						len[j] = length;
						cou[k] = count;

						j++;
						k++;
					}
					else if (isdigit(line[4]) && isdigit(line[5]))
					{
						num[0] = int(line[4]) - 48;
						num[1] = int(line[5]) - 48;
						length = num[0] * 10 + num[1];

						len[j] = length;
						cou[k] = count;

						j++;
						k++;
					}
				}
			}

		}
		else if (!line.empty())
		{
			count = blankspace(in_file, line);
			mdap.push_back(' ');
			i = 0;
		}
		else
		{
			if (line.empty() && i == 0)
			{
				mdap.push_back('\n');
				mdap.push_back('\n');
				i++;
			}
		}
	}
	control(mdap, cou, len);

}


int main(int argc, char* argv[])
{
	string str;
	ifstream myfile(argv[1]);
	blankline(myfile, str);
	return 0;
}
