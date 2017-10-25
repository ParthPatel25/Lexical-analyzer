/// Parth G Patel
/// Project - 4 
/// Date - 12/14/2016




#include "p2lex.h"
#include <string>
#include <istream>
#include <fstream>
#include <map>
#include <ctype.h>

using namespace std;

map<string, string> myvector;
map<string, string>::iterator myvector_it;
int isString = 0, notInt = 0;
int temp = 0, flag = 0, Brkt = 0, temp1 = 0, te =0;
string M_DString, var = "", i = "";
int jandha = 0;


string StringPlusString(string s1, string s2);
string StringStarINT(string s1, int s2);



string ConvertToString(string a)
{
	string NewString = "";
	for (int i = 0; i<a.length(); i++)
	{
		if (a[i] != '"')
			NewString+= a[i];
	}
	//cout << NewString << endl;
	return NewString;
}

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

int linenum = 0;
int globalErrorCount = 0;

/// error handler
void error(string msg, bool showline = true)
{
	if (showline)
		cout << linenum << ": ";
	cout << msg << endl;
	++globalErrorCount;
}


/////////
//// this class can be used to represent the parse result
//// the various different items that the parser might recognize can be
//// subclasses of ParseTree

class ParseTree {
private:
	ParseTree *leftChild;
	ParseTree *rightChild;

	int	whichLine;

public:
	ParseTree(ParseTree *left = 0, ParseTree *right = 0) : leftChild(left), rightChild(right) {
		whichLine = linenum;
	}

	int onWhichLine() { return whichLine; }

	int traverseAndCount(int (ParseTree::*f)()) {
		int cnt = 0;
		if (leftChild) cnt += leftChild->traverseAndCount(f);
		if (rightChild) cnt += rightChild->traverseAndCount(f);
		return cnt + (this->*f)();
	}

	int countUseBeforeSet(map<string, int>& symbols) {
		int cnt = 0;
		if (leftChild) cnt += leftChild->countUseBeforeSet(symbols);
		if (rightChild) cnt += rightChild->countUseBeforeSet(symbols);
		return cnt + this->checkUseBeforeSet(symbols);
	}

	virtual int checkUseBeforeSet(map<string, int>& symbols) {
		return 0;
	}
	virtual int eval(){ return 0; }
	virtual int isPrint(){ return 0;}
	virtual int isSet(){ return 0; }
	virtual int isPlus() { return 0; }
	virtual int isStar() { return 0; }
	virtual int isBrack() { return 0; }
	virtual int isEmptyString() { return 0; }
};


class Slist : public ParseTree {
public:
	Slist(ParseTree *left, ParseTree *right) : ParseTree(left, right) {}
};

class PrintStmt : public ParseTree {
public:
	PrintStmt(ParseTree *expr) : ParseTree(expr) {}
	int isPrint() { return 1; }
};

class SetStmt : public ParseTree {
private:
	string	ident;

public:
	SetStmt(){}
	SetStmt(string id, ParseTree *expr) : ParseTree(expr), ident(id) {}
	int isSet() { return 1; }
	int checkUseBeforeSet(map<string, int>& symbols) {
		symbols[ident]++;
		return 0;
	}
};

class PlusOp : public ParseTree {

public:
	PlusOp(){}
	PlusOp(ParseTree *left, ParseTree *right) : ParseTree(left, right) {}
	
	int isPlus() { return 1; }
};

class StarOp : public ParseTree {
public:
	StarOp(ParseTree *left, ParseTree *right) : ParseTree(left, right) {}
	int isStar() { return 1; }
};

class BracketOp : public ParseTree {
private:
	Token sTok;

public:
	BracketOp(const Token& sTok, ParseTree *left, ParseTree *right = 0) : ParseTree(left, right), sTok(sTok) {}
	int isBrack() { return 1; }
};

class StringConst : public ParseTree {
private:
	Token sTok;

public:
	StringConst(const Token& sTok) : ParseTree(), sTok(sTok) {}

	string	getString() { return sTok.getLexeme(); }
	int isEmptyString() {
		if (sTok.getLexeme().length() == 2) {
			error("Empty string not permitted on line " + to_string((long long int)onWhichLine()), false);
			return 1;
		}
		return 0;
	}
};

//// for example, an integer...
class Integer : public ParseTree {
private:
	Token	iTok;

public:
	Integer(){}
	Integer(const Token& iTok) : ParseTree(), iTok(iTok) {}

	int	getInteger() { return stoi(iTok.getLexeme()); }
};

class Identifier : public ParseTree {
private:
	Token	iTok;

public:
	Identifier(const Token& iTok) : ParseTree(), iTok(iTok) {}

