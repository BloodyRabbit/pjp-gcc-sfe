/** @file
 * @brief Definition of a symbol table.
 *
 * @author Jan Bobek
 * @since 13th June 2014
 */

#include "parser/SymTable.hpp"

/*************************************************************************/
/* SymTable                                                              */
/*************************************************************************/
SymTable::SymTable()
: mResDecl( NULL_TREE )
{
}

tree
SymTable::getRes() const
{
    return mResDecl;
}

bool
SymTable::registerRes(
    tree resDecl
    )
{
    if( NULL_TREE != mResDecl )
    {
        fprintf( stderr, "Trying to register more than one result declaration\n" );
        return false;
    }

    mResDecl = resDecl;
    return true;
}

bool
SymTable::unregisterRes()
{
    if( NULL_TREE == mResDecl )
    {
        fprintf( stderr, "Trying to unregister nonexistent result declaration\n" );
        return false;
    }

    mResDecl = NULL_TREE;
    return true;
}

tree
SymTable::lookupVar(
    const char* name
    ) const
{
    std::map< std::string, tree >::const_iterator itr;
    itr = mVarDecls.find( name );
    if( mVarDecls.end() == itr )
    {
#ifdef DEBUG_SYMTABLE
        fprintf( stderr, "Lookup of variable `%s' failed\n", name );
#endif /* DEBUG_SYMTABLE */

        return NULL_TREE;
    }

#ifdef DEBUG_SYMTABLE
    fprintf( stderr, "Variable `%s' looked up\n", name );
#endif /* DEBUG_SYMTABLE */

    return itr->second;
}

bool
SymTable::registerVar(
    const char* name,
    tree varDecl
    )
{
    std::pair< std::map< std::string, tree >::iterator, bool > res;
    res = mVarDecls.insert(
        std::make_pair( std::string( name ), varDecl ) );
    if( !res.second )
    {
        fprintf( stderr, "Variable `%s' already exists\n", name );
        return false;
    }

#ifdef DEBUG_SYMTABLE
    fprintf( stderr, "Registered variable `%s'\n", name );
    debug_tree( varDecl );
#endif /* DEBUG_SYMTABLE */

    return true;
}

bool
SymTable::unregisterVar(
    const char* name
    )
{
    if( mVarDecls.erase( name ) < 1 )
    {
        fprintf( stderr, "Variable `%s' does not exist\n", name );
        return false;
    }

#ifdef DEBUG_SYMTABLE
    fprintf( stderr, "Unregistered variable `%s'\n", name );
#endif /* DEBUG_SYMTABLE */
    return true;
}

tree
SymTable::lookupFun(
    const char* name
    ) const
{
    std::map< std::string, tree >::const_iterator itr;
    itr = mFunDecls.find( name );
    if( mFunDecls.end() == itr )
    {
#ifdef DEBUG_SYMTABLE
        fprintf( stderr, "Lookup of function `%s' failed\n", name );
#endif /* DEBUG_SYMTABLE */

        return NULL_TREE;
    }

#ifdef DEBUG_SYMTABLE
    fprintf( stderr, "Function `%s' looked up\n", name );
#endif /* DEBUG_SYMTABLE */

    return itr->second;
}

bool
SymTable::registerFun(
    const char* name,
    tree funDecl
    )
{
    std::pair< std::map< std::string, tree >::iterator, bool > res;
    res = mFunDecls.insert(
        std::make_pair( std::string( name ), funDecl ) );
    if( !res.second )
    {
        fprintf( stderr, "Function `%s' already exists\n", name );
        return false;
    }

#ifdef DEBUG_SYMTABLE
    fprintf( stderr, "Registered function `%s'\n", name );
    debug_tree( funDecl );
#endif /* DEBUG_SYMTABLE */

    return true;
}

bool
SymTable::unregisterFun(
    const char* name
    )
{
    if( mFunDecls.erase( name ) < 1 )
    {
        fprintf( stderr, "Function `%s' does not exist\n", name );
        return false;
    }

#ifdef DEBUG_SYMTABLE
    fprintf( stderr, "Unregistered function `%s'\n", name );
#endif /* DEBUG_SYMTABLE */

    return true;
}
