/**
 * @file
 *
 * This file contains the code for the Print traversal.
 * The traversal has the uid: PRT
 *
 *
 */

#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "palm/dbug.h"

/**
 * @fn PRTprogram
 */
// node_st *PRTprogram(node_st *node)
// {
//   TRAVs(node);
//   printf("Arithmetic Operators Count:\n");
//   printf("+ : %d\n", PROGRAM_PLUSCOUNT(node));
//   printf("- : %d\n", PROGRAM_MINUSCOUNT(node));
//   printf("* : %d\n", PROGRAM_MULCOUNT(node));
//   printf("/ : %d\n", PROGRAM_DIVCOUNT(node));
//   printf("%% : %d\n", PROGRAM_MODCOUNT(node));

//   TRAVchildren(node);
//   return node;
// }

/**
 * @fn PRTstmts
 */
node_st *PRTstmts(node_st *node)
{
  TRAVstmt(node);
  TRAVnext(node);
  return node;
}

/**
 * @fn PRTassign
 */
node_st *PRTassign(node_st *node)
{

  if (ASSIGN_LET(node) != NULL)
  {
    TRAVlet(node);
    printf(" = ");
  }

  TRAVexpr(node);
  printf(";\n");

  return node;
}

/**
 * @fn PRTbinop
 */
node_st *PRTbinop(node_st *node)
{
  char *tmp = NULL;
  printf("( ");

  TRAVleft(node);

  switch (BINOP_OP(node))
  {
  case BO_add:
    tmp = "+";
    break;
  case BO_sub:
    tmp = "-";
    break;
  case BO_mul:
    tmp = "*";
    break;
  case BO_div:
    tmp = "/";
    break;
  case BO_mod:
    tmp = "%";
    break;
  case BO_lt:
    tmp = "<";
    break;
  case BO_le:
    tmp = "<=";
    break;
  case BO_gt:
    tmp = ">";
    break;
  case BO_ge:
    tmp = ">=";
    break;
  case BO_eq:
    tmp = "==";
    break;
  case BO_ne:
    tmp = "!=";
    break;
  case BO_or:
    tmp = "||";
    break;
  case BO_and:
    tmp = "&&";
    break;
  case BO_NULL:
    DBUG_ASSERT(false, "unknown binop detected!");
  }

  printf(" %s ", tmp);

  TRAVright(node);

  printf(")(%d:%d-%d)", NODE_BLINE(node), NODE_BCOL(node), NODE_ECOL(node));

  return node;
}

/**
 * @fn PRTvarlet
 */
node_st *PRTvarlet(node_st *node)
{
  printf("%s(%d:%d)", VARLET_NAME(node), NODE_BLINE(node), NODE_BCOL(node));
  return node;
}

/**
 * @fn PRTvar
 */
node_st *PRTvar(node_st *node)
{
  printf("%s", VAR_NAME(node));
  return node;
}

/**
 * @fn PRTnum
 */
node_st *PRTnum(node_st *node)
{
  printf("%d", NUM_VAL(node));
  return node;
}

/**
 * @fn PRTfloat
 */
node_st *PRTfloat(node_st *node)
{
  printf("%f", FLOAT_VAL(node));
  return node;
}

/**
 * @fn PRTbool
 */
node_st *PRTbool(node_st *node)
{
  char *bool_str = BOOL_VAL(node) ? "true" : "false";
  printf("%s", bool_str);
  return node;
}

/**
 * @fn PRTifstmt
 */
node_st *PRTif(node_st *node)
{
  printf("if (");
  TRAVopt(IF_CONDITION(node));
  printf(") ");
  TRAVdo(IF_THENBRANCH(node));
  if (IF_ELSEBRANCH(node) != NULL)
  {
    printf(" else ");
    TRAVdo(IF_ELSEBRANCH(node));
  }
  return node;
}

/**
 * @fn PRTwhile
 */
node_st *PRTwhile(node_st *node)
{
  printf("while (");
  TRAVopt(WHILE_CONDITION(node));
  printf(") ");
  TRAVdo(WHILE_BODY(node));
  return node;
}

/**
 * @fn PRTfor
 */
node_st *PRTfor(node_st *node)
{
  printf("for (");
  TRAVopt(FOR_INIT(node));
  printf("; ");
  TRAVopt(FOR_CONDITION(node));
  printf("; ");
  TRAVopt(FOR_UPDATE(node));
  printf(") ");
  TRAVdo(FOR_BODY(node));
  return node;
}

/**
 * @fn PRTreturn
 */
node_st *PRTreturn(node_st *node)
{
  printf("return ");
  TRAVopt(RETURN_RETURNVALUE(node));
  printf(";\n");
  return node;
}

/**
 * @fn PRTblock
 */
node_st *PRTblock(node_st *node)
{
  printf("{\n");
  TRAVopt(BLOCK_STATEMENTS(node));
  printf("}\n");
  return node;
}

/**
 * @fn PRTmonop
 */
node_st *PRTmonop(node_st *node)
{
  switch (MONOP_OP(node))
  {
  case MO_not:
    printf("!");
    break;
  case MO_neg:
    printf("-");
    break;
  }
  TRAVopt(MONOP_OPERAND(node));
  return node;
}

/**
 * @fn PRTconst
 */
node_st *PRTconst(node_st *node)
{
  // Assuming CONST_VALUE is a macro to access the value of the Const node
  printf("%s", CONST_VALUE(node));
  return node;
}

/**
 * @fn PRTfuncallexpr
 */
node_st *PRTfuncallexpr(node_st *node)
{
  printf("%s(", FUNCALLEXPR_FUNCTIONNAME(node));
  TRAVopt(FUNCALLEXPR_ARGUMENTS(node));
  printf(")");
  return node;
}

/**
 * @fn PRTprogram
 */
node_st *PRTprogram(node_st *node)
{
  // Print the statements within the program
  TRAVopt(PROGRAM_STMTS(node));

  // Continue traversal for any other children of the Program node, if necessary
  TRAVchildren(node);
  return node;
}

/**
 * @fn PRTexprlist
 */
node_st *PRTexprlist(node_st *node)
{
  // Print the first expression in the list
  TRAVopt(EXPRLIST_EXPRESSION(node));

  // If there is another expression in the list, print a comma and traverse it
  if (EXPRLIST_NEXT(node) != NULL)
  {
    printf(", ");
    TRAVdo(EXPRLIST_NEXT(node));
  }
  return node;
}