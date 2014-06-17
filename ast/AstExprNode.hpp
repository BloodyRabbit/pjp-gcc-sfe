/** @file
 * @brief Declaration of AST expression nodes.
 *
 * @author Jan Bobek
 * @since 5th June 2014
 */

#ifndef SFE__AST__AST_EXPR_NODE_HPP__INCL__
#define SFE__AST__AST_EXPR_NODE_HPP__INCL__

#include <vector>

#include "ast/AstNode.hpp"
#include "parser/LexAnalyzer.hpp"

/**
 * @brief A generic AST expression node.
 *
 * @author Jan Bobek
 */
class AstExprNode
: public AstNode
{
};

/**
 * @brief A constant integer node.
 *
 * @author Jan Bobek
 */
class AstIntExprNode
: public AstExprNode
{
public:
    /**
     * @brief Initializes the node.
     *
     * @param[in] val
     *   Value of the integer.
     */
    AstIntExprNode( int val );

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
     * @brief Translates into appropriate tree node.
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
    /// Value of the number.
    int mVal;
};

/**
 * @brief A literal string node.
 *
 * @author Jan Bobek
 */
class AstStrExprNode
: public AstExprNode
{
public:
    /**
     * @brief Initializes the node.
     *
     * @param[in] str
     *   Literal value of the string.
     */
    AstStrExprNode( const char* str );

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
    /// Literal value of the string.
    std::string mStr;
};

/**
 * @brief A variable/constant reference node.
 *
 * @author Jan Bobek
 */
class AstVarExprNode
: public AstExprNode
{
public:
    /**
     * @brief Initializes the node.
     *
     * @param[in] name
     *   The identifier being referenced.
     */
    AstVarExprNode( const char* name );

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
    /// Identifier being referenced.
    std::string mName;
};

/**
 * @brief An array reference node.
 *
 * @author Jan Bobek
 */
class AstArrExprNode
: public AstExprNode
{
public:
    /**
     * @brief Initializes the node.
     *
     * @param[in] name
     *   Name of the referenced array.
     * @param[in] index
     *   The index expression.
     */
    AstArrExprNode(
        const char* name,
        AstExprNode* index
        );
    /**
     * @brief Frees the index expression.
     */
    ~AstArrExprNode();

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
    /// Name of the referenced array.
    std::string mName;
    /// The index expression.
    AstExprNode* mIndex;
};

/**
 * @brief A function call node.
 *
 * @author Jan Bobek
 */
class AstFunExprNode
: public AstExprNode
{
public:
    /**
     * @brief Initializes the object.
     *
     * @param[in] name
     *   Name of the called function.
     */
    AstFunExprNode( const char* name );
    /**
     * @brief Frees the argument expressions.
     */
    ~AstFunExprNode();

    /**
     * @brief Adds an argument to the call.
     *
     * @param[in] arg
     *   Argument expression.
     */
    void addArg( AstExprNode* arg );

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
    /// Name of the called function.
    std::string mName;
    /// An argument vector.
    std::vector< AstExprNode* > mArgs;
};

/**
 * @brief An unary operation node.
 *
 * @author Jan Bobek
 */
class AstUnopExprNode
: public AstExprNode
{
public:
    /**
     * @brief Initializes the node.
     *
     * @param[in] token
     *   Equivalent lexical token.
     * @param[in] op
     *   The single operand.
     */
    AstUnopExprNode(
        LexToken token,
        AstExprNode* op
        );
    /**
     * @brief Frees the operand.
     */
    ~AstUnopExprNode();

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
    /// Equivalent lexical token.
    LexToken mToken;
    /// Single operand.
    AstExprNode* mOp;
};

/**
 * @brief A binary operation node.
 *
 * @author Jan Bobek
 */
class AstBinopExprNode
: public AstExprNode
{
public:
    /**
     * @brief Initializes the node.
     *
     * @param[in] token
     *   Equivalent lexical token.
     * @param[in] left
     *   Left operand.
     * @param[in] right
     *   Right operand.
     */
    AstBinopExprNode(
        LexToken token,
        AstExprNode* left,
        AstExprNode* right
        );
    /**
     * @brief Frees both operands.
     */
    ~AstBinopExprNode();

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
    /// Equivalent lexical token.
    LexToken mToken;
    /// Left operand.
    AstExprNode* mLeft;
    /// Right operand.
    AstExprNode* mRight;
};

#endif /* !SFE__AST__AST_EXPR_NODE_HPP__INCL__ */
