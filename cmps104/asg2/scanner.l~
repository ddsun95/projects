/* $Id: lexer.l,v 1.3 2015-07-09 14:08:58-07 - - $ */

%{
#include "astree.h"
#include "lyutils.h"

#define YY_USER_ACTION  { lexer_useraction(); }
%}

%option 8bit
%option debug
%option nodefault
%option noinput
%option nounput
%option noyywrap
%option verbose
%option warn

LETTER            [A-Za-z_]
DIGIT             [0-9]
IDENT             ({LETTER}({LETTER}|{DIGIT})*)
INTCONST          ({DIGIT}+)
CHRCONST          (’([^\\’\n]|\\[\\’"0nt])’)
STRCONST          (\"([^\\"\n]|\\[\\’"0nt])*\")
BADIDENT          ({INTCONST}{IDENT})
BADSTR            (’([^\\’\n]|\\[\\’"0nt]))
BADCHR            (\"([^\\"\n]|\\[\\’"0nt])*)

%%

"#".*             { lexer_include(); }
[ \t]+            { }
\n                { lexer_newline(); }

"void"            { return yylval_token (TOK_VOID);   }
"bool"            { return yylval_token (TOK_BOOL);   }
"char"            { return yylval_token (TOK_CHAR);   }
"int"             { return yylval_token (TOK_INT);    }
"string"          { return yylval_token (TOK_STRING); }
"struct"          { return yylval_token (TOK_STRUCT); }
"if"              { return yylval_token (TOK_IF);     }
"else"            { return yylval_token (TOK_ELSE);   }
"while"           { return yylval_token (TOK_WHILE);  }
"return"          { return yylval_token (TOK_RETURN); }
"false"           { return yylval_token (TOK_FALSE);  }
"true"            { return yylval_token (TOK_TRUE);   }
"null"            { return yylval_token (TOK_NULL);   }
"ord"             { return yylval_token (TOK_ORD);    }
"chr"             { return yylval_token (TOK_CHR);    }
"new"             { return yylval_token (TOK_NEW);    }

{IDENT}           { return yylval_token (TOK_IDENT);     }
{INTCONST}        { return yylval_token (TOK_INTCON);    }
{CHRCONST}        { return yylval_token (TOK_CHARCON);   }
{STRCONST}        { return yylval_token (TOK_STRINGCON); }

"[]"              { return yylval_token (TOK_ARRAY); }
"("               { return yylval_token ('('); }
")"               { return yylval_token (')'); }
"["               { return yylval_token ('['); }
"]"               { return yylval_token (']'); }
"{"               { return yylval_token ('{'); }
"}"               { return yylval_token ('}'); }
";"               { return yylval_token (';'); }
","               { return yylval_token (','); }  
"."               { return yylval_token ('.'); }
"="               { return yylval_token ('='); }
"=="              { return yylval_token (TOK_EQ); }
"!="              { return yylval_token (TOK_NE); }
"<"               { return yylval_token (TOK_LT); }
"<="              { return yylval_token (TOK_LE); }
">"               { return yylval_token (TOK_GT); }
">="              { return yylval_token (TOK_GE); }
"+"               { return yylval_token ('+'); }
"-"               { return yylval_token ('-'); }
"*"               { return yylval_token ('*'); }
"/"               { return yylval_token ('/'); }
"%"               { return yylval_token ('%'); }
"!"               { return yylval_token ('!'); }

{BADIDENT}        { lexer_badtoken (yytext);
                    return yylval_token (TOK_IDENT); }

{BADCHR}          { lexer_badtoken (yytext);
                    return yylval_token (TOK_CHARCON); }

{BADSTR}          { lexer_badtoken (yytext); 
                    return yylval_token (TOK_STRINGCON); }

.                 { lexer_badchar (*yytext); }

%%