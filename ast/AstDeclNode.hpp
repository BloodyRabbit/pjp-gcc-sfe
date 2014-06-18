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
     * @brief Registers a symbol in the
     *   symbol table.
     *
     * @param[in] symTable
     *  The symbol table to use.
     *
     * @retval true
     *   Registration succeeded.
     * @retval false
     *   Registration failed.
     */
    virtual bool registerSym(
        SymTable& symTable
        ) const = 0;
    /**
     * @brief Unregisters an element from
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
    virtual bool unregisterSym(
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
     * @brief Registers the constant in the
     *   symbol table.
     *
     * @param[in] symTable
     *  The symbol table to use.
     *
     * @retval true
     *   Registration succeeded.
     * @retval false
     *   Registration failed.
     */
    bool registerSym(
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
    bool unregisterSym(
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
     * @brief Registers the variable in the
     *   symbol table.
     *
     * @param[in] symTable
     *  The symbol table to use.
     *
     * @retval true
     *   Registration succeeded.
     * @retval false
     *   Registration failed.
     */
    bool registerSym(
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
    bool unregisterSym(
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
     */
    AstFunDeclNode( const char* name );
    /**
     * @brief Frees the types and body.
     */
    ~AstFunDeclNode();

    /**
     * @brief Checks if this is a procedure.
     *
     * @retval true
     *   This is a procedure.
     * @retval false
     *   This is not a procedure.
     */
    bool isProcedure() const;
    /**
     * @brief Checks if this is forward declaration.
     *
     * @retval true
     *   This is a forward declaration.
     * @retval false
     *   This is not a forward declaration.
     */
    bool isForward() const;

    /**
     * @brief Adds an argument.
     *
     * @param[in] name
     *   Name of the argument.
     * @param[in] type
     *   Type of the argument.
     */
    void addArg(
        const char* name,
        AstTypeNode* type
        );
    /**
     * @brief Sets the result type.
     *
     * @param[in] type
     *   Type of the result.
     */
    void setResType( AstTypeNode* type );
    /**
     * @brief Sets body of the function.
     *
     * @param[in] blk
     *   Body of the function.
     */
    void setBody( AstBlkStmtNode* blk );

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
     * @brief Registers the function in the
     *   symbol table.
     *
     * @param[in] symTable
     *  The symbol table to use.
     *
     * @retval true
     *   Registration succeeded.
     * @retval false
     *   Registration failed.
     */
    bool registerSym(
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
    bool unregisterSym(
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
    AstProgDeclNode( const char* name );
    /**
     * @brief Frees the contained functions.
     */
    ~AstProgDeclNode();

    /**
     * @brief Adds a function declaration.
     *
     * @param[in] fun
     *   The function declaration to add.
     */
    void addFun( AstFunDeclNode* fun );

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
     * @brief Registers the program in the
     *   symbol table.
     *
     * @param[in] symTable
     *  The symbol table to use.
     *
     * @retval true
     *   Registration succeeded.
     * @retval false
     *   Registration failed.
     */
    bool registerSym(
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
    bool unregisterSym(
        SymTable& symTable
        ) const;

protected:
    /// Function declarations.
    std::vector< AstFunDeclNode* > mFunDecls;
};

#endif /* !SFE__AST__AST_DECL_NODE_HPP__INCL__ */
