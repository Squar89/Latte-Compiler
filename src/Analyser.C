#include <typeinfo>
#include "Analyser.H"
#include <iostream>

void Analyser::visitProgram(Program *t) {} //abstract class
void Analyser::visitTopDef(TopDef *t) {} //abstract class
void Analyser::visitArg(Arg *t) {} //abstract class
void Analyser::visitBlock(Block *t) {} //abstract class
void Analyser::visitStmt(Stmt *t) {} //abstract class
void Analyser::visitItem(Item *t) {} //abstract class
void Analyser::visitItemArr(ItemArr *t) {} //abstract class
void Analyser::visitSimpleType(SimpleType *t) {} //abstract class
void Analyser::visitArrType(ArrType *t) {} //abstract class
void Analyser::visitType(Type *t) {} //abstract class
void Analyser::visitExpr(Expr *t) {} //abstract class
void Analyser::visitAddOp(AddOp *t) {} //abstract class
void Analyser::visitMulOp(MulOp *t) {} //abstract class
void Analyser::visitRelOp(RelOp *t) {} //abstract class

/* Check if correct main function exists in the program
 * Correct main function has main as it's name,
 * takes no arguments and returns an int */
bool Analyser::correctMainExists() {
  /* Name is "main" */
  auto it = functionsMap.find("main");
  if (it == functionsMap.end()) {
    return false;
  }

  /* Function takes no arguments */
  if (!it->second->listarg_->empty()) {
    return false;
  }

  /* Return type is int */
  it->second->type_->accept(this);
  if (typesStack.top() != INT_CODE) {
    return false;
  }
  typesStack.pop();

  return true;
}

/* Entry point, start of the frontend to the compiler */
void Analyser::analyseProgram(Program *p, Program *lib)
{
  /* Prepare all used variables and structures */
  functionsMap.clear();
  variablesMap.clear();
  arraysMap.clear();
  currentDepth = 0;
  std::vector<std::pair<Ident, std::pair<int, unsigned long> > > overshadowedVariables;
  localRedeclVar.push_back(overshadowedVariables);
  std::vector<Ident> localVarSetup;
  localVar.push_back(localVarSetup);
  std::vector<std::pair<Ident, std::pair<bool, unsigned long> > > overshadowedArrays;
  localRedeclArr.push_back(overshadowedArrays);
  std::vector<Ident> localArrSetup;
  localArr.push_back(localArrSetup);

  /* Setup functionsMap with all functions definitions present in the input and those predefined */
  getFunctionsDefinitions((Prog*) lib);
  getFunctionsDefinitions((Prog*) p);

  /* Analyse input */
  p->accept(this);

  /* Check if proper main function was declared */
  if (!correctMainExists()) {
    fprintf(stderr, "ERROR\nMain not found. Main function must be of int type and take no arguments.\n");
    exit(MAIN_NOT_FOUND);
  }
}

/* This function puts all the function definitions present in the input to the functionsMap structure.
 * DISCLAIMER: This implementation assumes that function names must be unique.
 * e.g. There can't be two functions with different arguments sharing the same name */
void Analyser::getFunctionsDefinitions(Prog *prog)
{
  ListTopDef *list_top_def = prog->listtopdef_;

  /* The only thing we want from these definitions is to save them, there is no need to visit them yet */
  for (ListTopDef::iterator i = list_top_def->begin() ; i != list_top_def->end() ; ++i)
  {
    FnDef *fn_def = dynamic_cast<FnDef*> (*i);
    if (functionsMap.find(fn_def->ident_) != functionsMap.end()) {
      fprintf(stderr, "ERROR\nLine %d: trying to declare a function sharing name with previously declared function.\n",
              fn_def->type_->line_number);
      exit(DUPLICATE_FUN_NAME);
    }

    functionsMap.insert(std::make_pair(fn_def->ident_, fn_def));
  }
}

/* This function safely compares two types present on the typesStack
 * and exits with an error if they are different */
void Analyser::checkTypesMatch(int lineNumber)
{
  int type1, type2;

  if (!typesStack.empty()) {
    type1 = typesStack.top();
    typesStack.pop();
  }
  else {
    fprintf(stderr, "ERROR\nUnexpected error occurred at line %d (Trying to access empty stack).\n", lineNumber);
    exit(UNEXPECTED_ERROR);
  }

  if (!typesStack.empty()) {
    type2 = typesStack.top();
    typesStack.pop();
  }
  else {
    fprintf(stderr, "ERROR\nUnexpected error occurred at line %d (Trying to access empty stack).\n", lineNumber);
    exit(UNEXPECTED_ERROR);
  }

  if (type1 != type2) {
    fprintf(stderr, "ERROR\nLine %d: types do not match.\n", lineNumber);
    //fprintf(stderr, "Compared types: %d and %d\n", type1, type2);
    exit(TYPE_ERROR);
  }
}

/* This function looks for a declared function with name equal to ident in functionsMap
 * and then compares it's arguments list with call's arguments list provided in reversedCallTypes */
