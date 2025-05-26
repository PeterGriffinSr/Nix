%{
    #include <stdio.h>
    #include <string.h>
    #include "ast.h"

    extern FILE *yyin;
    extern ASTNode *root;

    ASTNode *root = NULL;
    int yylex(void);
    void yyerror(const char *s);

    void yyerror(const char *s) {
        fprintf(stderr, "Parse error: %s\n", s);
    }
%}

%union {
    int intval;
    double floatval;
    char* strval;
    char charval;
    int boolval;
    struct ASTNode* node;
}

%token <intval> IntLit
%token <floatval> FloatLit
%token <charval> CharLit
%token <strval> StringLit
%token <boolval> BoolLit
%token <strval> Ident

%left LogicalOr
%left LogicalAnd
%left Equal NotEqual
%left Less Greater LessEqual GreaterEqual
%left Plus Minus
%left Star Slash
%left PlusFloat MinusFloat
%left StarFloat SlashFloat
%right Not
%nonassoc Else

%token LParen RParen LBracket RBracket LBrace RBrace Plus Minus Star Slash Assignment Comma Dot Underscore Pipe Less Greater Colon Semi
%token Equal NotEqual LessEqual GreaterEqual ThiccArrow SkinnyArrow LogicalAnd LogicalOr
%token Val Mod Type Match With If Else None Some Ok Error Then
%token Int Float Char String Bool

%type <node> program statement statements primary_expr expr var_decl

%%

program:
    statements { root = $$; }
;

statements:
    statement { $$ = $1; }
    | statements statement { $$ = append_statements($1, $2); }
;

statement:
    expr Semi { $$ = $1; }
    | var_decl Semi { $$ = $1; }
;

expr:
    expr Plus expr { $$ = create_binary_node("+", $1, $3); }
  | expr Minus expr { $$ = create_binary_node("-", $1, $3); }
  | expr Star expr { $$ = create_binary_node("*", $1, $3); }
  | expr Slash expr { $$ = create_binary_node("/", $1, $3); }
  | expr Less expr { $$ = create_binary_node("<", $1, $3); }
  | expr Greater expr { $$ = create_binary_node(">", $1, $3); }
  | expr Equal expr { $$ = create_binary_node("==", $1, $3); }
  | expr NotEqual expr { $$ = create_binary_node("!=", $1, $3); }
  | expr LessEqual expr { $$ = create_binary_node("<=", $1, $3); }
  | expr GreaterEqual expr { $$ = create_binary_node(">=", $1, $3); }
  | expr LogicalAnd expr { $$ = create_binary_node("&&", $1, $3); }
  | expr LogicalOr expr { $$ = create_binary_node("||", $1, $3); }
  | primary_expr { $$ = $1; }
;

primary_expr:
    IntLit      { $$ = create_int_node($1); }
  | FloatLit    { $$ = create_float_node($1); }
  | CharLit     { $$ = create_char_node($1); }
  | StringLit   { $$ = create_string_node($1); }
  | BoolLit     { $$ = create_bool_node($1); }
  | Ident       { $$ = create_identifier_node($1); }
  | Int LParen expr RParen { ASTNode **args = malloc(sizeof(ASTNode*)); args[0] = $3; $$ = create_type_call_node("Int", args, 1); }
  | Float LParen expr RParen { ASTNode **args = malloc(sizeof(ASTNode*)); args[0] = $3; $$ = create_type_call_node("Float", args, 1); }
  | Bool LParen expr RParen { ASTNode **args = malloc(sizeof(ASTNode*)); args[0] = $3; $$ = create_type_call_node("Bool", args, 1); }
  | Char LParen expr RParen { ASTNode **args = malloc(sizeof(ASTNode*)); args[0] = $3; $$ = create_type_call_node("Char", args, 1); }
  | String LParen expr RParen { ASTNode **args = malloc(sizeof(ASTNode*)); args[0] = $3; $$ = create_type_call_node("String", args, 1); }
;

var_decl:
    Val Ident Assignment expr { $$ = create_var_node($2, $4); }
;