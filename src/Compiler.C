#include "Compiler.H"
#include <iostream>
#include <string>

void Compiler::visitProgram(Program *t) {} //abstract class
void Compiler::visitTopDef(TopDef *t) {} //abstract class
void Compiler::visitArg(Arg *t) {} //abstract class
void Compiler::visitBlock(Block *t) {} //abstract class
void Compiler::visitStmt(Stmt *t) {} //abstract class
void Compiler::visitItem(Item *t) {} //abstract class
void Compiler::visitType(Type *t) {} //abstract class
void Compiler::visitExpr(Expr *t) {} //abstract class
void Compiler::visitAddOp(AddOp *t) {} //abstract class
void Compiler::visitMulOp(MulOp *t) {} //abstract class
void Compiler::visitRelOp(RelOp *t) {} //abstract class

char* Compiler::compile(Program *p, Program *lib) {
  //std::cout << "compile" << std::endl;
  bufReset();
  variablesMap.clear();
  typesMap.clear();
  andOrLabelCounter = 0;
  condCounter = 0;
  whileCounter = 0;
  stackCounter = 0;
  argCounter = 0;
  functionLabelCounter = 0;
  stringConcatLabelCounter = 0;
  functionBlock = false;
  bufAppend(header);

  getFunctionsTypes((Prog*) lib);
  getFunctionsTypes((Prog*) p);

  p->accept(this);

  bufAppend(footer);
  //std::cout << "Exiting " << std::endl;
  return buf_;
}

void Compiler::getFunctionsTypes(Prog *prog)
{
  //std::cout << "getFunctionsDefinitions\n";
  ListTopDef *list_top_def = prog->listtopdef_;

  /* The only thing we want from these definitions is to save them, there is no need to visit them yet */
  for (ListTopDef::iterator i = list_top_def->begin() ; i != list_top_def->end() ; ++i)
  {
    FnDef *fn_def = dynamic_cast<FnDef*> (*i);
    fn_def->type_->accept(this);
    typesMap.insert(std::make_pair(fn_def->ident_, typesStack.top()));
    typesStack.pop();
  }
}

void Compiler::visitProg(Prog *prog)
{
  //std::cout << "visitProg" << std::endl;
  prog->listtopdef_->accept(this);
  //std::cout << "Exiting visitProg" << std::endl;
}

void Compiler::visitFnDef(FnDef *fn_def)
{
  //std::cout << "visitFnDef" << std::endl;
  functionLabelCounter++;
  stackCounterBeforeFn = stackCounter;

  fn_def->type_->accept(this);
  typesStack.pop();
  visitIdent(fn_def->ident_);
  bufAppend("_");
  bufAppend(fn_def->ident_);
  bufAppend(":\n");

  /* function prologue */
  bufAppend("push %rbp\n");
  bufAppend("movq %rsp, %rbp\n\n");

  /* insert arguments and their offsets into variablesMap */
  argCounter = 0;
  fn_def->listarg_->accept(this);
  argCounter = 0;

  /* visit function's block */
  functionBlock = true;
  fn_def->block_->accept(this);

  /* Create exitFnCounter label that all returns jump to and free resources before returning */
  bufAppend("\nexit");
  bufAppend(std::to_string(functionLabelCounter));
  bufAppend(":\n");

  /* function epilogue */
  bufAppend("movq %rbp, %rsp\n");
  bufAppend("pop %rbp\n");
  bufAppend("ret\n\n");

  /* clear arguments from variablesMap */
  variablesMap.clear();
  //std::cout << "Exiting visitFnDef" << std::endl;
}

void Compiler::visitAr(Ar *ar)
{
  //std::cout << "visitAr" << std::endl;
  ar->type_->accept(this);
  visitIdent(ar->ident_);

  argCounter++;
  variablesMap.insert(std::make_pair(ar->ident_, 8 + (argCounter * 8)));
  typesMap.insert(std::make_pair(ar->ident_, typesStack.top()));
  typesStack.pop();
  //std::cout << "Exiting visitAr" << std::endl;
}

