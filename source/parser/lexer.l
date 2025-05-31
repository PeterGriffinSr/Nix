%{
    #include "parser.h"
    #include <stdlib.h>
    #include <string.h>

    #if defined(_WIN32) || defined(_WIN64)
        #include <io.h>
        #include <windows.h>
        #define strdup _strdup
        #define fileno _fileno
        #define isatty _isatty
    #else
        #include <unistd.h>
    #endif

    int yycolumn = 1;
    const char *filename;
%}

%option noinput nounput
%option nounistd

Digit           [0-9]
Letter          [a-zA-Z_]
Identifier      {Letter}({Letter}|{Digit})*
IntLiteral      {Digit}+
FloatLiteral    ({Digit}+"."{Digit}*|{Digit}*"."{Digit}+)
CharLiteral     \'(\\.|[^\\'])\'
StringLiteral   \"(\\.|[^\\"])*\"

%%

"#".*           { /* Ignore single-line comments */ }

"("             { yycolumn += yyleng; return LParen; }
")"             { yycolumn += yyleng; return RParen; }
"["             { yycolumn += yyleng; return LBracket; }
"]"             { yycolumn += yyleng; return RBracket; }
"{"             { yycolumn += yyleng; return LBrace; }
"}"             { yycolumn += yyleng; return RBrace; }
"+"             { yycolumn += yyleng; return Plus; }
"-"             { yycolumn += yyleng; return Minus; }
"*"             { yycolumn += yyleng; return Star; }
"/"             { yycolumn += yyleng; return Slash; }
"="             { yycolumn += yyleng; return Assignment; }
","             { yycolumn += yyleng; return Comma; }
"."             { yycolumn += yyleng; return Dot; }
"_"             { yycolumn += yyleng; return Underscore; }
"|"             { yycolumn += yyleng; return Pipe; }
"<"             { yycolumn += yyleng; return Less; }
">"             { yycolumn += yyleng; return Greater; }
":"             { yycolumn += yyleng; return Colon; }
";"             { yycolumn += yyleng; return Semi; }

"=="            { yycolumn += yyleng; return Equal; }
"!="            { yycolumn += yyleng; return NotEqual; }
"<="            { yycolumn += yyleng; return LessEqual; }
">="            { yycolumn += yyleng; return GreaterEqual; }
"||"            { yycolumn += yyleng; return LogicalOr; }
"&&"            { yycolumn += yyleng; return LogicalAnd; }
"=>"            { yycolumn += yyleng; return ThiccArrow; }
"->"            { yycolumn += yyleng; return SkinnyArrow; }

"val"           { yycolumn += yyleng; return Val; }
"mod"           { yycolumn += yyleng; return Mod; }
"use"           { yycolumn += yyleng; return Use; }
"type"          { yycolumn += yyleng; return Type; }
"match"         { yycolumn += yyleng; return Match; }
"with"          { yycolumn += yyleng; return With; }
"if"            { yycolumn += yyleng; return If; }
"then"          { yycolumn += yyleng; return Then; }
"else"          { yycolumn += yyleng; return Else; }
"None"          { yycolumn += yyleng; return None; }
"Some"          { yycolumn += yyleng; return Some; }
"Ok"            { yycolumn += yyleng; return Ok; }
"Error"         { yycolumn += yyleng; return Error; }
"Int"           { yycolumn += yyleng; return Int; }
"Char"          { yycolumn += yyleng; return Char; }
"Bool"          { yycolumn += yyleng; return Bool; }
"Float"         { yycolumn += yyleng; return Float; }
"String"        { yycolumn += yyleng; return String; }
"True"          { yycolumn += yyleng; yylval.boolval = 1; return BoolLit; }
"False"         { yycolumn += yyleng; yylval.boolval = 0; return BoolLit; }

{CharLiteral}   { yycolumn += yyleng; yylval.charval = yytext[1]; return CharLit; }
{StringLiteral} { yycolumn += yyleng; yylval.strval = strdup(yytext); return StringLit; }
{IntLiteral}    { yycolumn += yyleng; yylval.intval = atoi(yytext); return IntLit; }
{FloatLiteral}  { yycolumn += yyleng; yylval.floatval = strtof(yytext, NULL); return FloatLit; }
{Identifier}    { yycolumn += yyleng; yylval.strval = strdup(yytext); return Ident; }

[ \t\r]+        { yycolumn += yyleng; }
\n              { yycolumn = 1; yylineno++; }

%%

int yywrap(void) { return 1; }