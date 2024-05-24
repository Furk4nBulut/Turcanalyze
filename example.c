//Elanur �leri, 200315038
//R�dvan �elebi, 200315066

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/* Global declarations */
/* Variables */
int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp, *fopen();

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();
void factor();
void term();
void expr();
void var();
void assign_op();
void start_op();
void finish_op();
void err();

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define VAR 12
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 50
#define RIGHT_PAREN 26
#define START_OP 30
#define FINISH_OP 31

/* Main driver */
main() {
	if ((in_fp = fopen("ex.txt", "r")) == NULL)
 		printf("Hata. \n");
 	else {
 		getChar();
 	do {
 		lex();
 		program();
 	} while (nextToken != EOF);
 }
}

/* Open the input data file and process its contents
 lookup - a function to lookup operators and parentheses
 and return the token */
int lookup(char ch) {
	switch (ch) {

 		case '(':
 			addChar();
 			nextToken = LEFT_PAREN;
 			break;

 		case ')':
 			addChar();
 			nextToken = RIGHT_PAREN;
 			break;

 		case '+':
			addChar();
			nextToken = ADD_OP;
			break;

 		case '-':
			addChar();
			nextToken = SUB_OP;
			break;

 		case '*':
			addChar();
			nextToken = MULT_OP;
			break;

  		case '/':
			addChar();
			nextToken = DIV_OP;
			break;

		/* Assigment operator assigned to "=" */
		case '=':
			addChar();
			nextToken = ASSIGN_OP;
			break;

		/* Start operator assigned to "<" */
		case '<':
 			addChar();
 			nextToken = START_OP;
 			break;

		/* Finish operator assigned to ">" */
		case '>':
 			addChar();
 			nextToken = FINISH_OP;
 			break;

 		default:
			addChar();
			nextToken = EOF;
			break;
 }
 	return nextToken;
}

/* addChar - a function to add nextChar to lexeme */
void addChar() {
 	if (lexLen <= 98) {
 		lexeme[lexLen++] = nextChar;
 		lexeme[lexLen] = 0;
 }
 	else
 		printf("Hata. \n");
}

/* getChar - a function to get the next character of
 input and determine its character class */
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

/* getNonBlank - a function to call getChar until it
 returns a non-whitespace character */
void getNonBlank() {
 	while (isspace(nextChar))
 	getChar();
}

/* lex - a simple lexical analyzer for arithmetic
 expressions */
int lex() {
 	lexLen = 0;
 	getNonBlank();
 	switch (charClass) {
 	/* Parse identifiers */
 	case LETTER:
		addChar();
	 	getChar();
 	while (charClass == LETTER || charClass == DIGIT) {
		addChar();
		getChar();
 }

/* If the read lexeme is a variable, it assigns the value VAR to the nextToken
If the read lexeme is a identifier, it assigns the value IDENT to the nextToken
(The isEqual function is defined below the code block) */
	if(isCorrectDataType(lexeme)){
		nextToken = VAR;
	}
	else{
		nextToken = IDENT;
	}
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
 } 	/* End of switch */

 	printf("Sonraki sozcuk: %d, sonraki sozcukbirim %s\n",
 	nextToken, lexeme);
 	return nextToken;
} /* End of function lex */

/* expr - Parses strings in the language generated by the rule:
 <expr> -> <term> + <term> | <term> - <term> */
void expr() {
 	printf("Giris -> <expr>\n");
 	/* Parse the first term */
 	term();
 	/* As long as the next token is + or -, get
 	the next token and parse the next term */
 	while (nextToken == ADD_OP || nextToken == SUB_OP) {
 		lex();
 		term();
 }
 	printf("Cikis -> <expr>\n");
} /* End of function expr */

/* term - Parses strings in the language generated by the rule:
 <term> -> <factor> {(* | /) <factor>) */
void term() {
 	printf("Giris -> <term>\n");
 	/* Parse the first factor */
 	factor();
 	/* As long as the next token is * or /, get the
 	next token and parse the next factor */
 	while (nextToken == MULT_OP || nextToken == DIV_OP) {
 		lex();
 		factor();
 }
 	printf("Cikis -> <term>\n");
}  /* End of function term */

/* factor - Parses strings in the language generated by the rule:
 <factor> -> id | int_constant | ( <expr ) */
void factor() {
 	printf("Giris -> <factor>\n");
	/* Determine which RHS */
 	if (nextToken == IDENT || nextToken == INT_LIT)
 	/* Get the next token */
 		lex();
 		/* If the RHS is ( <expr>), call lex to pass over the
 		left parenthesis, call expr, and check for the right
 		parenthesis */
 	else {
 		if (nextToken == LEFT_PAREN) {
 			lex();
 			expr();
 			if (nextToken == RIGHT_PAREN)
 				lex();
 			else
 				printf("Hata.");
 } /* End of if (nextToken == ... */
/* It was not an id, an integer literal, or a left
 parenthesis */
 		else
 			printf("Hata.");
 }
 	printf("Cikis -> <factor>\n");;
}

/* program - Parses strings in the language generated by the rule:
 <program> -> <start_op> <var> <assign_op> <expr> <finish_op> */
void program(){
	/* The program BNF, which directs the operation of the program,
 	follows these steps: */
	printf("Giris -> <program>\n");
	start_op();
	var();
	assign_op();
	expr();
	finish_op();
	printf("Cikis -> <program>\n");
}

/* var - Parses strings in the language generated by the rule:
<var> -> <type> id | id */
void var() {
	printf("Giris -> <var>\n");
	if(nextToken == VAR){
		/* The <type> method is executed if the nextToken is equal to
		any of the variable types in the language. */
		type();
	}
	if(nextToken == IDENT){
		/* The <lex> method is executed if the nextToken
		is equal to any of the identifier values in the language. */
		lex();

	} else{
		err();
	}
	printf("Cikis -> <var>\n");
}


void assign_op() {
 	printf("Giris -> <assign_op>\n");
 	/*If the nextToken expression is equal to the assign operarator,
	 it starts executing the lex() method.*/
 	if(nextToken == ASSIGN_OP){
 		/* Get the next token */
 		lex();

	 }
	 else{
	 	err();
	 }
 	printf("Cikis -> <assign_op>\n");
}

void finish_op() {
	printf("Giris -> <finish_op>\n");
	/*If the nextToken expression is equal to the finish operarator,
	 it starts executing the lex() method.*/
 	if(nextToken == FINISH_OP){
 		/* Get the next token */
 		lex();
	 }else{
	 	/* If nextToken is not equal to the
		 finish_op, the written err() method is called. */
	 	err();
	 }
 	printf("Cikis -> <finish_op>\n");
}

void start_op() {
	printf("Giris -> <start_op>\n");
	/*If the nextToken expression is equal to the start operator,
	 it starts executing the lex() method.*/
 	if(nextToken == START_OP){
 		/* Get the next token */
 		lex();
	 }else{
	 	err();
	 	/* If nextToken is not equal to the
		 start_op, the written err() method is called. */
	 }
 	printf("Cikis -> <start_op>\n");
}

/* err- It has been written in order to get errors
 in codes that do not comply with the rules of the language. */
void err(){
	printf("Hata.");
	exit(1);
}

void type() {
     printf("Giris -> <type>\n");
     /* Get the next token */
         lex();
     printf("Cikis -> <type>\n");;
}

/* isCorrectDataType - In the method with type restriction,
only digit and string values are defined. */
int isCorrectDataType(char* string) {
   if (!strcmp(string, "sayi") || !strcmp(string, "soz"))
   return 1;
   else{
   	return 0;
   }
}
