CC = musl-gcc
FLEX = flex
BISON = bison
AR = ar rc
RM = rm

CSTD = -std=c11
WARNINGS = \
	-Wall -Wextra -Wshadow -Wundef -Wwrite-strings -Wconversion \
	-Wstrict-overflow=2 -Wdeclaration-after-statement \
	-Wmissing-prototypes -Wnested-externs -Wc++-compat \
	-Wold-style-definition -Wlogical-op

CFLAGS = -O2 $(CSTD) $(WARNINGS) -fno-stack-protector -fno-common -march=native
LDFLAGS = 

SRC_DIR = source
INCLUDE_DIR = $(SRC_DIR)/include
PARSER_DIR = $(SRC_DIR)/parser
AST_DIR = $(SRC_DIR)/ast
CORE_DIR = $(SRC_DIR)/core

LEXER_SRC = $(PARSER_DIR)/lexer.l
PARSER_SRC = $(PARSER_DIR)/parser.y

LEXER_GEN = $(PARSER_DIR)/lexer.c
PARSER_GEN = $(PARSER_DIR)/parser.c
PARSER_HDR = $(PARSER_DIR)/parser.h

SRCS = \
	$(SRC_DIR)/nix.c \
	$(CORE_DIR)/common.c \
	$(AST_DIR)/ast.c \
	$(LEXER_GEN) \
	$(PARSER_GEN)

OBJS = $(SRCS:.c=.o)

CPPFLAGS = -I$(INCLUDE_DIR) -I$(PARSER_DIR)

TARGET = nix
