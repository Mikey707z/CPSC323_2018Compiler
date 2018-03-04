//
//  main.cpp
//  project1
//
//  Created by Ruby Abutaleb on 2/25/18.
//  Copyright © 2018 compilers223. All rights reserved.
//


#include<iostream>
#include<string>
#include<fstream>
#include<cstring>
#include<vector>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>


using namespace std;

bool isKeyWord(char buffer[]){
    //keywords array initialization
    string keywords[64] {"asm", "auto", "bool", "break", "case", "catch", "char", "class", "const", "const_cast", "continue", "default", "delete", "do", "double", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float", "for", "friend", "goto", "if", "inline", "int", "long", "mutable", "namespace", "new", "operator", "private", "protected", "public", "register", "reinterpret_cast", "return", "short", "signed", "sizeof", "static", "static_cast", "struct", "switch", "template", "this", "throw", "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while"};
    int i;
    bool result = 0;
    for (i = 0; i < 64; ++i){
        if(strcmp(keywords[i], buffer) == 0){
            result = 1;
            break;
        }
    }
    return result;
}

int main() {
    
    //enum states
    enum state {
        a,
        b,
        c,
        d,
        e
    };
    char ch, buffer[40], operators[] = "+-*/%=";
    char delimiters[] = {',', ';', ' '};
    int digits[] = {};
    fstream file;
    int k = 0, j = 0, n = 0;
    
    file.open("code.txt");
    //check if file is open
    if(!file.is_open()){
        cout << "error while opening the file\n";
        exit(0);
    }
    state go = a;
    
    while(!file.eof()){
        file >> ch;
        buffer[n] = ch;
        ++n;
    }
    
    while(k < n){
        
        //check if operator
        for (int i = 0; i < 6; ++i){
            if(buffer[k] == operators[i])
                cout << ch << "is operator\n";
                k++;
            break;
        }

        //check if letter
        if(isalpha(buffer[k])) {
            go = b;
            k++;
            break;
        //check if number
        }if (isdigit(buffer[k])){
            go = c;
            while (isdigit(buffer[k])){
                go = c;
                k++;
            }
        //check if dilimiter
        }for (int i = 0; i < 3; ++i){
            if(buffer[k] == delimiters[i]){
                cout << buffer[k] << " is a delimiter.\n";
            }
                else{
                    i++;
                }
            k++;
        }
        k++;
    }
   
    return 0;
}

