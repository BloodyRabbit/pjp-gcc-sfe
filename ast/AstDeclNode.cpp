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
    t = symTable.lookupVar(
        mName.c_str() );

    if( NULL_TREE == t )
    {
        t = build_decl(
            UNKNOWN_LOCATION, VAR_DECL,
            get_identifier( mName.c_str() ),
            build_qualified_type(
                integer_type_node, TYPE_QUAL_CONST ) );
        TREE_ADDRESSABLE( t ) = true;
        TREE_USED( t ) = true;

        if( NULL_TREE == ctx )
        {
            // Global constant
            TREE_STATIC( t ) = true;
            TREE_PUBLIC( t ) = true;
        }

        DECL_INITIAL( t ) =
            build_int_cst(
                integer_type_node, mVal );

        symTable.registerVar(
            mName.c_str(), t );
    }

    return true;
}

bool
AstConstDeclNode::unregister(
    SymTable& symTable
    ) const
{
    return symTable.unregisterVar(
        mName.c_str() );
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
    t = symTable.lookupVar(
        mName.c_str() );

    if( NULL_TREE == t )
    {
        tree type;
        if( !mType->translate( type, ctx, symTable ) )
            return false;

        t = build_decl(
            UNKNOWN_LOCATION, VAR_DECL,
            get_identifier( mName.c_str() ),
            type );
        TREE_ADDRESSABLE( t ) = true;
        TREE_USED( t ) = true;

        if( NULL_TREE == ctx )
        {
            // Global variable
            TREE_STATIC( t ) = true;
            TREE_PUBLIC( t ) = true;
        }

        symTable.registerVar(
            mName.c_str(), t );
    }

    return true;
}

bool
AstVarDeclNode::unregister(
    SymTable& symTable
    ) const
{
    return symTable.unregisterVar(
        mName.c_str() );
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
             off, ' ', (NULL != mResType ? "function" : "procedure"),
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

    if( NULL != mResType )
    {
        fprintf( fp, "%*c) :\n", off, ' ' );
        mResType->print( off + 1, fp );
        fprintf( fp, "%*c;\n", off, ' ' );
    }
    else
        fprintf( fp, "%*c);\n", off, ' ' );

    if( NULL != mBody )
        mBody->print( off + 1, fp );
    else
        fprintf( fp, "%*cforward;\n", off, ' ' );
}

bool
AstFunDeclNode::translate(
    tree& t,
    tree ctx,
    SymTable& symTable
    ) const
{
    t = symTable.lookupFun(
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
        if( NULL == mResType )
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

        symTable.registerFun(
            mName.c_str(), t );
    }

    if( NULL != mBody )
    {
        if( NULL_TREE != DECL_SAVED_TREE( t ) )
        {
            fprintf( stderr, "Function `%s' defined more than once\n",
                     mName.c_str() );
            return false;
        }

        tree params_decl = DECL_ARGUMENTS( t );

        ArgList::const_iterator cur, end;
        cur = mArgs.begin();
        end = mArgs.end();
        for(; cur != end; ++cur )
        {
            symTable.registerVar(
                cur->first.c_str(), params_decl );

            params_decl = TREE_CHAIN( params_decl );
        }

        tree resdecl = DECL_RESULT( t );
        symTable.registerRes( resdecl );

        tree bind;
        if( !mBody->translate( bind, t, symTable ) )
            return false;

        symTable.unregisterRes();

        cur = mArgs.begin();
        end = mArgs.end();
        for(; cur != end; ++cur )
            symTable.unregisterVar(
                cur->first.c_str() );

        tree block =
            TREE_OPERAND( bind, 2 );

        DECL_INITIAL( t ) = block;
        DECL_SAVED_TREE( t ) = bind;
    }

    return true;
}

bool
AstFunDeclNode::unregister(
    SymTable& symTable
    ) const
{
    return symTable.unregisterFun(
        mName.c_str() );
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

        register_global_function_declaration( fun );
    }

    t = NULL_TREE;
    return true;
}

bool
AstProgDeclNode::unregister(
    SymTable&
    ) const
{
    fprintf( stderr, "Program `%s' cannot be unregistered\n",
             mName.c_str() );
    return false;
}