void Compiler::alignStack(int numberOfArgs) {
  bufAppend("movq %rsp, %rax\n");
  bufAppend("subq $");
  bufAppend(std::to_string(8 * (numberOfArgs + 1)));//number of args + padding
  bufAppend(", %rax\n");
  bufAppend("xorq %rdx, %rdx\n");
  bufAppend("movq $16, %rcx\n");
  bufAppend("idivq %rcx\n");
  bufAppend("subq %rdx, %rsp\n");//pad RSP
  bufAppend("pushq %rdx\n");//push padding onto stack
}

void Compiler::removePadding() {
  bufAppend("popq %rdx\n");
  bufAppend("addq %rdx, %rsp\n");
}

void Compiler::visitBlk(Blk *blk)
{
  //std::cout << "visitBlk" << std::endl;
  std::unordered_map<Ident, long long> variablesMapCopy(variablesMap);
  std::unordered_map<Ident, int> typesMapCopy(typesMap);
  unsigned int stackCounterBeforeBlk = stackCounter;
  bool isFunctionBlock = functionBlock;
  functionBlock = false;

  blk->liststmt_->accept(this);

  variablesMap = variablesMapCopy;
  typesMap = typesMapCopy;

  /* Deallocate local variables from previous block and restore stackCounter */
  if (!isFunctionBlock && stackCounter - stackCounterBeforeBlk > 0) {
    int bytesToDeallocate = 8 * (stackCounter - stackCounterBeforeBlk);
    bufAppend("addq $");
    bufAppend(std::to_string(bytesToDeallocate));
    bufAppend(", %rsp\n");
  }
  stackCounter = stackCounterBeforeBlk;
  //std::cout << "Exiting visitBlk" << std::endl;
}

void Compiler::visitEmpty(Empty *empty) { /* Do nothing */ }

void Compiler::visitBStmt(BStmt *b_stmt)
{
  //std::cout << "visitBStmt" << std::endl;
  b_stmt->block_->accept(this);
  //std::cout << "Exiting visitBStmt" << std::endl;
}

void Compiler::visitDecl(Decl *decl)
{
  //std::cout << "visitDecl" << std::endl;
  decl->type_->accept(this);
  decl->listitem_->accept(this);

  typesStack.pop();
  //std::cout << "Exiting visitDecl" << std::endl;
}

void Compiler::visitAss(Ass *ass)
{
  //std::cout << "visitAss" << std::endl;
  visitIdent(ass->ident_);
  ass->expr_->accept(this);
  typesStack.pop();

  int offset = variablesMap.find(ass->ident_)->second;
  bufAppend("movq %rax, ");
  bufAppend(std::to_string(offset));
  bufAppend("(%rbp)\n");
  //std::cout << "Exiting visitAss" << std::endl;
}

void Compiler::visitIncr(Incr *incr)
{
  //std::cout << "visitIncr" << std::endl;
  visitIdent(incr->ident_);

  int offset = variablesMap.find(incr->ident_)->second;
  bufAppend("incq ");
  bufAppend(std::to_string(offset));
  bufAppend("(%rbp)\n");
  //std::cout << "Exiting visitIncr" << std::endl;
}

void Compiler::visitDecr(Decr *decr)
{
  //std::cout << "visitDecr" << std::endl;
  visitIdent(decr->ident_);

  int offset = variablesMap.find(decr->ident_)->second;
  bufAppend("decq ");
  bufAppend(std::to_string(offset));
  bufAppend("(%rbp)\n");
  //std::cout << "Exiting visitDecr" << std::endl;
}

