/**
 * @file makeassemblyhelper.c
 * @brief Provides utility functions for generating assembly.
 *
 * Contains all functions to generate the assembly nodes.
 * @author Jip van Sommeren
 * @date 31-03-24
 */

#include "palm/hash_table.h"
#include "palm/memory.h"
#include "ccn/ccn.h"
#include "ccngen/ast.h"
#include "ccngen/trav.h"
#include "ccngen/enum.h"
#include "ccngen/trav_data.h"
#include "stdio.h"
#include "string.h"
#include "palm/dbug.h"
#include "vartypetostring.h"
#include "palm/ctinfo.h"
#include "varinitregassignment/vardefhelper.h"
#include "arrayparam/arrayhelpers.h"
#include "global/globals.h"
#include "makeassemblyhelper.h"
#include "symboltable/sahelper.h"

#define MAX_STRING_SIZE 64
static int statementCounter = 0;
static bool pop = false;
// static int fundefCounter = 0;

void appendAssemblyListAndUpdateTail(node_st **assemblyList, node_st **assemblyListTail, node_st *assemblyEntry)
{
    // Check if the list is empty
    if (*assemblyList == NULL)
    {
        // This is the first fundefession in the list.
        *assemblyList = ASTassemblylist(assemblyEntry, NULL);
        *assemblyListTail = *assemblyList; // The tail is the first node itself.
    }
    else
    {
        // There are already expressions in the list. Append the new expression.
        node_st *newAssemblyListNode = ASTassemblylist(assemblyEntry, NULL);
        ASSEMBLYLIST_NEXT(*assemblyListTail) = newAssemblyListNode; // Append new expression to the end.
        *assemblyListTail = newAssemblyListNode;                    // Update the tail to the new node.
    }
}

void appendConstantTable(node_st **constantTable, node_st **constantTableTail, node_st *constantTableEntry)
{
    if (*constantTable == NULL)
    {
        // This is the first fundefession in the list.
        *constantTable = ASTconstanttable(constantTableEntry, NULL);
        *constantTableTail = *constantTable; // The tail is the first node itself.
    }
    else
    {
        // There are already expressions in the list. Append the new expression.
        node_st *constantTableEntryNode = ASTconstanttable(constantTableEntry, NULL);
        CONSTANTTABLE_NEXT(*constantTableTail) = constantTableEntryNode; // Append new expression to the end.
        *constantTableTail = constantTableEntryNode;                     // Update the tail to the new node.
    }
}

node_st *returnTypeAssembly(node_st *expr)
{
    char *str = malloc(MAX_STRING_SIZE * sizeof(char)); // Allocate memory dynamically
    node_st *assembly = NULL;
    if (!str)
        return NULL; // Check for allocation failure

    switch (NODE_TYPE(expr))
    {
    case NT_NUM:
        if (NUM_VAL(expr) != -1 && NUM_VAL(expr) != 0 && NUM_VAL(expr) != 1)
        {
            sprintf(str, "%d", CONSTANTENTRY_INDEX(NUM_LINK(expr)));
            assembly = ASTassembly(strdup("    iloadc "), strdup(str));
        }
        else if (NUM_VAL(expr) == -1)
        {
            assembly = ASTassembly(strdup("    iloadc_"), strdup("m1"));
        }
        else if (NUM_VAL(expr) == 0)
        {
            assembly = ASTassembly(strdup("    iloadc_"), strdup("0"));
        }
        else
        { // NUM_VAL(expr) == 1
            assembly = ASTassembly(strdup("    iloadc_"), strdup("1"));
        }
        break;
    case NT_FLOAT:
        if (FLOAT_VAL(expr) != 1.0f && FLOAT_VAL(expr) != 0.0f)
        {
            sprintf(str, "%d", CONSTANTENTRY_INDEX(FLOAT_LINK(expr)));
            assembly = ASTassembly(strdup("    floadc "), strdup(str));
        }
        else if (FLOAT_VAL(expr) == 1.0f)
        {
            assembly = ASTassembly(strdup("    floadc_"), strdup("1"));
        }
        else
        { // FLOAT_VAL(expr) == 0.0f
            assembly = ASTassembly(strdup("    floadc_"), strdup("0"));
        }
        break;
    case NT_BOOL:
        if (BOOL_VAL(expr))
        {
            assembly = ASTassembly(strdup("    bloadc_"), strdup("t"));
        }
        else
        {
            assembly = ASTassembly(strdup("    bloadc_"), strdup("f"));
        }
        break;

    default:
        // Free allocated memory if default case is hit
        return NULL;
    }
    free(str);
    return assembly;
}

