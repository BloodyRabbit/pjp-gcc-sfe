/** @file
 * @brief Declaration of AST statement nodes.
 *
 * @author Jan Bobek
 * @since 5th June 2014
 */

#ifndef SFE__AST__AST_STMT_NODE_HPP__INCL__
#define SFE__AST__AST_STMT_NODE_HPP__INCL__

#include <string>
#include <vector>

#include "ast/AstNode.hpp"

class AstExprNode;
class AstFunExprNode;
class AstLocDeclNode;

/**
 * @brief A generic statement node.
 *
 * @author Jan Bobek
 */
class AstStmtNode
: public AstNode
{
};

/**
 * @brief A binop statement node.
 *
 * @author Jan Bobek
 */
class AstBinopStmtNode
: public AstStmtNode
{
public:
    /**
     * @brief Initializes the node.
     *
     * @param[in] tok
     *   A lex token of the binop.
     * @param[in] lval
     *   L-value expression.
     * @param[in] rval
     *   R-value expression.
     */
    AstBinopStmtNode(
        LexToken tok,
        AstExprNode* lval,
        AstExprNode* rval
        );
    /**
     * @brief Frees L-value and R-value.
     */
    ~AstBinopStmtNode();

    /**
     * @brief Prints the node to a file.
     *
     * @param[in] off
     *   Line offset to use.
     * @param[in] fp
     *   File to print to.
     */
    void print(
        unsigned int off,
        FILE* fp
        ) const;
    /**
     * @brief Translates into appropriate tree.
     *
     * @param[out] t
     *   Where to store the translated tree.
     * @param[in,out] ctx
     *   Context in which the node appears.
     * @param[in,out] symTable
     *   A symbol table.
     *
     * @retval true
     *   Translation successful.
     * @retval false
     *   Translation failed.
     */
    bool translate(
        tree& t,
        tree ctx,
        SymTable& symTable
        ) const;

protected:
    /// Lex token of the binop.
    LexToken mToken;
    /// L-value expression.
    AstExprNode* mLval;
    /// R-value expression.
    AstExprNode* mRval;
};

/**
 * @brief A function call statement node.
 *
 * @author Jan Bobek
 */
class AstFunStmtNode
: public AstStmtNode
{
public:
    /**
     * @brief Initializes the node.
     *
     * @param[in] fun
     *   The funcall expression to wrap.
     */
    AstFunStmtNode( AstFunExprNode* fun );
    /**
     * @brief Frees the wrapped funcall expression.
     */
    ~AstFunStmtNode();

    /**
     * @brief Prints the node to a file.
     *
     * @param[in] off
     *   Line offset to use.
     * @param[in] fp
     *   File to print to.
     */
    void print(
        unsigned int off,
        FILE* fp
        ) const;
    /**
     * @brief Translates into appropriate tree.
     *
     * @param[out] t
     *   Where to store the translated tree.
     * @param[in,out] ctx
     *   Context in which the node appears.
     * @param[in,out] symTable
     *   A symbol table.
     *
     * @retval true
     *   Translation successful.
     * @retval false
     *   Translation failed.
     */
    bool translate(
        tree& t,
        tree ctx,
        SymTable& symTable
        ) const;

protected:
    /// Wrapped funcall expression.
    AstFunExprNode* mFun;
};

/**
 * @brief A block statement node.
 *
 * @author Jan Bobek
 */
