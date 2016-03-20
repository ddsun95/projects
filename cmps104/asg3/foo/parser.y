// David Sun (ddsun@ucsc.edu)
// Curtis Liew (cliew@ucsc.edu)

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
%token TOK_VARDECL TOK_PARAMLIST TOK_NEWSTRING TOK_INDEX TOK_STMT

%start start

%right TOK_IDENT
%right TOK_IF TOK_ELSE
%right '='
%left TOK_EQ TOK_NE TOK_LT TOK_LE TOK_GT TOK_GE
%left '+' '-'
%left '*' '/' '%'
%right TOK_POS TOK_NEG '!' TOK_ORD TOK_CHR
%left '[' '.' '(' 
%nonassoc TOK_NEW

%%

start          : program                       { yyparse_astree = $1; }
               ; 

program        : program structdef             { $$ = adopt1 ($1, $2); }
               | program function              { $$ = adopt1 ($1, $2); }
               | program statement             { $$ = adopt1 ($1, $2); }
               | program error '}'             { $$ = $1; }
               | program error ';'             { $$ = $1; }
               |                               { $$ = new_parseroot(); }
               ;

structdef      : structdef_ '}'           { free_ast ($2); $$ = $1; }
               ;

structdef_     : TOK_STRUCT TOK_IDENT '{' 
               { free_ast ($3); $$ = adopt1 ($1, sch ($2, TOK_TYPEID));}

               | structdef_ fielddecl ';' 
               { free_ast ($3); $$ = adopt1 ($1, $2); }
               ;

fielddecl      : basetype TOK_ARRAY TOK_IDENT 
               { $$ = adopt2 ($2, $1, sch ($3, TOK_FIELD)); }

               | basetype TOK_IDENT 
               { $$ = adopt1 ($1, sch ($2, TOK_FIELD)); }
               ;

basetype       : TOK_VOID     { $$ = $1; }
               | TOK_BOOL     { $$ = $1; }
               | TOK_CHAR     { $$ = $1; }
               | TOK_INT      { $$ = $1; }
               | TOK_STRING   { $$ = $1; }
               | TOK_IDENT    { $$ = sch ($1, TOK_TYPEID); }
               ;

function       : identdecl '(' ')' block 
               { free_ast ($3); $$ = porf ($1, $2, $4); }

               | identdecl fnargs ')' block 
               { free_ast ($3); $$ = porf ($1, $2, $4); }
               ;

fnargs         : fnargs ',' identdecl  
               { free_ast ($2); $$ = adopt1 ($1, $3); }

               | '(' identdecl         
               { $$ = adopt1 (sch ($1, TOK_PARAMLIST), $2); }
               ;

identdecl      : basetype TOK_ARRAY TOK_IDENT 
               { $$ = adopt2 ($2, $1, sch ($3, TOK_DECLID)); }

               | basetype TOK_IDENT           
               { $$ = adopt1 ($1, sch ($2, TOK_DECLID)); }
               ;

block          : block_ '}' { free_ast ($2); $$ = $1; }
               | ';'        { $$ = sch ($1, TOK_BLOCK); } 
               ;

block_         : block_ statement { $$ = adopt1 ($1, $2); }
               | '{'              { $$ = sch ($1, TOK_BLOCK); }
               ;

statement      : block     { $$ = $1; }
               | vardecl   { $$ = $1; }
               | while     { $$ = $1; }
               | ifelse    { $$ = $1; }
               | return    { $$ = $1; }
               | expr ';'  { $$ = $1; free_ast ($2); }
               ;


vardecl        : identdecl '=' expr ';' { $$ = adopt2 (sch ($2, 
                           TOK_VARDECL), $1, $3); free_ast ($4); }
               ;

while          : TOK_WHILE '(' expr ')' statement { free_ast2 ($2, $4); 
                                             $$ = adopt2 ($1, $3, $5); }
               ;

ifelse         : TOK_IF '(' expr ')' statement %prec TOK_IF       
               { free_ast2 ($2, $4); $$ = adopt2 ($1, $3, $5); }

               | TOK_IF '(' expr ')' statement TOK_ELSE statement 
               { free_ast2 ($2, $4); $$ = adopt3 (sch ($1, TOK_IFELSE), 
                                                          $3, $5, $7); }
               ;