node_st *returnVarAssembly(node_st *expr)
{
    char *str = malloc(MAX_STRING_SIZE * sizeof(char)); // Allocate memory dynamically
    bool global = false;
    bool externB = false;
    int index = VAR_INDEX(expr);
    int scopedif = 0;
    if (VAR_SYMBOLENTRY(expr) != NULL)
    {
        externB = SYMBOLENTRY_EXTERNB(VAR_SYMBOLENTRY(expr));
        global = SYMBOLENTRY_GLOBAL(VAR_SYMBOLENTRY(expr));
        index = SYMBOLENTRY_INDEX(VAR_SYMBOLENTRY(expr));
        scopedif = VAR_CURRENTSCOPE(expr) - SYMBOLENTRY_SCOPELEVEL(VAR_SYMBOLENTRY(expr));
    }
    node_st *assembly = NULL;
    if (!str)
        return NULL; // Check for allocation failure

    switch (VAR_TYPE(expr))
    {
    case CT_int:
        if (externB)
        {
            sprintf(str, "%d", index);
            assembly = ASTassembly(strdup("    iloade "), strdup(str));
        }
        else if (global)
        {
            sprintf(str, "%d", index);
            assembly = ASTassembly(strdup("    iloadg "), strdup(str));
        }
        else if (scopedif > 0)
        {
            sprintf(str, "%d %d", scopedif, index);
            assembly = ASTassembly(strdup("    iloadn "), strdup(str));
        }
        else if (index < 4)
        {
            sprintf(str, "%d", index);
            assembly = ASTassembly(strdup("    iload_"), strdup(str));
        }
        else
        {
            sprintf(str, "%d", index);
            assembly = ASTassembly(strdup("    iload "), strdup(str));
        }
        break;
    case CT_float:
        if (externB)
        {
            sprintf(str, "%d", index);
            assembly = ASTassembly(strdup("    floade "), strdup(str));
        }
        else if (global)
        {
            sprintf(str, "%d", index);
            assembly = ASTassembly(strdup("    floadg "), strdup(str));
        }
        else if (scopedif > 0)
        {
            sprintf(str, "%d %d", scopedif, index);
            assembly = ASTassembly(strdup("    floadn "), strdup(str));
        }
        else if (index < 4)
        {
            sprintf(str, "%d", index);
            assembly = ASTassembly(strdup("    fload_"), strdup(str));
        }
        else
        {
            sprintf(str, "%d", index);
            assembly = ASTassembly(strdup("    fload "), strdup(str));
        }
        break;
    case CT_bool:

        if (externB)
        {
            sprintf(str, "%d", index);
            assembly = ASTassembly(strdup("    bloade "), strdup(str));
        }
        else if (global)
        {
            sprintf(str, "%d", index);
            assembly = ASTassembly(strdup("    bloadg "), strdup(str));
        }
        else if (scopedif > 0)
        {
            sprintf(str, "%d %d", scopedif, index);
            assembly = ASTassembly(strdup("    bloadn "), strdup(str));
        }
        else if (index < 4)
        {
            sprintf(str, "%d", index);
            assembly = ASTassembly(strdup("    bload_"), strdup(str));
        }
        else
        {
            sprintf(str, "%d", index);
            assembly = ASTassembly(strdup("    bload "), strdup(str));
        }
        break;

    default:
        // Free allocated memory if default case is hit
        return NULL;
    }
    free(str);
    return assembly;
}

void generateTernaryAssembly(node_st **head, node_st **tail, node_st *ternNode)
{
    if (!ternNode || NODE_TYPE(ternNode) != NT_TERN)
    {
        // Handle invalid input
        return;
    }

    char otherwiseLabel[MAX_STRING_SIZE], endLabel[MAX_STRING_SIZE];
    sprintf(otherwiseLabel, "_lab%d_otherwise", statementCounter);
    statementCounter++;
    sprintf(endLabel, "_lab%d_end", statementCounter);
    statementCounter++;

    // Generate assembly for the condition
    generateAssemblyForExpr(head, tail, TERN_COND(ternNode));

    // Generate the 'false' branch label
    node_st *branchFalse = ASTassembly(strdup("    branch_f "), strdup(otherwiseLabel));
    appendAssemblyListAndUpdateTail(head, tail, branchFalse);

    // Generate assembly for the 'then' expression
    generateAssemblyForExpr(head, tail, TERN_THEN_EXPR(ternNode));

    // Generate the jump to end
    node_st *jumpToEnd = ASTassembly(strdup("    jump "), strdup(endLabel));
    appendAssemblyListAndUpdateTail(head, tail, jumpToEnd);

    // Generate the 'otherwise' label
    node_st *otherwiseLabelNode = ASTassembly(strdup(otherwiseLabel), strdup(":"));
    appendAssemblyListAndUpdateTail(head, tail, otherwiseLabelNode);

    // Generate assembly for the 'else' expression
    generateAssemblyForExpr(head, tail, TERN_ELSE_EXPR(ternNode));

    // Generate the 'end' label
    node_st *endLabelNode = ASTassembly(strdup(endLabel), strdup(":"));
    appendAssemblyListAndUpdateTail(head, tail, endLabelNode);
}

