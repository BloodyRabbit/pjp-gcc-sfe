/** @file
 * @brief Definition of AST type nodes.
 *
 * @author Jan Bobek
 * @since 5th June 2014
 */

#include "ast/AstTypeNode.hpp"
#include "parser/SymTable.hpp"

/*************************************************************************/
/* AstIntTypeNode                                                        */
/*************************************************************************/
AstTypeNode*
AstIntTypeNode::clone() const
{
    return new AstIntTypeNode();
}

tree
AstIntTypeNode::getSym(
    const char* name,
    SymTable& symTable
    ) const
{
    return symTable.getVar(
        name );
}

bool
AstIntTypeNode::addSym(
    const char* name,
    tree decl,
    SymTable& symTable
    ) const
{
    return symTable.addVar(
        name, decl );
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
    t = integer_type_node;
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

tree
AstArrTypeNode::getSym(
    const char* name,
    SymTable& symTable
    ) const
{
    int off;
    return symTable.getArr(
        name, off );
}

bool
AstArrTypeNode::addSym(
    const char* name,
    tree decl,
    SymTable& symTable
    ) const
{
    return symTable.addArr(
        name, -mBegin, decl );
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
    tree elemType;
    if( !mElemType->translate( elemType, ctx, symTable ) )
        return false;

    t = build_array_type(
        elemType,
        build_index_type(
            size_int( mEnd - mBegin ) ) );

    return true;
}
