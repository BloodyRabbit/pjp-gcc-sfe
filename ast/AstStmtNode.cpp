/** @file
 * @brief Definition of AST statement nodes.
 *
 * @author Jan Bobek
 * @since 5th June 2014
 */

#include "ast/AstDeclNode.hpp"
#include "ast/AstExprNode.hpp"
#include "ast/AstStmtNode.hpp"
#include "parser/SymTable.hpp"

/*************************************************************************/
/* AstBinopStmtNode                                                      */
/*************************************************************************/
AstBinopStmtNode::AstBinopStmtNode(
    LexToken tok,
    AstExprNode* lval,
    AstExprNode* rval
    )
: mToken( tok ),
  mLval( lval ),
  mRval( rval )
{
}

AstBinopStmtNode::~AstBinopStmtNode()
{
    delete mLval;
    delete mRval;
}

void
AstBinopStmtNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    mLval->print( off + 1, fp );

    switch( mToken )
    {
    case LEXTOK_ASSGN:  fprintf( fp, "%*c:=\n", off, ' ' ); break;

    case LEXTOK_KW_INC: fprintf( fp, "%*c+=\n", off, ' ' ); break;
    case LEXTOK_KW_DEC: fprintf( fp, "%*c-=\n", off, ' ' ); break;
    }

    mRval->print( off + 1, fp );
}

bool
AstBinopStmtNode::translate(
    tree& t,
    tree ctx,
    SymTable& symTable
    ) const
{
    tree lval, rval;
    if( !mLval->translate( lval, ctx, symTable ) ||
        !mRval->translate( rval, ctx, symTable ) )
        return false;

    switch( mToken )
    {
    case LEXTOK_ASSGN: t = build2( MODIFY_EXPR, TREE_TYPE( lval ), lval, rval ); return true;

    case LEXTOK_KW_INC: t = build2( PREINCREMENT_EXPR, TREE_TYPE( lval ), lval, rval ); return true;
    case LEXTOK_KW_DEC: t = build2( PREDECREMENT_EXPR, TREE_TYPE( lval ), lval, rval ); return true;

    default: return false;
    }
}

/*************************************************************************/
/* AstFunStmtNode                                                        */
/*************************************************************************/
AstFunStmtNode::AstFunStmtNode(
    AstFunExprNode* fun
    )
: mFun( fun )
{
}

AstFunStmtNode::~AstFunStmtNode()
{
    delete mFun;
}

void
AstFunStmtNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    mFun->print( off, fp );
}

bool
AstFunStmtNode::translate(
    tree& t,
    tree ctx,
    SymTable& symTable
    ) const
{
    return mFun->translate(
        t, ctx, symTable );
}

/*************************************************************************/
/* AstBlkStmtNode                                                        */
/*************************************************************************/
AstBlkStmtNode::AstBlkStmtNode()
{
}

AstBlkStmtNode::~AstBlkStmtNode()
{
    std::vector< AstLocDeclNode* >::iterator curd, endd;
    curd = mDecls.begin();
    endd = mDecls.end();
    for(; curd != endd; ++curd )
        delete *curd;
    mDecls.clear();

    std::vector< AstStmtNode* >::iterator curs, ends;
    curs = mStmts.begin();
    ends = mStmts.end();
    for(; curs != ends; ++curs )
        delete *curs;
    mStmts.clear();
}

void
AstBlkStmtNode::addDecl(
    AstLocDeclNode* decl
    )
{
    mDecls.push_back( decl );
}

void
AstBlkStmtNode::addStmt(
    AstStmtNode* stmt
    )
{
    mStmts.push_back( stmt );
}

void
AstBlkStmtNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    std::vector< AstLocDeclNode* >::const_iterator curd, endd;
    curd = mDecls.begin();
    endd = mDecls.end();
    for(; curd != endd; ++curd )
        (*curd)->print( off, fp );

    fprintf( fp, "%*cbegin\n", off, ' ' );

    std::vector< AstStmtNode* >::const_iterator curs, ends;
    curs = mStmts.begin();
    ends = mStmts.end();
    for(; curs != ends; ++curs )
    {
        (*curs)->print( off + 1, fp );

        if( curs + 1 != ends )
            fprintf( fp, "%*c;\n", off + 1, ' ' );
    }

    fprintf( fp, "%*cend\n", off, ' ' );
}

