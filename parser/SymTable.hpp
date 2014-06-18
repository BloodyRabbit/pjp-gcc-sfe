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
     * @brief Sets a result declaration.
     *
     * @param[in] resDecl
     *   The result declaration.
     *
     * @retval true
     *   Registration succeeded.
     * @retval false
     *   Registration failed.
     */
    bool setRes( tree resDecl );

    /**
     * @brief Looks up a variable declaration.
     *
     * @param[in] name
     *   Name of the variable.
     *
     * @return
     *   Found variable declaration.
     */
    tree getVar( const char* name ) const;
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
    bool addVar( const char* name, tree varDecl );

    /**
     * @brief Looks up an array declaration.
     *
     * @param[in] name
     *   Name of the array.
     * @param[out] off
     *   Array index offset.
     *
     * @return
     *   Found array declaration.
     */
    tree getArr( const char* name, int& off ) const;
    /**
     * @brief Registers an array declaration.
     *
     * @param[in] name
     *   Name of the variable.
     * @param[in] off
     *   Array index offset.
     * @param[in] arrDecl
     *   The array declaration.
     *
     * @retval true
     *   Registration succeeded.
     * @retval false
     *   Registration failed.
     */
    bool addArr(
        const char* name,
        int off,
        tree arrDecl
        );

    /**
     * @brief Looks up a function declaration.
     *
     * @param[in] name
     *   Name of the function.
     *
     * @return
     *   Found function declaration.
     */
    tree getFun( const char* name ) const;
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
    bool addFun( const char* name, tree funDecl );

protected:
    /// Result declaration.
    tree mResDecl;

    /// Variable/constant declarations.
    std::map< std::string, tree > mVarDecls;
    /// Array declarations.
    std::map< std::string, std::pair< int, tree > > mArrDecls;
    /// Function declarations.
    std::map< std::string, tree > mFunDecls;
};

#endif /* !SFE__PARSER__SYM_TABLE_HPP__INCL__ */
