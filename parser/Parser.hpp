/** @file
 * @brief Declaration of the parser.
 *
 * @author Jan Bobek
 * @since 31st May 2014
 */

#ifndef SFE__PARSER__PARSER_HPP__INCL__
#define SFE__PARSER__PARSER_HPP__INCL__

#include <vector>

#include "ast/AstDeclNode.hpp"
#include "ast/AstExprNode.hpp"
#include "ast/AstStmtNode.hpp"
#include "ast/AstTypeNode.hpp"
#include "parser/LexAnalyzer.hpp"

/**
 * @brief The parser class.
 *
 * @author Jan Bobek
 */
class Parser
{
public:
    /**
     * @brief Initializes the parser.
     *
     * @param[in] lexan
     *   The lexical analyzer to bind to.
     */
    Parser( LexAnalyzer& lexan );

    /**
     * @brief Parses the input.
     *
     * @param[out] ast
     *   Where to store the AST.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parse( AstNode*& ast );

protected:
    /**
     * @brief Peeks at the current lexelem.
     *
     * @return
     *   The current lexical token.
     */
    LexToken peek() const;
    /**
     * @brief Peeks at the current strval.
     *
     * @return
     *   The current strval.
     */
    const std::string& peekStr() const;
    /**
     * @brief Peeks at the current intval.
     *
     * @return
     *   The current intval.
     */
    int peekInt() const;

    /**
     * @brief Matches the current lexelem.
     *
     * @param[in] token
     *   The token to match.
     *
     * @retval true
     *   Match successful.
     * @retval false
     *   Token mismatch.
     */
    bool match( LexToken token );
    /**
     * @brief Reads the next lexelem.
     *
     * @retval true
     *   Reading successful.
     * @retval false
     *   Reading failed.
     */
    bool next();

