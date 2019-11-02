DEBUG ?= 0

PKG_CONFIG := $(shell pkg-config --libs --cflags libwnck-3.0 glib-2.0)
CC_FLAGS = -DDEBUG=$(DEBUG) -Wall -Wpedantic

OBJS = manager wnck common conditional statement system kill
SOURCE_FILES := $(addsuffix .c, $(OBJS))
OBJS += lex.yy y.tab
OBJS := $(addsuffix .o, $(OBJS))
OBJS := $(addprefix objs/, $(OBJS))

window-suspender: lex.yy.c y.tab.c $(OBJS) 
	$(CC) $(CC_FLAGS) $(PKG_CONFIG) objs/*.o window-suspender.c -o window-suspender

lex.yy.c:
	lex config.l

y.tab.c:
	yacc -d -t config.y

objs/%.o:
	@mkdir -p objs
	$(CC) $(CC_FLAGS) $(PKG_CONFIG) $*.c -c -o objs/$*.o

clean:
	rm -f window-suspender
	rm -rf objs

clean-yacc:
	rm -f lex.yy.c y.tab.c y.tab.h

LOC:
	wc -l $(SOURCE_FILES)

.force:

