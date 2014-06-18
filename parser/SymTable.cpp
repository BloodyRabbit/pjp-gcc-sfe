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
SymTable::setRes(
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

tree
SymTable::getVar(
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
SymTable::addVar(
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

tree
SymTable::getArr(
    const char* name,
    int& off
    ) const
{
    std::map< std::string, std::pair< int, tree > >::const_iterator itr;
    itr = mArrDecls.find( name );
    if( mArrDecls.end() == itr )
    {
#ifdef DEBUG_SYMTABLE
        fprintf( stderr, "Lookup of array `%s' failed\n", name );
#endif /* DEBUG_SYMTABLE */

        return NULL_TREE;
    }

#ifdef DEBUG_SYMTABLE
    fprintf( stderr, "Array `%s' looked up\n", name );
#endif /* DEBUG_SYMTABLE */

    off = itr->second.first;
    return itr->second.second;
}

bool
SymTable::addArr(
    const char* name,
    int off,
    tree arrDecl
    )
{
    std::pair< std::map< std::string, std::pair< int, tree > >::iterator, bool > res;
    res = mArrDecls.insert(
        std::make_pair(
            std::string( name ),
            std::make_pair( off, arrDecl ) ) );
    if( !res.second )
    {
        fprintf( stderr, "Array `%s' already exists\n", name );
        return false;
    }

#ifdef DEBUG_SYMTABLE
    fprintf( stderr, "Registered array `%s'\n", name );
    debug_tree( arrDecl );
#endif /* DEBUG_SYMTABLE */

    return true;
}

tree
SymTable::getFun(
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
SymTable::addFun(
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