node_st *returnVarletAssembly(node_st *expr)
{
    bool global = false;
    enum Type type = VARLET_TYPE(expr);
    int index = VARLET_INDEX(expr);
    int scopedif = 0;
    if (VARLET_SYMBOLENTRY(expr) != NULL)
    {
        type = SYMBOLENTRY_TYPE(VARLET_SYMBOLENTRY(expr));
        global = SYMBOLENTRY_GLOBAL(VARLET_SYMBOLENTRY(expr));
        index = SYMBOLENTRY_INDEX(VARLET_SYMBOLENTRY(expr));
        scopedif = VARLET_CURRENTSCOPE(expr) - SYMBOLENTRY_SCOPELEVEL(VARLET_SYMBOLENTRY(expr));
    }
    char *str = malloc(MAX_STRING_SIZE * sizeof(char)); // Allocate memory dynamically
    node_st *assembly = NULL;
    if (!str)
        return NULL; // Check for allocation failure
    switch (type)
    {
    case CT_int:
        if (global)
        {
            sprintf(str, "%d", index);
            assembly = ASTassembly(strdup("    istoreg "), strdup(str));
        }
        else if (scopedif > 0)
        {
            sprintf(str, "%d %d", scopedif, index);
            assembly = ASTassembly(strdup("    istoren "), strdup(str));
        }

        else
        {
            sprintf(str, "%d", index);
            assembly = ASTassembly(strdup("    istore "), strdup(str));
        }
        break;
    case CT_float:
        if (global)
        {
            sprintf(str, "%d", index);
            assembly = ASTassembly(strdup("    fstoreg "), strdup(str));
        }
        else if (scopedif > 0)
        {
            sprintf(str, "%d %d", scopedif, index);
            assembly = ASTassembly(strdup("    fstoren "), strdup(str));
        }

        else
        {
            sprintf(str, "%d", index);
            assembly = ASTassembly(strdup("    fstore "), strdup(str));
        }
        break;
    case CT_bool:
        if (global)
        {
            sprintf(str, "%d", index);
            assembly = ASTassembly(strdup("    bstoreg "), strdup(str));
        }
        else if (scopedif > 0)
        {
            sprintf(str, "%d %d", scopedif, index);
            assembly = ASTassembly(strdup("    bstoren "), strdup(str));
        }

        else
        {
            sprintf(str, "%d", index);
            assembly = ASTassembly(strdup("    bstore "), strdup(str));
        }
        break;

    default:
        // Free allocated memory if default case is hit
        return NULL;
    }
    free(str);
    return assembly;
}

char *getBinOpOpAssembly(enum BinOpType op, enum Type type)
{
    char *str = malloc(MAX_STRING_SIZE * sizeof(char)); // Allocate memory dynamically
    if (!str)
    {
        return NULL;
    }

    switch (op)
    {
    case BO_add:
        if (type == CT_int)
        {
            sprintf(str, "    iadd");
        }
        else if (type == CT_float)
        {
            sprintf(str, "    fadd");
        }
        else
        {
            sprintf(str, "    badd");
        }
        break;
    case BO_sub:
        printf("here\n");
        if (type == CT_int)
        {
            sprintf(str, "    isub");
        }
        else if (type == CT_float)
        {
            sprintf(str, "    fsub");
        }

        break;
    case BO_mul:
        if (type == CT_int)
        {
            sprintf(str, "    imul");
        }
        else if (type == CT_float)
        {
            sprintf(str, "    fmul");
        }
        else
        {
            sprintf(str, "    bmul");
        }
        break;
    case BO_div:
        if (type == CT_int)
        {
            sprintf(str, "    idiv");
        }
        else if (type == CT_float)
        {
            sprintf(str, "    fdiv");
        }
        else
        {
            fprintf(stderr, "Typeerrror\n");
        }
        break;
    case BO_mod:
        if (type == CT_int)
        {
            sprintf(str, "    irem");
        }
        else
        {
            // Modulo is not supported for floating point numbers in this context, handle error
            free(str);
            return NULL;
        }
        break;
    case BO_lt:
    case BO_le:
    case BO_gt:
    case BO_ge:
        if (type == CT_int)
        {
            sprintf(str, op == BO_lt   ? "    ilt"
                         : op == BO_le ? "    ile"
                         : op == BO_gt ? "    igt"
                                       : "    ige");
        }
        else if (type == CT_float)
        {
            sprintf(str, op == BO_lt   ? "    flt"
                         : op == BO_le ? "    fle"
                         : op == BO_gt ? "    fgt"
                                       : "    fge");
        }
        else
        {
            // Handle error: comparison for boolean not directly supported in this context
            free(str);
            return NULL;
        }
        break;

    case BO_eq:
    case BO_ne:
        // Comparison operations return boolean results but the instruction used depends on the operands' type
        if (type == CT_int)
        {
            sprintf(str, op == BO_eq ? "    ieq" : "    ine");
        }
        else if (type == CT_float)
        {
            sprintf(str, op == BO_eq ? "    feq" : "    fne");
        }
        else if (type == CT_bool)
        {
            sprintf(str, op == BO_eq ? "    beq" : "    bne");
        }
        else
        {
            // Handle error: comparison for boolean not directly supported in this context
            free(str);
            return NULL;
        }
        break;
    case BO_and:
    case BO_or:
        // Logical operations are only valid for booleans
        if (type == CT_bool)
        {
            sprintf(str, op == BO_and ? "    bmul" : "    badd");
        }
        else
        {
            // Handle error: logical operations for non-boolean not supported
            free(str);
            return NULL;
        }
        break;
    default:
        // Unrecognized binary operation, handle error
        free(str);
        return NULL;
    }

    return str;
}
node_st *leaveSubRoutine(enum Type type)
{
    node_st *leaveRoutine = NULL;

    switch (type)
    {
    case CT_int:
        leaveRoutine = ASTassembly(strdup("    ipop"), strdup(""));
        break;
    case CT_float:
        leaveRoutine = ASTassembly(strdup("    fpop"), strdup(""));
        break;
    case CT_bool:
        leaveRoutine = ASTassembly(strdup("    bpop"), strdup(""));
        break;

    default:
        break;
    }
    return leaveRoutine;
}

