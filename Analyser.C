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

bool Analyser::correctMainExists() {
  //name is "main"
  auto it = functionsMap.find("main");
  if (it == functionsMap.end()) {
    return false;
  }

  //function takes no arguments
  if (!it->second->listarg_->empty()) {
    return false;
  }

  //return type is int
  Int *typeCheck = dynamic_cast<Int*> (it->second->type_);
  if (typeCheck == NULL) {
    return false;
  }

  return true;
}

void Analyser::analyseProgram(Program *p)
{
  functionsMap.clear();
  variablesMap.clear();
  currentDepth = 0;
  std::vector< std::pair<Ident, std::pair<int, unsigned long> > > overshadowedVariables;
  localRedeclVar.push_back(overshadowedVariables);
  std::vector<Ident> localVarSetup;
  localVar.push_back(localVarSetup);

  getFunctionsDefinitions((Prog*) p);

  visitProg((Prog*) p);

  //check if proper main function was declared
  if (!correctMainExists()) {
    printf("Error: main not found. Main function must be of int type and take no arguments\n");
    exit(MAIN_NOT_FOUND);
  }

  /*//TODO DEBUG later so there is no loose values on stack
  while (!typesStack.empty()) {
    printf("%d\n", typesStack.top());
    typesStack.pop();
  }*/
}

//this implementation assumes that function names must be unique.
//e.g. There can't be two functions with different arguments sharing the same name
void Analyser::getFunctionsDefinitions(Prog *prog)
{
  ListTopDef *list_top_def = prog->listtopdef_;
  for (ListTopDef::iterator i = list_top_def->begin() ; i != list_top_def->end() ; ++i)
  {
    FnDef *fn_def = dynamic_cast<FnDef*> (*i);
    if (functionsMap.find(fn_def->ident_) != functionsMap.end()) {
      printf("Error at line %d: trying to declare a function sharing name with previously declared function\n",
              fn_def->type_->line_number);
      exit(DUPLICATE_FUN_NAME);
    }

    functionsMap.insert(std::make_pair(fn_def->ident_, fn_def));
  }
}

//This function assumes that types to check are already on the typesStack
void Analyser::checkTypesMatch(int lineNumber)
{
  int type_1, type_2;

  if (!typesStack.empty()) {
    type_1 = typesStack.top();
    typesStack.pop();
  }
  else {
    printf("Unexpected error occurred at line %d (Trying to access empty stack)\n", lineNumber);
    exit(UNEXPECTED_ERROR);
  }
  if (!typesStack.empty()) {
    type_2 = typesStack.top();
    typesStack.pop();
  }
  else {
    printf("Unexpected error occurred at line %d (Trying to access empty stack)\n", lineNumber);
    exit(UNEXPECTED_ERROR);
  }

  if (type_1 != type_2) {
    printf("Error at line %d: types do not match\n", lineNumber);
    exit(TYPE_ERROR);
  }
}

void Analyser::checkTypesFunctionCall(int lineNumber, Ident ident, std::vector<int> &reversedCallTypes) {
  auto it = functionsMap.find(ident);
  if (it == functionsMap.end()) {
    printf("Error at line %d: called function was not found. Define your function first\n", lineNumber);
    exit(FUNCTION_NOT_FOUND);
  }

  typesStack.push(PAUSE_CODE);

  /* Clear the variables map so there is no issue with their names or types while visiting FnDef arguments */
  std::unordered_map<Ident, std::pair<int, unsigned long> > variablesMapCopy(variablesMap);
  variablesMap.clear();
  it->second->listarg_->accept(this);
  variablesMap = variablesMapCopy;
  
  auto temp = reversedCallTypes.end();
  temp--;
  for (auto revIt = temp; revIt != reversedCallTypes.begin(); revIt--) {
    typesStack.push(*revIt);
    checkTypesMatch(lineNumber);
  }
  typesStack.push(*(reversedCallTypes.begin()));
  checkTypesMatch(lineNumber);
  typesStack.pop();//pop PAUSE_CODE
}

void Analyser::visitProg(Prog *prog)
{
  prog->listtopdef_->accept(this);
}

void Analyser::visitFnDef(FnDef *fn_def)
{
  fn_def->type_->accept(this);

  /* Store the function return type */
  currentReturnType = typesStack.top();
  typesStack.pop();

  /* Set all the return checking variables to false */
  returnFound = false;
  lastStmtWasReturn = false;
  ifReturn = false;
  elseReturn = false;

  visitIdent(fn_def->ident_);

  variablesMap.clear();
  fn_def->listarg_->accept(this);
  fn_def->block_->accept(this);
  variablesMap.clear();

  /* Check if the last statement in this function was a return */
  if (!lastStmtWasReturn && !(ifReturn && elseReturn)) {
    printf("Error at line %d: Function must end with return statement\n", fn_def->line_number);
    if (returnFound) {
      printf("Make sure that all possible paths inside this function end with a return.\n");
      printf("Also make sure that there is nothing after the final return.\n");
    }
    exit(RETURN_NOT_FOUND);
  }
}