return         : TOK_RETURN expr ';' 
               { $$ = adopt1 ($1, $2); free_ast ($3); } 

               | TOK_RETURN ';'      { $$ = $1; }
               ;

expr           : binop           { $$ = $1; }
               | unop            { $$ = $1; }
               | allocator       { $$ = $1; }
               | call            { $$ = $1; }
               | '(' expr ')'    { free_ast2 ($1, $3); $$ = $2; }
               | variable        { $$ = $1; }
               | constant        { $$ = $1; }
               ;

binop          : expr '+' expr { $$ = adopt2 ($2, $1, $3); }
               | expr '-' expr { $$ = adopt2 ($2, $1, $3); }
               | expr '*' expr { $$ = adopt2 ($2, $1, $3); }
               | expr '/' expr { $$ = adopt2 ($2, $1, $3); }
               | expr '%' expr { $$ = adopt2 ($2, $1, $3); }
               | expr '=' expr { $$ = adopt2 ($2, $1, $3); }
               | expr TOK_EQ expr { $$ = adopt2 ($2, $1, $3); }
               | expr TOK_NE expr { $$ = adopt2 ($2, $1, $3); }
               | expr TOK_LT expr { $$ = adopt2 ($2, $1, $3); }
               | expr TOK_LE expr { $$ = adopt2 ($2, $1, $3); }
               | expr TOK_GT expr { $$ = adopt2 ($2, $1, $3); }
               | expr TOK_GE expr { $$ = adopt2 ($2, $1, $3); }
               ;

unop           : '+' expr               { $$ = adopt1 ($1, $2); }
               | '-' expr               { $$ = adopt1 ($1, $2); }
               | '!' expr               { $$ = adopt1 ($1, $2); }
               | TOK_ORD expr           { $$ = adopt1 ($1, $2); }
               | TOK_CHR expr           { $$ = adopt1 ($1, $2); }
               ;

allocator      : TOK_NEW TOK_IDENT '(' ')'       { free_ast2 ($3, $4); 
                             $$ = adopt1 ($1, sch ($2, TOK_TYPEID)); }

               | TOK_NEW TOK_STRING '(' expr ')' 
               { free_ast3 ($2, $3, $5); 
                 $$ = adopt1 (sch ($1, TOK_NEWSTRING), $4); }

               | TOK_NEW basetype '[' expr ']'   
               { free_ast2 ($3, $5); 
                 $$ = adopt2 (sch ($1, TOK_NEWARRAY), $2, $4); }
               ;

call           : callargs ')' { free_ast ($2); $$ = $1; }

               | TOK_IDENT '(' ')' 
               { free_ast ($3); $$ = adopt1 (sch ($2, TOK_CALL), $1); }
               ;

callargs       : TOK_IDENT '(' expr 
               { $$ = adopt2 (sch ($2, TOK_CALL), $1, $3); }

               | callargs ',' expr 
               { free_ast ($2); $$ = adopt1 ($1, $3); }
               ;

variable       : TOK_IDENT          { $$ = $1; }
               | expr '[' expr ']'  { free_ast ($4); 
                           $$ = adopt2 (sch ($2, TOK_INDEX), $1, $3); }
               | expr '.' TOK_IDENT { $$ = adopt2 ($2, $1, 
                                       sch ($3, TOK_FIELD)); }
               ;

constant       : TOK_INTCON      { $$ = $1; } 
               | TOK_CHARCON     { $$ = $1; }
               | TOK_STRINGCON   { $$ = $1; }
               | TOK_FALSE       { $$ = $1; }
               | TOK_TRUE        { $$ = $1; }
               | TOK_NULL        { $$ = $1; }
               ;
%%

const char *get_yytname (int symbol) {
   return yytname [YYTRANSLATE (symbol)];
}

bool is_defined_token (int symbol) {
   return YYTRANSLATE (symbol) > YYUNDEFTOK;
}
