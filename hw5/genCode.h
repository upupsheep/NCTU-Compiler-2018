#ifndef _GENCODE_H_
#define _GENCODE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "symtab.h"
void initCodeFile(char *fn);
void genMethod(char *methodName, struct param_sem *par,
               struct PType *returnType);
void genEndMethod(struct PType *returnType);
void genMethodInvoke(char *methodName, struct SymTable *table);
void genReturn(struct PType *returnType);
void limitStack();
void genGlobalVariable(char *varName, struct PType *varType);
void genVariableLoad(struct expr_sem *expr, struct SymTable *table, int scope);
void genVariableStore(struct expr_sem *expr1, struct expr_sem *expr,
                      struct SymTable *table, int scope);
void genLiteralConst(struct expr_sem *expr, struct ConstAttr *constVal);
void genOP(struct expr_sem *expr, OPERATOR op);
void castType(struct expr_sem *expr1, struct expr_sem *expr2);
void genPrint(struct expr_sem *exp, struct SymTable *table, int scope);
void genRead(struct expr_sem *expr, struct SymTable *table, int scope);
void genReaderObj();
void genPrinterObj();
void genCondition();
void genGoToExitLabel();
void genExitLabel();
void genElseLabel(int pop);
void genWhileLabel();
void genGotoWhileLabel();
void genForConditionLabel();
void genForConditionEnd();
void genForBodyLabel();
void genForBodyEnd();
void genForIncrEnd();
#endif