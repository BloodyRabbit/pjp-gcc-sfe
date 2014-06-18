/** @file
 * @brief Definition of the AST declaration nodes.
 *
 * @author Jan Bobek
 * @since 5th June 2014
 */

#include "sfe-lang.h"
#include "ast/AstDeclNode.hpp"
#include "ast/AstStmtNode.hpp"
#include "ast/AstTypeNode.hpp"
#include "parser/SymTable.hpp"

/*************************************************************************/
/* AstDeclNode                                                           */
/*************************************************************************/
AstDeclNode::AstDeclNode(
    const char* name
    )
: mName( name )
{
}

/*************************************************************************/
/* AstLocDeclNode                                                        */
/*************************************************************************/
AstLocDeclNode::AstLocDeclNode(
    const char* name
    )
: AstDeclNode( name )
{
}

/*************************************************************************/
/* AstConstDeclNode                                                      */
/*************************************************************************/
AstConstDeclNode::AstConstDeclNode(
    const char* name,
    int val
    )
: AstLocDeclNode( name ),
  mVal( val )
{
}

void
AstConstDeclNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    fprintf( fp, "%*cconst %s = %d;\n",
             off, ' ', mName.c_str(), mVal );
}

bool
AstConstDeclNode::translate(
    tree& t,
    tree ctx,
    SymTable& symTable
    ) const
{
    t = symTable.getVar(
        mName.c_str() );

    if( NULL_TREE != t )
        return true;

    t = build_decl(
        UNKNOWN_LOCATION, VAR_DECL,
        get_identifier( mName.c_str() ),
        build_qualified_type(
            integer_type_node, TYPE_QUAL_CONST ) );
    TREE_ADDRESSABLE( t ) = true;
    TREE_USED( t ) = true;

    DECL_INITIAL( t ) =
        build_int_cst(
            integer_type_node, mVal );

    return symTable.addVar(
        mName.c_str(), t );
}

/*************************************************************************/
/* AstVarDeclNode                                                        */
/*************************************************************************/
AstVarDeclNode::AstVarDeclNode(
    const char* name,
    AstTypeNode* type
    )
: AstLocDeclNode( name ),
  mType( type )
{
}

AstVarDeclNode::~AstVarDeclNode()
{
    delete mType;
}

void
AstVarDeclNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    fprintf( fp, "%*cvar %s :\n",
             off, ' ', mName.c_str() );

    mType->print( off + 1, fp );

    fprintf( fp, "%*c;\n", off, ' ' );
}

bool
AstVarDeclNode::translate(
    tree& t,
    tree ctx,
    SymTable& symTable
    ) const
{
    t = mType->getSym(
        mName.c_str(), symTable );

    if( NULL_TREE != t )
        return true;

    tree type;
    if( !mType->translate(
            type, NULL_TREE, symTable ) )
        return false;

    t = build_decl(
        UNKNOWN_LOCATION, VAR_DECL,
        get_identifier( mName.c_str() ),
        type );
    TREE_ADDRESSABLE( t ) = true;
    TREE_USED( t ) = true;

    return mType->addSym(
        mName.c_str(), t, symTable );
}

/*************************************************************************/
/* AstFunDeclNode                                                        */
/*************************************************************************/
AstFunDeclNode::AstFunDeclNode(
    const char* name
    )
: AstDeclNode( name ),
  mResType( NULL ),
  mBody( NULL )
{
}

AstFunDeclNode::~AstFunDeclNode()
{
    ArgList::iterator cur, end;
    cur = mArgs.begin();
    end = mArgs.end();
    for(; cur != end; ++cur )
        delete cur->second;
    mArgs.clear();

    delete mResType;
    delete mBody;
}

bool
AstFunDeclNode::isProcedure() const
{
    return NULL == mResType;
}

bool
AstFunDeclNode::isForward() const
{
    return NULL == mBody;
}

void
AstFunDeclNode::addArg(
    const char* name,
    AstTypeNode* type
    )
{
    mArgs.push_back(
        Arg( name, type ) );
}

void
AstFunDeclNode::setResType(
    AstTypeNode* type
    )
{
    delete mResType;
    mResType = type;
}

void
AstFunDeclNode::setBody(
    AstBlkStmtNode* blk
    )
{
    delete mBody;
    mBody = blk;
}

void
AstFunDeclNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    fprintf( fp, "%*c%s %s(\n",
             off, ' ', (isProcedure() ? "procedure" : "function"),
             mName.c_str() );

    ArgList::const_iterator cur, end;
    cur = mArgs.begin();
    end = mArgs.end();
    for(; cur != end; ++cur )
    {
        fprintf( fp, "%*c%s :\n",
                 off + 1, ' ', cur->first.c_str() );
        cur->second->print( off + 2, fp );
    }

    if( isProcedure() )
        fprintf( fp, "%*c);\n", off, ' ' );
    else
    {
        fprintf( fp, "%*c) :\n", off, ' ' );
        mResType->print( off + 1, fp );
        fprintf( fp, "%*c;\n", off, ' ' );
    }

    if( isForward() )
        fprintf( fp, "%*cforward;\n", off, ' ' );
    else
        mBody->print( off + 1, fp );
}

