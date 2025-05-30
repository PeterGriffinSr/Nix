include config.mk

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    OS := linux
else ifeq ($(UNAME_S),Darwin)
    OS := macos
else
    OS := unknown
endif

ifeq ($(OS),linux)
    CFLAGS += -D_POSIX_C_SOURCE=200809L
    LDFLAGS += -Wl,-E
    RM = rm -f
endif

all: $(TARGET)

$(PARSER_GEN) $(PARSER_HDR): $(PARSER_SRC)
	$(BISON) -d -o $(PARSER_GEN) $(PARSER_SRC)

$(LEXER_GEN): $(LEXER_SRC)
	$(FLEX) -o $@ $<

$(SRC_DIR)/nix.o: $(PARSER_HDR)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

clean:
	$(RM) $(OBJS) $(LEXER_GEN) $(PARSER_GEN) $(PARSER_HDR) $(TARGET)

distclean: clean
	$(RM) *~ */*~ */*/*.o

depend:
	@$(CC) $(CFLAGS) -MM *.c > .depend

.PHONY: all clean distclean

-include .depend
