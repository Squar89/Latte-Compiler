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
  /* Code For Prog Goes Here */

  prog->listtopdef_->accept(this);

}

void Analyser::visitFnDef(FnDef *fn_def)
{
  fn_def->type_->accept(this);
  visitIdent(fn_def->ident_);

  variablesMap.clear();
  fn_def->listarg_->accept(this);
  fn_def->block_->accept(this);
  variablesMap.clear();
}

void Analyser::visitAr(Ar *ar)
{
  ar->type_->accept(this);
  visitIdent(ar->ident_);

  /* Try to add the argument's ident and type to the variablesMap */
  if (variablesMap.find(ar->ident_) != variablesMap.end()) {
    printf("Error at line %d: variable %s was declared earlier\n", ar->line_number, ar->ident_.c_str());
    exit(VARIABLE_REDECLARED);
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

void Analyser::visitEmpty(Empty *empty)
{
  /* Code For Empty Goes Here */


}

void Analyser::visitBStmt(BStmt *b_stmt)
{
  /* Code For BStmt Goes Here */

  b_stmt->block_->accept(this);

}

void Analyser::visitDecl(Decl *decl)
{
  /* This visit should leave declaration type on the stack */
  decl->type_->accept(this);

  /* Visit each item (Ident OR Ident = Expr) and add them to the declared variables */
  decl->listitem_->accept(this);

  /* Remove declaration type from the stack */
  typesStack.pop();
}

void Analyser::visitAss(Ass *ass)
{
  /* Code For Ass Goes Here */

  visitIdent(ass->ident_);
  ass->expr_->accept(this);

}

void Analyser::visitIncr(Incr *incr)
{
  /* Code For Incr Goes Here */

  visitIdent(incr->ident_);

}

void Analyser::visitDecr(Decr *decr)
{
  /* Code For Decr Goes Here */

  visitIdent(decr->ident_);

}

void Analyser::visitRet(Ret *ret)
{
  /* Code For Ret Goes Here */

  ret->expr_->accept(this);

}

void Analyser::visitVRet(VRet *v_ret)
{
  /* Code For VRet Goes Here */


}

void Analyser::visitCond(Cond *cond)
{
  /* Code For Cond Goes Here */

  cond->expr_->accept(this);
  cond->stmt_->accept(this);

}

void Analyser::visitCondElse(CondElse *cond_else)
{
  /* Code For CondElse Goes Here */

  cond_else->expr_->accept(this);
  cond_else->stmt_1->accept(this);
  cond_else->stmt_2->accept(this);

}

void Analyser::visitWhile(While *while_)
{
  /* Code For While Goes Here */

  while_->expr_->accept(this);
  while_->stmt_->accept(this);

}

void Analyser::visitSExp(SExp *s_exp)
{
  /* Code For SExp Goes Here */

  s_exp->expr_->accept(this);

}

void Analyser::declareVariable(Ident ident, int lineNumber) {
  /* Check if this variable wasn't declared before at this depth */
  if (auto it = variablesMap.find(ident); it != variablesMap.end()) {
    /* Check if this variable was declared in the current block */
    if (it->second.second == currentDepth) {
      printf("Error at line %d: variable %s was declared earlier\n", lineNumber, ident.c_str());
      exit(VARIABLE_REDECLARED);
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

  fun->type_->accept(this);
  fun->listtype_->accept(this);
}

void Analyser::visitEVar(EVar *e_var)
{
  /* Code For EVar Goes Here */

  visitIdent(e_var->ident_);

}

void Analyser::visitELitInt(ELitInt *e_lit_int)
{
  /* Code For ELitInt Goes Here */

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
  /* Code For EString Goes Here */

  visitString(e_string->string_);

}

void Analyser::visitNeg(Neg *neg)
{
  /* Code For Neg Goes Here */

  neg->expr_->accept(this);

}

void Analyser::visitNot(Not *not_)
{
  /* Code For Not Goes Here */

  not_->expr_->accept(this);

}

void Analyser::visitEMul(EMul *e_mul)
{
  /* Code For EMul Goes Here */

  e_mul->expr_1->accept(this);
  e_mul->mulop_->accept(this);
  e_mul->expr_2->accept(this);

  int resultType = typesStack.top();

  checkTypesMatch(e_mul->mulop_->line_number);
  typesStack.push(resultType);
}

void Analyser::visitEAdd(EAdd *e_add)
{
  e_add->expr_1->accept(this);
  e_add->addop_->accept(this);
  e_add->expr_2->accept(this);

  int resultType = typesStack.top();

  checkTypesMatch(e_add->addop_->line_number);
  typesStack.push(resultType);
}

void Analyser::visitERel(ERel *e_rel)
{
  e_rel->expr_1->accept(this);
  e_rel->relop_->accept(this);
  e_rel->expr_2->accept(this);

  checkTypesMatch(e_rel->relop_->line_number);
  typesStack.push(BOOL_CODE);
}

void Analyser::visitEAnd(EAnd *e_and)
{
  /* Code For EAnd Goes Here */

  e_and->expr_1->accept(this);
  e_and->expr_2->accept(this);

  typesStack.push(BOOL_CODE);
}

void Analyser::visitEOr(EOr *e_or)
{
  /* Code For EOr Goes Here */

  e_or->expr_1->accept(this);
  e_or->expr_2->accept(this);

  typesStack.push(BOOL_CODE);
}

void Analyser::visitPlus(Plus *plus)
{
  /* Code For Plus Goes Here */


}

void Analyser::visitMinus(Minus *minus)
{
  /* Code For Minus Goes Here */


}

void Analyser::visitTimes(Times *times)
{
  /* Code For Times Goes Here */


}

void Analyser::visitDiv(Div *div)
{
  /* Code For Div Goes Here */


}

void Analyser::visitMod(Mod *mod)
{
  /* Code For Mod Goes Here */


}

void Analyser::visitLTH(LTH *lth)
{
  /* Code For LTH Goes Here */


}

void Analyser::visitLE(LE *le)
{
  /* Code For LE Goes Here */


}

void Analyser::visitGTH(GTH *gth)
{
  /* Code For GTH Goes Here */


}

void Analyser::visitGE(GE *ge)
{
  /* Code For GE Goes Here */


}

void Analyser::visitEQU(EQU *equ)
{
  /* Code For EQU Goes Here */


}

void Analyser::visitNE(NE *ne)
{
  /* Code For NE Goes Here */


}


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

void Analyser::visitIdent(Ident x)
{
  
}



