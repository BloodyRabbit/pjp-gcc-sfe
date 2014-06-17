/** @file
 * @brief Declaration of the lexical analyzer.
 *
 * @author Jan Bobek
 * @since 30th May 2014
 */

#ifndef SFE__PARSER__LEXANALYZER_HPP__INCL__
#define SFE__PARSER__LEXANALYZER_HPP__INCL__

#include <cstdio>

#include <string>

/**
 * @brief The recognized lexical tokens.
 *
 * @author Jan Bobek
 */
enum LexToken
{
    LEXTOK_EOI = 0, ///< End of input.

    LEXTOK_IDENT,  ///< An identifier.
    LEXTOK_NUMBER, ///< A number.
    LEXTOK_STRING, ///< A string.

    LEXTOK_PLUS,  ///< A 'plus' symbol.
    LEXTOK_MINUS, ///< A 'minus' symbol.
    LEXTOK_MULT,  ///< A 'multiply' symbol.
    LEXTOK_DIV,   ///< A 'divide' keyword.
    LEXTOK_MOD,   ///< A 'modulo' keyword.

    LEXTOK_EQ,  ///< An 'equal' symbol.
    LEXTOK_NEQ, ///< A 'not-equal' symbol.
    LEXTOK_LT,  ///< A 'less-than' symbol.
    LEXTOK_LEQ, ///< A 'less-or-equal' symbol.
    LEXTOK_GT,  ///< A 'greater-than' symbol.
    LEXTOK_GEQ, ///< A 'greater-or-equal' symbol.

    LEXTOK_OR,  ///< An 'or' keyword.
    LEXTOK_AND, ///< An 'and' keyword.
    LEXTOK_NOT, ///< A 'not' symbol.

    LEXTOK_LPAR, ///< A 'left-paren' symbol.
    LEXTOK_RPAR, ///< A 'right-paren' symbol.
    LEXTOK_LBRA, ///< A 'left-bracket' symbol.
    LEXTOK_RBRA, ///< A 'right-bracket' symbol.

    LEXTOK_DOT,   ///< A 'dot' symbol.
    LEXTOK_DDOT,  ///< A 'double-dot' symbol.
    LEXTOK_COMMA, ///< A 'comma' symbol.
    LEXTOK_COL,   ///< A 'colon' symbol.
    LEXTOK_SCOL,  ///< A 'semicolon' symbol.
    LEXTOK_ASSGN, ///< An 'assign' symbol.

    LEXTOK_KW_VAR,   ///< A 'var' keyword.
    LEXTOK_KW_CONST, ///< A 'const' keyword.
    LEXTOK_KW_PROG,  ///< A 'program' keyword.
    LEXTOK_KW_FUNC,  ///< A 'function' keyword.
    LEXTOK_KW_PROC,  ///< A 'procedure' keyword.
    LEXTOK_KW_FWD,   ///< A 'forward' keyword.

    LEXTOK_KW_INT, ///< An 'integer' keyword.
    LEXTOK_KW_ARR, ///< An 'array' keyword.
    LEXTOK_KW_OF,  ///< An 'of' keyword.

    LEXTOK_KW_BEGIN, ///< A 'begin' keyword.
    LEXTOK_KW_END,   ///< An 'end' keyword.
    LEXTOK_KW_IF,    ///< An 'if' keyword.
    LEXTOK_KW_THEN,  ///< A 'then' keyword.
    LEXTOK_KW_ELSE,  ///< An 'else' keyword.
    LEXTOK_KW_WHILE, ///< A 'while' keyword.
    LEXTOK_KW_DO,    ///< A 'do' keyword.
    LEXTOK_KW_FOR,   ///< A 'for' keyword.
    LEXTOK_KW_TO,    ///< A 'to' keyword.
    LEXTOK_KW_DWNTO, ///< A 'downto' keyword.
    LEXTOK_KW_EXIT,  ///< An 'exit' keyword.

    LEXTOK_KW_INC,    ///< An 'inc' keyword.
    LEXTOK_KW_DEC,    ///< A 'dec' keyword.
    LEXTOK_KW_READLN, ///< A 'readln' keyword.
    LEXTOK_KW_WRITE,  ///< A 'write' keyword.
    LEXTOK_KW_WRITELN ///< A 'writeln' keyword.
};

/// Textual representation of the lexical tokens.
extern const char* const LEXTOK_STR[];

/**
 * @brief A lexical keyword.
 *
 * @author Jan Bobek
 */
struct LexKeyword
{
    /// Pointer to the keyword.
    const char* keyword;
    /// Lexical token of the keyword.
    LexToken token;
};

/// List of the recognized keywords.
extern const LexKeyword LEX_KEYWORDS[];
/// Number of recognized keywords.
extern const unsigned int LEX_KEYWORD_COUNT;

/**
 * @brief Looks up a string in the keyword table.
 *
 * @param[in] str
 *   The string to look up.
 *
 * @return
 *   Lexical token of the string.
 */
LexToken lexKeywordLookup( const char* str );

/**
 * @brief A single lexical element.
 *
 * @author Jan Bobek
 */
struct LexElem
{
    /// The lexical token.
    LexToken token;
    /// A string value, if applicable.
    std::string strval;
    /// An integer value, if applicable.
    int intval;
};

/**
 * @brief The lexical analyzer.
 *
 * @author Jan Bobek
 */
class LexAnalyzer
{
public:
    /**
     * @brief Initializes the stream.
     *
     * @param[in] file
     *   The stream to bind to.
     */
    LexAnalyzer( FILE* file );
    /**
     * @brief Closes the stream.
     */
    ~LexAnalyzer();

    /**
     * @brief Reads the next lexical element.
     *
     * @param[out] elem
     *   Where to store the lexelem info.
     *
     * @retval true
     *   Reading successful.
     * @retval false
     *   Reading failed.
     */
    bool read( LexElem& elem );

protected:
    /**
     * @brief Peeks at the next input character.
     *
     * @return
     *   The next input char.
     */
    int peekc() const;
    /**
     * @brief Obtains an input character and loads the next one.
     *
     * @return
     *   The input char.
     */
    int getc();

    /**
     * @brief Reads an identifier.
     *
     * @param[out] elem
     *   Where to store the lexelem info.
     *
     * @retval true
     *   Reading successful.
     * @retval false
     *   Reading failed.
     */
    bool readIdent( LexElem& elem );
    /**
     * @brief Reads an octal number.
     *
     * @param[out] elem
     *   Where to store the lexelem info.
     *
     * @retval true
     *   Reading successful.
     * @retval false
     *   Reading failed.
     */
    bool readNumberOct( LexElem& elem );
    /**
     * @brief Reads a decimal number.
     *
     * @param[out] elem
     *   Where to store the lexelem info.
     *
     * @retval true
     *   Reading successful.
     * @retval false
     *   Reading failed.
     */
    bool readNumberDec( LexElem& elem );
    /**
     * @brief Reads a hexadecimal number.
     *
     * @param[out] elem
     *   Where to store the lexelem info.
     *
     * @retval true
     *   Reading successful.
     * @retval false
     *   Reading failed.
     */
    bool readNumberHex( LexElem& elem );
    /**
     * @brief Reads a string.
     *
     * @param[out] elem
     *   Where to store the lexelem info.
     *
     * @retval true
     *   Reading successful.
     * @retval false
     *   Reading failed.
     */
    bool readString( LexElem& elem );

    /// The bound stream.
    FILE* mFile;
    /// Next character in the stream.
    int mNext;
};

#endif /* !SFE__PARSER__LEXANALYZER_HPP__INCL__ */
