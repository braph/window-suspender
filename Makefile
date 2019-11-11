# Options
DEBUG    ?= 0
CC_FLAGS ?= -Wall -Wpedantic -O2
# /Options

PKG_CONFIG_LIBS  := $(shell pkg-config --libs   libwnck-3.0 glib-2.0)
PKG_CONFIG_FLAGS := $(shell pkg-config --cflags libwnck-3.0 glib-2.0)
CC_FLAGS   += -DDEBUG=$(DEBUG) $(PKG_CONFIG_FLAGS)
LEXER_FILE = lexer_$(LEXER)

SRC_FILES := window-suspender manager wnck common conditional statement system kill config.parser.tab config.lexer
OBJ_FILES := $(addsuffix .o, $(SRC_FILES))
SRC_FILES := $(addsuffix .c, $(SRC_FILES))
OBJ_FILES := $(addprefix objs/, $(OBJ_FILES))

build: window-suspender
	@size objs/*
	@du -b window-suspender

# Parser
config.parser.tab.h: config.parser.tab.c
config.parser.tab.c:
	yacc -d -o config.parser.tab.c config.parser.y # -t for tracing

config.lexer.c: config.parser.tab.h
	gperf -m1000 -n config.lexer.gperf.c > config.lexer.c

objs/%.o: %.c common.h actions.gen.h
	@mkdir -p objs
	$(CC) $(CC_FLAGS) $*.c -c -o objs/$*.o

window-suspender: $(OBJ_FILES) .force
	$(CC) $(CC_FLAGS) $(PKG_CONFIG_LIBS) objs/*.o -o window-suspender

clean:
	rm -f window-suspender
	rm -rf objs

clean-parser: clean
	rm -f config.lexer.c config.parser.tab.c config.parser.tab.h

LOC:
	wc -l $(SRC_FILES)

.force:

