//Names: Dhvanil Shah
//       Ruby Abutaleb
//Due Date: 4/9/2018
//Class: CPSC-323-04
//Project 2

#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<stdlib.h>
#include<algorithm>
#include<iomanip>
using namespace std;

//Create variables to store keywords, operators, and separators
const string oper = "+-*/";
const string sps = "[]{}():;,";
const string compound_ops = "=<>^";
const string compound_sps = "%";

//Create Token Struct for doubly linked list used by Parser
struct Token{
    int type; // 0 = identifier, 1 = real, 2 = int, 3 = keywords, 4 = operators, 5 = seperators
    string lexeme;
    int line;
    struct Token *next;
    struct Token *prev;
    
    //set type function
    void setType(int num){
        type = num;
    }
    //set lexeme function
    void setLex(string var){
        lexeme = var;
    }
    
    //set line number
    void setLine(int num){
        line = num;
    }
    
    //get type function
    int getType(){
        return type;
    }
    //get lexeme function
    string getLex(){
        return lexeme;
    }
    //get line number
    int getLine(){
        return line;
    }
};


//Create an enumerated state transition type
enum state {
    first, //Starting State
    a,    //Initial State for recognizing Identifiers
    b,    //State for recognizing letters of Identifiers
    c,    //Final State for a Identifier ending with a letter, and declare a token
    d,    //Final State for a Identifier ending with a '$' sign, and declare a token
    e,    //State for recognizing digits within Identifiers
    f,  //Transition state for recognizing Integers, and declares a token if it's a valid Integer
    g,  //Transition state for recognizing Reals, and declares a token it it's a valid Real
    h    //Initial State for recognizing opearators and separators
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
            }    //End case d
                
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