	int checkUseBeforeSet(map<string, int>& symbols) {
		if (symbols.find(iTok.getLexeme()) == symbols.end()) {
			error("Symbol " + iTok.getLexeme() + " used without being set at line " + to_string((long long int)onWhichLine()), false);
			return 1;
		}
		return 0;
	}
};

/// function prototypes
ParseTree *Program(istream *in);
ParseTree *StmtList(istream *in);
ParseTree *Stmt(istream *in);
ParseTree *Expr(istream *in);
ParseTree *Term(istream *in);
ParseTree *Primary(istream *in);
ParseTree *String(istream *in);


ParseTree *Program(istream *in)
{
	ParseTree *result = StmtList(in);

	// make sure there are no more tokens...
	if (GetAToken(in).getTok() != DONE)
		return 0;

	return result;
}


ParseTree *StmtList(istream *in)
{
	ParseTree *stmt = Stmt(in);
	//ParseTree *a;

	if (stmt == 0)
		return 0;

	if (jandha == 0)
	{
		if (temp != 0){
			cout << temp << endl;
			temp = 0;
			flag = 0;
			M_DString = "";
		}

		if (!M_DString.empty()){
			M_DString=ConvertToString(M_DString);
			cout << M_DString << endl;
			flag = 0;
			isString = 0;
			M_DString = "";
		}
	}
	return new Slist(stmt, StmtList(in));
}


ParseTree *Stmt(istream *in)
{
	Token t;

	t = GetAToken(in);

	if (t.getTok() == ERR) {
		error("Invalid token");
		return 0;
	}

	if (t.getTok() == DONE)
		return 0;

	if (t.getTok() == PRINT) {
		// process PRINT
		jandha = 0;
		ParseTree *ex = Expr(in);

		if (ex == 0) {
			error("Expecting expression after print");
			return 0;
		}

		if (GetAToken(in).getTok() != SC) {
			error("Missing semicolon");
			return 0;
		}

		return new PrintStmt(ex);
	}
	else if (t.getTok() == SET) {
		// process SET
		jandha = 1;
		Token tid = GetAToken(in);
		M_DString = "";
		isString = 0;
		if (tid.getTok() != ID) {
			error("Expecting identifier after set");
			return 0;
		}
		
		ParseTree *ex = Expr(in);

		if (ex == 0) {
			error("Expecting expression after identifier");
			return 0;
		}

		if (GetAToken(in).getTok() != SC) {
			error("Missing semicolon");
			return 0;
		}
		if (isString == 1)
		{
			notInt = 1;
			myvector_it = myvector.find(tid.getLexeme());
			if (myvector_it != myvector.end())
			{
				myvector_it->second = M_DString;
			}
			else
			myvector.insert(pair<string, string>(tid.getLexeme(), M_DString));
		}
		else
		{
			notInt = 0;
			myvector_it = myvector.find(tid.getLexeme());
			if (myvector_it != myvector.end())
			{
				myvector_it->second = to_string((long long int)temp);
			}
			else
			myvector.insert(pair<string, string>(tid.getLexeme(), to_string((long long int)temp)));
		}
			

		return new SetStmt(tid.getLexeme(), ex);
	}
	else {
		error("Syntax error, invalid statement");
	}

	return 0;
}


ParseTree *Expr(istream *in)
{
	ParseTree *exp = Term(in);
	PlusOp value;
	if (exp == 0) return 0;

	while (true) {

		Token t = GetAToken(in);

		if (t.getTok() != PLUS) {
			PushbackToken(t);
			break;
		}
		else
		{
			flag = 1;
		}

		ParseTree *exp2 = Term(in);
		if (exp2 == 0) {
			error("missing operand after +");
			return 0;
		}

		exp = new PlusOp(exp, exp2);
	}

	return exp;
}


ParseTree *Term(istream *in)
{
	ParseTree *pri = Primary(in);

	if (pri == 0) return 0;

	while (true) {

		Token t = GetAToken(in);

		if (t.getTok() != STAR) {
			PushbackToken(t);
			break;
		}
		else
		{
			flag = 2;
		}

		ParseTree *pri2 = Primary(in);
		if (pri2 == 0) {
			error("missing operand after *");
			return 0;
		}

		pri = new StarOp(pri, pri2);
	}

	return pri;
}

