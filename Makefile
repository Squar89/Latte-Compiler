CC=g++
CCFLAGS=-g -W -Wall -std=c++17

FLEX=flex
FLEX_OPTS=-PLatte

BISON=bison
BISON_OPTS=-t -pLatte

OBJS=Absyn.o Lexer.o Parser.o Printer.o Analyser.o

.PHONY: clean distclean

all: TestLatte Compiler

clean:
	rm -f *.o TestLatte Latte.aux Latte.log Latte.pdf Latte.dvi Latte.ps Latte latc_x86_64

distclean: clean
	rm -f Absyn.C Absyn.H Test.C Parser.C Parser.H Lexer.C Skeleton.C Skeleton.H Printer.C Printer.H Makefile Latte.l Latte.y Latte.tex Analyser.C Analyser.H

Compiler: ${OBJS} Compiler.o
	${CC} ${CCFLAGS} ${OBJS} Compiler.o -o latc_x86_64

Compiler.o: Compiler.C Parser.H Printer.H Absyn.H
	${CC} ${CCFLAGS} -c Compiler.C

TestLatte: ${OBJS} Test.o
	@echo "Linking TestLatte..."
	${CC} ${CCFLAGS} ${OBJS} Test.o -o TestLatte

Absyn.o: Absyn.C Absyn.H
	${CC} ${CCFLAGS} -c Absyn.C

Lexer.C: Latte.l
	${FLEX} -oLexer.C Latte.l

Parser.C: Latte.y
	${BISON} Latte.y -o Parser.C

Lexer.o: Lexer.C Parser.H
	${CC} ${CCFLAGS} -c Lexer.C 

Parser.o: Parser.C Absyn.H
	${CC} ${CCFLAGS} -c Parser.C

Printer.o: Printer.C Printer.H Absyn.H
	${CC} ${CCFLAGS} -c Printer.C

Analyser.o: Analyser.C Analyser.H Absyn.H
	${CC} ${CCFLAGS} -Wno-unused-parameter -c Analyser.C

Skeleton.o: Skeleton.C Skeleton.H Absyn.H
	${CC} ${CCFLAGS} -Wno-unused-parameter -c Skeleton.C

Test.o: Test.C Parser.H Printer.H Absyn.H Analyser.H
	${CC} ${CCFLAGS} -c Test.C
