#include "Compiler.H"
#include <iostream>
#include <string>

void Compiler::visitProgram(Program *t) {} //abstract class
void Compiler::visitTopDef(TopDef *t) {} //abstract class
void Compiler::visitArg(Arg *t) {} //abstract class
void Compiler::visitBlock(Block *t) {} //abstract class
void Compiler::visitStmt(Stmt *t) {} //abstract class
void Compiler::visitItem(Item *t) {} //abstract class
void Compiler::visitItemArr(ItemArr *t) {} //abstract class
void Compiler::visitSimpleType(SimpleType *t) {} //abstract class
void Compiler::visitArrType(ArrType *t) {} //abstract class
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
  skipFreeCounter = 0;
  inBoundsCounter = 0;
  checkArraySizeCounter = 0;
  forEachCounter = 0;
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

void Compiler::freeStrings(std::unordered_set<int> allocatedStringsSet)
{
  if (!allocatedStringsSet.empty()) {
    bufAppend("pushq %rax\n");
    alignStack(0);
    
    for (auto it = allocatedStringsSet.begin(); it != allocatedStringsSet.end(); it++) {
      int offset = *it;
      bufAppend("movq ");
      bufAppend(std::to_string(offset));
      bufAppend("(%rbp), %rdi\n");
      bufAppend("decq %rdi\n");
      bufAppend("decb (%rdi)\n");
      bufAppend("cmpb $0, (%rdi)\n");
      bufAppend("jne skipFreeLabel");
      bufAppend(std::to_string(skipFreeCounter));
      bufAppend("\n");
      bufAppend("call free\n");
      bufAppend("skipFreeLabel");
      bufAppend(std::to_string(skipFreeCounter++));
      bufAppend(":\n");
    }
    
    removePadding();
    bufAppend("popq %rax\n");
  }
}

void Compiler::visitFnDef(FnDef *fn_def)
{
  //std::cout << "visitFnDef" << std::endl;
  functionLabelCounter++;
  stackCounterBeforeFn = stackCounter;
  allocatedStringsFunction.clear();

  fn_def->type_->accept(this);
  typesStack.pop();
  visitIdent(fn_def->ident_);
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
  if (typesStack.top() == STRING_CODE) {
    allocatedStringsFunction.insert(8 + (argCounter * 8));
  }
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
  std::unordered_map<Ident, int> variablesMapCopy(variablesMap);
  std::unordered_map<Ident, int> typesMapCopy(typesMap);
  std::unordered_set<int> allocatedStringsBlockCopy(allocatedStringsBlock);
  allocatedStringsBlock.clear();
  unsigned int stackCounterBeforeBlk = stackCounter;
  bool isFunctionBlock = functionBlock;
  functionBlock = false;

  blk->liststmt_->accept(this);

  /* Free allocated strings in the local block */
  if (!isFunctionBlock) {
    freeStrings(allocatedStringsBlock);
    for (auto it = allocatedStringsBlock.begin(); it != allocatedStringsBlock.end(); it++) {
      allocatedStringsFunction.erase(*it);
    }
  }

  /* Deallocate local variables from previous block and restore stackCounter */
  if (!isFunctionBlock && stackCounter - stackCounterBeforeBlk > 0) {
    int bytesToDeallocate = 8 * (stackCounter - stackCounterBeforeBlk);
    bufAppend("addq $");
    bufAppend(std::to_string(bytesToDeallocate));
    bufAppend(", %rsp\n");
  }
  stackCounter = stackCounterBeforeBlk;

  variablesMap = variablesMapCopy;
  typesMap = typesMapCopy;
  allocatedStringsBlock = allocatedStringsBlockCopy;
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
  decl->simpletype_->accept(this);
  decl->listitem_->accept(this);

  typesStack.pop();
  //std::cout << "Exiting visitDecl" << std::endl;
}

