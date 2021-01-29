#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int token; /* holds the current input character for the parse */
/* declarations to allow arbitrary recursion */
void command();
// changed 
double expr();
double term();
double factor();
double exponent();
double root();
double number();
double fraction();
int digit();

void error(char* message){
  printf("parse error: %s\n", message);
  exit(1);
}

void getToken(){
  /* tokens are characters */
  token = getchar();
}

void match(char c, char* message){
  if (token == c) 
    getToken();
  else 
    error(message);
}

void command(){
  /* command -> expr '\n' */
  double result = expr();
  if (token == '\n') { /* end the parse and print the result */
    if (fmod(result, result) == 0)
      printf("The result is: %d\n", (int)result);
    else 
      printf("The result is: %.5lf\n", result);
  } else 
  error("tokens after end of expression");
}

double expr(){
  /* expr -> term { '+' term } */
  double result = term();
  // added if-else to determine +|-
  if (token == '+') {
    while (token == '+'){
      match('+', "+ expected");
      result += term();
    }
  } else if (token == '-') {
    // added code block to calculate subtraction
    while (token == '-'){
      match('-', "- expected");
      result -= term();
    }    
  }
  return result;
}

double term(){
  /* term -> factor { '*' factor } */
  double result = factor();
  // added if-else to determine *|/|%
  if (token == '*') {
    while (token == '*'){
      match('*', "* expected");
      result *= factor();
    } 
  } else if (token == '/') {
    // added code block to calculate division
    while (token == '/'){
      match('/', "/ expected");
      result /= factor();
    }    
  } else if (token == '%') {
    // added code block to calculate modulus
    while (token == '%'){
      match('%', "% expected");
      result = fmod(result, factor());
    }    
  }
  return result;
}

// hijacked factor function to calculate exponents and roots
double factor() {
  double result = exponent();
  if (token == '^') {
    while (token == '^') {
      match('^', "^ expected");
      result = pow(result, factor());
    }
  } else if (token == '#') {
    while (token == '#') {
      match('#', "# expected");
      result = pow(result, 1.0/factor());
    }
  }
  return result;
}

// added exponent to only return root()
double exponent() {
  return root();
}

// renamed factor to root
double root(){
  /* factor -> '(' expr ')' | number */
  double result;
  if (token == '('){
    match('(', "( expected");
    result = expr();
    match(')', ") expected");
  } else
    result = number();
  return result;
}

double number() {
  double result;
   if (token == ('.')) {
     match('.', ". expected");
     result = fraction();
   } if (token == ('-')) {
     match('-', "- expected");
     result = fraction();
   } if (isdigit(token)) {
     result = digit();
     while (isdigit(token))
  /* the value of a number with a new trailing digit
  is its previous value shifted by a decimal place
  plus the value of the new digit
  */
      result = 10.0 * result + digit();
   }
   return result;
}

double fraction(){
  /* number -> digit { digit } */
  double result;
  if (isdigit(token)) {
    while (isdigit(token))
      result = 10 * result + digit();
  } else if (token == ('.')) {
      match('.', ". expected");
      double frac;
      result = modf(10 * result + digit(), &frac);
      result = 10 * result + digit();
      frac = frac + digit() * (1 / 10);
      result = result + frac;
  } 
  return result;
}

\
int digit(){
  /* digit -> '0' | '1' | '2' | '3' | '4'
  | '5' | '6' | '7' | '8' | '9' */
  int result;
  if (token == '-') {
    match('-', "- expected");
    result = result * -1;
  } else if (isdigit(token)){
    /* the numeric value of a digit character
    is the difference between its ascii value and the
    ascii value of the character '0'
    */
    result = token - '0';
    match(token, "( expected");
  }
  else
    error("digit expected");
  return result;
}

void parse(){
  getToken(); /* get the first token */
  command(); /* call the parsing procedure for the start symbol */
}

int main(){
  parse();
  return 0;
}