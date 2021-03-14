#include <stdio.h>
#include <ctype.h>
#include <iostream>

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


void tokenize() {
	getChar();
	do {
		lex();
		expr();
	} while (nextToken != EOF);
}

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

void addChar() {
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = 0;
	}
	else {
		printf("Error - lexeme is too long \n");
	}
}

void getChar() {
	if ((nextChar = getc(in_fp)) != EOF) {
		if (isalpha(nextChar))
			charClass = LETTER;
		else if (isdigit(nextChar))
			charClass = DIGIT;
		else charClass = UNKNOWN;
	}
	else
		charClass = EOF;
}

void getNonBlank() {
	while (isspace(nextChar))
		getChar();
}

int lex() {
	lexLen = 0;
	getNonBlank();
	switch (charClass) {
	case LETTER:
		addChar();
		getChar();
		while (charClass == LETTER || charClass == DIGIT) {
			addChar();
			getChar();
		}
		nextToken = IDENT;
		break;
		/* Parse integer literals */
	case DIGIT:
		addChar();
		getChar();
		while (charClass == DIGIT) {
			addChar();
			getChar();
		}
		nextToken = INT_LIT;
		break;
		/* Parentheses and operators */
	case UNKNOWN:
		lookup(nextChar);
		getChar();
		break;
		/* EOF */
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


void expr() {
	printf("\nEnter <expr>");
	term();
	while (nextToken == 21 || nextToken == 22) {
		lex();
		term();
	}
	printf("\nExit <expr>");
}

void term() {

	printf("\nEnter <term>");

	factor();

	while (nextToken == 23 || nextToken == 24 || nextToken == 27) {
		lex();
		factor();
	}
	printf("\nExit <term>");
	
}

void factor() {
	printf("\nEnter <factor>");

	form();

	if (nextToken == 28) {
		lex();
	}

	printf("\nExit <factor>");;
}


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