class AstBlkStmtNode
: public AstStmtNode
{
public:
    /**
     * @brief Initializes the node.
     */
    AstBlkStmtNode();
    /**
     * @brief Frees the contained declarations
     *   and statements.
     */
    ~AstBlkStmtNode();

    /**
     * @brief Adds a local declaration to the block.
     *
     * @param[in] decl
     *   The declaration to append.
     */
    void addDecl( AstLocDeclNode* decl );
    /**
     * @brief Adds a statement to the block.
     *
     * @param[in] stmt
     *   The statement to append.
     */
    void addStmt( AstStmtNode* stmt );

    /**
     * @brief Prints the node to a file.
     *
     * @param[in] off
     *   Line offset to use.
     * @param[in] fp
     *   File to print to.
     */
    void print(
        unsigned int off,
        FILE* fp
        ) const;
    /**
     * @brief Translates ito appropriate tree.
     *
     * @param[out] t
     *   Where to store the translated tree.
     * @param[in,out] ctx
     *   Context in which the node appears.
     * @param[in,out] symTable
     *   A symbol table.
     *
     * @retval true
     *   Translation successful.
     * @retval false
     *   Translation failed.
     */
    bool translate(
        tree& t,
        tree ctx,
        SymTable& symTable
        ) const;

protected:
    /// The declarations within.
    std::vector< AstLocDeclNode* > mDecls;
    /// The statements within.
    std::vector< AstStmtNode* > mStmts;
};

/**
 * @brief A conditional statement node.
 *
 * @author Jan Bobek
 */
class AstIfStmtNode
: public AstStmtNode
{
public:
    /**
     * @brief Initializes the node.
     *
     * @param[in] cond
     *   Conditional expression.
     * @param[in] thenBlk
     *   Conditional block.
     * @param[in] elseBlk
     *   Optional else block.
     */
    AstIfStmtNode(
        AstExprNode* cond,
        AstBlkStmtNode* thenBlk,
        AstBlkStmtNode* elseBlk
        );
    /**
     * @brief Frees the condition and blocks.
     */
    ~AstIfStmtNode();

    /**
     * @brief Prints the node to a file.
     *
     * @param[in] off
     *   Line offset to use.
     * @param[in] fp
     *   File to print to.
     */
    void print(
        unsigned int off,
        FILE* fp
        ) const;
    /**
     * @brief Translates into appropriate tree.
     *
     * @param[out] t
     *   Where to store the translated tree.
     * @param[in,out] ctx
     *   Context in which the node appears.
     * @param[in,out] symTable
     *   A symbol table.
     *
     * @retval true
     *   Translation successful.
     * @retval false
     *   Translation failed.
     */
    bool translate(
        tree& t,
        tree ctx,
        SymTable& symTable
        ) const;

protected:
    /// Conditional expression.
    AstExprNode* mCond;
    /// Conditional block.
    AstBlkStmtNode* mThenBlk;
    /// Optional else block.
    AstBlkStmtNode* mElseBlk;
};

/**
 * @brief A loop statement node.
 *
 * @author Jan Bobek
 */
class AstLoopStmtNode
: public AstStmtNode
{
public:
    /**
     * @brief Initializes the loop.
     *
     * @param[in] bodyBlk
     *   Body of the loop.
     */
    AstLoopStmtNode( AstBlkStmtNode* body );
    /**
     * @brief Frees the body.
     */
    ~AstLoopStmtNode();

    /**
     * @brief Prints the node to a file.
     *
     * @param[in] off
     *   Line offset to use.
     * @param[in] fp
     *   File to print to.
     */
    void print(
        unsigned int off,
        FILE* fp
        ) const;
    /**
     * @brief Translates into appropriate tree.
     *
     * @param[out] t
     *   Where to store the translated tree.
     * @param[in,out] ctx
     *   Context in which the node appears.
     * @param[in,out] symTable
     *   A symbol table.
     *
     * @retval true
     *   Translation successful.
     * @retval false
     *   Translation failed.
     */
    bool translate(
        tree& t,
        tree ctx,
        SymTable& symTable
        ) const;

protected:
    /// The loop body.
    AstBlkStmtNode* mBody;
};

/**
 * @brief A break statement node.
 *
 * @author Jan Bobek
 */
