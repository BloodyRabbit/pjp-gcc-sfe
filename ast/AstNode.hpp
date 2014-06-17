/** @file
 * @brief Declaration of a generic AST node.
 *
 * @author Jan Bobek
 * @since 5th June 2014
 */

#ifndef SFE__AST__AST_NODE_HPP__INCL__
#define SFE__AST__AST_NODE_HPP__INCL__

#include <cstdio>

#ifdef NO_GCC
typedef void* tree;
#   define NULL_TREE 0
#else /* !NO_GCC */
#   include "config.h"
#   include "system.h"
#   include "coretypes.h"

#   include "tree.h"
#   include "tree-iterator.h"

#   include "stringpool.h"
#   include "print-tree.h"
#endif /* !NO_GCC */

class SymTable;

/**
 * @brief A generic AST node.
 *
 * @author Jan Bobek
 */
class AstNode
{
public:
    /**
     * @brief Virtual destructor for proper destruction.
     */
    virtual ~AstNode() {}

    /**
     * @brief Prints the node to a file.
     *
     * @param[in] off
     *   Line offset to use.
     * @param[in] fp
     *   File to print to.
     */
    virtual void print(
        unsigned int off,
        FILE* fp
        ) const = 0;
    /**
     * @brief Translates the AST into equivalent GENERIC tree.
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
    virtual bool translate(
        tree& t,
        tree ctx,
        SymTable& symTable
        ) const = 0;
};

#endif /* !SFE__AST__AST_NODE_HPP__INCL__ */