void Analyser::checkTypesFunctionCall(int lineNumber, Ident ident, std::vector<int> &reversedCallTypes) {
  /* Check if function with provided name was declared */
  auto it = functionsMap.find(ident);
  if (it == functionsMap.end()) {
    fprintf(stderr, "ERROR\nLine %d: called function was not found. Define your function first.\n", lineNumber);
    exit(FUNCTION_NOT_FOUND);
  }

  /* PAUSE_CODE allows for counting of stack elements.
   * We won't access any stack element not owned by this function */
  typesStack.push(PAUSE_CODE);

  /* Clear the variables map so there is no issue with their names or types while visiting FnDef arguments */
  std::unordered_map<Ident, std::pair<int, unsigned long> > variablesMapCopy(variablesMap);
  variablesMap.clear();
  it->second->listarg_->accept(this);
  variablesMap = variablesMapCopy;
  
  /* Compare the call with function arguments types */
  if (reversedCallTypes.size() != 0) {    
    for (auto callIt = reversedCallTypes.begin(); callIt != reversedCallTypes.end(); callIt++) {
      if (typesStack.top() == PAUSE_CODE) {
        fprintf(stderr, "ERROR\nLine %d: called function was not found. Define your function first.\n", lineNumber);
        exit(FUNCTION_NOT_FOUND);
      }
      typesStack.push(*callIt);
      checkTypesMatch(lineNumber);
    }
  }

  /* Check if all arguments were compared */
  if (typesStack.top() != PAUSE_CODE) {
    fprintf(stderr, "ERROR\nLine %d: called function was not found. Define your function first.\n", lineNumber);
    exit(FUNCTION_NOT_FOUND);
  }
  typesStack.pop();//pop PAUSE_CODE
}

void Analyser::visitProg(Prog *prog)
{
  prog->listtopdef_->accept(this);
}

void Analyser::visitFnDef(FnDef *fn_def)
{
  visitIdent(fn_def->ident_);
  fn_def->type_->accept(this);

  /* Store the function return type */
  currentReturnType = typesStack.top();
  typesStack.pop();

  /* Set all the return checking variables to false */
  returnFound = false;
  lastStmtWasReturn = false;
  ifFullReturn = false;

  variablesMap.clear();
  arraysMap.clear();
  /* Visit arguments and clear the stack from it's types */
  typesStack.push(PAUSE_CODE);
  fn_def->listarg_->accept(this);
  while (typesStack.top() != PAUSE_CODE) {
    typesStack.pop();
  }
  typesStack.pop();//pop PAUSE_CODE

  fn_def->block_->accept(this);
  variablesMap.clear();

  /* Check if the last statement in this function was a return
   * OR if there was an if statement present which always returns */
  if (!lastStmtWasReturn && !ifFullReturn) {
    /* Void type functions don't need return, check if this is the case */
    auto it = functionsMap.find(fn_def->ident_);
    it->second->type_->accept(this);
    if (typesStack.top() == VOID_CODE) {
      /* Function is void so just pop the void code from the stack and continue */
      typesStack.pop();
    }
    else {
      fprintf(stderr, "ERROR\nLine %d: Function must end with return statement.\n", fn_def->line_number);
      if (returnFound) {
        fprintf(stderr, "Make sure that all possible paths inside this function end with a return.\n");
        fprintf(stderr, "Also make sure that there is nothing after the final return.\n");
      }
      exit(RETURN_NOT_FOUND);
    }
  }
}

void Analyser::visitAr(Ar *ar)
{
  ar->type_->accept(this);
  visitIdent(ar->ident_);

  /* Try to add the argument's ident and type to the variablesMap */
  if (variablesMap.find(ar->ident_) != variablesMap.end()) {
    fprintf(stderr, "ERROR\nLine %d: variable %s was declared earlier.\n", ar->line_number, ar->ident_.c_str());
    exit(VAR_REDECLARATION);
  }

  /* These are the function's arguments so we insert them with the depth of a first block of this function */
  variablesMap.insert(std::make_pair(ar->ident_, std::make_pair(typesStack.top(), currentDepth + 1)));
  if (typesStack.top() > AR_TYPE_OFFSET) {
    arraysMap.insert(std::make_pair(ar->ident_, std::make_pair(true, 0)));
  }
}

void Analyser::visitBlk(Blk *blk)
{
  currentDepth++;

  /* Prepare overshadowed variables vector for this block.
   * It will store variables and values that were written over in this block
   * to restore them on leave. */
  if (localRedeclVar.size() != currentDepth) {
    fprintf(stderr, "ERROR\nUnexpected error occurred at line %d (Wrong localRedeclVar size).\n", blk->line_number);
    exit(UNEXPECTED_ERROR);
  }
  std::vector<std::pair<Ident, std::pair<int, unsigned long> > > overshadowedVariables;
  localRedeclVar.push_back(overshadowedVariables);

  /* Prepare overshadowed arrays vector for this block.
   * It will store arrays and it's info to restore them on leave. */
  if (localRedeclArr.size() != currentDepth) {
    fprintf(stderr, "ERROR\nUnexpected error occurred at line %d (Wrong localRedeclArr size).\n", blk->line_number);
    exit(UNEXPECTED_ERROR);
  }
  std::vector<std::pair<Ident, std::pair<bool, unsigned long> > > overshadowedArrays;
  localRedeclArr.push_back(overshadowedArrays);

  /* Prepare local variables vector for this block.
   * This will store all idents of variables declared in this block.
   * They will be removed from variablesMap on leave */
  if (localVar.size() != currentDepth) {
    fprintf(stderr, "ERROR\nUnexpected error occurred at line %d (Wrong localVar size).\n", blk->line_number);
    exit(UNEXPECTED_ERROR);
  }
  std::vector<Ident> localVarSetup;
  localVar.push_back(localVarSetup);

  if (localArr.size() != currentDepth) {
    fprintf(stderr, "ERROR\nUnexpected error occurred at line %d (Wrong localArr size).\n", blk->line_number);
    exit(UNEXPECTED_ERROR);
  }
  std::vector<Ident> localArrSetup;
  localArr.push_back(localArrSetup);

  /* Visit the block with all statements in it */
  blk->liststmt_->accept(this);

  /* Remove all variables declared in this block */
  while (!localVar[currentDepth].empty()) {
    variablesMap.erase(localVar[currentDepth].back());
    localVar[currentDepth].pop_back();
  }
  localVar.pop_back();

  /* Remove all arrays declared in this block */
  while (!localArr[currentDepth].empty()) {
    arraysMap.erase(localArr[currentDepth].back());
    localArr[currentDepth].pop_back();
  }
  localArr.pop_back();  

  /* Restore overshadowed variables */
  while (!localRedeclVar[currentDepth].empty()) {
    Ident ident = localRedeclVar[currentDepth].back().first;
    int typeCode = localRedeclVar[currentDepth].back().second.first;
    int declDepth = localRedeclVar[currentDepth].back().second.second;

    variablesMap.insert(std::make_pair(ident, std::make_pair(typeCode, declDepth)));
    localRedeclVar[currentDepth].pop_back();
  }
  localRedeclVar.pop_back();

  /* Restore overshadowed arrays */
  while (!localRedeclArr[currentDepth].empty()) {
    Ident ident = localRedeclArr[currentDepth].back().first;
    bool isInitialized = localRedeclArr[currentDepth].back().second.first;
    int arrLength = localRedeclArr[currentDepth].back().second.second;

    arraysMap.insert(std::make_pair(ident, std::make_pair(isInitialized, arrLength)));
    localRedeclArr[currentDepth].pop_back();
  }
  localRedeclArr.pop_back();

  currentDepth--;
}