class AstBreakStmtNode
: public AstStmtNode
{
public:
    /**
     * @brief Initializes the node.
     *
     * @param[in] cond
     *   The breaking condition.
     */
    AstBreakStmtNode( AstExprNode* cond );
    /**
     * @brief Frees the condition expr.
     */
    ~AstBreakStmtNode();

    /**
     * @brief Prints the node to a file.
     *
     * @param[in] off
     *   Line offset to use.
     * @param[in] fp
     *   File to print to.
     */
    void print(
        unsigned int off,
        FILE* fp
        ) const;
    /**
     * @brief Translates into appropriate tree.
     *
     * @param[out] t
     *   Where to store the translated tree.
     * @param[in,out] ctx
     *   Context in which the node appears.
     * @param[in,out] symTable
     *   A symbol table.
     *
     * @retval true
     *   Translation successful.
     * @retval false
     *   Translation failed.
     */
    bool translate(
        tree& t,
        tree ctx,
        SymTable& symTable
        ) const;

protected:
    /// The break condition.
    AstExprNode* mCond;
};

/**
 * @brief An exit statement node.
 *
 * @author Jan Bobek
 */
class AstExitStmtNode
: public AstStmtNode
{
public:
    /**
     * @brief Prints the node to a file.
     *
     * @param[in] off
     *   Line offset to use.
     * @param[in] fp
     *   File to print to.
     */
    void print(
        unsigned int off,
        FILE* fp
        ) const;
    /**
     * @brief Translates into appropriate tree.
     *
     * @param[out] t
     *   Where to store the translated tree.
     * @param[in,out] ctx
     *   Context in which the node appears.
     * @param[in,out] symTable
     *   A symbol table.
     *
     * @retval true
     *   Translation successful.
     * @retval false
     *   Translation failed.
     */
    bool translate(
        tree& t,
        tree ctx,
        SymTable& symTable
        ) const;
};

/**
 * @brief A read node.
 *
 * @author Jan Bobek
 */
class AstReadStmtNode
: public AstStmtNode
{
public:
    /**
     * @brief Initializes the node.
     *
     * @param[in] fmt
     *   A format string.
     * @param[in] expr
     *   Where to store the read number.
     */
    AstReadStmtNode(
        const char* fmt,
        AstExprNode* expr
        );
    /**
     * @brief Deletes the format and expr.
     */
    ~AstReadStmtNode();

    /**
     * @brief Prints the node to a file.
     *
     * @param[in] off
     *   Line offset to use.
     * @param[in] fp
     *   File to print to.
     */
    void print(
        unsigned int off,
        FILE* fp
        ) const;
    /**
     * @brief Translates into appropriate tree.
     *
     * @param[out] t
     *   Where to store the translated tree.
     * @param[in,out] ctx
     *   Context in which the node appears.
     * @param[in,out] symTable
     *   A symbol table.
     *
     * @retval true
     *   Translation successful.
     * @retval false
     *   Translation failed.
     */
    bool translate(
        tree& t,
        tree ctx,
        SymTable& symTable
        ) const;

protected:
    /// The format string.
    AstStrExprNode* mFmt;
    /// The expression to print.
    AstExprNode* mExpr;
};

/**
 * @brief A write node.
 *
 * @author Jan Bobek
 */
class AstWriteStmtNode
: public AstStmtNode
{
public:
    /**
     * @brief Initializes the node.
     *
     * @param[in] fmt
     *   A format string.
     * @param[in] expr
     *   The expression to print.
     */
    AstWriteStmtNode(
        const char* fmt,
        AstExprNode* expr
        );
    /**
     * @brief Deletes the format and expr.
     */
    ~AstWriteStmtNode();

    /**
     * @brief Prints the node to a file.
     *
     * @param[in] off
     *   Line offset to use.
     * @param[in] fp
     *   File to print to.
     */
    void print(
        unsigned int off,
        FILE* fp
        ) const;
    /**
     * @brief Translates into appropriate tree.
     *
     * @param[out] t
     *   Where to store the translated tree.
     * @param[in,out] ctx
     *   Context in which the node appears.
     * @param[in,out] symTable
     *   A symbol table.
     *
     * @retval true
     *   Translation successful.
     * @retval false
     *   Translation failed.
     */
    bool translate(
        tree& t,
        tree ctx,
        SymTable& symTable
        ) const;

protected:
    /// The format string.
    AstStrExprNode* mFmt;
    /// The expression to print.
    AstExprNode* mExpr;
};

#endif /* !SFE__AST__AST_STMT_NODE_HPP__INCL__ */
