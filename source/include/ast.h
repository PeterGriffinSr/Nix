#ifndef AST_H
#define AST_H

typedef enum {
  NodeIntLiteral,
  NodeFloatLiteral,
  NodeCharLiteral,
  NodeStringLiteral,
  NodeBoolLiteral,
  NodeIdent,
  NodeBlock,
  NodeBinary,
  NodeVarDecl,
  NodeTypeCall,
} NodeType;

typedef struct ASTNode ASTNode;

struct ASTNode {
  NodeType type;

  union {
    int intval;
    double floatval;
    char *strval;
    char charval;
    int boolval;
  } u;
  struct {
    struct ASTNode **statements;
    int cap, count;
  } blockStmt;
  struct {
    const char *op;
    struct ASTNode *left, *right;
  } binaryExpr;
  struct {
    char *name;
    ASTNode *value;
  } varDecl;
  struct {
    const char *type_name;
    ASTNode **args;
    int arg_count;
  } typeCall;
};

ASTNode *create_block_node(void);
ASTNode *create_int_node(int value);
ASTNode *create_bool_node(int value);
ASTNode *create_char_node(char value);
ASTNode *create_float_node(double value);
ASTNode *create_string_node(char *value);
ASTNode *create_identifier_node(char *value);
ASTNode *create_var_node(char *name, ASTNode *value);
ASTNode *append_statements(ASTNode *block, ASTNode *stmts);
ASTNode *create_binary_node(const char *op, ASTNode *left, ASTNode *right);
ASTNode *create_type_call_node(const char *type_name, ASTNode **args,
                               int arg_count);

void freeAST(ASTNode *node);
void printAST(ASTNode *node, int indent);
void indent_print(int indent, const char *fmt, ...);

#endif // AST_H