node_st *enterRoutine(int scope)
{
    node_st *enterRoutine = NULL;

    if (scope == 0)
    {
        enterRoutine = ASTassembly(strdup("    isrl"), strdup(""));
    }
    else
    {
        enterRoutine = ASTassembly(strdup("    isrg"), strdup(""));
    }
    return enterRoutine;
}

void genAssemblyForFuncallArgs(node_st **head, node_st **tail, node_st *exprs)
{
    while (exprs != NULL)
    {

        node_st *expr = EXPRS_EXPR(exprs);
        generateAssemblyForExpr(head, tail, expr);
        exprs = EXPRS_NEXT(exprs);
    }
}

void generateFuncallAssembly(node_st **head, node_st **tail, node_st *expr)
{
    node_st *symbolentry = FUNCALL_SYMBOLENTRY(expr);
    char *str = malloc(MAX_STRING_SIZE * sizeof(char));
    char *str2 = malloc(MAX_STRING_SIZE * sizeof(char));
    node_st *enterRoutine = NULL;
    node_st *funcallAsm = NULL;
    node_st *exprs = FUNCALL_FUN_ARGS(expr);

    if (SYMBOLENTRY_SCOPELEVEL(symbolentry) == 0 && SYMBOLENTRY_INDEX(symbolentry) > -1 && !SYMBOLENTRY_EXTERNB(symbolentry))
    {
        enterRoutine = ASTassembly(strdup("    isrg"), strdup(""));
        sprintf(str, "%d _fun%d%s", FUNCALL_INPUTCOUNT(expr), SYMBOLENTRY_INDEX(symbolentry), SYMBOLENTRY_SCOPENAME(symbolentry));
        funcallAsm = ASTassembly(strdup("    jsr "), strdup(str));
    }
    else if (SYMBOLENTRY_INDEX(symbolentry) == -1 && !SYMBOLENTRY_EXTERNB(symbolentry))
    {
        enterRoutine = ASTassembly(strdup("    isrg"), strdup(""));
        sprintf(str, "%d %s", FUNCALL_INPUTCOUNT(expr), SYMBOLENTRY_SCOPENAME(symbolentry));
        funcallAsm = ASTassembly(strdup("    jsr "), strdup(str));
    }
    else if (SYMBOLENTRY_EXTERNB(symbolentry))
    {
        enterRoutine = ASTassembly(strdup("    isrg"), strdup(""));
        sprintf(str, "%d", SYMBOLENTRY_INDEX(symbolentry));
        funcallAsm = ASTassembly(strdup("    jsre "), strdup(str));
    }
    else if (SYMBOLENTRY_SCOPELEVEL(symbolentry) > 0 && (FUNCALL_CURRENTSCOPE(expr) - SYMBOLENTRY_SCOPELEVEL(symbolentry) == 1))
    {
        enterRoutine = ASTassembly(strdup("    isr"), strdup(""));
        sprintf(str, "%d _fun%d%s", FUNCALL_INPUTCOUNT(expr), SYMBOLENTRY_INDEX(symbolentry), SYMBOLENTRY_SCOPENAME(symbolentry));

        funcallAsm = ASTassembly(strdup("    jsr "), strdup(str));
    }
    else if (SYMBOLENTRY_SCOPELEVEL(symbolentry) > 0 && (FUNCALL_CURRENTSCOPE(expr) - SYMBOLENTRY_SCOPELEVEL(symbolentry) > 1))
    {
        int dif = FUNCALL_CURRENTSCOPE(expr) - SYMBOLENTRY_SCOPELEVEL(symbolentry) - 1;

        sprintf(str2, "    isrn %d", dif);
        enterRoutine = ASTassembly(strdup(str2), strdup(""));
        sprintf(str, "%d _fun%d%s", FUNCALL_INPUTCOUNT(expr), SYMBOLENTRY_INDEX(symbolentry), SYMBOLENTRY_SCOPENAME(symbolentry));
        funcallAsm = ASTassembly(strdup("    jsr "), strdup(str));
    }
    else
    {
        enterRoutine = ASTassembly(strdup("    isrl"), strdup(""));
        sprintf(str, "%d _fun%d%s", FUNCALL_INPUTCOUNT(expr), SYMBOLENTRY_INDEX(symbolentry), SYMBOLENTRY_SCOPENAME(symbolentry));

        funcallAsm = ASTassembly(strdup("    jsr "), strdup(str));
    }

    free(str);
    free(str2);

    node_st *leaveRoutine = NULL;
    if (pop)
    {
        leaveRoutine = leaveSubRoutine(SYMBOLENTRY_TYPE(symbolentry));
        pop = false;
    }
    appendAssemblyListAndUpdateTail(head, tail, enterRoutine);
    genAssemblyForFuncallArgs(head, tail, exprs);
    appendAssemblyListAndUpdateTail(head, tail, funcallAsm);
    if (leaveRoutine != NULL)
    {
        appendAssemblyListAndUpdateTail(head, tail, leaveRoutine);
    }
}