void Analyser::visitEmpty(Empty *empty) {/* Do nothing */}

void Analyser::visitBStmt(BStmt *b_stmt)
{
  b_stmt->block_->accept(this);
}

void Analyser::visitDecl(Decl *decl)
{
  lastStmtWasReturn = false;

  /* This visit should leave declaration type on the stack */
  decl->simpletype_->accept(this);

  /* Visit each item (Ident OR Ident = Expr) and add them to the declared variables */
  decl->listitem_->accept(this);

  /* Remove declaration type from the stack */
  typesStack.pop();
}

void Analyser::visitDeclArr(DeclArr *decl_arr)
{
  lastStmtWasReturn = false;

  /* This visit should leave declaration type on the stack */
  decl_arr->arrtype_->accept(this);

  decl_arr->itemarr_->accept(this);

  /* Remove declaration type from the stack */
  typesStack.pop();
}

void Analyser::visitInitArrSt(InitArrSt *init_arr_st)
{
  lastStmtWasReturn = false;

  /* This is a situation when we want to initialize previously declared, but not initialized array */
  visitIdent(init_arr_st->ident_);

  /* Check if array with this ident is visible */
  if (auto arrIt = arraysMap.find(init_arr_st->ident_); arrIt != arraysMap.end()) {
    if (arrIt->second.first) {
      fprintf(stderr, "ERROR\nLine %d. Trying to initialize array that was initialized before.\n", init_arr_st->line_number);
      exit(VAR_REDECLARATION);
    }
  }
  else {
    fprintf(stderr, "ERROR\nLine %d. Array wasn't declared before, declare your array first.\n", init_arr_st->line_number);
    exit(VAR_NOT_DECLARED);
  }
  
  init_arr_st->simpletype_->accept(this);
  /* Check if types of declaration and this init match */
  if (typesStack.top() != variablesMap.find(init_arr_st->ident_)->second.first - AR_TYPE_OFFSET) {
    fprintf(stderr, "ERROR\nLine %d: declaration and init types do not match.\n", init_arr_st->line_number);
    exit(TYPE_ERROR);
  }
  typesStack.pop();

  init_arr_st->expr_->accept(this);
  /* Check if length expression type is an int */
  if (typesStack.top() != INT_CODE) {
    fprintf(stderr, "ERROR\nLine %d: expression specifying array's length must be an int.\n", init_arr_st->line_number);
    exit(TYPE_ERROR);
  }
  typesStack.pop();

  /* Set this array to initialized and set it's length */
  arraysMap.find(init_arr_st->ident_)->second = std::make_pair(true, 0);//placeholder length
}

void Analyser::visitAss(Ass *ass)
{
  lastStmtWasReturn = false;
  visitIdent(ass->ident_);
  ass->expr_->accept(this);

  /* Check if this variable was declared in current environment and if var's type match the expression's type. */
  if (auto it = variablesMap.find(ass->ident_); it != variablesMap.end()) {
    if (it->second.first != typesStack.top()) {
      fprintf(stderr, "ERROR\nLine %d. Assigned value type is different than variable's type.\n", ass->line_number);
      exit(TYPE_ERROR);
    }
  }
  else {
    fprintf(stderr, "ERROR\nLine %d. Used variable \"%s\" needs to be declared first.\n",
            ass->line_number, ass->ident_.c_str());
    exit(VAR_NOT_DECLARED);
  }

  /* Pop expression type from the stack */
  typesStack.pop();
}

void Analyser::visitAssArr(AssArr *ass_arr)
{
  lastStmtWasReturn = false;
  visitIdent(ass_arr->ident_);

  /* Check if this array was declared and initialized in current environment */
  if (auto arrIt = arraysMap.find(ass_arr->ident_); arrIt != arraysMap.end()) {
    if (!arrIt->second.first) {
      fprintf(stderr, "ERROR\nLine %d. Array was not initialized.\n", ass_arr->line_number);
      exit(AR_NOT_INITIALIZED);
    }
  }
  else {
    fprintf(stderr, "ERROR\nLine %d. Couldn't find array with ident \"%s\".\n", ass_arr->line_number, ass_arr->ident_.c_str());
    exit(VAR_NOT_DECLARED);
  }

  /* Check if expression specifying index is of type int */
  ass_arr->expr_1->accept(this);
  if (typesStack.top() != INT_CODE) {
    fprintf(stderr, "ERROR\nLine %d. Index specifier must be of type int.\n", ass_arr->line_number);
    exit(TYPE_ERROR);
  }
  typesStack.pop();

  /* Check if expression's type matches the array's type */
  ass_arr->expr_2->accept(this);
  if (auto it = variablesMap.find(ass_arr->ident_); it != variablesMap.end()) {
    if (it->second.first - AR_TYPE_OFFSET != typesStack.top()) {
      fprintf(stderr, "ERROR\nLine %d. Assigned value type is different than array's type.\n", ass_arr->line_number);
      exit(TYPE_ERROR);
    }
  }
  else {
    fprintf(stderr, "ERROR\nUnexpected error occurred at line %d (Data structures mishap).\n", ass_arr->line_number);
    exit(UNEXPECTED_ERROR);
  }
  typesStack.pop();

}

