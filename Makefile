# Options
DEBUG    ?= 0
LEXER    ?= gperf # 'lex' for using lex, 'gperf' for using gperf
CC_FLAGS ?= -Wall -Wpedantic -O2
# /Options

PKG_CONFIG_LIBS  := $(shell pkg-config --libs   libwnck-3.0 glib-2.0)
PKG_CONFIG_FLAGS := $(shell pkg-config --cflags libwnck-3.0 glib-2.0)
CC_FLAGS   += -DDEBUG=$(DEBUG) $(PKG_CONFIG_FLAGS)
LEXER_FILE = lexer_$(LEXER)

SRC_FILES := window-suspender manager wnck common conditional statement system kill y.tab $(LEXER_FILE)
OBJ_FILES := $(addsuffix .o, $(SRC_FILES))
SRC_FILES := $(addsuffix .c, $(SRC_FILES))
OBJ_FILES := $(addprefix objs/, $(OBJ_FILES))

build: window-suspender
	@size objs/*
	@du -b window-suspender

# Parser
y.tab.h: y.tab.c
y.tab.c:
	yacc -d config.y # -t for tracing

# Use the lexer produced by `lex`
lexer_lex.c: y.tab.h
	lex -t config.lex.l > lexer_lex.c

# Use our hand written lexer (gperf is needed to build the hash table)
lexer_gperf.c: y.tab.h
	gperf -m1000 config.gperf.c > lexer_gperf.c

objs/%.o: %.c common.h actions.gen.h
	@mkdir -p objs
	$(CC) $(CC_FLAGS) $*.c -c -o objs/$*.o

window-suspender: $(OBJ_FILES) .force
	$(CC) $(CC_FLAGS) $(PKG_CONFIG_LIBS) objs/*.o -o window-suspender

clean:
	rm -f window-suspender
	rm -rf objs

clean-parser: clean
	rm -f lex.yy.c y.tab.c y.tab.h

LOC:
	wc -l $(SRC_FILES)

.force:

