#if defined(_WIN32) || defined(_WIN64)
#define strdup _strdup
#endif
#include "ast.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode *create_int_node(int value) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = NodeIntLiteral;
  node->u.intval = value;
  return node;
}

ASTNode *create_float_node(double value) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = NodeFloatLiteral;
  node->u.floatval = value;
  return node;
}

ASTNode *create_char_node(char value) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = NodeCharLiteral;
  node->u.charval = value;
  return node;
}

ASTNode *create_string_node(char *value) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = NodeStringLiteral;
  node->u.strval = value;
  return node;
}

ASTNode *create_bool_node(int value) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = NodeBoolLiteral;
  node->u.boolval = value;
  return node;
}

ASTNode *create_identifier_node(char *value) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = NodeIdent;
  node->u.strval = value;
  return node;
}

ASTNode *create_block_node(void) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = NodeBlock;
  node->blockStmt.count = 0;
  node->blockStmt.cap = 4;
  node->blockStmt.statements =
      malloc(sizeof(ASTNode *) * (size_t)node->blockStmt.cap);
  return node;
}

ASTNode *append_statements(ASTNode *block, ASTNode *stmt) {
  if (!block)
    block = create_block_node();
  if (block->type != NodeBlock) {
    ASTNode *new_block = create_block_node();
    new_block->blockStmt.statements[new_block->blockStmt.count++] = block;
    block = new_block;
  }

  if (block->blockStmt.count >= block->blockStmt.cap) {
    block->blockStmt.cap *= 2;
    block->blockStmt.statements =
        realloc(block->blockStmt.statements,
                sizeof(ASTNode *) * (size_t)block->blockStmt.cap);
  }

  block->blockStmt.statements[block->blockStmt.count++] = stmt;
  return block;
}

ASTNode *create_binary_node(const char *op, ASTNode *left, ASTNode *right) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = NodeBinary;
  node->binaryExpr.op = op;
  node->binaryExpr.left = left;
  node->binaryExpr.right = right;
  return node;
}

ASTNode *create_var_node(char *name, ASTNode *value) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = NodeVarDecl;
  node->varDecl.name = name;
  node->varDecl.value = value;
  return node;
}

ASTNode *create_type_call_node(const char *type_name, ASTNode **args,
                               int arg_count) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = NodeTypeCall;
  node->typeCall.type_name = type_name;
  node->typeCall.args = args;
  node->typeCall.arg_count = arg_count;
  return node;
}

ASTNode *create_function_node(char *name, ASTNode *param_list_node,
                              ASTNode *return_type, ASTNode *body) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = NodeFunction;
  node->functionCall.name = strdup(name);
  node->functionCall.param_count = param_list_node->functionCall.param_count;
  node->functionCall.params = param_list_node->functionCall.params;
  node->functionCall.return_type = return_type;
  node->functionCall.body = body;
  return node;
}

ASTNode *create_type_node(const char *type_name) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = _NodeType;
  node->_type.type_name = strdup(type_name);
  return node;
}

ASTNode *create_param_list_node(ASTNode **params, int count) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = NodeFunction;
  node->functionCall.params = params;
  node->functionCall.param_count = count;
  return node;
}

ASTNode *append_param_list(ASTNode *existing, ASTNode *new_param) {
  int old_count = existing->functionCall.param_count;
  ASTNode **new_list = malloc(sizeof(ASTNode *) * ((size_t)old_count + 1));
  for (int i = 0; i < old_count; i++) {
    new_list[i] = existing->functionCall.params[i];
  }
  new_list[old_count] = new_param;
  free(existing->functionCall.params);
  existing->functionCall.params = new_list;
  existing->functionCall.param_count += 1;
  return existing;
}

ASTNode *create_if_node(ASTNode *condition, ASTNode *then_branch,
                        ASTNode *else_branch) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = NodeIf;
  node->If.condition = condition;
  node->If.then_branch = then_branch;
  node->If.else_branch = else_branch;
  return node;
}

ASTNode *create_mod_node(const char *name, ASTNode *body) {
  ASTNode *node = malloc(sizeof(ASTNode));
  node->type = NodeMod;
  node->Mod.name = strdup(name);
  node->Mod.body = body;
  return node;
}

void indent_print(int indent, const char *fmt, ...) {
  for (int i = 0; i < indent; i++) {
    printf("  ");
  }

  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}

