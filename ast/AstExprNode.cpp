/** @file
 * @brief Definition of AST expression nodes.
 *
 * @author Jan Bobek
 * @since 5th June 2014
 */

#include "ast/AstExprNode.hpp"
#include "parser/SymTable.hpp"

/*************************************************************************/
/* AstIntExprNode                                                        */
/*************************************************************************/
AstIntExprNode::AstIntExprNode(
    int val
    )
: mVal( val )
{
}

void
AstIntExprNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    fprintf(
        fp,
        "%*c%d\n",
        off, ' ',
        mVal );
}

bool
AstIntExprNode::translate(
    tree& t,
    tree,
    SymTable&
    ) const
{
    t = build_int_cst(
        integer_type_node, mVal );

    return true;
}

/*************************************************************************/
/* AstStrExprNode                                                        */
/*************************************************************************/
AstStrExprNode::AstStrExprNode(
    const char* str
    )
: mStr( str )
{
}

void
AstStrExprNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    fprintf(
        fp,
        "%*c'%s'\n",
        off, ' ',
        mStr.c_str() );
}

bool
AstStrExprNode::translate(
    tree& t,
    tree,
    SymTable&
    ) const
{
    tree idx_type = build_index_type(
        size_int( mStr.length() ) );
    tree elem_type =
        build_qualified_type(
            unsigned_char_type_node,
            TYPE_QUAL_CONST );
    tree string_type =
        build_array_type( elem_type, idx_type );
    TYPE_STRING_FLAG( string_type ) = 1;

    tree str = build_string(
        mStr.length() + 1,
        mStr.c_str() );
    TREE_TYPE( str ) = string_type;

    t = build1(
        ADDR_EXPR, build_pointer_type(
            TREE_TYPE( str ) ),
        str );

    return true;
}

/*************************************************************************/
/* AstVarExprNode                                                        */
/*************************************************************************/
AstVarExprNode::AstVarExprNode(
    const char* name
    )
: mName( name )
{
}

void
AstVarExprNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    fprintf(
        fp,
        "%*c%s\n",
        off, ' ',
        mName.c_str() );
}

bool
AstVarExprNode::translate(
    tree& t,
    tree,
    SymTable& symTable
    ) const
{
    t = symTable.lookupVar(
        mName.c_str() );

    if( NULL_TREE == t )
    {
        fprintf( stderr, "Unknown variable `%s'\n",
                 mName.c_str() );
        return false;
    }

    return true;
}

/*************************************************************************/
/* AstArrExprNode                                                        */
/*************************************************************************/
AstArrExprNode::AstArrExprNode(
    const char* name,
    AstExprNode* index
    )
: mName( name ),
  mIndex( index )
{
}

AstArrExprNode::~AstArrExprNode()
{
    delete mIndex;
}

void
AstArrExprNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    fprintf(
        fp,
        "%*c%s[\n",
        off, ' ',
        mName.c_str() );

    mIndex->print( off + 1, fp );

    fprintf(
        fp,
        "%*c]\n",
        off, ' ' );
}

bool
AstArrExprNode::translate(
    tree& t,
    tree ctx,
    SymTable& symTable
    ) const
{
    int off;
    tree array = symTable.lookupArr(
        mName.c_str(), off );

    if( NULL_TREE == array )
    {
        fprintf( stderr, "Unknown array `%s'\n",
                 mName.c_str() );
        return false;
    }

    tree index;
    if( !mIndex->translate(
            index, ctx, symTable ) )
        return false;

    if( 0 < off )
        index = build2(
            PLUS_EXPR, TREE_TYPE( index ), index,
            build_int_cst( integer_type_node, off ) );
    else if( 0 > off )
        index = build2(
            MINUS_EXPR, TREE_TYPE( index ), index,
            build_int_cst( integer_type_node, -off ) );

    t = build4(
        ARRAY_REF, TREE_TYPE( TREE_TYPE( array ) ),
        array, index, NULL_TREE, NULL_TREE );

    return true;
}

/*************************************************************************/
/* AstFunExprNode                                                        */
/*************************************************************************/
AstFunExprNode::AstFunExprNode(
    const char* name
    )
: mName( name )
{
}

AstFunExprNode::~AstFunExprNode()
{
    std::vector< AstExprNode* >::iterator cur, end;
    cur = mArgs.begin();
    end = mArgs.end();
    for(; cur != end; ++cur )
        delete *cur;
}

void
AstFunExprNode::addArg(
    AstExprNode* arg
    )
{
    mArgs.push_back( arg );
}

void
AstFunExprNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    fprintf( fp, "%*c%s(\n", off, ' ', mName.c_str() );

    std::vector< AstExprNode* >::const_iterator cur, end;
    cur = mArgs.begin();
    end = mArgs.end();
    for(; cur != end; ++cur )
    {
        (*cur)->print( off + 1, fp );

        if( cur + 1 != end )
            fprintf( fp, "%*c,\n", off, ' ' );
    }

    fprintf( fp, "%*c)\n", off, ' ' );
}

bool
AstFunExprNode::translate(
    tree& t,
    tree ctx,
    SymTable& symTable
    ) const
{
    tree fndecl =
        symTable.lookupFun( mName.c_str() );

    if( NULL_TREE == fndecl )
    {
        fprintf( stderr, "Unknown function `%s'\n",
                 mName.c_str() );
        return false;
    }

    tree* argv = XNEWVEC( tree, mArgs.size() );
    for( unsigned int i = 0; i < mArgs.size(); ++i )
        if( !mArgs[i]->translate( argv[i], ctx, symTable ) )
            return false;

    t = build_call_expr_loc_array(
        UNKNOWN_LOCATION, fndecl,
        mArgs.size(), argv );
    SET_EXPR_LOCATION( t, UNKNOWN_LOCATION );
    TREE_USED( t ) = true;

    return true;
}

