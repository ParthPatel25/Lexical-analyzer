//Name: Parth G Patel
//Class: CS 280
//Professor: Sean Cusack
//project# 2



#include<fstream>
#include<map>
#include<vector>
#include<algorithm>
//#include<iostream>
#include "p2lex.h"
using namespace std;
int	linenum=0;
string token[] = { "id", "str", "int", "plus", "star", "leftsq", "rightsq", "print", "set", "sc", "lparen", "rparen", "done", "err" };
map<TokenType, int> mapStates;
map<TokenType, int>::iterator iter;
vector<string> myvector; int mycount, Int = 0, Str = 0, Id = 0;

int main(int argc, char* argv[])
{
	bool normode=false;
	bool vmode=false;
	string str, v = "-v", link;
	
	Token t1(ID, "");
	Token t2;

	if (argc == 2)
	{
		normode = true;
		link = argv[1];
	}
	else
	{
		if (argv[1] == v)
		{
			vmode = true;
			link = argv[2];
		}
		else if (argv[2] == v)
		{
			vmode = true;
			link = argv[1];
		}
	}
	ifstream file(link.c_str());

	if (normode)
	{
		while ( true)
		{
			if (t1.getTok() == ERR || t1.getTok() == DONE)
				break;
			t1=(getToken(&file));
			//cout << t1;
			switch (t1.getTok()) 
			{
				case 0:
				{
					myvector.push_back((t1.getLexeme()));
					if (mycount = count(myvector.begin(), myvector.end(), (t1.getLexeme())) <= 1)
					{
						Id++;
					}
					mapStates[t1.getTok()]+=1;
					break;
				}
				case 1:
				{
					myvector.push_back((t1.getLexeme()));
					if (mycount = count(myvector.begin(), myvector.end(), (t1.getLexeme())) <= 1)
					{
						Str++;
					}
					mapStates[t1.getTok()] += 1;
					break;
				}
				case 2:
				{
					myvector.push_back((t1.getLexeme()));
					if (mycount = count(myvector.begin(), myvector.end(), (t1.getLexeme())) <= 1)
					{
						Int++;
					}
					mapStates[t1.getTok()] += 1;
					break;
				}
				case 3:
				{
					mapStates[t1.getTok()] += 1;
					break;
				}
				case 4:
				{
					mapStates[t1.getTok()] += 1;
					break;
				}
				case 5:
				{
					mapStates[t1.getTok()] += 1;
					break;
				}
				case 6:
				{
					mapStates[t1.getTok()] += 1;
					break;
				}
				case 7:
				{
					mapStates[t1.getTok()] += 1;
					break;
				}
				case 8:
				{
					mapStates[t1.getTok()] += 1;
					break;
				}
				case 9:
				{
					mapStates[t1.getTok()] += 1;
					break;
				}
				case 10:
				{
					mapStates[t1.getTok()] += 1;
					break;
				}
				case 11:
				{
					mapStates[t1.getTok()] += 1;
					break;
				}
				case 13:
				{
					mapStates[t1.getTok()] += 1;
					break;
				}
			}
		}
   if (t1.getTok() != t2.getTok())
{
  for (iter = mapStates.begin(); iter != mapStates.end(); iter++)
  cout << token[iter->first]<< " " << iter->second << endl;
}

		//cout << Int << endl << Str << endl << Id;
  //cout << "Number of lines:" <<linenum<<endl;
  cout << "Number of unique lexemes for id:" << Id << endl;
  cout << "Number of unique lexemes for str:" << Str << endl;
  cout << "Number of unique lexemes for int:" << Int << endl;
  
  }
	else if(vmode)
	{
		while (true)
		{
			if (t1.getTok() == ERR || t1.getTok() == DONE)
      {
        cout<<t1.getTok();
				break;
			}
      t1 = (getToken(&file));
			cout << t1;
		}
   if (t1.getTok() != t2.getTok())
  {
  for (iter = mapStates.begin(); iter != mapStates.end(); iter++)
  cout << token[iter->first]<< " " << iter->second << endl;
  }

		//cout << Int << endl << Str << endl << Id;
  //cout << "Number of lines:" <<linenum<<endl;
  cout << "Number of unique lexemes for id:" << Id << endl;
  cout << "Number of unique lexemes for str:" << Str << endl;
  cout << "Number of unique lexemes for int:" << Int << endl;
  
	
	}

	return 0;
}



//if (t1.getTok() != t2.getTok())
//{
//	for (iter = mapStates.begin(); iter != mapStates.end(); iter++)
//		cout << iter->first << " " << iter->second << endl;
//}
////cout << Int << endl << Str << endl << Id;
//cout << "Number of unique lexemes for id:" << Id << endl;
//cout << "Number of unique lexemes for str:" << Str << endl;
//cout << "Number of unique lexemes for int:" << Int << endl;
