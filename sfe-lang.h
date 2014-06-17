/** @file
 * @brief Declarations for Sample FrontEnd.
 *
 * @author Jan Bobek
 * @since 1st June 2014
 */

#ifndef SFE__SFE_LANG_H__INCL__
#define SFE__SFE_LANG_H__INCL__

#include "parser/LexAnalyzer.hpp"
#include "parser/Parser.hpp"
#include "parser/SymTable.hpp"

/**
 * @brief Registers a global function.
 *
 * @param[in] functionDecl
 *   Declaration of the function.
 */
void register_global_function_declaration( tree functionDecl );
/**
 * @brief Registers a global variable.
 *
 * @param[in] variable
 *   Declaration of the variable.
 */
void register_global_variable_declaration( tree variable );

#endif /* !SFE__SFE_LANG_H__INCL__ */