void Analyser::visitIncr(Incr *incr)
{
  lastStmtWasReturn = false;
  visitIdent(incr->ident_);

  /* Variable should exist and be of type int to be incremented */
  if (auto it = variablesMap.find(incr->ident_); it != variablesMap.end()) {
    if (it->second.first != INT_CODE) {
      fprintf(stderr, "ERROR\nLine %d. Variable \"%s\" must be of type int to be incremented.\n",
              incr->line_number, incr->ident_.c_str());
      exit(TYPE_ERROR);
    }
  }
  else {
    fprintf(stderr, "ERROR\nLine %d. Used variable \"%s\" needs to be declared first.\n",
            incr->line_number, incr->ident_.c_str());
    exit(VAR_NOT_DECLARED);
  }
}

void Analyser::visitIncrArr(IncrArr *incr_arr)
{
  lastStmtWasReturn = false;
  visitIdent(incr_arr->ident_);
  
  /* Check if array exists and was initialized */
  if (auto arrIt = arraysMap.find(incr_arr->ident_); arrIt != arraysMap.end()) {
    if (!arrIt->second.first) {
      fprintf(stderr, "ERROR\nLine %d. Array was not initialized.\n", incr_arr->line_number);
      exit(AR_NOT_INITIALIZED);
    }
  }
  else {
    fprintf(stderr, "ERROR\nLine %d. Couldn't find array with ident \"%s\".\n", incr_arr->line_number, incr_arr->ident_.c_str());
    exit(VAR_NOT_DECLARED);
  }

  /* Check if array is of type int */
  if (auto it = variablesMap.find(incr_arr->ident_); it != variablesMap.end()) {
    if (it->second.first - AR_TYPE_OFFSET != INT_CODE) {
      fprintf(stderr, "ERROR\nLine %d. Variable must be of type int to be incremented.\n", incr_arr->line_number);
      exit(TYPE_ERROR);
    }
  }
  else {
    fprintf(stderr, "ERROR\nUnexpected error occurred at line %d (Data structures mishap).\n", incr_arr->line_number);
    exit(UNEXPECTED_ERROR);
  }

  /* Check if index expression is of type int */
  incr_arr->expr_->accept(this);
  if (typesStack.top() != INT_CODE) {
    fprintf(stderr, "ERROR\nLine %d. Index specifier must be of type int.\n", incr_arr->line_number);
    exit(TYPE_ERROR);
  }
  typesStack.pop();
}

void Analyser::visitDecr(Decr *decr)
{
  lastStmtWasReturn = false;
  visitIdent(decr->ident_);

  /* Variable should exist and be of type int to be decremented */
  if (auto it = variablesMap.find(decr->ident_); it != variablesMap.end()) {
    if (it->second.first != INT_CODE) {
      fprintf(stderr, "ERROR\nLine %d. Variable \"%s\" must be of type int to be decremented.\n",
              decr->line_number, decr->ident_.c_str());
      exit(TYPE_ERROR);
    }
  }
  else {
    fprintf(stderr, "ERROR\nLine %d. Used variable \"%s\" needs to be declared first.\n",
            decr->line_number, decr->ident_.c_str());
    exit(VAR_NOT_DECLARED);
  }
}

void Analyser::visitDecrArr(DecrArr *decr_arr)
{
  lastStmtWasReturn = false;
  visitIdent(decr_arr->ident_);

  /* Check if array exists and was initialized */
  if (auto arrIt = arraysMap.find(decr_arr->ident_); arrIt != arraysMap.end()) {
    if (!arrIt->second.first) {
      fprintf(stderr, "ERROR\nLine %d. Array was not initialized.\n", decr_arr->line_number);
      exit(AR_NOT_INITIALIZED);
    }
  }
  else {
    fprintf(stderr, "ERROR\nLine %d. Couldn't find array with ident \"%s\".\n", decr_arr->line_number, decr_arr->ident_.c_str());
    exit(VAR_NOT_DECLARED);
  }

  /* Check if array is of type int */
  if (auto it = variablesMap.find(decr_arr->ident_); it != variablesMap.end()) {
    if (it->second.first - AR_TYPE_OFFSET != INT_CODE) {
      fprintf(stderr, "ERROR\nLine %d. Variable must be of type int to be incremented.\n", decr_arr->line_number);
      exit(TYPE_ERROR);
    }
  }
  else {
    fprintf(stderr, "ERROR\nUnexpected error occurred at line %d (Data structures mishap).\n", decr_arr->line_number);
    exit(UNEXPECTED_ERROR);
  }

  /* Check if index expression is of type int */
  decr_arr->expr_->accept(this);
  if (typesStack.top() != INT_CODE) {
    fprintf(stderr, "ERROR\nLine %d. Index specifier must be of type int.\n", decr_arr->line_number);
    exit(TYPE_ERROR);
  }
  typesStack.pop();
}

