include config.mk

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

.PHONY: all clean distclean