void Compiler::visitRet(Ret *ret)
{
  //std::cout << "visitRet" << std::endl;
  ret->expr_->accept(this);
  typesStack.pop();

  /* Deallocate variables allocated in this function up to this return */
  if (stackCounter - stackCounterBeforeFn > 0) {
    int bytesToDeallocate = 8 * (stackCounter - stackCounterBeforeFn);
    bufAppend("addq $");
    bufAppend(std::to_string(bytesToDeallocate));
    bufAppend(", %rsp\n");
  }

  /* jump to current function's exit */
  bufAppend("jmp exit");
  bufAppend(std::to_string(functionLabelCounter));
  bufAppend("\n");
  //std::cout << "Exiting visitRet" << std::endl;
}

void Compiler::visitVRet(VRet *v_ret)
{
  //std::cout << "visitVRet\n";

  /* Deallocate variables allocated in this function up to this return */
  if (stackCounter - stackCounterBeforeFn > 0) {
    int bytesToDeallocate = 8 * (stackCounter - stackCounterBeforeFn);
    bufAppend("addq $");
    bufAppend(std::to_string(bytesToDeallocate));
    bufAppend(", %rsp\n");
  }

  /* jump to current function's exit */
  bufAppend("jmp exit");
  bufAppend(std::to_string(functionLabelCounter));
  bufAppend("\n");
  //std::cout << "Exiting visitVRet" << std::endl;
}

void Compiler::visitCond(Cond *cond)
{
  //std::cout << "visitCond" << std::endl;
  int currentCondCounter = condCounter;
  condCounter++;

  cond->expr_->accept(this);
  typesStack.pop();
  bufAppend("cmpq $0, %rax\n");
  bufAppend("je afterCondLabel");
  bufAppend(std::to_string(currentCondCounter));
  bufAppend("\n");
  cond->stmt_->accept(this);

  bufAppend("afterCondLabel");
  bufAppend(std::to_string(currentCondCounter));
  bufAppend(":\n");
  //std::cout << "Exiting visitCond" << std::endl;
}

void Compiler::visitCondElse(CondElse *cond_else)
{
  //std::cout << "visitCondElse" << std::endl;
  int currentCondCounter = condCounter;
  condCounter++;

  cond_else->expr_->accept(this);
  typesStack.pop();
  bufAppend("cmpq $0, %rax\n");
  bufAppend("je elseCondLabel");
  bufAppend(std::to_string(currentCondCounter));
  bufAppend("\n");
  cond_else->stmt_1->accept(this);
  bufAppend("jmp afterCondLabel");
  bufAppend(std::to_string(currentCondCounter));
  bufAppend("\n");

  bufAppend("elseCondLabel");
  bufAppend(std::to_string(currentCondCounter));
  bufAppend(":\n");
  cond_else->stmt_2->accept(this);

  bufAppend("afterCondLabel");
  bufAppend(std::to_string(currentCondCounter));
  bufAppend(":\n");
  //std::cout << "Exiting visitCondElse" << std::endl;
}

void Compiler::visitWhile(While *while_)
{
  //std::cout << "visitWhile" << std::endl;
  int currentWhileCounter = whileCounter;
  whileCounter++;

  bufAppend("jmp whileCondLabel");
  bufAppend(std::to_string(currentWhileCounter));
  bufAppend("\n");
  bufAppend("whileStmtLabel");
  bufAppend(std::to_string(currentWhileCounter));
  bufAppend(":\n");
  while_->stmt_->accept(this);

  bufAppend("whileCondLabel");
  bufAppend(std::to_string(currentWhileCounter));
  bufAppend(":\n");
  while_->expr_->accept(this);
  typesStack.pop();
  bufAppend("cmpq $0, %rax\n");
  bufAppend("jne whileStmtLabel");
  bufAppend(std::to_string(currentWhileCounter));
  bufAppend("\n");
  //std::cout << "Exiting visitWhile" << std::endl;
}

void Compiler::visitSExp(SExp *s_exp)
{
  //std::cout << "visitSExp" << std::endl;
  s_exp->expr_->accept(this);
  typesStack.pop();
  //std::cout << "Exiting visitSExp" << std::endl;
}

