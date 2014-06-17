/** @file
 * @brief Declaration of a symbol table.
 *
 * @author Jan Bobek
 * @since 13th June 2014
 */

#ifndef SFE__PARSER__SYM_TABLE_HPP__INCL__
#define SFE__PARSER__SYM_TABLE_HPP__INCL__

#include <map>
#include <string>

#include "ast/AstNode.hpp"

/**
 * @brief A symbol table.
 *
 * @author Jan Bobek
 */
class SymTable
{
public:
    /**
     * @brief Initializes the table.
     */
    SymTable();

    /**
     * @brief Obtains a registered result declaration.
     *
     * @return
     *   The result declaration.
     */
    tree getRes() const;
    /**
     * @brief Registers a result declaration.
     *
     * @param[in] resDecl
     *   The result declaration.
     *
     * @retval true
     *   Registration succeeded.
     * @retval false
     *   Registration failed.
     */
    bool registerRes( tree resDecl );
    /**
     * @brief Unregisters a result declaration.
     *
     * @retval true
     *   Unregistration succeeded.
     * @retval false
     *   Unregistration failed.
     */
    bool unregisterRes();

    /**
     * @brief Looks up a variable declaration.
     *
     * @param[in] name
     *   Name of the variable.
     *
     * @return
     *   Found variable declaration.
     */
    tree lookupVar( const char* name ) const;
    /**
     * @brief Registers a variable declaration.
     *
     * @param[in] name
     *   Name of the variable.
     * @param[in] varDecl
     *   The variable declaration.
     *
     * @retval true
     *   Registration succeeded.
     * @retval false
     *   Registration failed.
     */
    bool registerVar( const char* name, tree varDecl );
    /**
     * @brief Unregisters a variable declaration.
     *
     * @param[in] name
     *   Name of the variable.
     *
     * @retval true
     *   Unregistration succeeded.
     * @retval false
     *   Unregistration failed.
     */
    bool unregisterVar( const char* name );

    /**
     * @brief Looks up a function declaration.
     *
     * @param[in] name
     *   Name of the function.
     *
     * @return
     *   Found function declaration.
     */
    tree lookupFun( const char* name ) const;
    /**
     * @brief Registers a function declaration.
     *
     * @param[in] name
     *   Name of the function.
     * @param[in] funDecl
     *   The function declaration.
     *
     * @retval true
     *   Registration succeeded.
     * @retval false
     *   Registration failed.
     */
    bool registerFun( const char* name, tree funDecl );
    /**
     * @brief Unregisters a function declaration.
     *
     * @param[in] name
     *   Name of the function.
     *
     * @retval true
     *   Unregistration succeeded.
     * @retval false
     *   Unregistration failed.
     */
    bool unregisterFun( const char* name );

protected:
    /// Result declaration.
    tree mResDecl;

    /// Variable declarations.
    std::map< std::string, tree > mVarDecls;
    /// Function declarations.
    std::map< std::string, tree > mFunDecls;
};

#endif /* !SFE__PARSER__SYM_TABLE_HPP__INCL__ */
