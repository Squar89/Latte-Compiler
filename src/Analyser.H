#ifndef ANALYSER_HEADER
#define ANALYSER_HEADER

#include <unordered_map>
#include <stack>
#include "Absyn.H"
#include "Codes.H"

class Analyser : public Visitor
{
public:
  void analyseProgram(Program *p, Program *lib);
  void getFunctionsDefinitions(Prog *prog);

  void visitProgram(Program *p);
  void visitTopDef(TopDef *p);
  void visitArg(Arg *p);
  void visitBlock(Block *p);
  void visitStmt(Stmt *p);
  void visitItem(Item *p);
  void visitItemArr(ItemArr *p);
  void visitSimpleType(SimpleType *p);
  void visitArrType(ArrType *p);
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
  void visitDeclArr(DeclArr *p);
  void visitNoInit(NoInit *p);
  void visitInit(Init *p);
  void visitNoInitArr(NoInitArr *p);
  void visitInitArr(InitArr *p);
  void visitInitArrE(InitArrE *p);
  void visitInitArrSt(InitArrSt *p);
  void visitAss(Ass *p);
  void visitAssArr(AssArr *p);
  void visitIncr(Incr *p);
  void visitIncrArr(IncrArr *p);
  void visitDecr(Decr *p);
  void visitDecrArr(DecrArr *p);
  void visitRet(Ret *p);
  void visitVRet(VRet *p);
  void visitCond(Cond *p);
  void visitCondElse(CondElse *p);
  void visitWhile(While *p);
  void visitForEach(ForEach *p);
  void visitSExp(SExp *p);
  void visitInt(Int *p);
  void visitStr(Str *p);
  void visitBool(Bool *p);
  void visitVoid(Void *p);
  void visitArr(Arr *p);
  void visitAbsAT(AbsAT *p);
  void visitAbsST(AbsST *p);
  void visitEVar(EVar *p);
  void visitEVarArr(EVarArr *p);
  void visitEAtr(EAtr *p);
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
  void visitListSimpleType(ListSimpleType *p);
  void visitListArrType(ListArrType *p);
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
  void declareVariable(Ident ident, int lineNumber, bool isArray, bool arrayInitialized);

  unsigned long currentDepth;
  int currentReturnType;
  bool returnFound, lastStmtWasReturn, ifFullReturn;
  std::unordered_map<Ident, FnDef*> functionsMap;
  std::stack<int> typesStack;
  std::unordered_map<Ident, std::pair<int, unsigned long> > variablesMap;//<Ident, <typeCode, declarationDepth>>
  std::unordered_map<Ident, std::pair<bool, unsigned long> > arraysMap;//<Ident, <initialized, arrLength>>
  std::vector<std::vector<std::pair<Ident, std::pair<int, unsigned long> > > > localRedeclVar;//localRedeclVar[2] holds shadowed variables on depth of 2
  std::vector<std::vector<Ident> > localVar;//localVar[2] holds Idents of variables declared on depth of 2
  std::vector<std::vector<std::pair<Ident, std::pair<bool, unsigned long> > > > localRedeclArr;//localRedeclArr[2] holds shadowed arrays on depth of 2
  std::vector<std::vector<Ident> > localArr;//localArr[2] holds Idents of arrays declared on depth of 2
};


#endif /* ANALYSER_HEADER */