void Compiler::declareVariable(Ident ident)
{
  //std::cout << "declareVariable: " << ident << std::endl;
  stackCounter++;

  /* Check if this declaration overshadows variable from higher block */
  if (auto it = variablesMap.find(ident); it != variablesMap.end()) {
    /* The overshadowed variable offset will be recovered later on, when leaving the block */
    it->second = stackCounter * -8;
    typesMap.find(ident)->second = typesStack.top();
  }
  /* Otherwise just declare the variable */
  else {
    variablesMap.insert(std::make_pair(ident, stackCounter * -8));
    typesMap.insert(std::make_pair(ident, typesStack.top()));
  }

  bufAppend("pushq %rax\n");
  //std::cout << "Exiting declareVariable" << std::endl;
}

void Compiler::visitNoInit(NoInit *no_init)
{
  //std::cout << "visitNoInit" << std::endl;
  visitIdent(no_init->ident_);

  /* Initialize the variable to an empty string or 0 depending on type */
  if (typesStack.top() == STRING_CODE) {
    visitString("");
  }
  else {
    visitInteger(0);
  }
  declareVariable(no_init->ident_);
  typesStack.pop();
  //std::cout << "Exiting visitNoInit" << std::endl;
}

void Compiler::visitInit(Init *init)
{
  //std::cout << "visitInit" << std::endl;
  visitIdent(init->ident_);
  init->expr_->accept(this);

  declareVariable(init->ident_);
  typesStack.pop();
  //std::cout << "Exiting visitInit" << std::endl;
}

void Compiler::visitInt(Int *int_)
{
  //std::cout << "visitInt" << std::endl;
  typesStack.push(INT_CODE);
  //std::cout << "Exiting visitInt" << std::endl;
}

void Compiler::visitStr(Str *str)
{
  //std::cout << "visitStr" << std::endl;
  typesStack.push(STRING_CODE);
  //std::cout << "Exiting visitStr" << std::endl;
}

void Compiler::visitBool(Bool *bool_)
{
  //std::cout << "visitBool" << std::endl;
  typesStack.push(BOOL_CODE);
  //std::cout << "Exiting visitBool" << std::endl;
}

void Compiler::visitVoid(Void *void_)
{
  //std::cout << "visitVoid" << std::endl;
  typesStack.push(VOID_CODE);
  //std::cout << "Exiting visitVoid" << std::endl;
}

void Compiler::visitFun(Fun *fun)
{
  //std::cout << "visitFun" << std::endl;
  fun->type_->accept(this);
  //TODO pop stack?
  fun->listtype_->accept(this);
  //std::cout << "Exiting visitFun" << std::endl;
}

void Compiler::visitEVar(EVar *e_var)
{
  //std::cout << "visitEVar" << std::endl;
  visitIdent(e_var->ident_);

  int offset = variablesMap.find(e_var->ident_)->second;
  typesStack.push(typesMap.find(e_var->ident_)->second);
  bufAppend("movq ");
  bufAppend(std::to_string(offset));
  bufAppend("(%rbp), %rax\n");
  //std::cout << "Exiting visitEVar" << std::endl;
}

void Compiler::visitELitInt(ELitInt *e_lit_int)
{
  //std::cout << "visitELitInt" << std::endl;
  visitInteger(e_lit_int->integer_);
  //std::cout << "Exiting visitELitInt" << std::endl;
}

void Compiler::visitELitTrue(ELitTrue *e_lit_true) {
  //std::cout << "visitELitTrue" << std::endl;
  typesStack.push(BOOL_CODE);
  bufAppend("movq $1, %rax\n");
  //std::cout << "Exiting visitELitTrue" << std::endl;
}

void Compiler::visitELitFalse(ELitFalse *e_lit_false) {
  //std::cout << "visitELitFalse" << std::endl;
  typesStack.push(BOOL_CODE);
  bufAppend("movq $0, %rax\n");
  //std::cout << "Exiting visitELitFalse" << std::endl;
}

