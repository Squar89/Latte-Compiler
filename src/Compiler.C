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

char* Compiler::compile(Program *p) {
  bufReset();
  variablesMap.clear();
  andOrLabelCounter = 0;
  condCounter = 0;
  whileCounter = 0;
  stackCounter = 0;
  bufAppend(header);

  p->accept(this);

  bufAppend(footer);
  return buf_;
}

void Compiler::visitProg(Prog *prog)
{
  prog->listtopdef_->accept(this);
}

void Compiler::visitFnDef(FnDef *fn_def)
{
  fn_def->type_->accept(this);
  visitIdent(fn_def->ident_);
  bufAppend("_");
  bufAppend(fn_def->ident_);
  bufAppend(":\n");

  /* funtion prologue */
  bufAppend("push %rbp\n");
  bufAppend("movq %rsp, %rbp\n\n");
  //TODO do I need to reserve space for local variables here?

  fn_def->listarg_->accept(this);
  fn_def->block_->accept(this);

  /* function epilogue */
  bufAppend("\nmovq %rbp, %rsp\n");
  bufAppend("pop %rbp\n");
  bufAppend("ret\n");
}

void Compiler::visitAr(Ar *ar)
{
  /* Code For Ar Goes Here */

  ar->type_->accept(this);
  visitIdent(ar->ident_);

}

void Compiler::visitBlk(Blk *blk)
{
  /* Code For Blk Goes Here */

  blk->liststmt_->accept(this);

}

void Compiler::visitEmpty(Empty *empty)
{
  /* Code For Empty Goes Here */


}

void Compiler::visitBStmt(BStmt *b_stmt)
{
  /* Code For BStmt Goes Here */

  b_stmt->block_->accept(this);

}

void Compiler::visitDecl(Decl *decl)
{
  decl->type_->accept(this);
  decl->listitem_->accept(this);
}

void Compiler::visitAss(Ass *ass)
{
  visitIdent(ass->ident_);
  ass->expr_->accept(this);

  int offset = variablesMap.find(ass->ident_)->second;
  offset *= -8;
  bufAppend("movq %rax, ");
  bufAppend(std::to_string(offset));
  bufAppend("(%rbp)\n");
}

void Compiler::visitIncr(Incr *incr)
{
  visitIdent(incr->ident_);

  int offset = variablesMap.find(incr->ident_)->second;
  offset *= -8;
  bufAppend("incq ");
  bufAppend(std::to_string(offset));
  bufAppend("(%rbp)\n");
}

void Compiler::visitDecr(Decr *decr)
{
  visitIdent(decr->ident_);

  int offset = variablesMap.find(decr->ident_)->second;
  offset *= -8;
  bufAppend("decq ");
  bufAppend(std::to_string(offset));
  bufAppend("(%rbp)\n");
}

void Compiler::visitRet(Ret *ret)
{
  ret->expr_->accept(this);
}

void Compiler::visitVRet(VRet *v_ret){ /* Do nothing */ }

void Compiler::visitCond(Cond *cond)
{
  int currentCondCounter = condCounter;
  condCounter++;

  cond->expr_->accept(this);
  bufAppend("cmpq $0, %rax\n");
  bufAppend("je _afterCondLabel");
  bufAppend(std::to_string(currentCondCounter));
  bufAppend("\n");
  cond->stmt_->accept(this);

  bufAppend("_afterCondLabel");
  bufAppend(std::to_string(currentCondCounter));
  bufAppend(":\n");
}

void Compiler::visitCondElse(CondElse *cond_else)
{
  int currentCondCounter = condCounter;
  condCounter++;

  cond_else->expr_->accept(this);
  bufAppend("cmpq $0, %rax\n");
  bufAppend("je _elseCondLabel");
  bufAppend(std::to_string(currentCondCounter));
  bufAppend("\n");
  cond_else->stmt_1->accept(this);
  bufAppend("jmp _afterCondLabel");
  bufAppend(std::to_string(currentCondCounter));
  bufAppend("\n");

  bufAppend("_elseCondLabel");
  bufAppend(std::to_string(currentCondCounter));
  bufAppend(":\n");
  cond_else->stmt_2->accept(this);

  bufAppend("_afterCondLabel");
  bufAppend(std::to_string(currentCondCounter));
  bufAppend(":\n");
}

