#ifndef COMPILER_HEADER
#define COMPILER_HEADER

#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <cstring>
#include "Absyn.H"
#include "Codes.H"

#define BUFFER_INITIAL 2000

class Compiler : public Visitor
{
public:
  char* compile(Program *p, Program *lib);

protected:
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
  void getFunctionsTypes(Prog *p);
  void declareVariable(Ident ident);
  void pushArgsOntoStack(ListExpr *p);
  void countArgs(ListExpr *p);
  void performDiv();
  void alignStack(int numberOfArgs);
  void removePadding();
  void freeStrings(std::unordered_set<int> allocatedStringsSet);
  void checkInBounds();
  void mallocArray();

  char *buf_;
  int cur_, buf_size;
  unsigned int andOrLabelCounter, stackCounter, condCounter, whileCounter, argCounter, stringConcatLabelCounter;
  unsigned int functionLabelCounter, stackCounterBeforeFn, divisionCounter, skipFreeCounter, inBoundsCounter;
  unsigned int forEachCounter, checkArraySizeCounter;
  bool functionBlock;
  std::stack<int> typesStack;
  std::unordered_map<Ident, int> variablesMap;//<Ident, offset from RBP>
  std::unordered_map<Ident, int> typesMap;//<Ident, type>
  std::unordered_set<int> allocatedStringsBlock, allocatedStringsFunction;//<offset from RBP>

  std::string header = ".globl main\n";
  std::string footer = "\n";

  /* Functions related to buffer storing result */
  void inline bufAppend(const char *s)
  {
    int len = strlen(s);
    if (cur_ + len >= buf_size) {
      while (cur_ + len >= buf_size)
      {
        buf_size *= 2; /* Double the buffer size */
      }
      resizeBuffer();
    }
    for(int n = 0; n < len; n++)
    {
      buf_[cur_ + n] = s[n];
    }
    cur_ += len;
    buf_[cur_] = 0;
  }

  void inline bufAppend(const char c)
  {
    if (cur_ >= buf_size)
    {
      buf_size *= 2; /* Double the buffer size */
      resizeBuffer();
    }
    buf_[cur_] = c;
    cur_++;
    buf_[cur_] = 0;
  }

  void inline bufAppend(String str)
  {
    const char *s = str.c_str();
    bufAppend(s);
  }

  void inline bufReset(void)
  {
    if (buf_) free(buf_);
    buf_size = BUFFER_INITIAL;
    buf_ = (char *) malloc(buf_size);
    if (!buf_) {
      fprintf(stderr, "ERROR\nOut of memory while allocating buffer!\n");
      exit(1);
    }
    memset(buf_, 0, buf_size);
    cur_ = 0;
  }

  void inline resizeBuffer(void)
  {
    char *temp = (char *) malloc(buf_size);
    if (!temp)
    {
      fprintf(stderr, "ERROR\nOut of memory while attempting to grow buffer!\n");
      exit(1);
    }
    if (buf_)
    {
      strcpy(temp, buf_);
      free(buf_);
    }
    buf_ = temp;
  }
};

#endif /* COMPILER_HEADER */
