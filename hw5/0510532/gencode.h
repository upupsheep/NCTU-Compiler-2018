#ifndef _GENCODE_H_
#define _GENCODE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "symtab.h"

void initCodeFile(char *fn);
void genFuncDecl(char *methodName, struct param_sem *param,
                 struct PType *rtnType);
void genEndFunc(struct PType *returnType);
void genFuncInvoke(char *methodName, struct SymTable *table);
void genReturn(struct PType *returnType);
void limitStack();
void genGlobalVar(char *varName, struct PType *varType);
void genCheckNeg(struct expr_sem *expr, struct SymTable *table, int scope);
void genVarLoad(struct expr_sem *expr, struct SymTable *table, int scope);
void genVarStore(struct expr_sem *expr1, struct expr_sem *expr,
                 struct SymTable *table, int scope);
void genLiteralConst(struct expr_sem *expr, struct ConstAttr *constVal);
void genOperator(struct expr_sem *expr, OPERATOR op);
void castType(struct expr_sem *expr1, struct expr_sem *expr2);
void genPrint(struct expr_sem *exp, struct SymTable *table, int scope);
void genRead(struct expr_sem *expr, struct SymTable *table, int scope);
void genReadStream();
void genPrintStream();
void genCondition();
void genGoToExitLabel();
void genExitLabel();
void genElseLabel(int pop);
void genWhile();
void genGotoWhileLabel();
void genFor();
void genForConditionLabel();
void genForConditionEnd();
void genForBodyLabel();
void genForBodyEnd();
void genForIncrEnd();

#endif