/** @file
 * @brief Declaration of AST type nodes.
 *
 * @author Jan Bobek
 * @since 5th June 2014
 */

#ifndef SFE__AST__AST_TYPE_NODE_HPP__INCL__
#define SFE__AST__AST_TYPE_NODE_HPP__INCL__

#include "ast/AstNode.hpp"

/**
 * @brief A generic type node.
 *
 * @author Jan Bobek
 */
class AstTypeNode
: public AstNode
{
public:
    /**
     * @brief Clones the node.
     *
     * @return
     *   Clone of the node.
     */
    virtual AstTypeNode* clone() const = 0;

    /**
     * @brief Looks up a symbol according
     *  to the type.
     *
     * @param[in] name
     *   Name of the symbol.
     * @param[in] symTable
     *   The SymTable to use.
     *
     * @return
     *   Whatever returned by SymTable.
     */
    virtual tree lookupSym(
        const char* name,
        SymTable& symTable
        ) const = 0;
    /**
     * @brief Registers a symbol according
     *  to the type.
     *
     * @param[in] name
     *   Name of the symbol.
     * @param[in] decl
     *   Declaration of the symbol.
     * @param[in] symTable
     *   The SymTable to use.
     *
     * @return
     *   Whatever returned by SymTable.
     */
    virtual bool registerSym(
        const char* name,
        tree decl,
        SymTable& symTable
        ) const = 0;
    /**
     * @brief Unregisters a symbol according
     *   to the type.
     *
     * @param[in] name
     *   Name of the symbol.
     * @param[in] symTable
     *   The SymTable to use.
     *
     * @return
     *   Whatever returned by SymTable.
     */
    virtual bool unregisterSym(
        const char* name,
        SymTable& symTable
        ) const = 0;
};

/**
 * @brief An integer type node.
 *
 * @author Jan Bobek
 */
class AstIntTypeNode
: public AstTypeNode
{
public:
    /**
     * @brief Clones the node.
     *
     * @return
     *   Clone of the node.
     */
    AstTypeNode* clone() const;

    /**
     * @brief Looks up a symbol according
     *  to the type.
     *
     * @param[in] name
     *   Name of the symbol.
     * @param[in] symTable
     *   The SymTable to use.
     *
     * @return
     *   Whatever returned by SymTable.
     */
    tree lookupSym(
        const char* name,
        SymTable& symTable
        ) const;
    /**
     * @brief Registers a symbol according
     *  to the type.
     *
     * @param[in] name
     *   Name of the symbol.
     * @param[in] decl
     *   Declaration of the symbol.
     * @param[in] symTable
     *   The SymTable to use.
     *
     * @return
     *   Whatever returned by SymTable.
     */
    bool registerSym(
        const char* name,
        tree decl,
        SymTable& symTable
        ) const;
    /**
     * @brief Unregisters a symbol according
     *   to the type.
     *
     * @param[in] name
     *   Name of the symbol.
     * @param[in] symTable
     *   The SymTable to use.
     *
     * @return
     *   Whatever returned by SymTable.
     */
    bool unregisterSym(
        const char* name,
        SymTable& symTable
        ) const;

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
     * @brief Creates an appropriate type node.
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
 * @brief An array type node.
 *
 * @author Jan Bobek
 */
class AstArrTypeNode
: public AstTypeNode
{
public:
    /**
     * @brief Initializes the node.
     *
     * @param[in] begin
     *   Begin index.
     * @param[in] end
     *   End index.
     * @param[in] elemType
     *   Array element type.
     */
    AstArrTypeNode(
        int begin,
        int end,
        AstTypeNode* elemType
        );
    /**
     * @brief Frees the array element type.
     */
    ~AstArrTypeNode();

    /**
     * @brief Clones the node.
     *
     * @return
     *   Clone of the node.
     */
    AstTypeNode* clone() const;

    /**
     * @brief Looks up a symbol according
     *  to the type.
     *
     * @param[in] name
     *   Name of the symbol.
     * @param[in] symTable
     *   The SymTable to use.
     *
     * @return
     *   Whatever returned by SymTable.
     */
    tree lookupSym(
        const char* name,
        SymTable& symTable
        ) const;
    /**
     * @brief Registers a symbol according
     *  to the type.
     *
     * @param[in] name
     *   Name of the symbol.
     * @param[in] decl
     *   Declaration of the symbol.
     * @param[in] symTable
     *   The SymTable to use.
     *
     * @return
     *   Whatever returned by SymTable.
     */
    bool registerSym(
        const char* name,
        tree decl,
        SymTable& symTable
        ) const;
    /**
     * @brief Unregisters a symbol according
     *   to the type.
     *
     * @param[in] name
     *   Name of the symbol.
     * @param[in] symTable
     *   The SymTable to use.
     *
     * @return
     *   Whatever returned by SymTable.
     */
    bool unregisterSym(
        const char* name,
        SymTable& symTable
        ) const;

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
     * @brief Translates to appropriate array type.
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
    /// Begin index.
    int mBegin;
    /// End index.
    int mEnd;

    /// Array element type.
    AstTypeNode* mElemType;
};

#endif /* !SFE__AST__AST_TYPE_NODE_HPP__INCL__ */