void Analyser::visitRet(Ret *ret)
{
  ret->expr_->accept(this);

  returnFound = true;
  lastStmtWasReturn = true;

  /* Check if this return's expresion is of the same type as the current function */
  if (currentReturnType != typesStack.top()) {
    fprintf(stderr, "ERROR\nLine %d. The return and function types do not match.\n", ret->line_number);
    exit(TYPE_ERROR);
  }

  /* Pop return type from the stack */
  typesStack.pop();
}

void Analyser::visitVRet(VRet *v_ret)
{
  returnFound = true;
  lastStmtWasReturn = true;

  /* Check if current function's type is void */
  if (currentReturnType != VOID_CODE) {
    fprintf(stderr, "ERROR\nLine %d. Function expects non void return.\n", v_ret->line_number);
    exit(TYPE_ERROR);
  }
}

void Analyser::visitCond(Cond *cond)
{
  lastStmtWasReturn = false;
  cond->expr_->accept(this);

  /* Expression needs to be of boolean type */
  if (typesStack.top() != BOOL_CODE) {
    fprintf(stderr, "ERROR\nLine %d. Condition inside of if must be boolean.\n", cond->line_number);
    exit(TYPE_ERROR);
  }
  typesStack.pop();

  cond->stmt_->accept(this);

  /* Check if condition is a true literal for return handling */
  ELitTrue *litTrue = dynamic_cast<ELitTrue*> (cond->expr_);
  if (litTrue != NULL && lastStmtWasReturn) {
    ifFullReturn = true;
  }

  /* If statement ending with return doesn't mean the function will end there.
   * It does only if the condition is a true literal which is covered with ifFullReturn variable */
  lastStmtWasReturn = false;
}

void Analyser::visitCondElse(CondElse *cond_else)
{
  bool ifHadReturn = false;
  bool elseHadReturn = false;
  lastStmtWasReturn = false;

  cond_else->expr_->accept(this);

  /* Expression needs to be of boolean type */
  if (typesStack.top() != BOOL_CODE) {
    fprintf(stderr, "ERROR\nLine %d. Condition inside of if must be boolean.\n", cond_else->line_number);
    exit(TYPE_ERROR);
  }
  typesStack.pop();

  /* Visit both if and else blocks remembering whether it returned at the end or not */
  cond_else->stmt_1->accept(this);
  if(lastStmtWasReturn) {
    ifHadReturn = true;
    lastStmtWasReturn = false;
  }
  cond_else->stmt_2->accept(this);
  if (lastStmtWasReturn) {
    elseHadReturn = true;
    lastStmtWasReturn = false;
  }

  /* Check if condition is a true/false literal for return handling */
  ELitTrue *litTrue = dynamic_cast<ELitTrue*> (cond_else->expr_);
  if (litTrue != NULL && ifHadReturn) {
    ifFullReturn = true;
  }
  ELitFalse *litFalse = dynamic_cast<ELitFalse*> (cond_else->expr_);
  if (litFalse != NULL && elseHadReturn) {
    ifFullReturn = true;
  }

  /* If both if and else ended with return then this return is final */
  if (ifHadReturn && elseHadReturn) {
    ifFullReturn = true;
  }
}

void Analyser::visitWhile(While *while_)
{
  lastStmtWasReturn = false;
  while_->expr_->accept(this);

  /* Expression needs to be of boolean type */
  if (typesStack.top() != BOOL_CODE) {
    fprintf(stderr, "ERROR\nLine %d. Condition inside of if must be boolean.\n", while_->line_number);
    exit(TYPE_ERROR);
  }
  typesStack.pop();

  while_->stmt_->accept(this);
}

void Analyser::visitForEach(ForEach *for_each)
{
  lastStmtWasReturn = false;
  bool overshadow = false;
  bool overshadowWasArray = false;
  std::pair<int, unsigned long> shadowedVar;
  std::pair<bool, unsigned long> shadowedArr;

  for_each->simpletype_->accept(this);
  int varType = typesStack.top();
  typesStack.pop();

  visitIdent(for_each->ident_1);
  /* Check if this variable was declared before */
  if (auto it = variablesMap.find(for_each->ident_1); it != variablesMap.end()) {
    /* Check if this variable was declared in the current block */
    if (it->second.second == currentDepth) {
      fprintf(stderr, "ERROR\nLine %d: variable %s was declared earlier.\n", for_each->line_number, for_each->ident_1.c_str());
      exit(VAR_REDECLARATION);
    }
    /* Else add variable to the overshadowed vector and remove it from variablesMap */
    else {
      overshadow = true;
      shadowedVar = std::make_pair(it->second.first, it->second.second);
      variablesMap.erase(for_each->ident_1);

      /* Check if previous value was an array and store it's info if so */
      if (auto arrIt = arraysMap.find(for_each->ident_1); arrIt != arraysMap.end()) {
        overshadowWasArray = true;
        shadowedArr = std::make_pair(arrIt->second.first, arrIt->second.second);
        arraysMap.erase(for_each->ident_1);
      }
    }
  }

  variablesMap.insert(std::make_pair(for_each->ident_1, std::make_pair(varType, currentDepth)));

  visitIdent(for_each->ident_2);
  /* Check if this array exists and was initialized */
  if (auto arrIt = arraysMap.find(for_each->ident_2); arrIt != arraysMap.end()) {
    if (!arrIt->second.first) {
      fprintf(stderr, "ERROR\nLine %d. Array was not initialized.\n", for_each->line_number);
      exit(AR_NOT_INITIALIZED);
    }
  }
  else {
    fprintf(stderr, "ERROR\nLine %d. Couldn't find array with ident \"%s\".\n", for_each->line_number, for_each->ident_2.c_str());
    exit(VAR_NOT_DECLARED);
  }

  /* Check if varType matches the array's type */
  if (auto it = variablesMap.find(for_each->ident_2); it != variablesMap.end()) {
    if (it->second.first - AR_TYPE_OFFSET != varType) {
      fprintf(stderr, "ERROR\nLine %d. Assigned value type is different than array's type.\n", for_each->line_number);
      exit(TYPE_ERROR);
    }
  }
  else {
    fprintf(stderr, "ERROR\nUnexpected error occurred at line %d (Data structures mishap).\n", for_each->line_number);
    exit(UNEXPECTED_ERROR);
  }

  for_each->stmt_->accept(this);

  /* Restore previous variables and arrays */
  variablesMap.erase(for_each->ident_1);
  if (overshadow) {
    variablesMap.insert(std::make_pair(for_each->ident_1, shadowedVar));
    if (overshadowWasArray) {
      arraysMap.insert(std::make_pair(for_each->ident_1, shadowedArr));
    }
  }
}