/*************************************************************************/
/* AstUnopExprNode                                                       */
/*************************************************************************/
AstUnopExprNode::AstUnopExprNode(
    LexToken token,
    AstExprNode* op
    )
: mToken( token ),
  mOp( op )
{
}

AstUnopExprNode::~AstUnopExprNode()
{
    delete mOp;
}

void
AstUnopExprNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    switch( mToken )
    {
    case LEXTOK_MINUS: fprintf( fp, "%*c-\n", off, ' ' ); break;
    case LEXTOK_NOT:   fprintf( fp, "%*c!\n", off, ' ' ); break;
    }

    mOp->print( off + 1, fp );
}

bool
AstUnopExprNode::translate(
    tree& t,
    tree ctx,
    SymTable& symTable
    ) const
{
    tree op;
    if( !mOp->translate( op, ctx, symTable ) )
        return false;

    switch( mToken )
    {
    case LEXTOK_MINUS: t = build1( NEGATE_EXPR,    TREE_TYPE( op ), op ); return true;
    case LEXTOK_NOT:   t = build1( TRUTH_NOT_EXPR, TREE_TYPE( op ), op ); return true;

    default: t = NULL_TREE; return false;
    }
}

/*************************************************************************/
/* AstBinopExprNode                                                      */
/*************************************************************************/
AstBinopExprNode::AstBinopExprNode(
    LexToken token,
    AstExprNode* left,
    AstExprNode* right
    )
: mToken( token ),
  mLeft( left ),
  mRight( right )
{
}

AstBinopExprNode::~AstBinopExprNode()
{
    delete mLeft;
    delete mRight;
}

void
AstBinopExprNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    mLeft->print( off + 1, fp );

    switch( mToken )
    {
    case LEXTOK_PLUS:  fprintf( fp, "%*c+\n", off, ' ' ); break;
    case LEXTOK_MINUS: fprintf( fp, "%*c-\n", off, ' ' ); break;
    case LEXTOK_MULT:  fprintf( fp, "%*c*\n", off, ' ' ); break;
    case LEXTOK_DIV:   fprintf( fp, "%*cdiv\n", off, ' ' ); break;
    case LEXTOK_MOD:   fprintf( fp, "%*cmod\n", off, ' ' ); break;

    case LEXTOK_EQ:  fprintf( fp, "%*c=\n", off, ' ' ); break;
    case LEXTOK_NEQ: fprintf( fp, "%*c<>\n", off, ' ' ); break;
    case LEXTOK_LT:  fprintf( fp, "%*c<\n", off, ' ' ); break;
    case LEXTOK_LEQ: fprintf( fp, "%*c<=\n", off, ' ' ); break;
    case LEXTOK_GT:  fprintf( fp, "%*c>\n", off, ' ' ); break;
    case LEXTOK_GEQ: fprintf( fp, "%*c>=\n", off, ' ' ); break;

    case LEXTOK_OR:  fprintf( fp, "%*cor\n", off, ' ' ); break;
    case LEXTOK_AND: fprintf( fp, "%*cand\n", off, ' ' ); break;
    }

    mRight->print( off + 1, fp );
}

bool
AstBinopExprNode::translate(
    tree& t,
    tree ctx,
    SymTable& symTable
    ) const
{
    tree left, right;
    if( !mLeft->translate( left, ctx, symTable ) ||
        !mRight->translate( right, ctx, symTable ) )
        return false;

    switch( mToken )
    {
    case LEXTOK_PLUS:  t = build2( PLUS_EXPR,      TREE_TYPE( left ), left, right ); return true;
    case LEXTOK_MINUS: t = build2( MINUS_EXPR,     TREE_TYPE( left ), left, right ); return true;
    case LEXTOK_MULT:  t = build2( MULT_EXPR,      TREE_TYPE( left ), left, right ); return true;
    case LEXTOK_DIV:   t = build2( TRUNC_DIV_EXPR, TREE_TYPE( left ), left, right ); return true;
    case LEXTOK_MOD:   t = build2( TRUNC_MOD_EXPR, TREE_TYPE( left ), left, right ); return true;

    case LEXTOK_EQ:  t = build2( EQ_EXPR, TREE_TYPE( left ), left, right ); return true;
    case LEXTOK_NEQ: t = build2( NE_EXPR, TREE_TYPE( left ), left, right ); return true;
    case LEXTOK_LT:  t = build2( LT_EXPR, TREE_TYPE( left ), left, right ); return true;
    case LEXTOK_LEQ: t = build2( LE_EXPR, TREE_TYPE( left ), left, right ); return true;
    case LEXTOK_GT:  t = build2( GT_EXPR, TREE_TYPE( left ), left, right ); return true;
    case LEXTOK_GEQ: t = build2( GE_EXPR, TREE_TYPE( left ), left, right ); return true;

    case LEXTOK_OR:  t = build2( TRUTH_OR_EXPR,  TREE_TYPE( left ), left, right ); return true;
    case LEXTOK_AND: t = build2( TRUTH_AND_EXPR, TREE_TYPE( left ), left, right ); return true;

    default: t = NULL_TREE; return false;
    }
}
