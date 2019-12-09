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
  getFunctionsDefinitions((Prog*) p);
  //functionsCalled.clear();

  visitProg((Prog*) p);

  //check if proper main function was declared
  if (!correctMainExists()) {
    printf("Error: main not found. Main function must be of int type and take no arguments\n");
    exit(MAIN_NOT_FOUND);
  }
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

void Analyser::visitProg(Prog *prog)
{
  /* Code For Prog Goes Here */

  prog->listtopdef_->accept(this);

}

void Analyser::visitFnDef(FnDef *fn_def)
{

  fn_def->type_->accept(this);
  visitIdent(fn_def->ident_);
  fn_def->listarg_->accept(this);
  fn_def->block_->accept(this);
}

void Analyser::visitAr(Ar *ar)
{
  /* Code For Ar Goes Here */

  ar->type_->accept(this);
  visitIdent(ar->ident_);

}

void Analyser::visitBlk(Blk *blk)
{
  /* Code For Blk Goes Here */

  blk->liststmt_->accept(this);

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
  /* Code For Decl Goes Here */

  decl->type_->accept(this);
  decl->listitem_->accept(this);

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

void Analyser::visitNoInit(NoInit *no_init)
{
  /* Code For NoInit Goes Here */

  visitIdent(no_init->ident_);

}

void Analyser::visitInit(Init *init)
{
  /* Code For Init Goes Here */

  visitIdent(init->ident_);
  init->expr_->accept(this);

}

void Analyser::visitInt(Int *int_)
{
  /* Code For Int Goes Here */


}

void Analyser::visitStr(Str *str)
{
  /* Code For Str Goes Here */


}

void Analyser::visitBool(Bool *bool_)
{
  /* Code For Bool Goes Here */


}

void Analyser::visitVoid(Void *void_)
{
  /* Code For Void Goes Here */


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
  /* Code For ELitTrue Goes Here */


}

void Analyser::visitELitFalse(ELitFalse *e_lit_false)
{
  /* Code For ELitFalse Goes Here */


}

void Analyser::visitEApp(EApp *e_app)
{
  /* Code For EApp Goes Here */

  visitIdent(e_app->ident_);
  e_app->listexpr_->accept(this);

  /* get list of types from expressions above */
  //TODO
  //ListType* types = new ListType;
  //functionsCalled.insert(std::make_pair(e_app->ident_, types));
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

}

void Analyser::visitEAdd(EAdd *e_add)
{
  /* Code For EAdd Goes Here */

  e_add->expr_1->accept(this);
  e_add->addop_->accept(this);
  e_add->expr_2->accept(this);

}

void Analyser::visitERel(ERel *e_rel)
{
  /* Code For ERel Goes Here */

  e_rel->expr_1->accept(this);
  e_rel->relop_->accept(this);
  e_rel->expr_2->accept(this);

}

void Analyser::visitEAnd(EAnd *e_and)
{
  /* Code For EAnd Goes Here */

  e_and->expr_1->accept(this);
  e_and->expr_2->accept(this);

}

void Analyser::visitEOr(EOr *e_or)
{
  /* Code For EOr Goes Here */

  e_or->expr_1->accept(this);
  e_or->expr_2->accept(this);

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
  /* Code for Integer Goes Here */
}

void Analyser::visitChar(Char x)
{
  /* Code for Char Goes Here */
}

void Analyser::visitDouble(Double x)
{
  /* Code for Double Goes Here */
}

void Analyser::visitString(String x)
{
  /* Code for String Goes Here */
}

void Analyser::visitIdent(Ident x)
{
  /* Code for Ident Goes Here */
}