void Compiler::visitEApp(EApp *e_app)//TODO this should push function's type onto stack
{
  //std::cout << "visitEApp" << std::endl;
  visitIdent(e_app->ident_);

  typesStack.push(typesMap.find(e_app->ident_)->second);

  /* Align stack to 16 */
  int previousArgCounter = argCounter;
  argCounter = 0;
  countArgs(e_app->listexpr_);//this function won't execute these expressions, it will just count them

  alignStack(argCounter);

  /* Push args onto the stack */
  pushArgsOntoStack(e_app->listexpr_);

  int localArgCounter = argCounter;
  argCounter = previousArgCounter;

  /* Append function call */
  bufAppend("call _");
  bufAppend(e_app->ident_);
  bufAppend("\n");

  /* Remove args from stack */
  if (localArgCounter > 0) {
    int bytesToRemove = 8 * localArgCounter;
    bufAppend("addq $");
    bufAppend(std::to_string(bytesToRemove));
    bufAppend(", %rsp\n");
  }

  removePadding();
  //std::cout << "Exiting visitEApp" << std::endl;
}

void Compiler::visitEString(EString *e_string)
{
  //std::cout << "visitEString" << std::endl;
  visitString(e_string->string_);
  //std::cout << "Exiting visitEString" << std::endl;
}

void Compiler::visitNeg(Neg *neg)
{
  //std::cout << "visitNeg" << std::endl;
  neg->expr_->accept(this);
  bufAppend("neg %rax\n");
  //std::cout << "Exiting visitNeg" << std::endl;
}

void Compiler::visitNot(Not *not_)
{
  //std::cout << "visitNot" << std::endl;
  not_->expr_->accept(this);
  bufAppend("cmpq $0, %rax\n");
  bufAppend("movq $0, %rax\n");
  bufAppend("sete %al\n");
  //std::cout << "Exiting visitNot" << std::endl;
}

void Compiler::visitEMul(EMul *e_mul)
{
  //std::cout << "visitEMul" << std::endl;
  e_mul->expr_1->accept(this);
  bufAppend("pushq %rax\n");
  e_mul->expr_2->accept(this);
  bufAppend("movq %rax, %rbx\n");
  bufAppend("popq %rax\n");
  e_mul->mulop_->accept(this);

  /* pop one type code and leave the other */
  typesStack.pop();
  //std::cout << "Exiting visitEMul" << std::endl;
}

void Compiler::visitEAdd(EAdd *e_add)
{
  //std::cout << "visitEAdd" << std::endl;
  e_add->expr_1->accept(this);
  bufAppend("pushq %rax\n");
  e_add->expr_2->accept(this);
  bufAppend("movq %rax, %rbx\n");
  bufAppend("popq %rax\n");
  e_add->addop_->accept(this);

  /* pop one type code and leave the other */
  typesStack.pop();
  //std::cout << "Exiting visitEAdd" << std::endl;
}

void Compiler::visitERel(ERel *e_rel)
{
  //std::cout << "visitERel" << std::endl;
  e_rel->expr_1->accept(this);
  typesStack.pop();
  bufAppend("pushq %rax\n");
  e_rel->expr_2->accept(this);
  typesStack.pop();
  bufAppend("popq %rbx\n");

  //Append the shared comparison operations
  bufAppend("cmpq %rax, %rbx\n");
  bufAppend("movq $0, %rax\n");
  e_rel->relop_->accept(this);
  typesStack.push(BOOL_CODE);
  //std::cout << "Exiting visitERel" << std::endl;
}

void Compiler::visitEAnd(EAnd *e_and)
{
  //std::cout << "visitEAnd" << std::endl;
  e_and->expr_1->accept(this);
  typesStack.pop();
  bufAppend("cmpq $0, %rax\n");
  bufAppend("je andOrLabelSkip");
  bufAppend(std::to_string(andOrLabelCounter));
  bufAppend("\n");
  e_and->expr_2->accept(this);
  typesStack.pop();

  bufAppend("andOrLabelSkip");
  bufAppend(std::to_string(andOrLabelCounter));
  bufAppend(":\n");

  andOrLabelCounter++;
  typesStack.push(BOOL_CODE);
  //std::cout << "Exiting visitEAnd" << std::endl;
}