    /**
     * @brief Parses a program.
     *
     * @param[out] prog
     *   Where to store the program.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseProgram( AstProgDeclNode*& prog );
    /**
     * @brief Parses a function argument list
     *   declaration.
     *
     * @param[in,out] args
     *   Where to store arguments for the function.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseFuncArgListDecl(
        AstFunDeclNode::ArgList& args
        );
    /**
     * @brief Parses rest of a function argument
     *   list declaration.
     *
     * @param[in,out] args
     *   Where to store arguments for the function.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseFuncArgListDeclRest(
        AstFunDeclNode::ArgList& args
        );
    /**
     * @brief Parses a function argument list.
     *
     * @param[in,out] fun
     *   Where to store arguments for the function.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseFuncArgList( AstFunExprNode*& fun );
    /**
     * @brief Parses rest of a function argument list.
     *
     * @param[in,out] fun
     *   Where to store arguments for the function.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseFuncArgListRest( AstFunExprNode*& fun );
    /**
     * @brief Parses a function body.
     *
     * @param[in,out] blk
     *   The block to append the statements to.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseFuncBody( AstBlkStmtNode*& blk );

    /**
     * @brief Parses global declarations.
     *
     * @param[out] funs
     *   Where to append function declarations.
     * @param[out] vars
     *   Where to append variable declarations.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseDeclsGlob(
        std::vector< AstDeclNode* >& funs,
        std::vector< AstLocDeclNode* >& vars
        );
    /**
     * @brief Parses local declarations.
     *
     * @param[in,out] into
     *   Where to append the declarations.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseDeclsLoc( std::vector< AstLocDeclNode* >& into );
    /**
     * @brief Parses a single global declaration.
     *
     * @param[out] funs
     *   Where to append function declarations.
     * @param[out] vars
     *   Where to append variable declarations.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseDeclGlob(
        std::vector< AstDeclNode* >& funs,
        std::vector< AstLocDeclNode* >& vars
        );
    /**
     * @brief Parses a single local declaration.
     *
     * @param[in,out] into
     *   Where to append the declarations.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseDeclLoc( std::vector< AstLocDeclNode* >& into );
    /**
     * @brief Parses rest of the const section.
     *
     * @param[in,out] into
     *   Where to append the declarations.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseConstRest( std::vector< AstLocDeclNode* >& into );
    /**
     * @brief Parses rest of the var section.
     *
     * @param[in,out] into
     *   Where to append the declarations.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseVarRest( std::vector< AstLocDeclNode* >& into );

    /**
     * @brief Parses a block.
     *
     * @param[in,out] blk
     *   The block to append the statements to.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseBlock( AstBlkStmtNode*& blk );
    /**
     * @brief Parses rest of a block.
     *
     * @param[in,out] blk
     *   The block to append the statements to.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseBlockRest( AstBlkStmtNode*& blk );
    /**
     * @brief Parses a statement.
     *
     * @param[in,out] blk
     *   The block to append the statement to.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseStmt( AstBlkStmtNode*& blk );
    /**
     * @brief Parses a statement starting with
     *   an identifier.
     *
     * @param[in] name
     *   The encountered identifier.
     * @param[in,out] blk
     *   The block to append the statement to.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseStmtIdent(
        const std::string& name,
        AstBlkStmtNode*& blk
        );
    /**
     * @brief Parses an lval statement starting with
     *   an identifier.
     *
     * @param[in] name
     *   The encountered identifier.
     * @param[out] expr
     *   Where to store the lval expr.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseStmtLvalIdent(
        const std::string& name,
        AstExprNode*& expr
        );
    /**
     * @brief Parses an else branch.
     *
     * @param[out] blk
     *   Where to store the else branch.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseElseBranch( AstBlkStmtNode*& blk );
    /**
     * @brief Parses a for direction.
     *
     * @param[out] inc
     *   true = to; false = downto
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseForDirection( bool& inc );

    /**
     * @brief Parses a 0-expression.
     *
     * @param[out] expr
     *   Where to store the expression.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseExpr0( AstExprNode*& expr );
    /**
     * @brief Parses rest of a 0-expression.
     *
     * @param[in,out] expr
     *   Where to store the expression.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseExpr0Rest( AstExprNode*& expr );
    /**
     * @brief Parses a 1-expression.
     *
     * @param[out] expr
     *   Where to store the expression.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseExpr1( AstExprNode*& expr );
    /**
     * @brief Parses rest of a 1-expression.
     *
     * @param[in,out] expr
     *   Where to store the expression.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseExpr1Rest( AstExprNode*& expr );
    /**
     * @brief Parses a 2-expression.
     *
     * @param[out] expr
     *   Where to store the expression.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseExpr2( AstExprNode*& expr );
    /**
     * @brief Parses rest of a 2-expression.
     *
     * @param[in,out] expr
     *   Where to store the expression.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseExpr2Rest( AstExprNode*& expr );
    /**
     * @brief Parses a 3-expression.
     *
     * @param[out] expr
     *   Where to store the expression.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseExpr3( AstExprNode*& expr );
    /**
     * @brief Parses rest of a 3-expression.
     *
     * @param[in,out] expr
     *   Where to store the expression.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseExpr3Rest( AstExprNode*& expr );
    /**
     * @brief Parses a 4-expression.
     *
     * @param[out] expr
     *   Where to store the expression.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseExpr4( AstExprNode*& expr );
    /**
     * @brief Parses rest of a 4-expression.
     *
     * @param[in,out] expr
     *   Where to store the expression.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseExpr4Rest( AstExprNode*& expr );
    /**
     * @brief Parses a 5-expression.
     *
     * @param[out] expr
     *   Where to store the expression.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseExpr5( AstExprNode*& expr );
    /**
     * @brief Parses a 5-expression starting
     *   with an identifier.
     *
     * @param[in] name
     *   The encountered identifier.
     * @param[out] expr
     *   Where to store the expression.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseExpr5Ident(
        const std::string& name,
        AstExprNode*& expr
        );

    /**
     * @brief Parses a number with optional minus.
     *
     * @param[out] num
     *   Where to store the parsed number.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseMinusNumber( int& num );
    /**
     * @brief Parses a type identifier.
     *
     * @param[out] type
     *   Where to store the parsed AstTypeNode.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseTypeIdent( AstTypeNode*& type );
    /**
     * @brief Parses rest of an identifier list.
     *
     * @param[in,out] idents
     *   Where to store the identifiers.
     *
     * @retval true
     *   Parsing successful.
     * @retval false
     *   Parsing failed.
     */
    bool parseIdentListRest(
        std::vector< std::string >& idents
        );

    /**
     * @brief Prints a parsing error.
     *
     * @return false
     */
    bool error();

    /// The bound lexical analyzer.
    LexAnalyzer& mLexan;
    /// The current lexical element.
    LexElem mElem;
};

#endif /* !SFE__PARSER__PARSER_HPP__INCL__ */