void Compiler::visitWhile(While *while_)
{
  int currentWhileCounter = whileCounter;

  bufAppend("jmp _whileCondLabel");
  bufAppend(std::to_string(currentWhileCounter));
  bufAppend("\n");
  bufAppend("_whileStmtLabel");
  bufAppend(std::to_string(currentWhileCounter));
  bufAppend(":\n");
  while_->stmt_->accept(this);

  bufAppend("_whileCondLabel");
  bufAppend(std::to_string(currentWhileCounter));
  bufAppend(":\n");
  while_->expr_->accept(this);
  bufAppend("cmpq $0, %rax\n");
  bufAppend("jne _whileStmtLabel");
  bufAppend(std::to_string(currentWhileCounter));
  bufAppend("\n");
}

void Compiler::visitSExp(SExp *s_exp)
{
  /* Code For SExp Goes Here */

  s_exp->expr_->accept(this);

}

void Compiler::declareVariable(Ident ident) {
  stackCounter++;

  //TODO add suport for overshadowing variables
  variablesMap.insert(std::make_pair(ident, stackCounter));
  bufAppend("pushq %rax\n");
}

void Compiler::visitNoInit(NoInit *no_init)
{
  visitIdent(no_init->ident_);

  /* Initialize the variable to 0 TODO how about strings? */
  bufAppend("movq $0, %rax\n");
  declareVariable(no_init->ident_);
}

void Compiler::visitInit(Init *init)
{ 
  visitIdent(init->ident_);
  init->expr_->accept(this);

  declareVariable(init->ident_);
}

void Compiler::visitInt(Int *int_)
{
  /* Code For Int Goes Here */
}

void Compiler::visitStr(Str *str)
{
  /* Code For Str Goes Here */


}

void Compiler::visitBool(Bool *bool_)
{
  /* Code For Bool Goes Here */


}

void Compiler::visitVoid(Void *void_)
{
  /* Code For Void Goes Here */


}

void Compiler::visitFun(Fun *fun)
{
  /* Code For Fun Goes Here */

  fun->type_->accept(this);
  fun->listtype_->accept(this);

}

void Compiler::visitEVar(EVar *e_var)
{
  visitIdent(e_var->ident_);

  int offset = variablesMap.find(e_var->ident_)->second;
  offset *= -8;
  bufAppend("movq ");
  bufAppend(std::to_string(offset));
  bufAppend("(%rbp), %rax\n");
}

void Compiler::visitELitInt(ELitInt *e_lit_int)
{
  visitInteger(e_lit_int->integer_);
}

void Compiler::visitELitTrue(ELitTrue *e_lit_true) { /* Do nothing */ }

void Compiler::visitELitFalse(ELitFalse *e_lit_false) { /* Do nothing */ }

void Compiler::visitEApp(EApp *e_app)
{
  /* Code For EApp Goes Here */

  visitIdent(e_app->ident_);
  e_app->listexpr_->accept(this);

}

void Compiler::visitEString(EString *e_string)
{
  /* Code For EString Goes Here */

  visitString(e_string->string_);

}

void Compiler::visitNeg(Neg *neg)
{
  neg->expr_->accept(this);
  bufAppend("neg %rax\n");
}

void Compiler::visitNot(Not *not_)
{
  not_->expr_->accept(this);
  bufAppend("cmpq $0, %rax\n");
  bufAppend("movq $0, %rax\n");
  bufAppend("sete %al\n");
}

void Compiler::visitEMul(EMul *e_mul)
{
  e_mul->expr_1->accept(this);
  bufAppend("pushq %rax\n");
  e_mul->expr_2->accept(this);
  bufAppend("movq %rax, %rbx\n");
  bufAppend("popq %rax\n");
  e_mul->mulop_->accept(this);
}

void Compiler::visitEAdd(EAdd *e_add)
{
  e_add->expr_1->accept(this);
  bufAppend("pushq %rax\n");
  e_add->expr_2->accept(this);
  bufAppend("movq %rax, %rbx\n");
  bufAppend("popq %rax\n");
  e_add->addop_->accept(this);
}

