all: floc

floc.tab.c floc.tab.h:	floc.y
	bison -d floc.y

lex.yy.c: floc.l floc.tab.h
	flex -d floc.l

floc: lex.yy.c floc.tab.c floc.tab.h ast.c ast.h
	gcc -o floc floc.tab.c lex.yy.c ast.c

clean:
	rm floc floc.tab.c lex.yy.c floc.tab.h