void Compiler::visitEOr(EOr *e_or)
{
  //std::cout << "visitEOr" << std::endl;
  e_or->expr_1->accept(this);
  typesStack.pop();
  bufAppend("cmpq $0, %rax\n");
  bufAppend("jne andOrLabelSkip");
  bufAppend(std::to_string(andOrLabelCounter));
  bufAppend("\n");
  e_or->expr_2->accept(this);
  typesStack.pop();

  bufAppend("andOrLabelSkip");
  bufAppend(std::to_string(andOrLabelCounter));
  bufAppend(":\n");

  andOrLabelCounter++;
  typesStack.push(BOOL_CODE);
  //std::cout << "Exiting visitEOr" << std::endl;
}

void Compiler::visitPlus(Plus *plus)
{
  //std::cout << "visitPlus" << std::endl;
  if (typesStack.top() == STRING_CODE) {
    bufAppend("pushq %r12\n");
    bufAppend("pushq %r13\n");
    bufAppend("pushq %r14\n");
    /* Save original locations of both strings */
    bufAppend("movq %rax, %r12\n");//original address of first string
    bufAppend("movq %rbx, %r13\n");//original address of second string
    alignStack(0);

    /* Get length of first string */
    bufAppend("movq %r12, %rax\n");
    bufAppend("xorq %rdi, %rdi\n");
    bufAppend("decq %rax\n");
    bufAppend("string1Count");
    bufAppend(std::to_string(stringConcatLabelCounter));
    bufAppend(":\n");
    bufAppend("incq %rdi\n");
    bufAppend("incq %rax\n");
    bufAppend("cmpb $0, (%rax)\n");
    bufAppend("jne string1Count");
    bufAppend(std::to_string(stringConcatLabelCounter));
    bufAppend("\n");
    
    /* Get length of second string */
    bufAppend("movq %r13, %rax\n");
    bufAppend("decq %rax\n");
    bufAppend("string2Count");
    bufAppend(std::to_string(stringConcatLabelCounter));
    bufAppend(":\n");
    bufAppend("incq %rdi\n");
    bufAppend("incq %rax\n");
    bufAppend("cmpb $0, (%rax)\n");
    bufAppend("jne string2Count");
    bufAppend(std::to_string(stringConcatLabelCounter));
    bufAppend("\n");
    /* Call malloc */
    bufAppend("subq $1, %rdi\n");
    bufAppend("call _malloc\n");
    bufAppend("movq %rax, %r14\n");//result string address
    removePadding();
    /* Copy first string */
    bufAppend("jmp concat1StringsWriteCheck");
    bufAppend(std::to_string(stringConcatLabelCounter));
    bufAppend("\n");
    bufAppend("concat1StringsWrite");
    bufAppend(std::to_string(stringConcatLabelCounter));
    bufAppend(":\n");
    bufAppend("movb (%r12), %dil\n");
    bufAppend("movb %dil, (%r14)\n");
    bufAppend("incq %r12\n");
    bufAppend("incq %r14\n");
    bufAppend("concat1StringsWriteCheck");
    bufAppend(std::to_string(stringConcatLabelCounter));
    bufAppend(":\n");
    bufAppend("cmpb $0, (%r12)\n");
    bufAppend("jne concat1StringsWrite");
    bufAppend(std::to_string(stringConcatLabelCounter));
    bufAppend("\n");
    /* Copy second string */
    bufAppend("concat2StringsWrite");
    bufAppend(std::to_string(stringConcatLabelCounter));
    bufAppend(":\n");
    bufAppend("movb (%r13), %dil\n");
    bufAppend("movb %dil, (%r14)\n");
    bufAppend("incq %r13\n");
    bufAppend("incq %r14\n");
    bufAppend("cmpb $0, (%r13)\n");
    bufAppend("jne concat2StringsWrite");
    bufAppend(std::to_string(stringConcatLabelCounter));
    bufAppend("\n");
    bufAppend("movb $0, (%r14)\n");

    bufAppend("popq %r14\n");
    bufAppend("popq %r13\n");
    bufAppend("popq %r12\n");

    stringConcatLabelCounter++;
  }
  else {
    bufAppend("addq %rbx, %rax\n");
  }
  //std::cout << "Exiting visitPlus" << std::endl;
}