void Analyser::visitSExp(SExp *s_exp)
{
  lastStmtWasReturn = false;
  s_exp->expr_->accept(this);

  /* Pop the type of the expression as it won't be needed */
  typesStack.pop();
}

void Analyser::declareVariable(Ident ident, int lineNumber, bool isArray, bool arrayInitialized) {
  /* Check if this variable was declared before */
  if (auto it = variablesMap.find(ident); it != variablesMap.end()) {
    /* Check if this variable was declared in the current block */
    if (it->second.second == currentDepth) {
      fprintf(stderr, "ERROR\nLine %d: variable %s was declared earlier.\n", lineNumber, ident.c_str());
      exit(VAR_REDECLARATION);
    }
    /* Else add variable to the overshadowed vector and remove it from variablesMap */
    else {
      int typeCode = it->second.first;
      int declDepth = it->second.second;

      localRedeclVar[currentDepth].push_back(std::make_pair(ident, std::make_pair(typeCode, declDepth)));
      variablesMap.erase(ident);
      /* Check if previous value was an array and store it's info if so */
      if (auto arrIt = arraysMap.find(ident); arrIt != arraysMap.end()) {
        bool wasInitialized = arrIt->second.first;
        unsigned long arrLength = arrIt->second.second;
        localRedeclArr[currentDepth].push_back(std::make_pair(ident, std::make_pair(wasInitialized, arrLength)));
        arraysMap.erase(ident);
      }
    }
  }

  /* Insert pair <VariableIdent, <TypeCode, declarationDepth> >. This variable's type is on top of the stack (from visitDecl) */
  variablesMap.insert(std::make_pair(ident, std::make_pair(typesStack.top(), currentDepth)));

  /* Insert this ident to locally declared variables vector */
  localVar[currentDepth].push_back(ident);

  /* Insert this into arraysMap if it is an array */
  if(isArray) {
    arraysMap.insert(std::make_pair(ident, std::make_pair(arrayInitialized, 0)));//placeholder length
    localArr[currentDepth].push_back(ident);
  }
}

void Analyser::visitNoInit(NoInit *no_init)
{
  visitIdent(no_init->ident_);
  declareVariable(no_init->ident_, no_init->line_number, false, false);
}

void Analyser::visitInit(Init *init)
{
  visitIdent(init->ident_);
  init->expr_->accept(this);

  /* Save expression type and restore declaration type as top of stack */
  int exprType = typesStack.top();
  typesStack.pop();

  /* Check if types of declaration and assignment match */
  if (typesStack.top() != exprType) {
    fprintf(stderr, "ERROR\nLine %d: declaration and assignment types do not match.\n", init->line_number);
    //fprintf(stderr, "Declaration type: %d, assignment type: %d.\n", typesStack.top(), exprType);
    exit(TYPE_ERROR);
  }

  declareVariable(init->ident_, init->line_number, false, false);
}

void Analyser::visitNoInitArr(NoInitArr *no_init_arr)
{
  visitIdent(no_init_arr->ident_);
  declareVariable(no_init_arr->ident_, no_init_arr->line_number, true, false);
}

void Analyser::visitInitArr(InitArr *init_arr)
{
  visitIdent(init_arr->ident_);
  init_arr->simpletype_->accept(this);
  
  /* Save type used with init and restore declaration type as top of stack */
  int initType = typesStack.top();
  typesStack.pop();

  /* Check if types of declaration and init match */
  if (typesStack.top() - AR_TYPE_OFFSET != initType) {
    fprintf(stderr, "ERROR\nLine %d: declaration and init types do not match.\n", init_arr->line_number);
    fprintf(stderr, "Declaration type: %d, init type: %d\n", typesStack.top(), initType);
    exit(TYPE_ERROR);
  }

  /* Check if length expression type is an int */
  init_arr->expr_->accept(this);
  if (typesStack.top() != INT_CODE) {
    fprintf(stderr, "ERROR\nLine %d: expression specifying array's length must be an int.\n", init_arr->line_number);
    exit(TYPE_ERROR);
  }
  typesStack.pop();

  declareVariable(init_arr->ident_, init_arr->line_number, true, true);
}

void Analyser::visitInitArrE(InitArrE *init_arr_e)
{
  visitIdent(init_arr_e->ident_);
  init_arr_e->expr_->accept(this);

  /* Save expression type and restore declaration type as top of stack */
  int exprType = typesStack.top();
  typesStack.pop();

  /* Check if types of declaration and assignment match */
  if (typesStack.top() != exprType) {
    fprintf(stderr, "ERROR\nLine %d: declaration and assignment types do not match.\n", init_arr_e->line_number);
    exit(TYPE_ERROR);
  }

  declareVariable(init_arr_e->ident_, init_arr_e->line_number, true, true);
}

void Analyser::visitInt(Int *int_)
{
  typesStack.push(INT_CODE);
}

void Analyser::visitStr(Str *str)
{
  typesStack.push(STRING_CODE);
}

