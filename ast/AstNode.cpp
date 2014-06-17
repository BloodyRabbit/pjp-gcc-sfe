/** @file
 * @brief Definition of a generic AST node.
 *
 * @author Jan Bobek
 * @since 17th June 2014
 */

#include "ast/AstNode.hpp"
#include "parser/SymTable.hpp"

/*************************************************************************/
/* AstNode                                                               */
/*************************************************************************/
bool
AstNode::translate() const
{
    tree t;
    SymTable symTable;
    return translate( t, NULL_TREE, symTable );
}