bool
AstBlkStmtNode::translate(
    tree& t,
    tree ctx,
    SymTable& symTable
    ) const
{
    tree decls = NULL_TREE;
    tree stmts = alloc_stmt_list();

    std::vector< AstLocDeclNode* >::const_iterator curd, endd;
    curd = mDecls.begin();
    endd = mDecls.end();
    for(; curd != endd; ++curd )
    {
        if( !(*curd)->registerSym( symTable ) )
            return false;

        tree decl;
        if( !(*curd)->translate( decl, ctx, symTable ) )
            return false;

        decls = chainon( decls, decl );
        append_to_statement_list(
            build1( DECL_EXPR, void_type_node, decl ),
            &stmts );
    }

    tree block = build_block(
        decls, NULL_TREE, NULL_TREE, NULL_TREE );
    TREE_USED( block ) = true;

    std::vector< AstStmtNode* >::const_iterator curs, ends;
    curs = mStmts.begin();
    ends = mStmts.end();
    for(; curs != ends; ++curs )
    {
        tree stmt;
        if( !(*curs)->translate( stmt, block, symTable ) )
            return false;

        append_to_statement_list( stmt, &stmts );
    }

    curd = mDecls.begin();
    endd = mDecls.end();
    for(; curd != endd; ++curd )
        if( !(*curd)->unregisterSym( symTable ) )
            return false;

    t = build3(
        BIND_EXPR, void_type_node,
        BLOCK_VARS( block ), stmts, block );
    TREE_SIDE_EFFECTS( t ) = true;

    BLOCK_SUPERCONTEXT( block ) = ctx;
    if( NULL_TREE != ctx && BLOCK == TREE_CODE( ctx ) )
    {
        tree* pp = &BLOCK_SUBBLOCKS( ctx );
        while( NULL_TREE != *pp )
            pp = &BLOCK_CHAIN( *pp );
        *pp = block;
    }

    return true;
}

/*************************************************************************/
/* AstIfStmtNode                                                         */
/*************************************************************************/
AstIfStmtNode::AstIfStmtNode(
    AstExprNode* cond,
    AstBlkStmtNode* thenBlk,
    AstBlkStmtNode* elseBlk
    )
: mCond( cond ),
  mThenBlk( thenBlk ),
  mElseBlk( elseBlk )
{
}

AstIfStmtNode::~AstIfStmtNode()
{
    delete mCond;
    delete mThenBlk;
    delete mElseBlk;
}

void
AstIfStmtNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    fprintf( fp, "%*cif\n", off, ' ' );
    mCond->print( off + 1, fp );

    fprintf( fp, "%*cthen\n", off, ' ' );
    mThenBlk->print( off + 1, fp );

    if( NULL != mElseBlk )
    {
        fprintf( fp, "%*celse\n", off, ' ' );
        mElseBlk->print( off + 1, fp );
    }
}

bool
AstIfStmtNode::translate(
    tree& t,
    tree ctx,
    SymTable& symTable
    ) const
{
    tree cond, thenBlk, elseBlk;
    if( !mCond->translate( cond, ctx, symTable ) ||
        !mThenBlk->translate( thenBlk, ctx, symTable ) )
        return false;

    if( NULL == mElseBlk )
        elseBlk = NULL_TREE;
    else if( !mElseBlk->translate( elseBlk, ctx, symTable ) )
        return false;

    t = build3(
        COND_EXPR, void_type_node,
        cond, thenBlk, elseBlk );

    return true;
}

/*************************************************************************/
/* AstLoopStmtNode                                                       */
/*************************************************************************/
AstLoopStmtNode::AstLoopStmtNode(
    AstBlkStmtNode* body
    )
: mBody( body )
{
}

AstLoopStmtNode::~AstLoopStmtNode()
{
    delete mBody;
}

void
AstLoopStmtNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    fprintf(
        fp,
        "%*cloop\n",
        off, ' ' );

    mBody->print( off + 1, fp );
}

bool
AstLoopStmtNode::translate(
    tree& t,
    tree ctx,
    SymTable& symTable
    ) const
{
    tree body;
    if( !mBody->translate( body, ctx, symTable ) )
        return false;

    t = build1( LOOP_EXPR, void_type_node, body );
    return true;
}

/*************************************************************************/
/* AstBreakStmtNode                                                      */
/*************************************************************************/
AstBreakStmtNode::AstBreakStmtNode(
    AstExprNode* cond
    )
: mCond( cond )
{
}

AstBreakStmtNode::~AstBreakStmtNode()
{
    delete mCond;
}

void
AstBreakStmtNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    fprintf(
        fp,
        "%*cbreak if\n",
        off, ' ' );

    mCond->print( off + 1, fp );
}

bool
AstBreakStmtNode::translate(
    tree& t,
    tree ctx,
    SymTable& symTable
    ) const
{
    tree cond;
    if( !mCond->translate( cond, ctx, symTable ) )
        return false;

    t = build1( EXIT_EXPR, void_type_node, cond );
    return true;
}

/*************************************************************************/
/* AstExitStmtNode                                                       */
/*************************************************************************/
void
AstExitStmtNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    fprintf( fp, "%*cexit\n", off, ' ' );
}

bool
AstExitStmtNode::translate(
    tree& t,
    tree,
    SymTable& symTable
    ) const
{
    t = build1(
        RETURN_EXPR,
        void_type_node,
        symTable.getRes() );
    return true;
}

