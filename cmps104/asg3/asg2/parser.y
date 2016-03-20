%{
#include "lyutils.h"
#include "astree.h"
%}

%debug
%defines
%error-verbose
%token-table
%verbose

%token TOK_VOID TOK_BOOL TOK_CHAR TOK_INT TOK_STRING
%token TOK_IF TOK_ELSE TOK_WHILE TOK_RETURN TOK_STRUCT
%token TOK_FALSE TOK_TRUE TOK_NULL TOK_NEW TOK_ARRAY
%token TOK_EQ TOK_NE TOK_LT TOK_LE TOK_GT TOK_GE
%token TOK_IDENT TOK_INTCON TOK_CHARCON TOK_STRINGCON

%token TOK_BLOCK TOK_CALL TOK_IFELSE TOK_INITDECL
%token TOK_POS TOK_NEG TOK_NEWARRAY TOK_TYPEID TOK_FIELD
%token TOK_ORD TOK_CHR TOK_ROOT TOK_PROTOTYPE TOK_FUNCTION TOK_DECLID


%start program

%right TOK_IF TOK_ELSE
%right '='
%left TOK_EQ TOK_NE TOK_LT TOK_LE TOK_GT TOK_GE
%left '+' '-'
%left '*' '/' '%'
%right TOK_POS TOK_NEG '!' TOK_ORD TOK_CHR
%left TOK_ARRAY TOK_FIELD TOK_FUNCTION

%%
program        : program structdef             { $$ = adopt1 ($1, $2); }
               | program function              { $$ = adopt1 ($1, $2); }
               | program statement             { $$ = adopt1 ($1, $2); }
               | program error '}'                          { $$ = $1; }
               | program error ';'                          { $$ = $1; }
               |                               { $$ = new_parseroot(); }
               ;

structdef      : structdef_ '}' {}
               ;

structdef_     : structdef_ fielddecl ';' {}
               | TOK_STRUCT TOK_IDENT '{' {}
               ;

fielddecl      : basetype TOK_ARRAY TOK_IDENT {}
               | basetype TOK_IDENT {}

basetype       : TOK_VOID     { }
               | TOK_BOOL     { }
               | TOK_CHAR     { }
               | TOK_INT      { }
               | TOK_STRING   { }
               | TOK_IDENT    { }
               ;

function       : identdecl '(' ')' ';'       { }
               | identdecl '(' ')' block     { }
               | identdecl fnargs ')' ';'    { }
               | identdecl fnargs ')' block  { }
               ;

fnargs         : fnargs ',' identdecl { }
               | '(' identdecl        { }
               ;

identdecl      : basetype TOK_ARRAY TOK_DECLID { }
               | basetype TOK_DECLID           { }
               ;

block          : '{' '}'    { }
               | block_ '}' { }
               | ';'        { }
               ;

block_         : block_ statement { }
               | '{'              { }
               ;

statement      : block     { }
               | vardecl   { }
               | while     { }
               | ifelse    { }
               | return    { }
               | expr ';'  { }
               ;

vvardecl       : identdecl '=' expr { }
               ;

while          : TOK_WHILE '(' expr ')' statement { }
               ;

ifelse         : TOK_IF '(' expr ')' statement { }
               | TOK_IF '(' expr ')' statement TOK_ELSE statement { }


%%

const char *get_yytname (int symbol) {
   return yytname [YYTRANSLATE (symbol)];
}


bool is_defined_token (int symbol) {
   return YYTRANSLATE (symbol) > YYUNDEFTOK;
}

