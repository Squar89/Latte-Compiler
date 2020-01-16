CC=g++
CCFLAGS=-g -W -Wall -std=c++17

FLEX=flex
FLEX_OPTS=-PLatte

BISON=bison
BISON_OPTS=-t -pLatte

OBJS=Absyn.o Lexer.o Parser.o Printer.o Analyser.o Compiler.o

SRC_DIR=src

.PHONY: clean distclean

all: TestLatte

clean:
	rm -f *.o TestLatte asm_result out.s

TestLatte: ${OBJS} Test.o
	@echo "Linking TestLatte..."
	${CC} ${CCFLAGS} ${OBJS} Test.o -o TestLatte

Absyn.o: ${SRC_DIR}/Absyn.C ${SRC_DIR}/Absyn.H
	${CC} ${CCFLAGS} -Wno-unused-parameter -c ${SRC_DIR}/Absyn.C

Lexer.C: ${SRC_DIR}/Latte.l
	${FLEX} -Wno-unused-parameter -o ${SRC_DIR}/Lexer.C ${SRC_DIR}/Latte.l

Parser.C: ${SRC_DIR}/Latte.y
	${BISON} -Wno-unused-parameter ${SRC_DIR}/Latte.y -o ${SRC_DIR}/Parser.C

Lexer.o: ${SRC_DIR}/Lexer.C ${SRC_DIR}/Parser.H
	${CC} ${CCFLAGS} -Wno-unused-parameter -c ${SRC_DIR}/Lexer.C 

Parser.o: ${SRC_DIR}/Parser.C ${SRC_DIR}/Absyn.H
	${CC} ${CCFLAGS} -Wno-unused-parameter -c ${SRC_DIR}/Parser.C

Printer.o: ${SRC_DIR}/Printer.C ${SRC_DIR}/Printer.H ${SRC_DIR}/Absyn.H
	${CC} ${CCFLAGS} -Wno-unused-parameter -c ${SRC_DIR}/Printer.C

Compiler.o: ${SRC_DIR}/Compiler.C ${SRC_DIR}/Compiler.H ${SRC_DIR}/Absyn.H ${SRC_DIR}/Codes.H
	${CC} ${CCFLAGS} -Wno-unused-parameter -c ${SRC_DIR}/Compiler.C

Analyser.o: ${SRC_DIR}/Analyser.C ${SRC_DIR}/Analyser.H ${SRC_DIR}/Absyn.H ${SRC_DIR}/Codes.H
	${CC} ${CCFLAGS} -Wno-unused-parameter -c ${SRC_DIR}/Analyser.C

Test.o: ${SRC_DIR}/Test.C ${SRC_DIR}/Parser.H ${SRC_DIR}/Printer.H ${SRC_DIR}/Absyn.H ${SRC_DIR}/Analyser.H ${SRC_DIR}/Compiler.H
	${CC} ${CCFLAGS} -c ${SRC_DIR}/Test.C