void generateCastAssembly(node_st **head, node_st **tail, node_st *expr)
{
    generateAssemblyForExpr(head, tail, CAST_EXPR(expr));
    enum Type typeExpr = getType(CAST_EXPR(expr));
    enum Type typeCast = CAST_TYPE(expr);

    node_st *cast = NULL;
    if (typeCast == typeExpr)
    {
        return;
    }
    if (typeExpr != CT_int && typeExpr != CT_float)
    {
        // The type is neither int nor float
        return;
    }
    if (typeCast == CT_int)
    {
        cast = ASTassembly(strdup("    f2i"), strdup(""));
    }
    else
    {
        cast = ASTassembly(strdup("    i2f"), strdup(""));
    }
    appendAssemblyListAndUpdateTail(head, tail, cast);
}

char *getMonOpOpAssembly(enum MonOpType op, enum Type type)
{
    char *str = malloc(20 * sizeof(char)); // Allocate memory dynamically
    if (!str)
    {
        return NULL;
    }

    switch (op)
    {
    case MO_neg:
        if (type == CT_int || CT_float)
        {
            sprintf(str, type == CT_int ? "    ineg" : "    fneg");
        }
        else
        {
            fprintf(stderr, "invalid monoptype");
        }

        break;
    case MO_not:
        if (type == CT_bool)
        {
            sprintf(str, "    bnot");
        }
        break;

    default:
        // Unrecognized binary operation, handle error
        free(str);
        return NULL;
    }

    return str;
}

void generateAssemblyForExpr(node_st **head, node_st **tail, node_st *expr)
{
    if (!expr)
        return;
    switch (NODE_TYPE(expr))
    {
    case NT_MONOP:
    {
        char *monopAssembly = getMonOpOpAssembly(MONOP_OP(expr), MONOP_TYPE(expr));
        generateAssemblyForExpr(head, tail, MONOP_OPERAND(expr));
        node_st *monopNode = ASTassembly(strdup(monopAssembly), strdup(""));
        free(monopAssembly);
        appendAssemblyListAndUpdateTail(head, tail, monopNode);
        break;
    }
    case NT_BINOP:
    {
        generateAssemblyForExpr(head, tail, BINOP_LEFT(expr));
        generateAssemblyForExpr(head, tail, BINOP_RIGHT(expr));
        // Combine left and right lists
        char *binopAssembly = getBinOpOpAssembly(BINOP_OP(expr), getType(BINOP_LEFT(expr)));
        node_st *binopNode = ASTassembly(strdup(binopAssembly), strdup(""));
        free(binopAssembly);
        appendAssemblyListAndUpdateTail(head, tail, binopNode);

        break;
    }
    case NT_NUM:
    case NT_FLOAT:
    case NT_BOOL:
    {

        node_st *loadNode = returnTypeAssembly(expr);
        appendAssemblyListAndUpdateTail(head, tail, loadNode);
        break;
    }
    case NT_VAR:
    {
        node_st *loadNode = returnVarAssembly(expr);
        appendAssemblyListAndUpdateTail(head, tail, loadNode);
        break;
    }
    case NT_TERN:
    {
        generateTernaryAssembly(head, tail, expr);
        break;
    }
    case NT_CAST:
        generateCastAssembly(head, tail, expr);
        break;
    case NT_FUNCALL:
        generateFuncallAssembly(head, tail, expr);
        break;
    default:
        // Handle other types as needed
        break;
    }

    return;
}

void generateAssemblyForIfElse(node_st **head, node_st **tail, node_st *ifElseStmt);
void generateAssemblyForWhile(node_st **head, node_st **tail, node_st *whileStmt);
void generateAssemblyForDoWhile(node_st **head, node_st **tail, node_st *doWhileStmt);
void generateAssemblyForAssign(node_st **head, node_st **tail, node_st *assignStmt);
void generateAssemblyForReturn(node_st **head, node_st **tail, node_st *returnStmt);
void generateAssemblyForExprStmt(node_st **head, node_st **tail, node_st *exprStmt);