ParseTree *Primary(istream *in)
{
	
	Token t = GetAToken(in);

	if (t.getTok() == ID) {																///some changes need to be require......

		int no = 0;
		myvector_it = myvector.find(t.getLexeme());
		///cout << isdigit(ids_it->second[0]) << endl;
		if (myvector_it != myvector.end())
		{
			if (!isdigit(myvector_it->second[0]))
			{
				M_DString = myvector_it->second;
			}
			else
			{
				no = stoi(myvector_it->second);
			}
		}

		if (flag == 1)
		{
			M_DString = i + M_DString;
			flag = 0;
		}

		else if (flag == 2)
		{
			if (!M_DString.empty())
			{
				M_DString = StringStarINT(M_DString, temp);
				temp = 0;
			}
			else
				temp *= no;
		}
		else
			temp = no;
		i = M_DString;
		return new Identifier(t);
	}
	else if (t.getTok() == INT) {
		int num = 0;
		Integer no(t);
		num = no.getInteger();
		
		if (Brkt == 1)
		{
			if (flag == 1)
			{
				temp1 += num;
			}
			else if (flag == 2)
			{
				if (isString == 1)
					temp1 = 1;
				temp1 *= num;
			}
			else
				temp1 = num;
		}
		else if (flag == 1)
		{
			temp += num;
			flag = 0;
		}
		else if (flag == 2)
		{
			if (!M_DString.empty() || isString == 1)
				temp = 1;
			temp *= num;
			
			if (!M_DString.empty())
			{
				M_DString = StringStarINT(M_DString, temp);
				temp = 0;
			}
			flag = 0;
		}
		else
			temp = num;
		return new Integer(t);
	}
	else if (t.getTok() == STR)
	{
		isString = 1;
		StringConst stob(t);
		var = stob.getString();
		if (var == "\"\"")
		{
			error("Empty string not permitted on line:");
			return 0;
		}
		if (flag == 1)
		{
			M_DString = StringPlusString(M_DString, var);
			i = M_DString;
			flag = 0;
		}
		else if (flag == 2)
		{
			M_DString = StringStarINT(var, temp);
			temp = 0;
		}
		else
		{
			M_DString = var;
		}
		PushbackToken(t);
		return String(in);
	}
	else if (t.getTok() == LPAREN)
	{
		if (Brkt == 0)
		{
			if (temp != 0 || !M_DString.empty())
			{
				te = flag;
				flag = 0;
				Brkt = 1;
			}
		}
		else
		{
			flag = te;
			if (flag == 1)
			{
				temp += temp1;
			}
			else if (flag == 2)
			{
				if (temp != 0)
					temp1 = 1;
				temp *= temp1;
			}
			Brkt = 0;
		}
		ParseTree *ex = Expr(in);
		if (ex == 0)
			return 0;
		t = GetAToken(in);
		if (t.getTok() != RPAREN) {
			error("expected right parens");
			return 0;
		}

		if (Brkt == 1)
		{
			flag = te;
			if (flag == 1)
			{
				temp += temp1;
			}
			else if (flag == 2)
			{
				if (!M_DString.empty())
					temp = 1;
				temp *= temp1;
				if (!M_DString.empty())
				{

					M_DString = StringStarINT(M_DString, temp);
					temp = 0;
				}
			}
			Brkt = 0;
		}
		return ex;
	}
	
	return 0;
}


ParseTree *String(istream *in)
{
	Token t = GetAToken(in); // I know it's a string!
	ParseTree *lexpr, *rexpr;
	Token lb = GetAToken(in);
	
	if (lb.getTok() != LEFTSQ) {
	//	M_DString = var;
		PushbackToken(lb);
		return new StringConst(t);
	}
	

	lexpr = Expr(in);
	if (lexpr == 0) {
		error("missing expression after [");
		return 0;
	}

	lb = GetAToken(in);
	if (lb.getTok() == RIGHTSQ) {
		return new BracketOp(t, lexpr);
	}
	else if (lb.getTok() != SC) {
		error("expected ; after first expression in []");
		return 0;
	}

	rexpr = Expr(in);
	if (rexpr == 0) {
		error("missing expression after ;");
		return 0;
	}

	lb = GetAToken(in);
	if (lb.getTok() == RIGHTSQ) {
		return new BracketOp(t, lexpr, rexpr);
	}

	error("expected ]");
	return 0;
}


string StringPlusString(string s1, string s2)
{
	string s = "";
	s = s1 + s2;
	return s;
}
string StringStarINT(string s1, int s2)
{
	string NewString = "";
	for (int i = 0; i < s2; i++)
	{
		NewString += s1;
	}
	return NewString;
}


int
main(int argc, char *argv[])
{
	ifstream infile(argv[1]);

	ParseTree *prog = Program(&infile);

	if (prog == 0 || globalErrorCount != 0) {
		return 0;
	}
	return 0;
}