void Analyser::visitAr(Ar *ar)
{
  ar->type_->accept(this);
  visitIdent(ar->ident_);

  /* Try to add the argument's ident and type to the variablesMap */
  if (variablesMap.find(ar->ident_) != variablesMap.end()) {
    printf("Error at line %d: variable %s was declared earlier\n", ar->line_number, ar->ident_.c_str());
    exit(VAR_REDECLARATION);
  }

  /* These are the function's arguments so we insert them with the depth of a first block of this function */
  variablesMap.insert(std::make_pair(ar->ident_, std::make_pair(typesStack.top(), currentDepth + 1)));//TODO no problems with not popping stack here?
}

void Analyser::visitBlk(Blk *blk)
{
  currentDepth++;

  /* Prepare overshadowed variables vector for this block */
  if (localRedeclVar.size() != currentDepth) {
    printf("Unexpected error occurred at line %d (Wrong localRedeclVar size)\n", blk->line_number);
    exit(UNEXPECTED_ERROR);
  }
  std::vector< std::pair<Ident, std::pair<int, unsigned long> > > overshadowedVariables;
  localRedeclVar.push_back(overshadowedVariables);

  /* Prepare local variables vector for this block */
  if (localVar.size() != currentDepth) {
    printf("Unexpected error occurred at line %d (Wrong localVar size)\n", blk->line_number);
    exit(UNEXPECTED_ERROR);
  }
  std::vector<Ident> localVarSetup;
  localVar.push_back(localVarSetup);

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
  b_stmt->block_->accept(this);
}

void Analyser::visitDecl(Decl *decl)
{
  lastStmtWasReturn = false;
  ifReturn = false;
  elseReturn = false;

  /* This visit should leave declaration type on the stack */
  decl->type_->accept(this);

  /* Visit each item (Ident OR Ident = Expr) and add them to the declared variables */
  decl->listitem_->accept(this);

  /* Remove declaration type from the stack */
  typesStack.pop();
}

void Analyser::visitAss(Ass *ass)
{
  lastStmtWasReturn = false;
  ifReturn = false;
  elseReturn = false;

  visitIdent(ass->ident_);
  ass->expr_->accept(this);

  /* Check if this variable exists in current environment and if var's type match the expressios's type */
  if (auto it = variablesMap.find(ass->ident_); it != variablesMap.end()) {
    if (it->second.first != typesStack.top()) {
      printf("Error at line %d. Assigned value type is different than variable's type.\n", ass->line_number);
      exit(TYPE_ERROR);
    }
  }
  else {
    printf("Error at line %d. Used variable \"%s\" needs to be declared first.\n",
            ass->line_number, ass->ident_.c_str());
    exit(VAR_NOT_DECLARED);
  }

  /* Pop expression type from the stack */
  typesStack.pop();
}

void Analyser::visitIncr(Incr *incr)
{
  lastStmtWasReturn = false;
  ifReturn = false;
  elseReturn = false;

  visitIdent(incr->ident_);

  /* Variable should exist and be of type int to be incremented */
  if (auto it = variablesMap.find(incr->ident_); it != variablesMap.end()) {
    if (it->second.second != INT_CODE) {
      printf("Error at line %d. Variable \"%s\" must be of type int to be incremented.\n",
              incr->line_number, incr->ident_.c_str());
      exit(TYPE_ERROR);
    }
  }
  else {
    printf("Error at line %d. Used variable \"%s\" needs to be declared first..\n",
            incr->line_number, incr->ident_.c_str());
    exit(VAR_NOT_DECLARED);
  }
}

void Analyser::visitDecr(Decr *decr)
{
  lastStmtWasReturn = false;
  ifReturn = false;
  elseReturn = false;

  visitIdent(decr->ident_);

  /* Variable should exist and be of type int to be decremented */
  if (auto it = variablesMap.find(decr->ident_); it != variablesMap.end()) {
    if (it->second.second != INT_CODE) {
      printf("Error at line %d. Variable \"%s\" must be of type int to be incremented.\n",
              decr->line_number, decr->ident_.c_str());
      exit(TYPE_ERROR);
    }
  }
  else {
    printf("Error at line %d. Used variable \"%s\" needs to be declared first..\n",
            decr->line_number, decr->ident_.c_str());
    exit(VAR_NOT_DECLARED);
  }
}

void Analyser::visitRet(Ret *ret)
{
  ret->expr_->accept(this);

  returnFound = true;
  lastStmtWasReturn = true;

  /* Check if this return's expresion is of the same type as the current function */
  if (currentReturnType != typesStack.top()) {
    printf("Error at line %d. The return and function types do not match\n", ret->line_number);
    exit(TYPE_ERROR);
  }


}

void Analyser::visitVRet(VRet *v_ret)
{
  returnFound = true;
  lastStmtWasReturn = true;

  /* Check if current function's type is void */
  if (currentReturnType != VOID_CODE) {
    printf("Error at line %d. Function expects non void return\n", v_ret->line_number);
    exit(TYPE_ERROR);
  }
}