void Compiler::visitDeclArr(DeclArr *decl_arr)
{
  decl_arr->arrtype_->accept(this);
  decl_arr->itemarr_->accept(this);

  typesStack.pop();
}

/* Array's length is in $rax */
void Compiler::mallocArray() {
  /* Check if array's length is a positive number */
  bufAppend("cmpq $0, %rax\n");
  bufAppend("jg checkArraySize");
  bufAppend(std::to_string(checkArraySizeCounter));
  bufAppend("\n");
  alignStack(0);
  bufAppend("call error\n");
  bufAppend("checkArraySize");
  bufAppend(std::to_string(checkArraySizeCounter++));
  bufAppend(":\n");
  bufAppend("pushq %rax\n");//push array's length onto stack
  bufAppend("imulq $8, %rax\n");//each index should have 8 bytes of space
  bufAppend("addq $8, %rax\n");//add 8 additional bytes for storing array length 8 bytes before first element
  bufAppend("movq %rax, %rdi\n");
  alignStack(0);
  bufAppend("call malloc\n");
  removePadding();
  bufAppend("popq %rdx\n");
  bufAppend("movq %rdx, (%rax)\n");
  bufAppend("addq $8, %rax\n");
}

void Compiler::visitInitArrSt(InitArrSt *init_arr_st)
{
  visitIdent(init_arr_st->ident_);
  init_arr_st->simpletype_->accept(this);
  typesStack.pop();
  
  init_arr_st->expr_->accept(this);
  typesStack.pop();
  
  mallocArray();
  int offset = variablesMap.find(init_arr_st->ident_)->second;
  bufAppend("movq %rax, ");
  bufAppend(std::to_string(offset));
  bufAppend("(%rbp)\n");
}

void Compiler::visitAss(Ass *ass)
{
  //std::cout << "visitAss" << std::endl;
  visitIdent(ass->ident_);
  int offset = variablesMap.find(ass->ident_)->second;

  ass->expr_->accept(this);

  /* If variable is of string type then free/decrease reference count for previously held value */
  if (typesStack.top() == STRING_CODE) {
    bufAppend("pushq %rax\n");
    alignStack(0);
    bufAppend("movq ");
    bufAppend(std::to_string(offset));
    bufAppend("(%rbp), %rdi\n");
    bufAppend("decq %rdi\n");
    bufAppend("decb (%rdi)\n");
    bufAppend("cmpb $0, (%rdi)\n");
    bufAppend("jne skipFreeLabel");
    bufAppend(std::to_string(skipFreeCounter));
    bufAppend("\n");
    bufAppend("call free\n");
    bufAppend("skipFreeLabel");
    bufAppend(std::to_string(skipFreeCounter++));
    bufAppend(":\n");
    removePadding();
    bufAppend("popq %rax\n");

    /* Increase reference count for new value */
    bufAppend("incb -1(%rax)\n");
  }
  typesStack.pop();

  bufAppend("movq %rax, ");
  bufAppend(std::to_string(offset));
  bufAppend("(%rbp)\n");
  //std::cout << "Exiting visitAss" << std::endl;
}