bool
AstFunDeclNode::translate(
    tree& t,
    tree ctx,
    SymTable& symTable
    ) const
{
    t = symTable.getFun(
        mName.c_str() );

    if( NULL_TREE == t )
    {
        tree params = NULL_TREE;
        tree params_decl = NULL_TREE;

        ArgList::const_iterator cur, end;
        cur = mArgs.begin();
        end = mArgs.end();
        for(; cur != end; ++cur )
        {
            tree param;
            if( !cur->second->translate(
                    param, ctx, symTable ) )
                return false;

            tree param_decl =
                build_decl(
                    UNKNOWN_LOCATION,
                    PARM_DECL,
                    get_identifier( cur->first.c_str() ),
                    param );
            DECL_ARG_TYPE( param_decl ) = param;

            params = chainon(
                params,
                tree_cons(
                    NULL_TREE,
                    param,
                    NULL_TREE ) );
            params_decl = chainon(
                params_decl,
                param_decl );
        }

        tree restype;
        if( isProcedure() )
            restype = void_type_node;
        else if( !mResType->translate(
                     restype, ctx, symTable ) )
            return false;

        tree resdecl =
            build_decl(
                BUILTINS_LOCATION,
                RESULT_DECL,
                NULL_TREE,
                restype );
        tree fntype =
            build_function_type(
                TREE_TYPE( resdecl ),
                params );

        t = build_decl(
            UNKNOWN_LOCATION,
            FUNCTION_DECL,
            get_identifier( mName.c_str() ),
            fntype );
        DECL_ARGUMENTS( t ) = params_decl;
        DECL_RESULT( t ) = resdecl;
        TREE_STATIC( t ) = true;
        TREE_PUBLIC( t ) = true;

        if( !symTable.addFun(
                mName.c_str(), t ) )
            return false;
    }

    if( !isForward() )
    {
        if( NULL_TREE != DECL_SAVED_TREE( t ) )
        {
            fprintf( stderr, "Function `%s' defined more than once\n",
                     mName.c_str() );
            return false;
        }

        SymTable subTable( symTable );

        tree resdecl = DECL_RESULT( t );
        if( !isProcedure() &&
            (!subTable.setRes( resdecl ) ||
             !mResType->addSym(
                 mName.c_str(), resdecl, subTable )) )
            return false;

        tree params_decl = DECL_ARGUMENTS( t );
        ArgList::const_iterator cur, end;
        cur = mArgs.begin();
        end = mArgs.end();
        for(; cur != end; ++cur )
        {
            if( !cur->second->addSym(
                    cur->first.c_str(),
                    params_decl, subTable ) )
                return false;

            params_decl = TREE_CHAIN( params_decl );
        }

        tree bind;
        if( !mBody->translate( bind, t, subTable ) )
            return false;

        tree block =
            TREE_OPERAND( bind, 2 );

        DECL_INITIAL( t ) = block;
        DECL_SAVED_TREE( t ) = bind;
    }

    return true;
}

/*************************************************************************/
/* AstProgDeclNode                                                       */
/*************************************************************************/
AstProgDeclNode::AstProgDeclNode(
    const char* name
    )
: AstDeclNode( name )
{
}

AstProgDeclNode::~AstProgDeclNode()
{
    std::vector< AstFunDeclNode* >::iterator curf, endf;
    curf = mFunDecls.begin();
    endf = mFunDecls.end();
    for(; curf != endf; ++curf )
        delete *curf;
    mFunDecls.clear();
}

void
AstProgDeclNode::addFun(
    AstFunDeclNode* fun
    )
{
    mFunDecls.push_back( fun );
}

void
AstProgDeclNode::print(
    unsigned int off,
    FILE* fp
    ) const
{
    fprintf( fp, "%*cprogram %s\n", off, ' ', mName.c_str() );

    std::vector< AstFunDeclNode* >::const_iterator curf, endf;
    curf = mFunDecls.begin();
    endf = mFunDecls.end();
    for(; curf != endf; ++curf )
        (*curf)->print( off, fp );
}

bool
AstProgDeclNode::translate(
    tree& t,
    tree ctx,
    SymTable& symTable
    ) const
{
    std::vector< AstFunDeclNode* >::const_iterator curf, endf;
    curf = mFunDecls.begin();
    endf = mFunDecls.end();
    for(; curf != endf; ++curf )
    {
        tree fun;
        if( !(*curf)->translate( fun, ctx, symTable ) )
            return false;

        if( !(*curf)->isForward() )
            register_global_function_declaration( fun );
    }

    t = NULL_TREE;
    return true;
}