void Analyser::visitBool(Bool *bool_)
{
  typesStack.push(BOOL_CODE);
}

void Analyser::visitVoid(Void *void_)
{
  typesStack.push(VOID_CODE);
}

void Analyser::visitArr(Arr *arr)
{
  arr->simpletype_->accept(this);
  typesStack.top() += AR_TYPE_OFFSET;
}

void Analyser::visitAbsAT(AbsAT *abs_at)
{
  abs_at->arrtype_->accept(this);
}

void Analyser::visitAbsST(AbsST *abs_st)
{
  abs_st->simpletype_->accept(this);
}

void Analyser::visitEVar(EVar *e_var)
{
  visitIdent(e_var->ident_);

  /* Check if variable with this ident was declared and if so, push it's type onto the stack */
  if (auto it = variablesMap.find(e_var->ident_); it != variablesMap.end()) {
    typesStack.push(it->second.first);
  }
  else {
    fprintf(stderr, "ERROR\nLine %d. Unknown variable used \"%s\".\n", e_var->line_number, e_var->ident_.c_str());
    exit(VAR_NOT_DECLARED);
  }
}

void Analyser::visitEVarArr(EVarArr *e_var_arr)
{
  visitIdent(e_var_arr->ident_);

  /* Check if this array was declared and initialized in current environment */
  if (auto arrIt = arraysMap.find(e_var_arr->ident_); arrIt != arraysMap.end()) {
    if (!arrIt->second.first) {
      fprintf(stderr, "ERROR\nLine %d. Array was not initialized.\n", e_var_arr->line_number);
      exit(AR_NOT_INITIALIZED);
    }
  }
  else {
    fprintf(stderr, "ERROR\nLine %d. Couldn't find array with ident \"%s\".\n", e_var_arr->line_number, e_var_arr->ident_.c_str());
    exit(VAR_NOT_DECLARED);
  }

  /* Check if expression specifying index is of type int */
  e_var_arr->expr_->accept(this);
  if (typesStack.top() != INT_CODE) {
    fprintf(stderr, "ERROR\nLine %d. Index specifier must be of type int.\n", e_var_arr->line_number);
    exit(TYPE_ERROR);
  }
  typesStack.pop();

  /* Push type of array's elements onto stack */
  if (auto it = variablesMap.find(e_var_arr->ident_); it != variablesMap.end()) {
    typesStack.push(it->second.first - AR_TYPE_OFFSET);
  }
  else {
    fprintf(stderr, "ERROR\nUnexpected error occurred at line %d (Data structures mishap).\n", e_var_arr->line_number);
    exit(UNEXPECTED_ERROR);
  }
}

void Analyser::visitEAtr(EAtr *e_atr)
{
  visitIdent(e_atr->ident_);

  /* Check if such array exists and if it was initialized */
  if (auto arrIt = arraysMap.find(e_atr->ident_); arrIt != arraysMap.end()) {
    if (arrIt->second.first) {
      typesStack.push(INT_CODE);
    }
    else {
      fprintf(stderr, "ERROR\nLine %d. Array was not initialized.\n", e_atr->line_number);
      exit(AR_NOT_INITIALIZED);
    }
  }
  else {
    fprintf(stderr, "ERROR\nLine %d. Couldn't find array with ident \"%s\".\n", e_atr->line_number, e_atr->ident_.c_str());
    exit(VAR_NOT_DECLARED);
  }
}

void Analyser::visitELitInt(ELitInt *e_lit_int)
{
  visitInteger(e_lit_int->integer_);
}

void Analyser::visitELitTrue(ELitTrue *e_lit_true)
{
  typesStack.push(BOOL_CODE);
}

void Analyser::visitELitFalse(ELitFalse *e_lit_false)
{
  typesStack.push(BOOL_CODE);
}

void Analyser::visitEApp(EApp *e_app)
{
  /* PAUSE_CODE allows for counting of stack elements.
   * We won't access any stack element not owned by this function */
  typesStack.push(PAUSE_CODE);

  visitIdent(e_app->ident_);
  e_app->listexpr_->accept(this);

  /* Prepare functionTypes vector holding reversed types of this function call */
  std::vector<int> functionTypes;
  while (typesStack.top() != PAUSE_CODE) {
    functionTypes.push_back(typesStack.top());
    typesStack.pop();
  }
  typesStack.pop();//pop PAUSE_CODE
  checkTypesFunctionCall(e_app->line_number, e_app->ident_, functionTypes);

  /* Find out function's type and push it onto stack */
  auto it = functionsMap.find(e_app->ident_);//function with this name is guaranteed to exist
  it->second->type_->accept(this);//this will push this type onto stack
}

void Analyser::visitEString(EString *e_string)
{
  visitString(e_string->string_);
}

void Analyser::visitNeg(Neg *neg)
{
  neg->expr_->accept(this);

  /* Expression must be of type int or boolean */
  if (typesStack.top() != INT_CODE && typesStack.top() != BOOL_CODE) {
    fprintf(stderr, "ERROR\nLine %d. Expression must be of type int or bool to be negated.\n", neg->line_number);
    exit(TYPE_ERROR);
  }
}

void Analyser::visitNot(Not *not_)
{
  not_->expr_->accept(this);

  /* Expression must be of type boolean */
  if (typesStack.top() != BOOL_CODE) {
    fprintf(stderr, "ERROR\nLine %d. Expression must be of type bool to use \"not\" on it.\n", not_->line_number);
    exit(TYPE_ERROR);
  }
}

