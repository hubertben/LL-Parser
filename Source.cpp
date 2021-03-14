#include <stdio.h>
#include <ctype.h>
#include <iostream>

// This warning is disable to allow the use of the fopen() function.
// For some reason, the compiler didnt like this and this solution 
// allowed it to work with my code.
#pragma warning(disable : 4996)

using namespace std;

int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE* in_fp, * fopen();

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
#define INT_LIT 10
#define IDENT 11

/*
Forward Declaration of functions
*/
void tokenize();
int lookup(char ch);
void addChar();
void getChar();
void getNonBlank();
int lex();
void expr();
void term();
void factor();
void form();

/* 
Driver Function to make initial call to get the next character, 
and draw a lex followed by evaluating the next token in the expr() 
funtion.
*/
void tokenize() {
	getChar();
	do {
		lex();
		expr();
	} while (nextToken != EOF);
}

/*
A refactored version of the textbook's lookup method. Switch 
statement are a bit slow so I changed it to a loop structure.
*/
int lookup(char ch) {
	char matcher[] = { '=', '+', '-', '*', '/', '(', ')', '%', '!' };
	for (int i = 0; i < sizeof(matcher); i++) {
		if (ch == matcher[i]) {
			addChar();
			nextToken = i + 20;
			break;
		}
	}
	return nextToken;
}

/*
Adds the nextChar character to the end of the current working 
lexeme.
*/
void addChar() {
	// Acting as a Guard Clause
	if (lexLen > 98) {
		printf("Error - lexeme is too long \n");
		return;
	}

	lexeme[lexLen++] = nextChar;
	lexeme[lexLen] = 0;
}

/*
Checks the nextChar character to see if it is a number or letter.
If neither, set to UNKNOWN.
*/
void getChar() {
	if ((nextChar = getc(in_fp)) == EOF) {
		charClass = EOF;
		return;
	}
	if (isalpha(nextChar)){
		charClass = LETTER;
		return;
	}
	if (isdigit(nextChar)) {
		charClass = DIGIT;
		return;
	}
	charClass = UNKNOWN;
}

/*
Sets nextChar to the next, non white space, character.
*/
void getNonBlank() {
	while (isspace(nextChar))
		getChar();
}

// Helper function for lex()
void addGetPair() {
	addChar();
	getChar();
}

/*
The Lexical Analyzer for simple arithmetic equations
*/
int lex() {
	lexLen = 0;
	getNonBlank();
	switch (charClass) {
	case LETTER:
		addGetPair();
		while (charClass == LETTER || charClass == DIGIT) {
			addGetPair();
		}
		nextToken = IDENT;
		break;
	case DIGIT:
		addGetPair();
		while (charClass == DIGIT) {
			addGetPair();
		}
		nextToken = INT_LIT;
		break;
	case UNKNOWN:
		lookup(nextChar);
		getChar();
		break;
	case EOF:
		nextToken = EOF;
		lexeme[0] = 'E';
		lexeme[1] = 'O';
		lexeme[2] = 'F';
		lexeme[3] = 0;
		break;
	}
	printf("\nNext token is: %d, Next lexeme is %s",
		nextToken, lexeme);
	return nextToken;
}

/*
Sub-Program for <expr>
*/
void expr() {
	printf("\nEnter <expr>");
	term();
	while (nextToken == 21 || nextToken == 22) {
		lex();
		term();
	}
	printf("\nExit <expr>");
}

/*
Sub-Program for <term>
*/
void term() {
	printf("\nEnter <term>");
	factor();
	while (nextToken == 23 || nextToken == 24 || nextToken == 27) {
		lex();
		factor();
	}
	printf("\nExit <term>");
}

/*
Sub-Program for <factor>
*/
void factor() {
	printf("\nEnter <factor>");
	form();
	if (nextToken == 28) {
		lex();
	}
	printf("\nExit <factor>");;
}

/*
Sub-Program for <form>
*/
void form() {
	printf("\nEnter <form>");
	if (nextToken == IDENT || nextToken == INT_LIT)
		lex();
	else {
		if (nextToken == 25) {
			lex();
			expr();
			if (nextToken == 26)
				lex();
			else {
				cout << "\nError!" << endl;
				exit(1);
			}
		}
		else {
			cout << "\nError!" << endl;
			exit(1);
		}
	}
	printf("\nExit <form>");;
}

int main() {
	in_fp = fopen("input.txt", "r");
	if (in_fp == NULL)
		printf("ERROR - cannot open input.txt \n");
	else {
		tokenize();
	}
	cout << "\n" << endl;
}
