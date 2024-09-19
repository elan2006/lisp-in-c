#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>
#include <stdbool.h>

enum Type {
  LPAREN,
  RPAREN,
  INT,
  STR,
  FLOAT,
  NIL
};

struct Token {
  int type;
  char value[100];
};

typedef struct Token Token;
bool debug = false;

void tok_dump(Token tok){
  printf("Token type: %d\t Token value:%s\n", tok.type, tok.value);
}
int pospointer;

Token test_func() {
  Token tok;
  tok.type = STR;
  strcpy(tok.value, "This is a test");
  return tok;
}

Token debug_func(){
  debug = true;
  Token tok;
  tok.type = NIL;
  strcpy(tok.value, "NIL");
  return tok;
}

void exit_func(){
  printf("Bye.\n");
  exit(0);
}

Token print_func(Token args[], int size) {
  for(int i = 0; i < size; i++){    
    printf("%s\n",args[i].value);
  }
  Token tok;
  tok.type = NIL;
  strcpy(tok.value, "NIL");
  return tok;
}

Token add_func(Token args[], int size) {
  int result = 0;
  char res[100];
  bool floating = false;
  for(int i = 0; i < size; i++){
    if(args[i].type == FLOAT){
      floating = true;
      break;
    }
  }
  Token tok;
  if(floating){

    double result = 0;
    for(int i = 0; i < size; i++){
      result += atof(args[i].value);
    }
    tok.type = FLOAT;
    sprintf(res, "%f", result);
    strcpy(tok.value, res);

  } else {
    for(int i = 0; i < size; i++){
      result += atoi(args[i].value);
    }
    tok.type = INT;

    sprintf(res, "%d", result);
    strcpy(tok.value, res);
  }
  return tok;
}



Token mul_func(Token args[], int size) {
  int result = 1;
  char res[100];
  bool floating = false;
  for(int i = 0; i < size; i++){
    if(args[i].type == FLOAT){
      floating = true;
      break;
    }
  }
  Token tok;
  if(floating){

    double result = 1;
    for(int i = 0; i < size; i++){
      result *= atof(args[i].value);
    }
    tok.type = FLOAT;
    sprintf(res, "%f", result);
    strcpy(tok.value, res);

  } else {
    for(int i = 0; i < size; i++){
      result *= atoi(args[i].value);
    }
    tok.type = INT;

    sprintf(res, "%d", result);
    strcpy(tok.value, res);
  }
  return tok;
}

Token sub_func(Token args[], int size) {
  int result;
  char res[100];
  bool floating = false;
  for(int i = 0; i < size; i++){
    if(args[i].type == FLOAT){
      floating = true;
      break;
    }
  }
  Token tok;
  if(floating){

    double result = 0;
    for(int i = 0; i < size; i++){
      if(i == 0){
	result = atof(args[i].value);
      } else
	result -= atof(args[i].value);
    }
    tok.type = FLOAT;
    sprintf(res, "%f", result);
    strcpy(tok.value, res);

  } else {
    for(int i = 0; i < size; i++){
      if(i == 0){
	result = atof(args[i].value);
	} else
	result -= atoi(args[i].value);
    }
    tok.type = INT;

    sprintf(res, "%d", result);
    strcpy(tok.value, res);
  }
  return tok;
}

Token div_func(Token args[], int size) {
  double result;
  char res[100];
  bool floating = false;
  for(int i = 0; i < size; i++){
    if(args[i].type == FLOAT){
      floating = true;
      break;
    }
  }
  Token tok;
  for(int i = 0; i < size; i++){
    if(i == 0){
      result = atof(args[i].value);
    } else
      result /= atoi(args[i].value);
  }
  tok.type = FLOAT; 
  sprintf(res, "%f", result);
  strcpy(tok.value, res);
  return tok;
}

void throw(char* msg){
  fprintf(stderr, "%s\n", msg);
  exit(1);
}