/*************************************************************************/
/* AstReadStmtNode                                                       */
/*************************************************************************/
AstReadStmtNode::AstReadStmtNode(
    const char* fmt,
    AstExprNode* expr
    )
: mFmt( new AstStrExprNode( fmt ) ),
  mExpr( expr )
{
}

AstReadStmtNode::~AstReadStmtNode()
{
    delete mFmt;
    delete mExpr;
}

void
AstReadStmtNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    fprintf(
        fp,
        "%*cscanf(\n",
        off, ' ' );

    mFmt->print( off + 1, fp );

    fprintf(
        fp,
        "%*c,\n",
        off, ' ' );

    mExpr->print( off + 1, fp );

    fprintf(
        fp,
        "%*c)\n",
        off, ' ' );
}

bool
AstReadStmtNode::translate(
    tree& t,
    tree ctx,
    SymTable& symTable
    ) const
{
    tree fmt, expr;
    if( !mFmt->translate( fmt, ctx, symTable ) ||
        !mExpr->translate( expr, ctx, symTable ) )
        return false;

    expr = build1(
        ADDR_EXPR, build_pointer_type(
            TREE_TYPE( expr ) ),
        expr );

    tree params = NULL_TREE;
    chainon( params, tree_cons(
                 NULL_TREE,
                 TREE_TYPE( fmt ),
                 NULL_TREE ) );
    chainon( params, tree_cons(
                 NULL_TREE,
                 TREE_TYPE( expr ),
                 NULL_TREE ) );

    tree resdecl = build_decl(
        BUILTINS_LOCATION, RESULT_DECL,
        NULL_TREE, integer_type_node );
    DECL_ARTIFICIAL( resdecl ) = true;
    DECL_IGNORED_P( resdecl ) = true;

    tree fntype = build_function_type(
        TREE_TYPE( resdecl ), params );
    tree fndecl = build_decl(
        UNKNOWN_LOCATION, FUNCTION_DECL,
        get_identifier( "scanf" ), fntype );
    DECL_ARGUMENTS( fndecl ) = NULL_TREE;
    DECL_RESULT( fndecl ) = resdecl;
    DECL_EXTERNAL( fndecl ) = true;

    tree* args = XNEWVEC( tree, 2 );
    args[0] = fmt;
    args[1] = expr;

    t = build_call_expr_loc_array(
        UNKNOWN_LOCATION, fndecl, 2, args );
    SET_EXPR_LOCATION( t, UNKNOWN_LOCATION );
    TREE_USED( t ) = true;

    return true;
}

/*************************************************************************/
/* AstWriteStmtNode                                                      */
/*************************************************************************/
AstWriteStmtNode::AstWriteStmtNode(
    const char* fmt,
    AstExprNode* expr
    )
: mFmt( new AstStrExprNode( fmt ) ),
  mExpr( expr )
{
}

AstWriteStmtNode::~AstWriteStmtNode()
{
    delete mFmt;
    delete mExpr;
}

void
AstWriteStmtNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    fprintf(
        fp,
        "%*cprintf(\n",
        off, ' ' );

    mFmt->print( off + 1, fp );

    fprintf(
        fp,
        "%*c,\n",
        off, ' ' );

    mExpr->print( off + 1, fp );

    fprintf(
        fp,
        "%*c)\n",
        off, ' ' );
}

bool
AstWriteStmtNode::translate(
    tree& t,
    tree ctx,
    SymTable& symTable
    ) const
{
    tree fmt, expr;
    if( !mFmt->translate( fmt, ctx, symTable ) ||
        !mExpr->translate( expr, ctx, symTable ) )
        return false;

    tree params = NULL_TREE;
    chainon( params, tree_cons(
                 NULL_TREE,
                 TREE_TYPE( fmt ),
                 NULL_TREE ) );
    chainon( params, tree_cons(
                 NULL_TREE,
                 TREE_TYPE( expr ),
                 NULL_TREE ) );

    tree resdecl = build_decl(
        BUILTINS_LOCATION, RESULT_DECL,
        NULL_TREE, integer_type_node );
    DECL_ARTIFICIAL( resdecl ) = true;
    DECL_IGNORED_P( resdecl ) = true;

    tree fntype = build_function_type(
        TREE_TYPE( resdecl ), params );
    tree fndecl = build_decl(
        UNKNOWN_LOCATION, FUNCTION_DECL,
        get_identifier( "printf" ), fntype );
    DECL_ARGUMENTS( fndecl ) = NULL_TREE;
    DECL_RESULT( fndecl ) = resdecl;
    DECL_EXTERNAL( fndecl ) = true;

    tree* args = XNEWVEC( tree, 2 );
    args[0] = fmt;
    args[1] = expr;

    t = build_call_expr_loc_array(
        UNKNOWN_LOCATION, fndecl, 2, args );
    SET_EXPR_LOCATION( t, UNKNOWN_LOCATION );
    TREE_USED( t ) = true;

    return true;
}