void Analyser::visitCond(Cond *cond)
{
  lastStmtWasReturn = false;
  ifReturn = false;
  elseReturn = false;

  cond->expr_->accept(this);
  /* TODO I could also check if there is a true/false literal here will it be needed */

  /* Expression needs to be of boolean type */
  if (typesStack.top() != BOOL_CODE) {
    printf("Error at line %d. Condition inside of if must be boolean\n", cond->line_number);
    exit(TYPE_ERROR);
  }
  typesStack.pop();

  cond->stmt_->accept(this);
  lastStmtWasReturn = false;
}

void Analyser::visitCondElse(CondElse *cond_else)
{
  lastStmtWasReturn = false;
  ifReturn = false;
  elseReturn = false;

  cond_else->expr_->accept(this);

  /* Expression needs to be of boolean type */
  if (typesStack.top() != BOOL_CODE) {
    printf("Error at line %d. Condition inside of if must be boolean\n", cond_else->line_number);
    exit(TYPE_ERROR);
  }
  typesStack.pop();

  cond_else->stmt_1->accept(this);
  if(lastStmtWasReturn) {
    ifReturn = true;
    lastStmtWasReturn = false;
  }
  cond_else->stmt_2->accept(this);
  if (lastStmtWasReturn) {
    elseReturn = true;
    lastStmtWasReturn = false;
  }
}

void Analyser::visitWhile(While *while_)
{
  lastStmtWasReturn = false;
  ifReturn = false;
  elseReturn = false;

  while_->expr_->accept(this);

  /* Expression needs to be of boolean type */
  if (typesStack.top() != BOOL_CODE) {
    printf("Error at line %d. Condition inside of if must be boolean\n", while_->line_number);
    exit(TYPE_ERROR);
  }
  typesStack.pop();

  while_->stmt_->accept(this);
}

void Analyser::visitSExp(SExp *s_exp)
{
  lastStmtWasReturn = false;
  ifReturn = false;
  elseReturn = false;

  s_exp->expr_->accept(this);
}

void Analyser::declareVariable(Ident ident, int lineNumber) {
  /* Check if this variable wasn't declared before at this depth */
  if (auto it = variablesMap.find(ident); it != variablesMap.end()) {
    /* Check if this variable was declared in the current block */
    if (it->second.second == currentDepth) {
      printf("Error at line %d: variable %s was declared earlier\n", lineNumber, ident.c_str());
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
  visitIdent(no_init->ident_);
  declareVariable(no_init->ident_, no_init->line_number);
}

void Analyser::visitInit(Init *init)
{
  visitIdent(init->ident_);
  declareVariable(init->ident_, init->line_number);

  init->expr_->accept(this);

  /* Save expression type and restore declaration type as top of stack */
  int exprType = typesStack.top();
  typesStack.pop();

  /* Check if types of declaration and assignment match */
  if (typesStack.top() != exprType) {
    printf("Error at line %d: declaration and assignment types do not match\n", init->line_number);
    printf("Declaration type: %d, assignment type: %d\n", typesStack.top(), exprType);
    exit(TYPE_ERROR);
  }
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

void Analyser::visitFun(Fun *fun)
{
  /* Code For Fun Goes Here */
  printf("Hello there, are you lost?\n"); /* TODO */

  fun->type_->accept(this);
  fun->listtype_->accept(this);
}

void Analyser::visitEVar(EVar *e_var)
{
  visitIdent(e_var->ident_);

  if (auto it = variablesMap.find(e_var->ident_); it != variablesMap.end()) {
    typesStack.push(it->second.first);
  }
  else {
    printf("Error at line %d. Unknown variable used \"%s\"\n", e_var->line_number, e_var->ident_.c_str());
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
  typesStack.push(PAUSE_CODE);
  visitIdent(e_app->ident_);
  e_app->listexpr_->accept(this);

  std::vector<int> functionTypes;
  while (typesStack.top() != PAUSE_CODE) {
    functionTypes.push_back(typesStack.top());
    typesStack.pop();
  }
  typesStack.pop();//pop PAUSE_CODE
  checkTypesFunctionCall(e_app->line_number, e_app->ident_, functionTypes);
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
    printf("Error at line %d. Expression must be of type int or bool to be negated\n", neg->line_number);
    exit(TYPE_ERROR);
  }
}

void Analyser::visitNot(Not *not_)
{
  not_->expr_->accept(this);

  /* Expression must be of type boolean */
  if (typesStack.top() != BOOL_CODE) {
    printf("Error at line %d. Expression must be of type bool\n", not_->line_number);
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
    printf("Error at line %d. Expressions must be of type int to use multiplication", e_mul->line_number);
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
    printf("Error at line %d. Expressions must be of type int or string to use sum", e_add->line_number);
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

  /* Check if both expressions aren't of type void */
  if (resultType == VOID_CODE) {
    printf("Error at line %d. Expressions can't be of type void", e_rel->line_number);
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
    printf("Error at line %d. Expressions must be of type bool", e_and->line_number);
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
    printf("Error at line %d. Expressions must be of type bool", e_or->line_number);
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
