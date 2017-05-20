all: calc

calc.tab.c calc.tab.h:	calc.y
	bison -d calc.y

lex.yy.c: calc.l calc.tab.h
	flex -d calc.l

calc: lex.yy.c calc.tab.c calc.tab.h astgen.c astgen.h
	gcc -o calc calc.tab.c lex.yy.c astgen.c

clean:
	rm calc calc.tab.c lex.yy.c calc.tab.h
