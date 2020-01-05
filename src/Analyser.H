#ifndef ANALYSER_HEADER
#define ANALYSER_HEADER

#include <unordered_map>
#include <stack>
#include "Absyn.H"

/* ERROR CODES */
#define UNEXPECTED_ERROR    -1
#define MAIN_NOT_FOUND      1
#define DUPLICATE_FUN_NAME  2
#define TYPE_ERROR          3
#define FUNCTION_NOT_FOUND  4
#define VAR_REDECLARATION   5
#define VAR_NOT_DECLARED    6
#define RETURN_NOT_FOUND    7

/* STACK CODES */
#define PAUSE_CODE          -1
#define INT_CODE            1
#define STRING_CODE         2
#define VOID_CODE           3
#define BOOL_CODE           4
#define DOUBLE_CODE         5
#define CHAR_CODE           6
#define IDENT_CODE          7

class Analyser : public Visitor
{
public:
  void analyseProgram(Program *p);
  void getFunctionsDefinitions(Prog *prog);

  void visitProgram(Program *p);
  void visitTopDef(TopDef *p);
  void visitArg(Arg *p);
  void visitBlock(Block *p);
  void visitStmt(Stmt *p);
  void visitItem(Item *p);
  void visitType(Type *p);
  void visitExpr(Expr *p);
  void visitAddOp(AddOp *p);
  void visitMulOp(MulOp *p);
  void visitRelOp(RelOp *p);
  void visitProg(Prog *p);
  void visitFnDef(FnDef *p);
  void visitAr(Ar *p);
  void visitBlk(Blk *p);
  void visitEmpty(Empty *p);
  void visitBStmt(BStmt *p);
  void visitDecl(Decl *p);
  void visitNoInit(NoInit *p);
  void visitInit(Init *p);
  void visitAss(Ass *p);
  void visitIncr(Incr *p);
  void visitDecr(Decr *p);
  void visitRet(Ret *p);
  void visitVRet(VRet *p);
  void visitCond(Cond *p);
  void visitCondElse(CondElse *p);
  void visitWhile(While *p);
  void visitSExp(SExp *p);
  void visitInt(Int *p);
  void visitStr(Str *p);
  void visitBool(Bool *p);
  void visitVoid(Void *p);
  void visitFun(Fun *p);
  void visitEVar(EVar *p);
  void visitELitInt(ELitInt *p);
  void visitELitTrue(ELitTrue *p);
  void visitELitFalse(ELitFalse *p);
  void visitEApp(EApp *p);
  void visitEString(EString *p);
  void visitNeg(Neg *p);
  void visitNot(Not *p);
  void visitEMul(EMul *p);
  void visitEAdd(EAdd *p);
  void visitERel(ERel *p);
  void visitEAnd(EAnd *p);
  void visitEOr(EOr *p);
  void visitPlus(Plus *p);
  void visitMinus(Minus *p);
  void visitTimes(Times *p);
  void visitDiv(Div *p);
  void visitMod(Mod *p);
  void visitLTH(LTH *p);
  void visitLE(LE *p);
  void visitGTH(GTH *p);
  void visitGE(GE *p);
  void visitEQU(EQU *p);
  void visitNE(NE *p);
  void visitListTopDef(ListTopDef *p);
  void visitListArg(ListArg *p);
  void visitListStmt(ListStmt *p);
  void visitListItem(ListItem *p);
  void visitListType(ListType *p);
  void visitListExpr(ListExpr *p);

  void visitInteger(Integer x);
  void visitChar(Char x);
  void visitDouble(Double x);
  void visitString(String x);
  void visitIdent(Ident x);

private:
  bool correctMainExists();
  void checkTypesMatch(int lineNumber);
  void checkTypesFunctionCall(int lineNumber, Ident ident, std::vector<int> &reversedCallTypes);
  void declareVariable(Ident ident, int lineNumber);

  unsigned long currentDepth;
  int currentReturnType;
  bool returnFound, lastStmtWasReturn, ifFullReturn;
  std::unordered_map<Ident, FnDef*> functionsMap;
  std::stack<int> typesStack;
  std::unordered_map<Ident, std::pair<int, unsigned long> > variablesMap;//<Ident, <typeCode, declarationDepth>>
  std::vector<std::vector<std::pair<Ident, std::pair<int, unsigned long> > > > localRedeclVar;//localRedeclVar[2] holds shadowed variables on depth of 2
  std::vector<std::vector<Ident> > localVar;//localVar[2] holds Idents of variables declared on depth of 2
};


#endif /* ANALYSER_HEADER */