Token eval(const char code[], int pos){
  int codelength = strlen(code);
  //trim whitespace
  //  (
  while((code[pos] == ' ' || code[pos] == '\n' || code[pos] == '\t') && pos < codelength)
    pos++;
  char c = code[pos];
  if(isdigit(c)){
    char val[30];
    int index = 0;
    while(isdigit(c)){
      val[index++] = c;
      c = code[++pos];
    }
    if(c == '.'){
      val[index++] = '.';     
      if(pos<codelength)
	pos++;
      c = code[pos];
      while(isdigit(c)){
	val[index++] = c;
	c = code[++pos];
      }
      val[index] = '\0';
      Token tok;
      tok.type = FLOAT;
      strcpy(tok.value,val);
      pospointer = pos;
    return tok;
    }
    val[index] = '\0';
    Token tok;
    tok.type = INT;
    strcpy(tok.value,val);
    pospointer = pos;
    return tok;
  }

  if(c == '"'){
     if(pos<codelength)
	pos++;
     char string[100];
     int index = 0;
     while(code[pos] != '"' && code[pos] != '\0'){
       string[index++] = code[pos];
       if(pos<codelength)
	pos++;
     }
     string[index] = '\0';
     if(pos<codelength)
	pos++;
     pospointer  = pos;
     Token tok;
     tok.type = STR;
     strcpy(tok.value, string);
     return tok;
  }

  if(c == ')'){
     if(pos<codelength)
	pos++;
     pospointer = pos;
    Token tok = {LPAREN, ")"};
    return tok;
  }

  if(c == '('){
    if(pos<codelength)
      pos++;
    while((code[pos] == ' ' || code[pos] == '\n' || code[pos] == '\t') && pos < codelength)
      pos++;
    char funcname[30];
    int index1 = 0;
    while(!isspace(code[pos]) && code[pos] != '\0' && code[pos] != ')'){
      funcname[index1++] = code[pos];      
      if(pos<codelength)
	pos++;
    }
    funcname[index1] = '\0';

    Token arguments[10];
    int argsize = 0;
    Token tok = eval(code, pos);
    while(tok.type != LPAREN){
      pos = pospointer;
      arguments[argsize++] = tok;
      tok = eval(code, pos);
      if(code[pos] == '\0')
	throw("Error: expected )");
      
      if(pos<codelength)
	pos++;
    }      

    if(strcmp(funcname, "test") == 0)
      return test_func();
    if(strcmp(funcname, "print") == 0){
      return print_func(arguments, argsize);
    }
    if(strcmp(funcname, "+") == 0){
      return add_func(arguments, argsize);
    }
    if(strcmp(funcname, "-") == 0){
      return sub_func(arguments, argsize);
    }
    if(strcmp(funcname, "*") == 0)
      return mul_func(arguments, argsize);
    if(strcmp(funcname, "/") == 0)
      return div_func(arguments, argsize);
    if(strcmp(funcname, "exit") == 0)
      exit_func();
    if(strcmp(funcname, "debug") == 0)
      return debug_func();
  }

  throw("Unrecognized token");
  
}

Token evaluate(const char code[]) {
  int pos = 0;
  return eval(code, pos);
}
char* readline() {
  int bufsize = 1024;
  int position = 0;
  char c;
  char* buffer = malloc(sizeof(char) * bufsize);
  if(!buffer) {
    fprintf(stderr, "allocation error\n");
    exit(1);    
  }
  printf("> ");
  for(;;){
    c = getchar();
    if(c == '\0' || c == '\n'){
      buffer[position] = '\0';
      return buffer;
    }    
    buffer[position++] = c;

    if(position >= bufsize) {
      bufsize += bufsize;
      buffer = realloc(buffer, bufsize);
      if(!buffer){
	fprintf(stderr, "allocation error\n");
	exit(1);    
      }
    }        
  }

}
void repl(){
  for(;;){
    const char* code = readline();
    Token tok = evaluate(code);
    printf("%s\n", tok.value);
    if(debug)
      tok_dump(tok);
    free(code);
  }

}
int main(){
  printf("Press ^C (Ctrl+C) or enter (exit) to exit\n");
  repl();
  return 0;
}