void generateAssemblyForStmt(node_st **head, node_st **tail, node_st *node)
{
    if (!node)
        return;

    switch (NODE_TYPE(node))
    {
    case NT_IFELSE:

        generateAssemblyForIfElse(head, tail, node);
        break;
    case NT_WHILE:

        generateAssemblyForWhile(head, tail, node);
        break;
    case NT_DOWHILE:

        generateAssemblyForDoWhile(head, tail, node);
        break;
    case NT_FOR:

        break;
    case NT_ASSIGN:
    {
        generateAssemblyForAssign(head, tail, node);
        break;
    }
    case NT_EXPRSTMT:
    {
        generateAssemblyForExprStmt(head, tail, node);
        break;
    }
    case NT_RETURN:
    {
        generateAssemblyForReturn(head, tail, node);
        break;
    }
    default:
        // Handle other statement types or fallback case
        break;
    }
}
void generateAssemblyForWhile(node_st **head, node_st **tail, node_st *whileStmt)
{
    // Generate unique labels for the loop start and end
    // Ensure unique labels for each loop
    char labelStart[MAX_STRING_SIZE];
    char labelEnd[MAX_STRING_SIZE];
    snprintf(labelStart, sizeof(labelStart), "_lab%d_while", statementCounter);
    statementCounter++;
    snprintf(labelEnd, sizeof(labelEnd), "_lab%d_end", statementCounter);
    statementCounter++;
    // Label for loop start
    appendAssemblyListAndUpdateTail(head, tail, ASTassembly(strdup(labelStart), strdup(":")));

    // Generate assembly for the condition and check it
    if (WHILE_COND(whileStmt))
    {
        generateAssemblyForExpr(head, tail, WHILE_COND(whileStmt));
        appendAssemblyListAndUpdateTail(head, tail, ASTassembly(strdup("    branch_f "), strdup(labelEnd)));
    }

    // Generate assembly for the loop's body
    node_st *innerStmts = WHILE_BLOCK(whileStmt);
    while (innerStmts != NULL)
    {
        generateAssemblyForStmt(head, tail, STMTS_STMT(innerStmts));
        innerStmts = STMTS_NEXT(innerStmts);
    }

    // Jump back to the start to re-evaluate the condition
    appendAssemblyListAndUpdateTail(head, tail, ASTassembly(strdup("    jump "), strdup(labelStart)));

    // Label for loop end
    appendAssemblyListAndUpdateTail(head, tail, ASTassembly(strdup(labelEnd), strdup(":")));
}

void generateAssemblyForDoWhile(node_st **head, node_st **tail, node_st *dowhileStmt)
{
    // Generate unique labels for the loop start and end
    // Ensure unique labels for each loop
    char labelStart[MAX_STRING_SIZE];
    char labelEnd[MAX_STRING_SIZE];
    snprintf(labelStart, sizeof(labelStart), "_lab%d_while", statementCounter);
    statementCounter++;
    snprintf(labelEnd, sizeof(labelEnd), "_lab%d_end", statementCounter);

    // Label for loop start
    appendAssemblyListAndUpdateTail(head, tail, ASTassembly(strdup(labelStart), strdup(":")));

    // Generate assembly for the condition and check it
    if (DOWHILE_COND(dowhileStmt))
    {
        generateAssemblyForExpr(head, tail, DOWHILE_COND(dowhileStmt));
        appendAssemblyListAndUpdateTail(head, tail, ASTassembly(strdup("    branch_f "), strdup(labelEnd)));
    }

    // Generate assembly for the loop's body
    node_st *innerStmts = DOWHILE_BLOCK(dowhileStmt);
    while (innerStmts != NULL)
    {
        generateAssemblyForStmt(head, tail, STMTS_STMT(innerStmts));
        innerStmts = STMTS_NEXT(innerStmts);
    }

    // Jump back to the start to re-evaluate the condition
    appendAssemblyListAndUpdateTail(head, tail, ASTassembly(strdup("    jump "), strdup(labelStart)));

    // Label for loop end
    appendAssemblyListAndUpdateTail(head, tail, ASTassembly(strdup(labelEnd), strdup(":\n")));
}

void generateAssemblyForReturn(node_st **head, node_st **tail, node_st *returnStmt)
{
    if (RETURN_EXPR(returnStmt) != NULL)
    {
        generateAssemblyForExpr(head, tail, RETURN_EXPR(returnStmt));
    }
    char *str = malloc(MAX_STRING_SIZE * sizeof(char));
    switch (RETURN_TYPE(returnStmt))
    {
    case CT_int:
        sprintf(str, "    ireturn");
        break;
    case CT_bool:
        sprintf(str, "    breturn");
        break;
    case CT_float:
        sprintf(str, "    freturn");
        break;
    case CT_void:
        sprintf(str, "    return");
        break;

    default:
        break;
    }

    appendAssemblyListAndUpdateTail(head, tail, ASTassembly(strdup(str), strdup("")));
    free(str);
}

