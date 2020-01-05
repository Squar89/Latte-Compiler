#include <typeinfo>
#include "Analyser.H"
#include <iostream>

void Analyser::visitProgram(Program *t) {} //abstract class
void Analyser::visitTopDef(TopDef *t) {} //abstract class
void Analyser::visitArg(Arg *t) {} //abstract class
void Analyser::visitBlock(Block *t) {} //abstract class
void Analyser::visitStmt(Stmt *t) {} //abstract class
void Analyser::visitItem(Item *t) {} //abstract class
void Analyser::visitType(Type *t) {} //abstract class
void Analyser::visitExpr(Expr *t) {} //abstract class
void Analyser::visitAddOp(AddOp *t) {} //abstract class
void Analyser::visitMulOp(MulOp *t) {} //abstract class
void Analyser::visitRelOp(RelOp *t) {} //abstract class

/* Check if correct main function exists in the program
 * Correct main function has main as it's name,
 * takes no arguments and returns an int */
bool Analyser::correctMainExists() {
  //std::cout << "correctMainExists\n";

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
void Analyser::analyseProgram(Program *p)
{
  //std::cout << "analyseProgram\n";
  /* Prepare all used variables and structures */
  functionsMap.clear();
  variablesMap.clear();
  currentDepth = 0;
  std::vector< std::pair<Ident, std::pair<int, unsigned long> > > overshadowedVariables;
  localRedeclVar.push_back(overshadowedVariables);
  std::vector<Ident> localVarSetup;
  localVar.push_back(localVarSetup);

  /* Setup functionsMap with all functions definitions present in the input */
  getFunctionsDefinitions((Prog*) p);

  /* Analyse input */
  visitProg((Prog*) p);

  /* Check if proper main function was declared */
  if (!correctMainExists()) {
    fprintf(stderr, "ERROR\nMain not found. Main function must be of int type and take no arguments.\n");
    exit(MAIN_NOT_FOUND);
  }

  //TODO DEBUG
  printf("Printing loose values left on stack:\n");
  while (!typesStack.empty()) {
    printf("%d\n", typesStack.top());
    typesStack.pop();
  }
  printf("Printing done.\n");
  //END DEBUG
}

/* This function puts all the function definitions present in the input to the functionsMap structure.
 * DISCLAIMER: This implementation assumes that function names must be unique.
 * e.g. There can't be two functions with different arguments sharing the same name */
void Analyser::getFunctionsDefinitions(Prog *prog)
{
  //std::cout << "getFunctionsDefinitions\n";
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
  //std::cout << "checkTypesMatch\n";
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
  //std::cout << "checkTypesFunctionCall\n";
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
  //std::cout << "Exiting checkTypesFunctionCall\n";
}

void Analyser::visitProg(Prog *prog)
{
  //std::cout << "Prog\n";
  prog->listtopdef_->accept(this);
}

void Analyser::visitFnDef(FnDef *fn_def)
{
  //std::cout << "FnDef\n";
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
   * OR if there was and if statement present which always returns */
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
  //std::cout << "Ar\n";
  ar->type_->accept(this);
  visitIdent(ar->ident_);

  /* Try to add the argument's ident and type to the variablesMap */
  if (variablesMap.find(ar->ident_) != variablesMap.end()) {
    fprintf(stderr, "ERROR\nLine %d: variable %s was declared earlier.\n", ar->line_number, ar->ident_.c_str());
    exit(VAR_REDECLARATION);
  }

  /* These are the function's arguments so we insert them with the depth of a first block of this function */
  variablesMap.insert(std::make_pair(ar->ident_, std::make_pair(typesStack.top(), currentDepth + 1)));
  //std::cout << "Exiting Ar\n";
}

void Analyser::visitBlk(Blk *blk)
{
  //std::cout << "Blk\n";
  currentDepth++;

  /* Prepare overshadowed variables vector for this block.
   * It will store variables and values that were written over in this block
   * to restore them on leave. */
  if (localRedeclVar.size() != currentDepth) {
    fprintf(stderr, "ERROR\nUnexpected error occurred at line %d (Wrong localRedeclVar size).\n", blk->line_number);
    exit(UNEXPECTED_ERROR);
  }
  std::vector< std::pair<Ident, std::pair<int, unsigned long> > > overshadowedVariables;
  localRedeclVar.push_back(overshadowedVariables);

  /* Prepare local variables vector for this block.
   * This will store all idents of variables declared in this block.
   * They will be removed from variablesMap on leave */
  if (localVar.size() != currentDepth) {
    fprintf(stderr, "ERROR\nUnexpected error occurred at line %d (Wrong localVar size).\n", blk->line_number);
    exit(UNEXPECTED_ERROR);
  }
  std::vector<Ident> localVarSetup;
  localVar.push_back(localVarSetup);

  /* Visit the block with all statements in it */
  blk->liststmt_->accept(this);

  /* Remove all variables declared in this block */
  while (!localVar[currentDepth].empty()) {
    variablesMap.erase(localVar[currentDepth].back());
    localVar[currentDepth].pop_back();
  }
  localVar.pop_back();

  /* Restore overshadowed variables */
  while (!localRedeclVar[currentDepth].empty()) {
    Ident ident = localRedeclVar[currentDepth].back().first;
    int typeCode = localRedeclVar[currentDepth].back().second.first;
    int declDepth = localRedeclVar[currentDepth].back().second.second;

    variablesMap.insert(std::make_pair(ident, std::make_pair(typeCode, declDepth)));
    localRedeclVar[currentDepth].pop_back();
  }
  localRedeclVar.pop_back();

  currentDepth--;
}

void Analyser::visitEmpty(Empty *empty) {/* Do nothing */}

void Analyser::visitBStmt(BStmt *b_stmt)
{
  //std::cout << "BStmt\n";
  b_stmt->block_->accept(this);
}

void Analyser::visitDecl(Decl *decl)
{
  //std::cout << "Decl\n";
  //std::cout << typesStack.size() << "<-sizeEntry\n";
  lastStmtWasReturn = false;

  /* This visit should leave declaration type on the stack */
  decl->type_->accept(this);

  //std::cout << typesStack.top() << "<-type\n";

  /* Visit each item (Ident OR Ident = Expr) and add them to the declared variables */
  decl->listitem_->accept(this);

  /* Remove declaration type from the stack */
  typesStack.pop();
  //std::cout << typesStack.size() << "<-sizeLeave\n";
}

void Analyser::visitAss(Ass *ass)
{
  //std::cout << "Ass\n";
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

void Analyser::visitIncr(Incr *incr)
{
  //std::cout << "Incr\n";
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

void Analyser::visitDecr(Decr *decr)
{
  //std::cout << "Decr\n";
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

void Analyser::visitRet(Ret *ret)
{
  //std::cout << "Ret\n";
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
  //std::cout << "VRet\n";
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
  //std::cout << "Cond\n";
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
  //std::cout << "CondElse\n";
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
  //std::cout << "While\n";
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

void Analyser::visitSExp(SExp *s_exp)
{
  //std::cout << "SExp\n";
  lastStmtWasReturn = false;
  s_exp->expr_->accept(this);

  /* Pop the type of the expression as it won't be needed */
  typesStack.pop();
}

void Analyser::declareVariable(Ident ident, int lineNumber) {
  //std::cout << "declareVariable\n";
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
    }
  }

  /* Insert pair <VariableIdent, <TypeCode, declarationDepth> >. This variable's type is on top of the stack (from visitDecl) */
  variablesMap.insert(std::make_pair(ident, std::make_pair(typesStack.top(), currentDepth)));

  /* Insert this ident to locally declared variables vector */
  localVar[currentDepth].push_back(ident);
}

void Analyser::visitNoInit(NoInit *no_init)
{
  //std::cout << "NoInit\n";
  visitIdent(no_init->ident_);
  declareVariable(no_init->ident_, no_init->line_number);
}

void Analyser::visitInit(Init *init)
{
  //std::cout << "Init\n";
  visitIdent(init->ident_);
  declareVariable(init->ident_, init->line_number);

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
}

void Analyser::visitInt(Int *int_)
{
  //std::cout << "visitInt\n";
  typesStack.push(INT_CODE);
}

void Analyser::visitStr(Str *str)
{
  //std::cout << "visitStr\n";
  typesStack.push(STRING_CODE);
}

void Analyser::visitBool(Bool *bool_)
{
  //std::cout << "visitBool\n";
  typesStack.push(BOOL_CODE);
}

void Analyser::visitVoid(Void *void_)
{
  //std::cout << "visitVoid\n";
  typesStack.push(VOID_CODE);
}

void Analyser::visitFun(Fun *fun)
{
  //std::cout << "visitFun\n";
  fprintf(stderr, "Hello there, are you lost?\n"); /* TODO */

  fun->type_->accept(this);
  fun->listtype_->accept(this);
  /* TODO if I leave this function here, the stack should be cleared properly */
}

void Analyser::visitEVar(EVar *e_var)
{
  //std::cout << "visitEVar\n";
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

void Analyser::visitELitInt(ELitInt *e_lit_int)
{
  //std::cout << "visitELitInt\n";
  visitInteger(e_lit_int->integer_);
}

void Analyser::visitELitTrue(ELitTrue *e_lit_true)
{
  //std::cout << "visitELitTrue\n";
  typesStack.push(BOOL_CODE);
}

void Analyser::visitELitFalse(ELitFalse *e_lit_false)
{
  //std::cout << "visitELitFalse\n";
  typesStack.push(BOOL_CODE);
}

void Analyser::visitEApp(EApp *e_app)
{
  //std::cout << "visitEApp\n";
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
  //std::cout << "visitEString\n";
  visitString(e_string->string_);
}

void Analyser::visitNeg(Neg *neg)
{
  //std::cout << "visitNeg\n";
  neg->expr_->accept(this);

  /* Expression must be of type int or boolean */
  if (typesStack.top() != INT_CODE && typesStack.top() != BOOL_CODE) {
    fprintf(stderr, "ERROR\nLine %d. Expression must be of type int or bool to be negated.\n", neg->line_number);
    exit(TYPE_ERROR);
  }
}

void Analyser::visitNot(Not *not_)
{
  //std::cout << "visitNot\n";
  not_->expr_->accept(this);

  /* Expression must be of type boolean */
  if (typesStack.top() != BOOL_CODE) {
    fprintf(stderr, "ERROR\nLine %d. Expression must be of type bool to use \"not\" on it.\n", not_->line_number);
    exit(TYPE_ERROR);
  }
}

void Analyser::visitEMul(EMul *e_mul)
{
  //std::cout << "visitEMul\n";
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
  //std::cout << "visitEAdd\n";
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
  //std::cout << "visitERel\n";
  e_rel->expr_1->accept(this);
  e_rel->relop_->accept(this);
  e_rel->expr_2->accept(this);

  int resultType = typesStack.top();

  /* Check if both expressions have matching types */
  checkTypesMatch(e_rel->relop_->line_number);

  /* Check if both expressions aren't of type void */
  if (resultType == VOID_CODE) {
    fprintf(stderr, "ERROR\nLine %d. Expressions can't be of type void.", e_rel->line_number);
    exit(TYPE_ERROR);
  }

  /* Result of ERel will always be boolean */
  typesStack.push(BOOL_CODE);
}

void Analyser::visitEAnd(EAnd *e_and)
{
  //std::cout << "visitEAnd\n";
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
  //std::cout << "visitEOr\n";
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

void Analyser::visitMinus(Minus *minus) {/* Do nothing */}

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
  //std::cout << "visitListTopDef\n";
  for (ListTopDef::iterator i = list_top_def->begin() ; i != list_top_def->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Analyser::visitListArg(ListArg *list_arg)
{
  //std::cout << "visitListArg\n";
  for (ListArg::iterator i = list_arg->begin() ; i != list_arg->end() ; ++i)
  {
    (*i)->accept(this);
  }
  //std::cout << "Exiting visitListArg\n";
}

void Analyser::visitListStmt(ListStmt *list_stmt)
{
  //std::cout << "visitListStmt\n";
  for (ListStmt::iterator i = list_stmt->begin() ; i != list_stmt->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Analyser::visitListItem(ListItem *list_item)
{
  //std::cout << "visitListItem\n";
  for (ListItem::iterator i = list_item->begin() ; i != list_item->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Analyser::visitListType(ListType *list_type)
{
  //std::cout << "visitListType\n";
  for (ListType::iterator i = list_type->begin() ; i != list_type->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Analyser::visitListExpr(ListExpr *list_expr)
{
  //std::cout << "visitListExpr\n";
  for (ListExpr::iterator i = list_expr->begin() ; i != list_expr->end() ; ++i)
  {
    (*i)->accept(this);
  }
}


void Analyser::visitInteger(Integer x)
{
  //std::cout << "visitInteger\n";
  typesStack.push(INT_CODE);
}

void Analyser::visitChar(Char x)
{
  //std::cout << "visitChar\n";
  typesStack.push(CHAR_CODE);
}

void Analyser::visitDouble(Double x)
{
  //std::cout << "visitDouble\n";
  typesStack.push(DOUBLE_CODE);
}

void Analyser::visitString(String x)
{
  //std::cout << "visitString\n";
  typesStack.push(STRING_CODE);
}

void Analyser::visitIdent(Ident x) {/* Do nothing */}