void Compiler::visitERel(ERel *e_rel)
{
  e_rel->expr_1->accept(this);
  bufAppend("pushq %rax\n");
  e_rel->expr_2->accept(this);
  bufAppend("popq %rbx\n");

  //Append the shared comparison operations
  bufAppend("cmpq %rax, %rbx\n");
  bufAppend("movq $0, %rax\n");
  e_rel->relop_->accept(this);
}

void Compiler::visitEAnd(EAnd *e_and)
{
  e_and->expr_1->accept(this);
  bufAppend("cmpq $0, $rax\n");
  bufAppend("je _andOrLabelSkip");
  bufAppend(std::to_string(andOrLabelCounter));
  bufAppend("\n");
  e_and->expr_2->accept(this);

  bufAppend("_andOrLabelSkip");
  bufAppend(std::to_string(andOrLabelCounter));
  bufAppend(":\n");

  andOrLabelCounter++;
}

void Compiler::visitEOr(EOr *e_or)
{
  e_or->expr_1->accept(this);
  bufAppend("cmpq $0, %rax\n");
  bufAppend("jne _andOrLabelSkip");
  bufAppend(std::to_string(andOrLabelCounter));
  bufAppend("\n");
  e_or->expr_2->accept(this);

  bufAppend("_andOrLabelSkip");
  bufAppend(std::to_string(andOrLabelCounter));
  bufAppend(":\n");

  andOrLabelCounter++;
}

void Compiler::visitPlus(Plus *plus)
{
  bufAppend("addq %rbx, %rax\n");
}

void Compiler::visitMinus(Minus *minus)
{
  bufAppend("subq %rbx, %rax\n");
}

void Compiler::visitTimes(Times *times)
{
  bufAppend("imulq %rbx, %rax\n");
}

void Compiler::visitDiv(Div *div)
{
  //TODO check for division by 0
  bufAppend("cdq\n");
  bufAppend("idiv %rbx\n");
}

void Compiler::visitMod(Mod *mod)
{
  //TODO check for division by 0
  bufAppend("cdq\n");
  bufAppend("idiv %rbx\n");
  //The remainder is stored in rdx, move it into rax
  bufAppend("movq %rdx, %rax\n");
}

void Compiler::visitLTH(LTH *lth)
{
  bufAppend("setl %al\n");
}

void Compiler::visitLE(LE *le)
{
  bufAppend("setle %al\n");
}

void Compiler::visitGTH(GTH *gth)
{
  bufAppend("setg %al\n");
}

void Compiler::visitGE(GE *ge)
{
  bufAppend("setge %al\n");
}

void Compiler::visitEQU(EQU *equ)
{
  bufAppend("sete %al\n");
}

void Compiler::visitNE(NE *ne)
{
  bufAppend("setne %al\n");
}


void Compiler::visitListTopDef(ListTopDef *list_top_def)
{
  for (ListTopDef::iterator i = list_top_def->begin() ; i != list_top_def->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Compiler::visitListArg(ListArg *list_arg)
{
  for (ListArg::iterator i = list_arg->begin() ; i != list_arg->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Compiler::visitListStmt(ListStmt *list_stmt)
{
  for (ListStmt::iterator i = list_stmt->begin() ; i != list_stmt->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Compiler::visitListItem(ListItem *list_item)
{
  for (ListItem::iterator i = list_item->begin() ; i != list_item->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Compiler::visitListType(ListType *list_type)
{
  for (ListType::iterator i = list_type->begin() ; i != list_type->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Compiler::visitListExpr(ListExpr *list_expr)
{
  for (ListExpr::iterator i = list_expr->begin() ; i != list_expr->end() ; ++i)
  {
    (*i)->accept(this);
  }
}


void Compiler::visitInteger(Integer x)
{
  bufAppend("mov $");
  bufAppend(std::to_string(x));
  bufAppend(", %rax\n");
}

void Compiler::visitChar(Char x)
{
  /* Code for Char Goes Here */
}

void Compiler::visitDouble(Double x)
{
  /* Code for Double Goes Here */
}

void Compiler::visitString(String x)
{
  /* Code for String Goes Here */
}

void Compiler::visitIdent(Ident x)
{
  /* Code for Ident Goes Here */
}