node_st *returnStepAssembly(enum BinOpType type, int step, int index)
{
    char *str = malloc(MAX_STRING_SIZE * sizeof(char));
    node_st *assembly = NULL;

    switch (type)
    {
    case BO_sub:

        if (step == 1)
        {
            sprintf(str, "%d %d", step, index);
            assembly = ASTassembly(strdup("    idec_"), strdup(str));
            free(str);
        }
        else
        {
            sprintf(str, "%d %d", step, index);
            assembly = ASTassembly(strdup("    idec "), strdup(str));
            free(str);
        }
        break;
    case BO_add:
        if (step == 1)
        {
            sprintf(str, "%d %d", step, index);
            assembly = ASTassembly(strdup("    iinc_"), strdup(str));
            free(str);
        }
        else
        {
            sprintf(str, "%d %d", step, index);
            assembly = ASTassembly(strdup("    iinc "), strdup(str));
            free(str);
        }
        break;

    default:
        free(str);
        break;
    }
    return assembly;
}

void generateAssemblyForAssign(node_st **head, node_st **tail, node_st *assignStmt)
{
    if (ASSIGN_UPDATE(assignStmt))
    {
        int stepIndex = 1;
        if (NUM_VAL(BINOP_RIGHT(ASSIGN_EXPR(assignStmt))) == 1)
        {
            stepIndex = 1;
        }
        else if (NUM_LINK(BINOP_RIGHT(ASSIGN_EXPR(assignStmt))) != NULL)
        {
            stepIndex = CONSTANTENTRY_INDEX(NUM_LINK(BINOP_RIGHT(ASSIGN_EXPR(assignStmt))));
        }
        node_st *assembly = returnStepAssembly(BINOP_OP(ASSIGN_EXPR(assignStmt)), stepIndex, VARLET_INDEX(ASSIGN_LET(assignStmt)));
        appendAssemblyListAndUpdateTail(head, tail, assembly);
    }
    else
    {

        node_st *varletAsm = returnVarletAssembly(ASSIGN_LET(assignStmt));

        generateAssemblyForExpr(head, tail, ASSIGN_EXPR(assignStmt));
        // appendAssemblyListAndUpdateTail(head, tail, CCNcopy(exprAsm));
        appendAssemblyListAndUpdateTail(head, tail, varletAsm);
    }

    // insertAssembly(&ASSIGN_ASSEMBLY(assignStmt), CCNcopy(exprAsm), varletAsm);
}

void generateAssemblyForExprStmt(node_st **head, node_st **tail, node_st *exprStmt)
{
    pop = true;
    generateAssemblyForExpr(head, tail, EXPRSTMT_EXPR(exprStmt));
}

void generateAssemblyForIfElse(node_st **head, node_st **tail, node_st *ifElseStmt)
{
    // Generate unique labels for the then and else parts, and the end of the if-else block

    char labelElse[MAX_STRING_SIZE];
    char labelEnd[MAX_STRING_SIZE];
    snprintf(labelElse, sizeof(labelElse), "_lab%d_else", statementCounter);
    statementCounter++;
    snprintf(labelEnd, sizeof(labelEnd), "_lab%d_end", statementCounter);

    // Generate assembly for the if condition
    if (IFELSE_COND(ifElseStmt))
    {
        generateAssemblyForExpr(head, tail, IFELSE_COND(ifElseStmt));
        if (IFELSE_ELSE_BLOCK(ifElseStmt))
        {
            appendAssemblyListAndUpdateTail(head, tail, ASTassembly(strdup("    branch_f "), strdup(labelElse)));
        }
        else
        {
            appendAssemblyListAndUpdateTail(head, tail, ASTassembly(strdup("    branch_f "), strdup(labelEnd)));
        }
    }

    // Generate assembly for the then-block
    node_st *innerStmts = IFELSE_THEN(ifElseStmt);
    while (innerStmts != NULL)
    {
        generateAssemblyForStmt(head, tail, STMTS_STMT(innerStmts));
        innerStmts = STMTS_NEXT(innerStmts);
    }

    // Label for else part

    // Generate assembly for the else-block, if it exists
    if (IFELSE_ELSE_BLOCK(ifElseStmt))
    {
        appendAssemblyListAndUpdateTail(head, tail, ASTassembly(strdup("    jump "), strdup(labelEnd)));
        appendAssemblyListAndUpdateTail(head, tail, ASTassembly(strdup(labelElse), strdup(":")));
        node_st *innerStmts = IFELSE_ELSE_BLOCK(ifElseStmt);
        while (innerStmts != NULL)
        {
            generateAssemblyForStmt(head, tail, STMTS_STMT(innerStmts));
            innerStmts = STMTS_NEXT(innerStmts);
        }
    }

    // Label for end of if-else
    appendAssemblyListAndUpdateTail(head, tail, ASTassembly(strdup(labelEnd), strdup(":")));
}

