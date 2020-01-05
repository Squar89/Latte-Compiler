CC=g++
CCFLAGS=-g -W -Wall -std=c++17

FLEX=flex
FLEX_OPTS=-PLatte

BISON=bison
BISON_OPTS=-t -pLatte

OBJS=Absyn.o Lexer.o Parser.o Printer.o Analyser.o

SRC_DIR=src

.PHONY: clean distclean

all: TestLatte Compiler

clean:
	rm -f *.o TestLatte latc_x86_64

distclean: clean
	rm -f Absyn.C Absyn.H Test.C Parser.C Parser.H Lexer.C Skeleton.C Skeleton.H Printer.C Printer.H Makefile Latte.l Latte.y Latte.tex Analyser.C Analyser.H

Compiler: ${OBJS} Compiler.o
	${CC} ${CCFLAGS} ${OBJS} Compiler.o -o latc_x86_64

Compiler.o: ${SRC_DIR}/Compiler.C ${SRC_DIR}/Parser.H ${SRC_DIR}/Printer.H ${SRC_DIR}/Absyn.H
	${CC} ${CCFLAGS} -c ${SRC_DIR}/Compiler.C

TestLatte: ${OBJS} Test.o
	@echo "Linking TestLatte..."
	${CC} ${CCFLAGS} ${OBJS} Test.o -o TestLatte

Absyn.o: ${SRC_DIR}/Absyn.C ${SRC_DIR}/Absyn.H
	${CC} ${CCFLAGS} -c ${SRC_DIR}/Absyn.C

Lexer.C: ${SRC_DIR}/Latte.l
	${FLEX} -o ${SRC_DIR}/Lexer.C ${SRC_DIR}/Latte.l

Parser.C: ${SRC_DIR}/Latte.y
	${BISON} ${SRC_DIR}/Latte.y -o ${SRC_DIR}/Parser.C

Lexer.o: ${SRC_DIR}/Lexer.C ${SRC_DIR}/Parser.H
	${CC} ${CCFLAGS} -c ${SRC_DIR}/Lexer.C 

Parser.o: ${SRC_DIR}/Parser.C ${SRC_DIR}/Absyn.H
	${CC} ${CCFLAGS} -c ${SRC_DIR}/Parser.C

Printer.o: ${SRC_DIR}/Printer.C ${SRC_DIR}/Printer.H ${SRC_DIR}/Absyn.H
	${CC} ${CCFLAGS} -c ${SRC_DIR}/Printer.C

Analyser.o: ${SRC_DIR}/Analyser.C ${SRC_DIR}/Analyser.H ${SRC_DIR}/Absyn.H
	${CC} ${CCFLAGS} -Wno-unused-parameter -c ${SRC_DIR}/Analyser.C

Skeleton.o: ${SRC_DIR}/Skeleton.C ${SRC_DIR}/Skeleton.H ${SRC_DIR}/Absyn.H
	${CC} ${CCFLAGS} -Wno-unused-parameter -c ${SRC_DIR}/Skeleton.C

Test.o: ${SRC_DIR}/Test.C ${SRC_DIR}/Parser.H ${SRC_DIR}/Printer.H ${SRC_DIR}/Absyn.H ${SRC_DIR}/Analyser.H
	${CC} ${CCFLAGS} -c ${SRC_DIR}/Test.C
