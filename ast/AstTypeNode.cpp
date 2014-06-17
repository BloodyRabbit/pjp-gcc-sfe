/** @file
 * @brief Definition of AST type nodes.
 *
 * @author Jan Bobek
 * @since 5th June 2014
 */

#include "ast/AstTypeNode.hpp"

/*************************************************************************/
/* AstIntTypeNode                                                        */
/*************************************************************************/
AstTypeNode*
AstIntTypeNode::clone() const
{
    return new AstIntTypeNode();
}

void
AstIntTypeNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    fprintf( fp, "%*cinteger\n", off, ' ' );
}

bool
AstIntTypeNode::translate(
    tree& t,
    tree,
    SymTable&
    ) const
{
#ifdef NO_GCC
    t = NULL_TREE;
#else /* !NO_GCC */
    t = integer_type_node;
#endif /* !NO_GCC */

    return true;
}

/*************************************************************************/
/* AstArrTypeNode                                                        */
/*************************************************************************/
AstArrTypeNode::AstArrTypeNode(
    int begin,
    int end,
    AstTypeNode* elemType
    )
: mBegin( begin ),
  mEnd( end ),
  mElemType( elemType )
{
}

AstArrTypeNode::~AstArrTypeNode()
{
    delete mElemType;
}

AstTypeNode*
AstArrTypeNode::clone() const
{
    return new AstArrTypeNode(
        mBegin, mEnd,
        mElemType->clone() );
}

void
AstArrTypeNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    fprintf(
        fp, "%*carray [ %d .. %d ] of\n",
        off, ' ', mBegin, mEnd );

    mElemType->print( off + 1, fp );
}

bool
AstArrTypeNode::translate(
    tree& t,
    tree ctx,
    SymTable& symTable
    ) const
{
#ifdef NO_GCC
    t = NULL_TREE;
#else /* !NO_GCC */
    tree elemType;
    if( !mElemType->translate( elemType, ctx, symTable ) )
        return false;

    t = build_array_type(
        elemType,
        build_index_type(
            size_int( mEnd - mBegin + 1 ) ) );
#endif /* !NO_GCC */

    return true;
}