void Compiler::visitMinus(Minus *minus)
{
  //std::cout << "visitMinus" << std::endl;
  bufAppend("subq %rbx, %rax\n");
  //std::cout << "Exiting visitMinus" << std::endl;
}

void Compiler::visitTimes(Times *times)
{
  //std::cout << "visitTimes" << std::endl;
  bufAppend("imulq %rbx, %rax\n");
  //std::cout << "Exiting visitTimes" << std::endl;
}

void Compiler::performDiv() {
  bufAppend("cmpq $0, %rax\n");
  bufAppend("jne PerformDivLabel");
  bufAppend(std::to_string(divisionCounter));
  bufAppend("\n");
  /* Division by 0 attempt discovered */
  alignStack(0);
  bufAppend("call _exit\n");

  bufAppend("PerformDivLabel");
  bufAppend(std::to_string(divisionCounter));
  bufAppend(":\n");
  bufAppend("xor %rdx, %rdx\n");
  bufAppend("cdq\n");
  bufAppend("idiv %rbx\n");
}

void Compiler::visitDiv(Div *div)
{
  //std::cout << "visitDiv" << std::endl;
  performDiv();
  //std::cout << "Exiting visitDiv" << std::endl;
}

void Compiler::visitMod(Mod *mod)
{
  //std::cout << "visitMod" << std::endl;
  performDiv();
  //The remainder is stored in rdx, move it into rax
  bufAppend("movq %rdx, %rax\n");
  //std::cout << "Exiting visitMod" << std::endl;
}

void Compiler::visitLTH(LTH *lth)
{
  //std::cout << "visitLTH" << std::endl;
  bufAppend("setl %al\n");
  //std::cout << "Exiting visitLTH" << std::endl;
}

void Compiler::visitLE(LE *le)
{
  //std::cout << "visitLE" << std::endl;
  bufAppend("setle %al\n");
  //std::cout << "Exiting visitLE" << std::endl;
}

void Compiler::visitGTH(GTH *gth)
{
  //std::cout << "visitGTH" << std::endl;
  bufAppend("setg %al\n");
  //std::cout << "Exiting visitGTH" << std::endl;
}

void Compiler::visitGE(GE *ge)
{
  //std::cout << "visitGE" << std::endl;
  bufAppend("setge %al\n");
  //std::cout << "Exiting visitGE" << std::endl;
}

void Compiler::visitEQU(EQU *equ)
{
  //std::cout << "visitEQU" << std::endl;
  bufAppend("sete %al\n");
  //std::cout << "Exiting visitEQU" << std::endl;
}

void Compiler::visitNE(NE *ne)
{
  //std::cout << "visitNE" << std::endl;
  bufAppend("setne %al\n");
  //std::cout << "Exiting visitNE" << std::endl;
}


void Compiler::visitListTopDef(ListTopDef *list_top_def)
{
  //std::cout << "visitListTopDef" << std::endl;
  for (ListTopDef::iterator i = list_top_def->begin() ; i != list_top_def->end() ; ++i)
  {
    (*i)->accept(this);
  }
  //std::cout << "Exiting visitListTopDef" << std::endl;
}