void synt(Token before, Token curr, Token after){
    switch(curr.type){
        //case for identifiers
        case 0:
            if (before.lexeme == "function" || before.lexeme == "int" || before.lexeme == "boolean" || before.lexeme == "real"){
                if(after.lexeme == "," || after.lexeme == "="){
                    if(before.lexeme == "function")
                        cout << "<Function Definitions'> -> <Function> <Function Definitions'>\n" << "<Function> -> function <Identifier>  [ <Opt Parameter List> ]  <Opt Declaration List>  <Body>\n";
                    else if(before.lexeme == "int" || before.lexeme == "boolean" || before.lexeme == "real"){
                        if (after.lexeme == ",")
                        cout << "<Decleration List'> -> <Decleration>\n" << "<Decleration> -> <int | boolean | real > <IDs>\n" << "<IDs> -> <Identifier>, <IDs>\n";
                        else
                           cout << "<Decleration List'> -> <Decleration>\n" << "<Decleration> -> <int | boolean | real > <IDs>\n" << "<IDs> -> <Identifier>\n";
                    }
                }
                else{
                    cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                }
            }
            else{
                cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
            }
            break;
        //case for real
        case 1:
            if(before.lexeme == "-")
            {
                cout << "<Factor> -> - <Primary>\n" << "<Primary> -> <Real>\n";
            }
            else {
                cout << "<Factor> -> <Primary>\n" << "<Primary> -> <Real>\n";
            }
            break;
        //case for integers
        case 2:
            if(before.lexeme == "-")
            {
                cout << "<Factor> -> - <Primary>\n" << "<Primary> -> <Integer>\n";
            }
            else {
                cout << "<Factor> -> <Primary>\n" << "<Primary> -> <Integer>\n";
            }
            break;
        //case for keywords
        case 3:
            //return
            if(curr.lexeme == "return"){
                    //check if second is an expression : if "-", identifier, integer, "(", real, "true", "false" empty???
                    //possible productions: Statement -> Return -> return;
                    //or                    Statement -> Return -> return; <Expression>
                    if(before.lexeme == "-" || after.type == 0 || after.type == 2 || after.lexeme == "(" || after.type == 1 ||after.lexeme == "true" || after.lexeme == "false"){
                        cout << "Statement> -> <Return>\n" << "<Return> -> return; <Expression>\n";
                    }
                    else {
                         cout << "<Statement> -> <Return>\n" << "<Return> -> return;\n";
                    }
                }
            // function
            else if (curr.lexeme == "function"){
                if (after.type == 0){
                  cout << "<Function Definitions’> -> <Function> <Function Definition’>\n" << "<Function> -> function  <Identifier>  [ <Opt Parameter List> ]  <Opt Declaration List>  <Body>";
                }
                else{
                    cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                }
            }
            // int
            else if (curr.lexeme == "int"){
                if (before.lexeme == ":"){
                    cout << "<Parameter> -> <IDs> : <Qualifier>\n" << "<Qualifier> -> int\n";
                }
                else{
                    cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                }
            }
            // boolean
            else if (curr.lexeme == "boolean"){
                if (before.lexeme == ":"){
                    cout << "<Parameter> -> <IDs> : <Qualifier>\n" << "<Qualifier> -> boolean\n";
                }
                else{
                    cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                }
            }
            // real
            else if (curr.lexeme == "real"){
                if (before.lexeme == ":"){
                    cout << "<Parameter> -> <IDs> : <Qualifier>\n" << "<Qualifier> -> real\n";
                }
                else{
                    cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                }
            }
            else if (curr.lexeme == "if"){
                cout << "\n";
            }
            else if (curr.lexeme == "endif"){
                cout << "\n";
            }
            else if (curr.lexeme == "else"){
                cout << "\n";
            }
            // put
            else if (curr.lexeme == "put"){
                if (after.lexeme == "("){
                    cout << "<Statement> -> <Print>\n" << "<Print> -> put (<Expression>);\n";
                }
                else{
                    cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                }
            }
            // get
            else if (curr.lexeme == "get"){
                if (after.lexeme == "("){
                    cout << "<Statement> -> <Scan>\n" << "<Scan> -> get (<IDs>);\n";
                }
                else{
                    cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                }
            }
            // while
            else if (curr.lexeme == "while"){
                if (after.lexeme == "("){
                    cout << "<Statement> -> <While>\n" << "<While> -> while (<Condition>) <Statement>\n";
                }
                else{
                    cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                }
            }
            // true
            else if (curr.lexeme == "true"){
                if(before.lexeme == "-")
                    cout << "<Factor> -> - <Primary>\n" << "<Primary> -> true\n";
                else
                    cout << "<Factor> -> <Primary>\n" << "<Primary> -> true\n";
            }
            // false
            else if (curr.lexeme == "false"){
                if(before.lexeme == "-")
                    cout << "<Factor> -> - <Primary>\n" << "<Primary> -> false\n";
                else
                    cout << "<Factor> -> <Primary>\n" << "<Primary> -> false\n";
            }
            break;
            
        //case for operators "+-*/"
        case 4:
            // +
            if(curr.lexeme == "+"){
               //check if before and after is term
                if(before.type == 0 || before.type == 2 || before.lexeme == "(" || before.type == 1 || before.lexeme == "true" || before.lexeme == "false"){
                    if(after.type == 0 || after.type == 2 || after.lexeme == "(" || after.type == 1 || after.lexeme == "true" || after.lexeme == "false"){
                        cout << "<Expression> -> <Term> <Expression'>\n" << "<Expression'> -> +<Term><Expression'>\n";
                    }
                    else{
                        cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                    }
                }
                else{
                    cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                }
            }
            // -
            else if (curr.lexeme == "-"){
                //check if before and after is term or primary
                if(before.type == 0 || before.type == 2 || before.lexeme == "(" || before.type == 1 || before.lexeme == "true" || before.lexeme == "false"){
                    if(after.type == 0 || after.type == 2 || after.lexeme == "(" || after.type == 1 || after.lexeme == "true" || after.lexeme == "false"){
                        cout << "<Expression> -> <Term> <Expression''>\n" << "<Expression''> -> -<Term> <Expression'>\n";
                    }
                    else{
                        cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                    }
                }
                else{
                    cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                }
            }
            // *
            else if (curr.lexeme == "*"){
                //check if before and after is factor/primary
                if(before.type == 0 || before.type == 2 || before.lexeme == "(" || before.type == 1 || before.lexeme == "true" || before.lexeme == "false"){
                    if(after.type == 0 || after.type == 2 || after.lexeme == "(" || after.type == 1 || after.lexeme == "true" || after.lexeme == "false"){
                        cout << "<Term> -> <Factor> <Term'>\n" << "<Term'> -> *<Factor><Term'>\n";
                    }
                    else{
                        cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                    }
                }
                else {
                    cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                }
            }
            // /
            else if (curr.lexeme == "/"){
                //check if after is factor
                if(before.type == 0 || before.type == 2 || before.lexeme == "(" || before.type == 1 || before.lexeme == "true" || before.lexeme == "false"){
                    if(after.type == 0 || after.type == 2 || after.lexeme == "(" || after.type == 1 || after.lexeme == "true" || after.lexeme == "false"){
                        cout << "<Term> -> <Factor> <Term''>\n" << "<Term'> -> /<Factor><Term''>\n";
                    }
                    else{
                        cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                    }
                }
                else {
                    cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                }
            }
            //compound ops "=<>^"
            // =
            else if (curr.lexeme == "="){
                //check if before is expression, or after is also "=" or ">" or "<" or if after is expression
                if(before.type == 0 || before.type == 2 || before.lexeme == "(" || before.type == 1 || before.lexeme == "true" || before.lexeme == "false" || before.lexeme =="=" || before.lexeme == "^"){
                    if(after.type == 0 || after.type == 2 || after.lexeme == "(" || after.type == 1 || after.lexeme == "true" || after.lexeme == "false" || after.lexeme == "=" || after.lexeme == "<" || after.lexeme == ">"){
                        // ==
                        if(before.lexeme == "=" || after.lexeme == "=")
                            cout << "<Condition> -> <Expression> <Relop> <Expression>\n" << "<Relop> -> ==\n";
                        // ^=
                        else if (before.lexeme == "^")
                            cout << "Condition> -> <Expression> <Relop> <Expression>\n" << "<Relop> -> ^=\n";
                        // =>
                        else if (after.lexeme == ">")
                            cout << "Condition> -> <Expression> <Relop> <Expression>\n" << "<Relop> -> =>\n";
                        // =<
                        else if (after.lexeme == "<")
                            cout << "Condition> -> <Expression> <Relop> <Expression>\n" << "<Relop> -> =<\n";
                    }
                    else{
                        cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                    }
                }
                else{
                    cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                }
            }
            // <
            else if (curr.lexeme == "<"){
                //check if before and after are expressions
                if(before.type == 0 || before.type == 2 || before.lexeme == "(" || before.type == 1 || before.lexeme == "true" || before.lexeme == "false" || before.lexeme == "="){
                    if(after.type == 0 || after.type == 2 || after.lexeme == "(" || after.type == 1 || after.lexeme == "true" || after.lexeme == "false"){
                        // =<
                        if(before.lexeme == "=")
                            cout << "<Condition> -> <Expression> <Relop> <Expression>\n" << "<Relop> -> =<\n";
                        // <
                        else
                            cout << "<Condition> -> <Expression> <Relop> <Expression>\n" << "<Relop> -> <\n";
                    }
                    else{
                        cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                    }
                }
                else{
                        cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                }
            }
            // >
            else if (curr.lexeme == ">"){
                //check if before and after are expressions
                if(before.type == 0 || before.type == 2 || before.lexeme == "(" || before.type == 1 || before.lexeme == "true" || before.lexeme == "false" || before.lexeme == "="){
                    if(after.type == 0 || after.type == 2 || after.lexeme == "(" || after.type == 1 || after.lexeme == "true" || after.lexeme == "false"){
                        // =<
                        if(before.lexeme == "=")
                            cout << "<Condition> -> <Expression> <Relop> <Expression>\n" << "<Relop> -> =>\n";
                        // <
                        else
                            cout << "<Condition> -> <Expression> <Relop> <Expression>\n" << "<Relop> -> >\n";
                    }
                    else{
                        cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                    }
                }
            }
            // ^
            else if (curr.lexeme == "^"){
                //check if before is expression and after is '='
                if(before.type == 0 || before.type == 2 || before.lexeme == "(" || before.type == 1 || before.lexeme == "true" || before.lexeme == "false"){
                    if(after.lexeme == "="){
                        // ^=
                        cout << "<Condition> -> <Expression> <Relop> <Expression>\n" << "<Relop> -> ^=\n";
                    }
                    else{
                        cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
                    }
                }
            }
            break;
            
        //case for seperators "[]{}():;,"
        case 5:
            
            if (curr.lexeme == "["){
                cout << "\n";
            }
            else if (curr.lexeme == "]"){
                cout << "\n";
            }
            else if (curr.lexeme == "{"){
                cout << "\n";
            }
            else if (curr.lexeme == "("){
                cout << "\n";
            }
            else if (curr.lexeme == ")"){
                cout << "\n";
            }
            else if (curr.lexeme == ":"){
                cout << "\n";
            }
            else if (curr.lexeme == ";"){
                cout << "\n";
            }
            else if (curr.lexeme == ","){
                cout << "\n";
            }
            //compound seperators "%"
            else if (curr.lexeme == "%"){
                cout << "\n";
            }
            break;
        default: cout << "Invalid token, no production identified for: " << curr.lexeme << " lexeme of token type: " << curr.type << endl;
    }
}//End Synt



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