void printAST(ASTNode *node, int indent) {
  if (!node) {
    indent_print(indent, "NULL\n");
    return;
  }

  switch (node->type) {
  case NodeIntLiteral:
    indent_print(indent, "IntLiteral: %d\n", node->u.intval);
    break;
  case NodeFloatLiteral:
    indent_print(indent, "FloatLiteral: %lf\n", node->u.floatval);
    break;
  case NodeCharLiteral:
    indent_print(indent, "CharLiteral: %c\n", node->u.charval);
    break;
  case NodeStringLiteral:
    indent_print(indent, "StringLiteral: %s\n", node->u.strval);
    break;
  case NodeBoolLiteral:
    indent_print(indent, "BoolLiteral: %d\n", node->u.boolval);
    break;
  case NodeIdent:
    indent_print(indent, "Identifier: %s\n", node->u.strval);
    break;
  case NodeBlock:
    indent_print(indent, "Block:\n");
    for (int i = 0; i < node->blockStmt.count; ++i) {
      printAST(node->blockStmt.statements[i], indent + 1);
    }
    break;
  case NodeBinary:
    indent_print(indent, "BinaryOp: '%s'\n", node->binaryExpr.op);
    printAST(node->binaryExpr.left, indent + 1);
    printAST(node->binaryExpr.right, indent + 1);
    break;
  case NodeVarDecl:
    indent_print(indent, "VarDecl: %s\n", node->varDecl.name);
    printAST(node->varDecl.value, indent + 1);
    break;
  case NodeTypeCall:
    indent_print(indent, "TypeCall: %s\n", node->typeCall.type_name);
    for (int i = 0; i < node->typeCall.arg_count; ++i) {
      printAST(node->typeCall.args[i], indent + 1);
    }
    break;
  case NodeFunction:
    indent_print(indent, "Function: %s\n", node->functionCall.name);
    indent_print(indent + 1, "Params:\n");
    for (int i = 0; i < node->functionCall.param_count; ++i) {
      printAST(node->functionCall.params[i], indent + 2);
    }
    indent_print(indent + 1, "Return Type:\n");
    printAST(node->functionCall.return_type, indent + 2);
    indent_print(indent + 1, "Body:\n");
    printAST(node->functionCall.body, indent + 2);
    break;
  case _NodeType:
    indent_print(indent, "Type: %s\n", node->_type.type_name);
    break;
  case NodeIf:
    indent_print(indent, "If:\n");
    indent_print(indent + 1, "Condition:\n");
    printAST(node->If.condition, indent + 2);
    indent_print(indent + 1, "Then:\n");
    printAST(node->If.then_branch, indent + 2);
    if (node->If.else_branch) {
      indent_print(indent + 1, "Else:\n");
      printAST(node->If.else_branch, indent + 2);
    }
    break;
  case NodeMod:
    indent_print(indent, "Mod: %s\n", node->Mod.name);
    printAST(node->Mod.body, indent + 1);
    break;
  default:
    indent_print(indent, "Unknown node type\n");
    break;
  }
}

void freeAST(ASTNode *node) {
  if (!node) {
    return;
  }

  switch (node->type) {
  case NodeIntLiteral:
  case NodeBoolLiteral:
  case NodeFloatLiteral:
  case NodeCharLiteral:
    break;
  case NodeStringLiteral:
    free(node->u.strval);
    break;
  case NodeIdent:
    free(node->u.strval);
    break;
  case NodeBlock:
    for (int i = 0; i < node->blockStmt.count; ++i) {
      freeAST(node->blockStmt.statements[i]);
    }
    free(node->blockStmt.statements);
    break;
  case NodeBinary:
    freeAST(node->binaryExpr.left);
    freeAST(node->binaryExpr.right);
    break;
  case NodeVarDecl:
    free(node->varDecl.name);
    freeAST(node->varDecl.value);
    break;
  case NodeTypeCall:
    for (int i = 0; i < node->typeCall.arg_count; ++i) {
      freeAST(node->typeCall.args[i]);
    }
    free(node->typeCall.args);
    break;
  case NodeFunction:
    free(node->functionCall.name);
    for (int i = 0; i < node->functionCall.param_count; ++i) {
      freeAST(node->functionCall.params[i]);
    }
    free(node->functionCall.params);
    freeAST(node->functionCall.return_type);
    freeAST(node->functionCall.body);
    break;
  case _NodeType:
    free((char *)node->_type.type_name);
    break;
  case NodeIf:
    freeAST(node->If.condition);
    freeAST(node->If.then_branch);
    freeAST(node->If.else_branch);
    break;
  case NodeMod:
    free(node->Mod.name);
    freeAST(node->Mod.body);
    break;
  default:
    fprintf(stderr, "Warning: Unknown node type in freeAST\n");
    break;
  }

  free(node);
}
