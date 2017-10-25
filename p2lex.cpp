//Name: Parth G Patel
//Class: CS 280


#include <string>
#include <map>
#include <stdio.h>
#include <vector>
#include<iomanip>
#include <algorithm>

#include "p2lex.h"

using namespace std;
//extern string token[] = { "id", "str", "int", "plus", "star", "leftsq", "rightsq", "print", "set", "sc", "lparen", "rparen", "done", "err" };
extern map<TokenType, int> mapStates;
extern map<TokenType, int>::iterator iter;
extern vector<string> myvector; extern int mycount, Int,Str,Id;

Token getToken(std::istream *instream)
{
	string lexeme;
	char m;
	while (m=instream->get())
	{
		if (isdigit(m))										// This IF statement count the digits only if there is an +,-,*,\,
		{													// any character that is not digit does not take it 
			while (m != ' ' && m != '\n' && m != '\r')
			{
				if (!isdigit(m))
				{
					instream->unget();
					break;
				}
				lexeme.push_back(m);
				m=instream->get();
			}
			return Token(INT, lexeme);
		}
		if (m == '/')										// this IF statement remove the comment frome the line..
		{													// every character after this "//" are not include until the new line
			m=instream->get();
			if (m == '/')
			{
				m=instream->get();
				while (m)
				{
					if (m == '\r' || m == '\n')
					{
						break;
					}
					m=instream->get();
				}
			}
		}
   if (m=='\n')
   {
   linenum++;
   }
		switch (m)
		{
			case '+':
			{
				//plus++;
				return Token(PLUS, "+");
			}
			case '*':
			{
				//star++;
				return Token(STAR, "*");
			}
			case '[':
				return Token(LEFTSQ,"[");
			case ']':
				return Token(RIGHTSQ,"]");
			case ';':
				return Token(SC, ";");
			case '(':
				return Token(LPAREN,"(");
			case ')':
				return Token(RPAREN,")");
			case EOF:
				return Token(DONE, "");
			default:
				break;
		}
		if (m == '"')
		{
			lexeme.push_back(m);
			m=instream->get();
			if (isalpha(m))
			{
				while (m != '"')
				{
					if (m == '\r' || m == '\n')
						return Token(ERR, lexeme);
					lexeme.push_back(m);
					m=instream->get();
				}
				lexeme.push_back(m);
				return Token(STR, lexeme);
			}
		}
		if (isalpha(m))
		{
			while (m != ' ' && m!='\n' && m!='\r')
			{
				if (m == EOF)
				{
					break;
				}
				if (!isalpha(m))
				{
					//instream->unget();
					break;
				}
				lexeme.push_back(m);
				m=instream->get();
			}

			if (lexeme == "set")
				return Token(SET, lexeme);
			else if (lexeme == "print")
				return Token(PRINT, lexeme);
			else
				return Token(ID, lexeme);
			//continue;
		}
	}
	return Token(DONE,"");
}


ostream& operator<<(ostream& out,const Token& t) 
{ 
	string token[] = { "id", "str", "int", "plus", "star", "leftsq", "rightsq", "print", "set", "sc", "lparen", "rparen", "done", "err" };
	if (t.getTok() == 0)
	{
		myvector.push_back((t.getLexeme()));
		if (mycount = count(myvector.begin(), myvector.end(), (t.getLexeme())) <= 1)
		{
			Id++;
		}
		mapStates[t.getTok()] += 1;
		out << token[0]<< "(" << t.getLexeme() << ")" << endl;
		return out;
	}
	if (t.getTok() == 1)
	{
		myvector.push_back((t.getLexeme()));
		if (mycount = count(myvector.begin(), myvector.end(), (t.getLexeme())) <= 1)
		{
			Str++;
		}
		mapStates[t.getTok()] += 1;
		out << token[1]<< "(" << t.getLexeme() << ")" << endl;
		return out;
	}
	if (t.getTok() == 2)
	{
		myvector.push_back((t.getLexeme()));
		if (mycount = count(myvector.begin(), myvector.end(), (t.getLexeme())) <= 1)
		{
			Int++;
		}
		mapStates[t.getTok()] += 1;
		out << token[2]<< "(" << t.getLexeme() << ")" << endl;
		return out;
	}
	if (t.getTok() == 13)
	{
    mapStates[t.getTok()]+=1;
		out << token[13] << "(" << t.getLexeme() << ")" << endl;
		return out;
	}
	if (t.getTok() != 12)
	{
		mapStates[t.getTok()] += 1;
		out << token[t.getTok()] << endl;
	}
	return out;
}

