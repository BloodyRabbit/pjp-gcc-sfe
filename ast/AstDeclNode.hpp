/** @file
 * @brief Declaration of a generic AST decl/def node.
 *
 * @author Jan Bobek
 * @since 5th June 2014
 */

#ifndef SFE__AST__AST_DECL_NODE_HPP__INCL__
#define SFE__AST__AST_DECL_NODE_HPP__INCL__

#include <string>
#include <vector>

#include "ast/AstNode.hpp"

class AstStmtNode;
class AstBlkStmtNode;
class AstTypeNode;

/**
 * @brief A generic declaration/definition node.
 *
 * @author Jan Bobek
 */
class AstDeclNode
: public AstNode
{
public:
    /**
     * @brief Initializes the node.
     *
     * @param[in] name
     *   Name of the element being declared/defined.
     */
    AstDeclNode( const char* name );

    /**
     * @brief Unregisters the element from
     *   the symbol table.
     *
     * @param[in] symTable
     *   The symbol table.
     *
     * @retval true
     *   Unregistration succeeded.
     * @retval false
     *   Unregistration failed.
     */
    virtual bool unregister(
        SymTable& symTable
        ) const = 0;

protected:
    /// Name of the element.
    std::string mName;
};

/**
 * @brief A local declaration/definition node.
 *
 * @author Jan Bobek
 */
class AstLocDeclNode
: public AstDeclNode
{
public:
    /**
     * @brief Initializes the node.
     *
     * @param[in] name
     *   Name of the element being declared/defined.
     */
    AstLocDeclNode( const char* name );
};

/**
 * @brief A constant definition node.
 *
 * @author Jan Bobek
 */
class AstConstDeclNode
: public AstLocDeclNode
{
public:
    /**
     * @brief Initializes the node.
     *
     * @param[in] name
     *   Name of the constant.
     * @param[in] val
     *   Value of the constant.
     */
    AstConstDeclNode(
        const char* name,
        int val
        );

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
    /**
     * @brief Unregisters the constant
     *   from the symbol table.
     *
     * @param[in] symTable
     *   The symbol table to use.
     *
     * @retval true
     *   Unregistration succeeded.
     * @retval false
     *   Unregistration failed.
     */
    bool unregister(
        SymTable& symTable
        ) const;

protected:
    /// Value of the constant.
    int mVal;
};

/**
 * @brief A variable declaration node.
 *
 * @author Jan Bobek
 */
class AstVarDeclNode
: public AstLocDeclNode
{
public:
    /**
     * @brief Initializes the node.
     *
     * @param[in] name
     *   Name of the variable.
     * @param[in] type
     *   Type of the variable.
     */
    AstVarDeclNode(
        const char* name,
        AstTypeNode* type
        );
    /**
     * @brief Frees the type.
     */
    ~AstVarDeclNode();

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
    /**
     * @brief Unregisters the variable
     *  from the symbol table.
     *
     * @param[in] symTable
     *   The symbol table to use.
     *
     * @retval true
     *   Unregistration succeeded.
     * @retval false
     *   Unregistration failed.
     */
    bool unregister(
        SymTable& symTable
        ) const;

protected:
    /// Type of the variable.
    AstTypeNode* mType;
};

/**
 * @brief A function declaration/definition node.
 *
 * @author Jan Bobek
 */
class AstFunDeclNode
: public AstDeclNode
{
public:
    /// Declarative description of an argument.
    typedef std::pair< std::string, AstTypeNode* > Arg;
    /// List of arguments.
    typedef std::vector< Arg > ArgList;

    /**
     * @brief Initializes the node.
     *
     * @param[in] name
     *   Name of the function.
     * @param[in] args
     *   Names and types of the arguments.
     * @param[in] resType
     *   Type of the result.
     * @param[in] body
     *   Body of the function.
     */
    AstFunDeclNode(
        const char* name,
        const ArgList& args,
        AstTypeNode* resType,
        AstBlkStmtNode* body
        );
    /**
     * @brief Frees the types and body.
     */
    ~AstFunDeclNode();

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
    /**
     * @brief Unregisters the function
     *   from the symbol table.
     *
     * @param[in] symTable
     *   The symbol table to use.
     *
     * @retval true
     *   Unregistration succeeded.
     * @retval false
     *   Unregistration failed.
     */
    bool unregister(
        SymTable& symTable
        ) const;

protected:
    /// Names and types of the arguments.
    ArgList mArgs;
    /// Type of the result.
    AstTypeNode* mResType;
    /// Body of the function.
    AstBlkStmtNode* mBody;
};

/**
 * @brief A program definition node.
 *
 * @author Jan Bobek
 */
class AstProgDeclNode
: public AstDeclNode
{
public:
    /**
     * @brief Initializes the node.
     *
     * @param[in] name
     *   Name of the program.
     * @param[in] funs
     *   Global function declarations.
     * @param[in] vars
     *   Global variable declarations.
     * @param[in] mainBlk
     *   Main block.
     */
    AstProgDeclNode(
        const char* name,
        const std::vector< AstDeclNode* >& funs,
        const std::vector< AstLocDeclNode* >& vars,
        AstBlkStmtNode* mainBlk
        );
    /**
     * @brief Frees the contained declarations and statements.
     */
    ~AstProgDeclNode();

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
     * @brief Translates the program.
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
    /**
     * @brief Unregisters the program
     *   from the symbol table.
     *
     * @param[in] symTable
     *   The symbol table to use.
     *
     * @retval true
     *   Unregistration succeeded.
     * @retval false
     *   Unregistration failed.
     */
    bool unregister(
        SymTable& symTable
        ) const;

protected:
    /// Global function declarations.
    std::vector< AstDeclNode* > mFunDecls;
    /// Global variable declarations.
    std::vector< AstLocDeclNode* > mVarDecls;
};

#endif /* !SFE__AST__AST_DECL_NODE_HPP__INCL__ */