void Analyser::visitEMul(EMul *e_mul)
{
  e_mul->expr_1->accept(this);
  e_mul->mulop_->accept(this);
  e_mul->expr_2->accept(this);

  int resultType = typesStack.top();

  /* Check if both expressions have matching types */
  checkTypesMatch(e_mul->mulop_->line_number);

  /* Check if both expressions are of type int */
  if (resultType != INT_CODE) {
    fprintf(stderr, "ERROR\nLine %d. Expressions must be of type int to use multiplication.", e_mul->line_number);
    exit(TYPE_ERROR);
  }

  /* Both types were popped so push type of multiplication result */
  typesStack.push(resultType);
}

void Analyser::visitEAdd(EAdd *e_add)
{
  e_add->expr_1->accept(this);
  e_add->addop_->accept(this);
  e_add->expr_2->accept(this);

  int resultType = typesStack.top();

  /* Check if both expressions have matching types */
  checkTypesMatch(e_add->addop_->line_number);

  /* Check if both expressions are of type int or string */
  if (resultType != INT_CODE && resultType != STRING_CODE) {
    fprintf(stderr, "ERROR\nLine %d. Expressions must be of type int or string to use sum.", e_add->line_number);
    exit(TYPE_ERROR);
  }

  /* Both types were popped so push type of multiplication result */
  typesStack.push(resultType);
}

void Analyser::visitERel(ERel *e_rel)
{
  e_rel->expr_1->accept(this);
  e_rel->relop_->accept(this);
  e_rel->expr_2->accept(this);

  int resultType = typesStack.top();

  /* Check if both expressions have matching types */
  checkTypesMatch(e_rel->relop_->line_number);

  /* Check if both expressions aren't of type void or string*/
  if (resultType == VOID_CODE || resultType == STRING_CODE) {
    fprintf(stderr, "ERROR\nLine %d. Expressions can't be of type void or string.", e_rel->line_number);
    exit(TYPE_ERROR);
  }

  /* Result of ERel will always be boolean */
  typesStack.push(BOOL_CODE);
}

void Analyser::visitEAnd(EAnd *e_and)
{
  e_and->expr_1->accept(this);
  e_and->expr_2->accept(this);

  int resultType = typesStack.top();

  /* Check if both expressions have matching types */
  checkTypesMatch(e_and->line_number);

  /* Check if both expressions are of type bool */
  if (resultType != BOOL_CODE) {
    fprintf(stderr, "ERROR\nLine %d. Expressions must be of type bool.", e_and->line_number);
    exit(TYPE_ERROR);
  }

  /* Result of EAnd will always be boolean */
  typesStack.push(BOOL_CODE);
}

void Analyser::visitEOr(EOr *e_or)
{
  e_or->expr_1->accept(this);
  e_or->expr_2->accept(this);

  int resultType = typesStack.top();

  /* Check if both expressions have matching types */
  checkTypesMatch(e_or->line_number);

  /* Check if both expressions are of type bool */
  if (resultType != BOOL_CODE) {
    fprintf(stderr, "ERROR\nLine %d. Expressions must be of type bool.", e_or->line_number);
    exit(TYPE_ERROR);
  }

  /* Result of EOr will always be boolean */
  typesStack.push(BOOL_CODE);
}

void Analyser::visitPlus(Plus *plus) {/* Do nothing */}

void Analyser::visitMinus(Minus *minus)
{
  if (typesStack.top() != INT_CODE) {
    fprintf(stderr, "ERROR\nLine %d. Expressions must be of type int use Minus.", minus->line_number);
    exit(TYPE_ERROR);
  }
}

void Analyser::visitTimes(Times *times) {/* Do nothing */}

void Analyser::visitDiv(Div *div) {/* Do nothing */}

void Analyser::visitMod(Mod *mod) {/* Do nothing */}

void Analyser::visitLTH(LTH *lth) {/* Do nothing */}

void Analyser::visitLE(LE *le) {/* Do nothing */}

void Analyser::visitGTH(GTH *gth) {/* Do nothing */}

void Analyser::visitGE(GE *ge) {/* Do nothing */}

void Analyser::visitEQU(EQU *equ) {/* Do nothing */}

void Analyser::visitNE(NE *ne) {/* Do nothing */}

void Analyser::visitListTopDef(ListTopDef *list_top_def)
{
  for (ListTopDef::iterator i = list_top_def->begin() ; i != list_top_def->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Analyser::visitListArg(ListArg *list_arg)
{
  for (ListArg::iterator i = list_arg->begin() ; i != list_arg->end() ; ++i)
  {
    (*i)->accept(this);
  }
  
}

void Analyser::visitListStmt(ListStmt *list_stmt)
{
  for (ListStmt::iterator i = list_stmt->begin() ; i != list_stmt->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Analyser::visitListItem(ListItem *list_item)
{
  for (ListItem::iterator i = list_item->begin() ; i != list_item->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Analyser::visitListSimpleType(ListSimpleType *list_simple_type)
{
  for (ListSimpleType::iterator i = list_simple_type->begin() ; i != list_simple_type->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Analyser::visitListArrType(ListArrType *list_arr_type)
{
  for (ListArrType::iterator i = list_arr_type->begin() ; i != list_arr_type->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Analyser::visitListType(ListType *list_type)
{
  for (ListType::iterator i = list_type->begin() ; i != list_type->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Analyser::visitListExpr(ListExpr *list_expr)
{
  for (ListExpr::iterator i = list_expr->begin() ; i != list_expr->end() ; ++i)
  {
    (*i)->accept(this);
  }
}


void Analyser::visitInteger(Integer x)
{
  typesStack.push(INT_CODE);
}

void Analyser::visitChar(Char x)
{
  typesStack.push(CHAR_CODE);
}

void Analyser::visitDouble(Double x)
{
  typesStack.push(DOUBLE_CODE);
}

void Analyser::visitString(String x)
{
  typesStack.push(STRING_CODE);
}

void Analyser::visitIdent(Ident x) {/* Do nothing */}