void Compiler::visitAssArr(AssArr *ass_arr)
{
  visitIdent(ass_arr->ident_);
  ass_arr->expr_1->accept(this);
  typesStack.pop();

  int offset = variablesMap.find(ass_arr->ident_)->second;
  bufAppend("movq ");
  bufAppend(std::to_string(offset));
  bufAppend("(%rbp), %rbx\n");
  checkInBounds();
  bufAppend("imulq $8, %rax\n");
  bufAppend("addq %rax, %rbx\n");
  bufAppend("pushq %rbx\n");
  
  ass_arr->expr_2->accept(this);
  typesStack.pop();

  bufAppend("popq %rbx\n");
  bufAppend("movq %rax, (%rbx)\n");
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

/* Array's address is in %rbx, index to test is in %rax */
void Compiler::checkInBounds() {
  bufAppend("cmpq %rax, -8(%rbx)\n");
  bufAppend("jg inBoundsCheckLabel");
  bufAppend(std::to_string(inBoundsCounter));
  bufAppend("\n");
  alignStack(0);
  bufAppend("call error\n");
  bufAppend("inBoundsCheckLabel");
  bufAppend(std::to_string(inBoundsCounter++));
  bufAppend(":\n");
}

void Compiler::visitIncrArr(IncrArr *incr_arr)
{
  visitIdent(incr_arr->ident_);
  incr_arr->expr_->accept(this);
  typesStack.pop();

  int offset = variablesMap.find(incr_arr->ident_)->second;
  bufAppend("movq ");
  bufAppend(std::to_string(offset));
  bufAppend("(%rbp), %rbx\n");
  checkInBounds();
  bufAppend("imulq $8, %rax\n");
  bufAppend("addq %rax, %rbx\n");
  bufAppend("incq (%rbx)\n");
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

void Compiler::visitDecrArr(DecrArr *decr_arr)
{
  visitIdent(decr_arr->ident_);
  decr_arr->expr_->accept(this);
  typesStack.pop();

  int offset = variablesMap.find(decr_arr->ident_)->second;
  bufAppend("movq ");
  bufAppend(std::to_string(offset));
  bufAppend("(%rbp), %rbx\n");
  checkInBounds();
  bufAppend("imulq $8, %rax\n");
  bufAppend("addq %rax, %rbx\n");
  bufAppend("decq (%rbx)\n");
}

void Compiler::visitRet(Ret *ret)
{
  //std::cout << "visitRet" << std::endl;
  ret->expr_->accept(this);
  int offsetToSkip;
  bool skip = false;

  /* If we are returning a string and it was stored in a variable, prevent it from being freed in a second */
  if (typesStack.top() == STRING_CODE) {
    EVar *e_var = dynamic_cast<EVar*>(ret->expr_);
    if (e_var != NULL) {
      offsetToSkip = variablesMap.find(e_var->ident_)->second;
      skip = true;
      allocatedStringsFunction.erase(offsetToSkip);
    }
  }
  typesStack.pop();

  freeStrings(allocatedStringsFunction);
  if (skip) {
    allocatedStringsFunction.insert(offsetToSkip);
  }

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

  freeStrings(allocatedStringsFunction);

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

void Compiler::visitForEach(ForEach *for_each)
{
  int localforEachCounter = forEachCounter++;
  int localStackCounter = stackCounter;
  std::unordered_map<Ident, int> variablesMapCopy(variablesMap);
  std::unordered_map<Ident, int> typesMapCopy(typesMap);

  for_each->simpletype_->accept(this);
  visitIdent(for_each->ident_1);
  visitIdent(for_each->ident_2);

  declareVariable(for_each->ident_1);
  typesStack.pop();

  stackCounter += 2;//storing current array pointer and number of element's left

  /* Load array address into %rax and array's length into %rdi */
  int offsetArr = variablesMap.find(for_each->ident_2)->second;
  int offsetVar = variablesMap.find(for_each->ident_1)->second;
  bufAppend("movq ");
  bufAppend(std::to_string(offsetArr));
  bufAppend("(%rbp), %rax\n");
  bufAppend("movq -8(%rax), %rdi\n");

  /* Start loop */
  bufAppend("forEachLabel");
  bufAppend(std::to_string(localforEachCounter));
  bufAppend(":\n");
  bufAppend("movq (%rax), %rbx\n");
  bufAppend("movq %rbx, ");
  bufAppend(std::to_string(offsetVar));
  bufAppend("(%rbp)\n");
  bufAppend("pushq %rax\n");
  bufAppend("pushq %rdi\n");
  for_each->stmt_->accept(this);

  bufAppend("popq %rdi\n");
  bufAppend("popq %rax\n");
  bufAppend("addq $8, %rax\n");
  bufAppend("decq %rdi\n");
  bufAppend("cmpq $0, %rdi\n");
  bufAppend("jne forEachLabel");
  bufAppend(std::to_string(localforEachCounter));
  bufAppend("\n");

  stackCounter -= 2;//registers stored on stack were already popped
  if (stackCounter - localStackCounter > 0) {
    int bytesToDeallocate = 8 * (stackCounter - localStackCounter);
    bufAppend("addq $");
    bufAppend(std::to_string(bytesToDeallocate));
    bufAppend(", %rsp\n");
  }
  stackCounter = localStackCounter;

  variablesMap = variablesMapCopy;
  typesMap = typesMapCopy;
}

void Compiler::visitSExp(SExp *s_exp)
{
  //std::cout << "visitSExp" << std::endl;
  s_exp->expr_->accept(this);
  
  /* If the expression was of string type and is not a variable then free the string */
  if (typesStack.top() == STRING_CODE) {
    EVar *e_var = dynamic_cast<EVar*>(s_exp->expr_);
    if (e_var == NULL) {
      /* Now we know that this expression wasn't a variable */
      bufAppend("movq %rax, %rdi\n");
      alignStack(0);
      bufAppend("decq %rdi\n");
      bufAppend("call free\n");
      removePadding();
    }
  }

  typesStack.pop();
  //std::cout << "Exiting visitSExp" << std::endl;
}

void Compiler::declareVariable(Ident ident)
{
  //std::cout << "declareVariable: " << ident << std::endl;
  stackCounter++;
  int offset = stackCounter * -8;

  /* Check if this declaration overshadows variable from higher block */
  if (auto it = variablesMap.find(ident); it != variablesMap.end()) {
    /* The overshadowed variable offset will be recovered later on, when leaving the block */
    it->second = offset;
    typesMap.find(ident)->second = typesStack.top();
  }
  /* Otherwise just declare the variable */
  else {
    variablesMap.insert(std::make_pair(ident, offset));
    typesMap.insert(std::make_pair(ident, typesStack.top()));
  }

  if (typesStack.top() == STRING_CODE) {
    allocatedStringsBlock.insert(offset);
    allocatedStringsFunction.insert(offset);

    /* Increase reference count for current value */
    bufAppend("incb -1(%rax)\n");
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

void Compiler::visitNoInitArr(NoInitArr *no_init_arr)
{
  visitIdent(no_init_arr->ident_);
  declareVariable(no_init_arr->ident_);
}

void Compiler::visitInitArr(InitArr *init_arr)
{
  visitIdent(init_arr->ident_);
  init_arr->simpletype_->accept(this);
  typesStack.pop();

  init_arr->expr_->accept(this);
  typesStack.pop();
  
  mallocArray();
  declareVariable(init_arr->ident_);
}

void Compiler::visitInitArrE(InitArrE *init_arr_e)
{
  visitIdent(init_arr_e->ident_);
  init_arr_e->expr_->accept(this);
  typesStack.pop();

  declareVariable(init_arr_e->ident_);
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

void Compiler::visitArr(Arr *arr)
{
  arr->simpletype_->accept(this);
  typesStack.top() += AR_TYPE_OFFSET;
}

void Compiler::visitAbsAT(AbsAT *abs_at)
{
  abs_at->arrtype_->accept(this);
}

void Compiler::visitAbsST(AbsST *abs_st)
{
  abs_st->simpletype_->accept(this);
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

void Compiler::visitEVarArr(EVarArr *e_var_arr)
{
  visitIdent(e_var_arr->ident_);
  e_var_arr->expr_->accept(this);
  typesStack.pop();

  int offset = variablesMap.find(e_var_arr->ident_)->second;
  typesStack.push(typesMap.find(e_var_arr->ident_)->second);

  bufAppend("movq ");
  bufAppend(std::to_string(offset));
  bufAppend("(%rbp), %rbx\n");
  checkInBounds();
  bufAppend("imulq $8, %rax\n");
  bufAppend("addq %rax, %rbx\n");
  bufAppend("movq (%rbx), %rax\n");
}

void Compiler::visitEAtr(EAtr *e_atr)
{
  visitIdent(e_atr->ident_);
  typesStack.push(INT_CODE);

  int offset = variablesMap.find(e_atr->ident_)->second;
  bufAppend("movq ");
  bufAppend(std::to_string(offset));
  bufAppend("(%rbp), %rbx\n");
  bufAppend("subq $8, %rbx\n");
  bufAppend("movq (%rbx), %rax\n");
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

void Compiler::visitEApp(EApp *e_app)
{
  //std::cout << "visitEApp" << std::endl;
  visitIdent(e_app->ident_);

  std::unordered_set<int> allocatedStringsFunctionCopy(allocatedStringsFunction);
  allocatedStringsFunction.clear();
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
  bufAppend("call ");
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
  allocatedStringsFunction = allocatedStringsFunctionCopy;
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
  bufAppend("pushq %r12\n");
  bufAppend("pushq %r13\n");
  
  e_add->expr_1->accept(this);
  bufAppend("pushq %rax\n");
  e_add->expr_2->accept(this);
  bufAppend("movq %rax, %rbx\n");
  bufAppend("popq %rax\n");
  bufAppend("movq %rax, %r12\n");//store first string's address
  bufAppend("movq %rbx, %r13\n");//store second string's address
  e_add->addop_->accept(this);
  bufAppend("pushq %rax\n");

  /* Check if any of the used expressions was a string that needs to be freed */
  if (typesStack.top() == STRING_CODE) {
    EVar *e_var = dynamic_cast<EVar*>(e_add->expr_1);
    if (e_var == NULL) {
      /* Now we know that this expression wasn't a variable */
      bufAppend("movq %r12, %rdi\n");
      alignStack(0);
      bufAppend("decq %rdi\n");
      bufAppend("call free\n");
      removePadding();
    }
    e_var = dynamic_cast<EVar*>(e_add->expr_2);
    if (e_var == NULL) {
      /* Now we know that this expression wasn't a variable */
      bufAppend("movq %r13, %rdi\n");
      alignStack(0);
      bufAppend("decq %rdi\n");
      bufAppend("call free\n");
      removePadding();
    }
  }

  bufAppend("popq %rax\n");
  bufAppend("popq %r13\n");
  bufAppend("popq %r12\n");

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
    bufAppend("call malloc\n");
    bufAppend("movb $0, (%rax)\n");
    bufAppend("incq %rax\n");
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
  bufAppend("cmpq $0, %rbx\n");
  bufAppend("jne PerformDivLabel");
  bufAppend(std::to_string(divisionCounter));
  bufAppend("\n");
  /* Division by 0 attempt discovered */
  alignStack(0);
  bufAppend("call error\n");

  bufAppend("PerformDivLabel");
  bufAppend(std::to_string(divisionCounter++));
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

void Compiler::visitListSimpleType(ListSimpleType *list_simple_type)
{
  for (ListSimpleType::iterator i = list_simple_type->begin() ; i != list_simple_type->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Compiler::visitListArrType(ListArrType *list_arr_type)
{
  for (ListArrType::iterator i = list_arr_type->begin() ; i != list_arr_type->end() ; ++i)
  {
    (*i)->accept(this);
  }
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
    if (typesStack.top() == STRING_CODE) {
      bufAppend("incb -1(%rax)\n");
    }
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
  bufAppend(std::to_string(x.length() + 2));//one more for null limiter and one more for reference counting
  bufAppend(", %rdi\n");

  /* (ReferenceCount|Characters|Null limiter) with sizes (1|x|1) */
  bufAppend("call malloc\n");
  bufAppend("movb $0, (%rax)\n");
  bufAppend("incq %rax\n");
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



