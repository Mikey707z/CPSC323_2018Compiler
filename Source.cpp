//Names: Dhvanil Shah
//Due Date: 3/5/2018
//Class: CPSC-323-04
//Project 1

#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<stdlib.h>
#include<algorithm>
#include<iomanip>
using namespace std;

//Create variables to store keywords, operators, and separators
const char keywords[] = { "function, int, boolean, real, if, endif, else, return, put, get, while, true, false" };
const string oper = "+-*/";
const string sps = "[]{}():;,";
const string compound_ops = "=<>^";
const string compound_sps = "%";



//Create an enumerated state transition type
enum state {
	first, //Starting State
	a,	//Initial State for recognizing Identifiers
	b,	//State for recognizing letters of Identifiers
	c,	//Final State for a Identifier ending with a letter, and declare a token
	d,	//Final State for a Identifier ending with a '$' sign, and declare a token
	e,	//State for recognizing digits within Identifiers
	f,  //Transition state for recognizing Integers, and declares a token if it's a valid Integer
	g,  //Transition state for recognizing Reals, and declares a token it it's a valid Real
	h	//Initial State for recognizing opearators and separators
};


//Declare a state type object called "go" and set it to the starting state
state go = first;


//Function to determine tokens and lexemes from
//Accepts a pointer to an array of char, inputted from a text file
void lex(char *buffer, ofstream &out) {
	
	//Declare Current character pointer, and a start pointer
	char *cc, *st;
	
	//Set those pointers to point to the location where the characters from text file are stored
	cc = st = buffer;
	
	//Initialize a variable that will concatenate characters once a token is found
	string toks = "";
	
	//Print the heading of the output
	cout << setw(20) << left << "Token" << "Lexeme\n" << setw(20) << right;
	//Send to output file
	out << setw(20) << left << "Token" << "Lexeme\n" << setw(20) << right;
	cout << std::string(35, '_') << endl;
	out << std::string(35, '_') << endl;
	
	//Loop that will iterate through the array, until the end of file character is read
	do {
		//Internal loop that will iterate through each character of array 
		//until an accepting state is found for each token
		
		switch (go) {

		//Case for the starting state
		case first:
			st = cc;
			//Statement that will ignore all whitespaces, horizontal tabs, and newline feeds and take the state back to initial state
			if (*cc == char(32) || *cc == char(9) || *cc == char(10)) {
				while (*cc == char(32) || *cc == char(9) || *cc == char(10)) {
					cc++;
				}
				st = cc;
				go = first;
				break;
			}
			//Statement that will recognize a comment, and annouce a token for comment
			if (*cc == char(33)) {
				cc++;
				while (*cc != char(33)) {
					cc++;
				}
				cc++;
				cout << setw(20) << left << "Comment" << "!...!\n" << setw(20) << right;;
				//Send to output file
				out << setw(20) << left << "Comment" << "!...!\n" << setw(20) << right;;
				st = cc;
				go = first;
				break;
			}

			//If a digit is read, the statement will transition the go object to f, which
			//is the state that will declare an integer token if valid integer is found
			if (isdigit(*cc)) {
				go = f;
				break;
			}
			//If a letter is read, the statement will transition the go object to a, which
			//is the starting state for recognizing identifiers/keywords
			if (isalpha(*cc)) {
				go = a;
				break;
			}
			//If an operator, or separator is read, the statement will transition the go object
			//to c, that will annouce opearators or separators
			if (oper.find(*cc) != std::string::npos || sps.find(*cc) != std::string::npos || 
				compound_ops.find(*cc) != std::string::npos ||compound_sps.find(*cc) != std::string::npos){
			go = h;
			break;
			}
			
			break;
		//End case first

		//Case a
		case a: {
			//If the character read is a letter, the statement will transition 'go' to state b
			 if (isalpha(*cc)) {
				go = b;
				cc++;
			}
			break;
		}
		//End case a

		//Case b
		case b: {
			//If a '$' sign is read, go to state d
			if (*cc == char(36)) {
				go = d;
				cc++;
			}
			//Continue to be in stat b until letters are being read
			else if (isalpha(*cc)) {
				go = b;
				cc++;
			}
			//If a digit is read, go to state e
			else if (isdigit(*cc)) {
				go = e;
				cc++;
			}
			//Go to state c if a space, tab, new line is read, or if an opearator or separator is read
			else if (*cc == char(32) || oper.find(*cc) != std::string::npos || 
				sps.find(*cc) != std::string::npos || *cc == (10)
				|| *cc == char(9) || compound_ops.find(*cc) != std::string::npos ||
				compound_sps.find(*cc) != std::string::npos) {
				go = c;
			}
			break;
		}
		//End case b

		//Case c
		case c: {
			//Set the toks variable to empty
			toks = "";
			//Loop until the start pointer is not equal to the current character pointer
			while (st != cc) {
				//Concatenate characters to toks variable
				toks += *st;
				st++;
			}

			//Announce a token identifer or keyword
			if (toks == "function" || toks == "int" || toks == "boolean" || toks == "real" ||
				toks == "if" || toks == "endif" || toks == "else" || toks == "return" ||
				toks == "put" || toks == "get" || toks == "while" || toks == "true" || toks == "false"){
				cout << setw(20) << left << "Keyword" << toks << setw(20) << right << endl;
			}
			else {
				cout << setw(20) << left << "Idenfier" << toks << setw(20) << right << endl;
				//Send to output file
				out << setw(20) << left << "Idenfier" << toks << setw(20) << right << endl;
			}
			go = first;
			break;
		}
		
				//End case c

				//Case d
		case d: {
			//Announce a token since a $ is marks an end of an identifier
			toks = "";
			while (st != cc) {
				toks += *st;
				st++;
			}
			cout << setw(20) << left << "Identifier" << toks << setw(20) << right << endl;
			//Send to output file
			out << setw(20) << left << "Identifier" << toks << setw(20) << right << endl;
			st = cc;
			go = first;
			break;
		}	//End case d

			//Case e
		case e: {
			//If a digit is read inbetween identifer, continue to accept digits
			if (isdigit(*cc)) {
				cc++;
				go = e;
			}
			//If a letter is read, go to state b
			else if (isalpha(*cc)) {
				go = b;
				cc++;
			}
			//If a $ sign is read after a digit, go to state d
			else if (*cc == char(36)) {
				go = d;
				cc++;
			}
			//If anything except a letter or $ sign is read after a digit, announce an invalid identifier
			else if (*cc == char(32) || *cc == char(9) ||*cc == char(10)||
				oper.find(*cc) != std::string::npos || sps.find(*cc) != std::string::npos ||
				compound_ops.find(*cc) != std::string::npos ||compound_sps.find(*cc) != std::string::npos) {
				toks = "";
				while (st != cc) {
					toks += *st;
					st++;
				}
				cout << setw(20) << left << toks << "Invalid Idenfier\n" << setw(20) << right;
				//Send to output file
				out << setw(20) << left << toks << "Invalid Idenfier\n" << setw(20) << right;
				go = first;
			}
			break;
			//End case e

			//Case f
		case f:
			//If a digit is read after the initial state, continue to read digits until a non-digit character is read
			st = cc;
			if (isdigit(*cc)) {
				toks = "";
				while (isdigit(*cc)) {
					toks += *cc;
					cc++;
				}
				//If a floating point is read after an integer, go to state g
				if (*cc == char(46)) {
					go = g;
					break;
				}
				//Announce a valid integer
				else {
					cout << setw(20) << left << "Integer" << toks << setw(20) << right << endl;
					//Send to output file
					out << setw(20) << left << "Integer" << toks << setw(20) << right << endl;
					st = cc;
					go = first;
					break;
				}
			}//End f

			 //Case g
		case g: {
			//Concatenate the digits before the floating point
			toks = "";
			while (st != cc) {
				toks += *st;
				st++;
			}
			cc++;
			//Concatenate the floating point and the digits after the floating point
			while (isdigit(*cc)) {
				cc++;
			}while (st != cc) {
				toks += *st;
				st++;
			}
			//Announe a Real
			cout << setw(20) << left << "Real" << toks << setw(20) << right << endl;
			//Send to output file
			out << setw(20) << left << "Real" << toks << setw(20) << right << endl;
			st = cc;
			go = first;
			break;
		}
			//End case g

			//Case h
		case h: {
			//If an operator (+-*/) is read go to the following statement
			if (oper.find(*cc) != std::string::npos) {
				//Check if the variable is an operator, or a valid unary variable, or not
				char * unary = cc;
				switch (*cc) {
					//If the operator is +, /, *, check that its a valid operator, and not a unary 
				case '+': case '/': case '*': {
					unary--;
					if (*unary == char(32)) {
						while (*unary == char(32)) {
							unary--;
						}
					}
					//If the operator (+,/,*) turns out to be unary, announce an invalid unary operator 
					if (sps.find(*unary) != string::npos || compound_ops.find(*unary) != string::npos || compound_sps.find(*unary) != string::npos) {
						cout << setw(20) << left << "Invalid Unary" << *cc << setw(20) << right << endl;
						//Send to output file
						out << setw(20) << left << "Invalid Unary" << *cc << setw(20) << right << endl;
						cc++;
						go = first;
						break;
					}//Else announce a valid unary operator
					else {
						cout << setw(20) << left << "Operator" << *cc << setw(20) << right << endl;
						//Send to output file
						out << setw(20) << left << "Operator" << *cc << setw(20) << right << endl;
						cc++;
						go = first;
						break;
					}
				}
						  //If the operator read is a '-', check if its a unary operator, or a minus sign 
				case '-': {
					unary--;
					if (*unary == char(32)) {
						while (*unary == char(32)) {
							unary--;
						}
					}
					//If its a valid unary operator, announce a valid unary operator
					if (sps.find(*unary) != string::npos || compound_ops.find(*unary) != string:: npos|| compound_sps.find(*unary) != string::npos) {
						cout << setw(20) << left << "Unary Operator" << *cc << setw(20) << right << endl;
						//Send to output file
						out << setw(20) << left << "Unary Operator" << *cc << setw(20) << right << endl;
						cc++;
						go = first;
						break;
					}
					//Else announce a minus sign operator 
					else {
						cout << setw(20) << left << "Operator" << *cc << setw(20) << right << endl;
						//Send to output file
						out << setw(20) << left << "Operator" << *cc << setw(20) << right << endl;
						cc++;
						go = first;
						break;
					}
				
				
				}
				}
			}//End Switch
			//If the operator read is a separator, announce a separator token
			else if (sps.find(*cc) != std::string::npos) {
				
				cout << setw(20) << left << "Separator" << *cc << setw(20) << right << endl;
				//sendto output file
				out << setw(20) << left << "Separator" << *cc << setw(20) << right << endl;
				cc++;
				st = cc;
				go = first;
				break;
			}
			//Else if an operator that is read is a =,<,>,^, or %, check whether or not its a compound operator or compound separator
			else if (compound_ops.find(*cc) != std::string::npos || compound_sps.find(*cc) != std::string::npos) {
				char * f;
				f = cc;
				f++;
				st = f;
				//If it is a compound operator or compound separator, announce a token
				if (*f == char(9) || *f == char(10) || *f == char(32) || isalpha(*f) || isdigit(*f)
					|| oper.find(*f) != string::npos || sps.find(*f) != string::npos){
					if (compound_ops.find(*cc) != std::string::npos) {
						cout << setw(20) << left << "Operator" << *cc << setw(20) << right << endl;
						//Send to output file
						out << setw(20) << left << "Operator" << *cc << setw(20) << right << endl;
					}
				
					else if (compound_sps.find(*cc) != std::string::npos) {
						cout << setw(20) << left << "Separator" << *cc << setw(20) << right << endl;
						//Send to output file
						out << setw(20) << left << "Separator" << *cc << setw(20) << right << endl;
					}
					st = cc = f;
					go = first;
					break;
				}
				//Else if it isn't a compound operator or separator, annouce a regualr operator token
				else if (compound_ops.find(*f) != std::string::npos || compound_sps.find(*f) != std::string::npos) {
					toks = "";
					f++;
					while (cc != f) {
						toks += *cc;
						cc++;
					}
					if (compound_ops.find(*st) != std::string::npos) {
						cout << setw(20) << left << "Operator" << toks << setw(20) << right << endl;
						//Sendto output file
						out << setw(20) << left << "Operator" << toks << setw(20) << right << endl;

					}
					else if (compound_sps.find(*st) != std::string::npos) {
						cout << setw(20) << left << "Separator" << toks << setw(20) << right << endl;
						//Send to output file
						out << setw(20) << left << "Separator" << toks << setw(20) << right << endl;
					}
					st = cc = f;
					go = first;
					break;
				}
				//If all the statements fail, announce an invalid operator
				else {
					cout << setw(20) << left << *cc << "Inavlid\n" << setw(20) << right << endl;
					//Send to output file
					out << setw(20) << left << *cc << "Inavlid\n" << setw(20) << right << endl;
					cc++;
					go = first;
					break;
				}
			}

			break;
		}
			//End case h

		//If all else fails, announce an error
		default:
			cout << "Error\n";
			//Send to output file
			out << "Error\n";
			break;
		}

		}//End Switch
		//Loop until the End Of File character is not read
	} while (*cc != EOF);//End While

}//End Lex



int main() {

	cout << endl;
	//Create a ifstream variable

	ifstream in("testcase1.txt");
	ofstream out("outputcase.txt");
	//Create a string variable
	string message;
	while (in) {
		//Read in the character of the source file into a string
		message.push_back(in.get());

	}
	//create a pointer to the string of characters
	char* strp;
	strp = &message[0];
	out << message << endl << endl;
	//Pass the pointer to the lex function
	lex(strp,out);

	cout << endl << endl;
	system("Pause");
	return 0;
}