void Compiler::visitListArg(ListArg *list_arg)
{
  //std::cout << "visitListArg" << std::endl;
  for (ListArg::iterator i = list_arg->begin() ; i != list_arg->end() ; ++i)
  {
    (*i)->accept(this);
  }
  //std::cout << "Exiting visitListArg" << std::endl;
}

void Compiler::visitListStmt(ListStmt *list_stmt)
{
  //std::cout << "visitListStmt" << std::endl;
  for (ListStmt::iterator i = list_stmt->begin() ; i != list_stmt->end() ; ++i)
  {
    (*i)->accept(this);
  }
  //std::cout << "Exiting visitListStmt" << std::endl;
}

void Compiler::visitListItem(ListItem *list_item)
{
  //std::cout << "visitListItem" << std::endl;
  for (ListItem::iterator i = list_item->begin() ; i != list_item->end() ; ++i)
  {
    (*i)->accept(this);
  }
  //std::cout << "Exiting visitListItem" << std::endl;
}

void Compiler::visitListType(ListType *list_type)
{
  //std::cout << "visitListType" << std::endl;
  for (ListType::iterator i = list_type->begin() ; i != list_type->end() ; ++i)
  {
    (*i)->accept(this);
  }
  //std::cout << "Exiting visitListType" << std::endl;
}

void Compiler::visitListExpr(ListExpr *list_expr)
{
  //std::cout << "visitListExpr" << std::endl;
  for (ListExpr::iterator i = list_expr->begin() ; i != list_expr->end() ; ++i)
  {
    (*i)->accept(this);
  }
  //std::cout << "Exiting visitListExpr" << std::endl;
}

void Compiler::countArgs(ListExpr *list_expr)
{
  //std::cout << "countArgs" << std::endl;
  for (ListExpr::iterator i = list_expr->begin() ; i != list_expr->end() ; ++i)
  {
    argCounter++;
  }
  //std::cout << "Exiting countArgs" << std::endl;
}

void Compiler::pushArgsOntoStack(ListExpr *list_expr)
{
  //std::cout << "pushArgsOntoStack" << std::endl;
  for (ListExpr::reverse_iterator i = list_expr->rbegin() ; i != list_expr->rend() ; ++i)
  {
    (*i)->accept(this);
    typesStack.pop();
    bufAppend("pushq %rax\n");
  }
  //std::cout << "Exiting pushArgsOntoStack" << std::endl;
}

void Compiler::visitInteger(Integer x)
{
  //std::cout << "visitInteger" << std::endl;
  typesStack.push(INT_CODE);
  bufAppend("movq $");
  bufAppend(std::to_string(x));
  bufAppend(", %rax\n");
  //std::cout << "Exiting visitInteger" << std::endl;
}

void Compiler::visitChar(Char x)
{
  //std::cout << "visitChar" << std::endl;
  typesStack.push(CHAR_CODE);
  //std::cout << "Exiting visitChar" << std::endl;
}

void Compiler::visitDouble(Double x)
{
  //std::cout << "visitDouble" << std::endl;
  typesStack.push(DOUBLE_CODE);
  //std::cout << "Exiting visitDouble" << std::endl;
}

void Compiler::visitString(String x)
{
  //std::cout << "visitString" << std::endl;
  typesStack.push(STRING_CODE);

  /* Align stack before calling malloc */
  alignStack(0);

  bufAppend("movq $");
  bufAppend(std::to_string(x.length() + 1));
  bufAppend(", %rdi\n");

  bufAppend("call _malloc\n");
  bufAppend("movq %rax, %rbx\n");
  for (size_t i = 0; i < x.length(); i++) {
    bufAppend("movb $");
    bufAppend(std::to_string((int) x[i]));
    bufAppend(", (%rbx)\n");
    bufAppend("addb $1, %bl\n");
  }
  bufAppend("movb $0, (%rbx)\n");

  removePadding();

  //std::cout << "Exiting visitString" << std::endl;
}

void Compiler::visitIdent(Ident x) {/* Do nothing */}



