
// Name : Parth G Patel
// Class : CS 280
// Prof : Sean Cusack
// Project : 3


#include "p2lex.h"
#include <string>
#include <istream>
#include <fstream>
#include <map>
#include <vector>
using namespace std;

extern string getPrintName(TokenType t);

map<string,string> myvector;
map<string,string>::iterator myvector_it;

/////////
//// this class can be used to represent the parse result
//// the various different items that the parser might recognize can be
//// subclasses of ParseTree

/// Token Wrapper
Token saved;				
bool isSaved = false;
Token GetAToken(istream *in) {
	if (isSaved) {
		isSaved = false;
		return saved;
	}
	return getToken(in);
}

void PushbackToken(Token& t) {
	if (isSaved) {
		cerr << "Can't push back more than one token!!!";
		exit(0);
	}
	saved = t;
	isSaved = true;
}

class ParseTree {
private:
	ParseTree *leftChild;
	ParseTree *rightChild;
	int	whichLine;

public:
	ParseTree(ParseTree *left = 0, ParseTree *right = 0) : leftChild(left), rightChild(right) {
		whichLine = linenum;
	}
	void printleft()
	{
		cout << leftChild; 
	}
	void printright()
	{
		cout << rightChild;
	}
	int onWhichLine() { return whichLine; }
};



//// for example, an integer...
class Integer : public ParseTree {
private:
	Token	iTok;

public:
	Integer(const Token& iTok) : ParseTree(), iTok(iTok) {}

	int	getInteger() { return stoi(iTok.getLexeme()); }
};

///// an string
class SString : public ParseTree {
private:
	Token	iTok1;
public:
	SString(const Token& iTok) : ParseTree(), iTok1(iTok) {}
	string	getString() { return iTok1.getLexeme(); }
};
class print : public ParseTree
{
public:
	print(ParseTree* exe) : ParseTree(exe){}
};
class set : public ParseTree
{
public:
	set(ParseTree* exe) : ParseTree(exe){}
};



int linenum = 0;
int globalErrorCount = 0;

/// error handler
void error(string msg)
{
	cout << msg << ": " << linenum << endl;
	++globalErrorCount;
}

/// function prototypes
ParseTree *Program(istream *in);
ParseTree *StmtList(istream *in);
ParseTree *Stmt(istream *in);
ParseTree *Expr(istream *in);
ParseTree *Term(istream *in);
ParseTree *Primary(istream *in);
ParseTree *String(istream *in);
string strvar = "";
int pl = 0;
int star =0;
int square = 0;


ParseTree *Program(istream *in)
{
	ParseTree *result = StmtList(in);

	cout << "Count of + operators: " << pl << endl;
	cout << "Count of * operators: " << star << endl;
	cout << "Count of [] operators: " << square << endl;

	if (getToken(in).getTok() != DONE)
		return 0;
	return result;
}


ParseTree *StmtList(istream *in)
{
	ParseTree *result = Stmt(in);
	Token t;
	t = GetAToken(in);
	if (t.getTok() == PRINT || t.getTok() == SET)
	{
		PushbackToken(t);
		result = StmtList(in);
	}
	else if (t.getTok() == DONE)
	{
		//error("Syntax error, invalid statement");
		return result;
	}
	else
	{
		error("Syntax error, invalid statement");
		return 0;
	}
	return result;
}


ParseTree *Stmt(istream *in)
{

	Token t;
	//print *result = 0;
	ParseTree *result = 0;
	//ParseTree *result = Expr(in);
	t = GetAToken(in);

	if (t.getTok() == PRINT) 
	{
		result = Expr(in);
		t = GetAToken(in);
		if (t.getTok() != SC || t.getTok()==ERR) 
		{
			error("expected semi colon");
			return 0;
		}
		return new print(result);
	}
	else if (t.getTok() == SET) 
	{
		t = GetAToken(in);
		if (t.getTok() == ID)
		{
			myvector.insert(pair<string,string>(t.getLexeme()," "));
			result = Expr(in);
			t = GetAToken(in);
			if (t.getTok() != SC)
			{
				error("expected semi colon");
				return 0;
			}
			return new set(result);
		}
		else
		{
			error("expected a ID");
		}
	}
	else
		error("Invalid statement");

	return result;
}


ParseTree *Expr(istream *in)
{
	ParseTree *result = 0;
	Token t;
		result = Term(in);
		t = GetAToken(in);
		if (t.getTok() == PLUS)
		{
			pl += 1;
			result = Expr(in);
		}
		else
		{
			PushbackToken(t);
		}
	return result;
}


ParseTree *Term(istream *in)
{
	ParseTree *result = 0;
	Token t;
		result = Primary(in);
		t = GetAToken(in);
		if (t.getTok() == STAR)
		{
			star += 1;
			result = Term(in);
		}
		else
		{
			PushbackToken(t);
		}
	
	return result;
}


ParseTree *Primary(istream *in)
{
	Token t = GetAToken(in);

	if (t.getTok() == ID) 
	{
		myvector_it = myvector.find(t.getLexeme());
		if (myvector_it != myvector.end())
		{
			
		}
		else
		{
			error("Symbol " + t.getLexeme() + " used without being set at");
		}
	}
	else if (t.getTok() == INT) 
	{
		return new Integer(t);
	}
	else if (t.getTok() == STR) 
	{
		ParseTree *result = 0;
		strvar = t.getLexeme();
		
		if (strvar == "\"\"")
		{
			error("Empty string not permitted on line");
			return 0;
		}
		else
		{
			ParseTree *result = String(in);
		}
		return result;
	}
	else if (t.getTok() == LPAREN) 
	{
		ParseTree *ex = Expr(in);
		t = GetAToken(in);
		if (t.getTok() != RPAREN) 
		{
			error("expected right parens");
			return 0;
		}
		return ex;
	}
	else
	{
		error("Invalid statement");
	}

	return 0;
}


ParseTree *String(istream *in)
{
	Token t = GetAToken(in);
	
		if (t.getTok() == LEFTSQ)
		{
			square += 1;
			ParseTree *ex = Expr(in);
			if (ex == 0)							
				return 0;							
			t = GetAToken(in);						
			if (t.getTok() == SC)					
			{
				ParseTree *ex = Expr(in);			
				if (ex == 0)						
				{
					square -= 1;
					error("expected an Expr");
					return 0;						
				}
				t = GetAToken(in);					 
				if (t.getTok() != RIGHTSQ)			
				{
					square -= 1;
					error("expected right square");		
					return 0;							
				}
			}
			else if (t.getTok() != RIGHTSQ)			
			{
				square -= 1;
				error("expected right square");		
				return 0;							
			}
			return ex;								
		}
		else
		{
			PushbackToken(t);
			return new SString(t);
		}
	return 0;
}




int main(int argc, char* argv[])
{
        fstream readfile(argv[1]);
        ParseTree *prog = Program(&readfile);

        if (prog == 0 || globalErrorCount != 0)
        {
                cout << "Parse failed, exiting" << endl;
                return 0;
        }

        cout << "Success. Congrats!" << endl;
        return 0;
}


