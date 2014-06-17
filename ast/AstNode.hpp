/** @file
 * @brief Declaration of a generic AST node.
 *
 * @author Jan Bobek
 * @since 5th June 2014
 */

#ifndef SFE__AST__AST_NODE_HPP__INCL__
#define SFE__AST__AST_NODE_HPP__INCL__

#include <cstdio>

#ifdef IN_GCC
#   include "config.h"
#   include "system.h"
#   include "coretypes.h"
#   include "tree.h"
#   include "tree-iterator.h"
#   include "stringpool.h"
#   include "print-tree.h"
#else /* !IN_GCC */
typedef void* tree;

#   define NULL_TREE 0
#   define UNKNOWN_LOCATION 0

#   define VAR_DECL 0
#   define RESULT_DECL 0
#   define PARM_DECL 0

#   define ARRAY_REF 0
#   define DECL_EXPR 0
#   define ADDR_EXPR 0
#   define PLUS_EXPR 0
#   define MINUS_EXPR 0
#   define MULT_EXPR 0
#   define DIV_EXPR 0
#   define MOD_EXPR 0
#   define EQ_EXPR 0
#   define NE_EXPR 0
#   define LT_EXPR 0
#   define LE_EXPR 0
#   define GT_EXPR 0
#   define GE_EXPR 0
#   define BIND_EXPR 0
#   define COND_EXPR 0
#   define LOOP_EXPR 0
#   define EXIT_EXPR 0
#   define RETURN_EXPR 0

#   define TREE_ADDRESSABLE( a ) (bool&)a
#   define TREE_CHAIN( a ) a
#   define TREE_CODE( a ) a
#   define TREE_OPERAND( a, b ) a
#   define TREE_PUBLIC( a ) (bool&)a
#   define TREE_SIDE_EFFECTS( a ) (bool&)a
#   define TREE_STATIC( a ) (bool&)a
#   define TREE_TYPE( a ) a
#   define TREE_USED( a ) (bool&)a
#   define BLOCK NULL_TREE
#   define BLOCK_CHAIN( a ) a
#   define BLOCK_SUBBLOCKS( a ) a
#   define BLOCK_SUPERCONTEXT( a ) a
#   define DECL_ARGUMENTS( a ) a
#   define DECL_ARG_TYPE( a ) a
#   define DECL_ARTIFICIAL( a ) (bool&)a
#   define DECL_EXTERNAL( a ) (bool&)a
#   define DECL_IGNORED_P( a ) (bool&)a
#   define DECL_INITIAL( a ) a
#   define DECL_RESULT( a ) a
#   define DECL_SAVED_TREE( a ) a
#   define SET_EXPR_LOCATION( a, b )
#   define TYPE_STRING_FLAG( a ) (bool&)a

#   define void_type_node NULL_TREE
#   define integer_type_node NULL_TREE
#   define unsigned_char_type_node NULL_TREE
#   define get_identifier( a ) NULL_TREE
#   define chainon( a, b ) NULL_TREE
#   define tree_cons( a, b, c ) NULL_TREE
#   define alloc_stmt_list() NULL_TREE
#   define append_to_statement_list( a, b ) NULL_TREE
#   define build1( a, b, c ) NULL_TREE
#   define build2( a, b, c, d ) NULL_TREE
#   define build3( a, b, c, d, e ) NULL_TREE
#   define build4( a, b, c, d, e, f ) NULL_TREE
#   define build_decl( a, b, c, d ) NULL_TREE
#   define build_block( a, b, c, d ) NULL_TREE
#   define build_string( a, b ) NULL_TREE
#   define build_int_cst( a, b ) NULL_TREE
#   define build_qualified_type( a, b ) NULL_TREE
#   define build_index_type( a ) NULL_TREE
#   define build_array_type( a, b ) NULL_TREE
#   define build_function_type( a, b ) NULL_TREE
#   define build_call_expr_loc_array( a, b, c, d ) NULL_TREE
#   define debug_tree( a )
#   define XNEWVEC( a, b ) NULL_TREE
#endif /* !IN_GCC */

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