void insertAssembly(node_st **head, node_st *exprAssembly, node_st *varletAsm)
{
    if (!head || !exprAssembly || !varletAsm)
    {
        // Handle null pointers gracefully
        return;
    }

    if (*head == NULL)
    {
        // If the current list is empty, simply point the head to exprAssembly
        *head = exprAssembly;
    }
    else
    {
        // Find the last node of the current list
        node_st *current = *head;
        while (ASSEMBLYLIST_NEXT(current) != NULL)
        {
            current = ASSEMBLYLIST_NEXT(current); // Move to the next node
        }
        // Attach exprAssembly list to the end of the current list
        ASSEMBLYLIST_NEXT(current) = exprAssembly;
    }

    // Now, find the last node of the modified list (which includes exprAssembly)
    node_st *last = *head;
    while (ASSEMBLYLIST_NEXT(last) != NULL)
    {
        last = ASSEMBLYLIST_NEXT(last); // Move to the next node
    }

    // Attach varletAsm to the very end of the list
    ASSEMBLYLIST_NEXT(last) = ASTassemblylist(varletAsm, NULL);
}
node_st *returnExprAssembly(node_st *expr)
{
    node_st *exprAssembly = NULL;
    switch (NODE_TYPE(expr))
    {
    case NT_NUM:
        exprAssembly = NUM_ASSEMBLY(expr);
        break;
    case NT_FLOAT:
        exprAssembly = FLOAT_ASSEMBLY(expr);
        break;
    case NT_BOOL:
        exprAssembly = BOOL_ASSEMBLY(expr);
        break;
    case NT_BINOP:
        exprAssembly = BINOP_ASSEMBLY(expr);
        break;
    case NT_MONOP:
        exprAssembly = MONOP_ASSEMBLY(expr);
        break;
    case NT_TERN:
        exprAssembly = TERN_ASSEMBLY(expr);
        break;
    case NT_VAR:
        exprAssembly = VAR_ASSEMBLY(expr);
        break;
    case NT_CAST:
        exprAssembly = CAST_ASSEMBLY(expr);
        break;
    default:
        break;
    }
    return exprAssembly;
}

int countVarDecls(node_st *vardecls)
{
    int count = 0;
    while (vardecls != NULL)
    {
        count++;
        vardecls = VARDECLS_NEXT(vardecls);
    }
    return count;
}

void FundefDefAssembly(node_st **head, node_st **tail, node_st *fundef)
{

    node_st *assembly = NULL;
    char *str = malloc(MAX_STRING_SIZE * sizeof(char));
    if (FUNDEF_INDEX(fundef) > -1)
    {

        sprintf(str, "_fun%d%s:", FUNDEF_INDEX(fundef), FUNDEF_SCOPENAME(fundef));
        assembly = ASTassembly(strdup(str), strdup(""));
    }
    else if (FUNDEF_INDEX(fundef) == -1)
    {

        sprintf(str, "%s:", FUNDEF_NAME(fundef));
        assembly = ASTassembly(strdup(str), strdup(""));
    }

    int vardeclcount = countVarDecls(FUNBODY_VARDECLS(FUNDEF_BODY(fundef)));
    char *countStr = malloc(5 * sizeof(char));
    sprintf(countStr, "%d", vardeclcount);
    appendAssemblyListAndUpdateTail(head, tail, assembly);

    if (vardeclcount > 0)
    {
        node_st *esrAssembly = ASTassembly(strdup("    esr "), strdup(countStr));
        appendAssemblyListAndUpdateTail(head, tail, esrAssembly);
    }

    free(str);
    free(countStr);
}

void setToOutputFile(node_st *node)
{
    while (node != NULL)
    {
        node_st *assemblyEntry = ASSEMBLYLIST_ASSEMBLY(node);
        printf("%s", ASSEMBLY_INSTRUCTION(assemblyEntry));
        printf("%s\n", ASSEMBLY_OPERANDS(assemblyEntry));
        fprintf(global.outputFile, "%s%s\n", ASSEMBLY_INSTRUCTION(assemblyEntry), ASSEMBLY_OPERANDS(assemblyEntry));
        node = ASSEMBLYLIST_NEXT(node);
    }
}

void generateAssemblyForFundef(node_st **head, node_st **tail, node_st *node)
{
    // statementCounter = 0;
    FundefDefAssembly(head, tail, node);
    node_st *stmts = FUNBODY_STMTS(FUNDEF_BODY(node));
    while (stmts != NULL)
    {
        node_st *stmt = STMTS_STMT(stmts);
        generateAssemblyForStmt(head, tail, stmt);
        stmts = STMTS_NEXT(stmts);
    }
    if (FUNDEF_TYPE(node) == CT_void)
    {
        appendAssemblyListAndUpdateTail(head, tail, ASTassembly(strdup("    return"), strdup("")));
    }
    appendAssemblyListAndUpdateTail(head, tail, ASTassembly(strdup("    "), strdup("")));
}