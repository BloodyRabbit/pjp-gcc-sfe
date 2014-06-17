/** @file
 * @brief Implementation of the lexical analyzer.
 *
 * @author Jan Bobek
 * @since 30th May 2014
 */

#include <cctype>
#include <cstring>

#include "parser/LexAnalyzer.hpp"

const char* const LEXTOK_STR[] =
{
    "end of input", // LEXTOK_EOI

    "identifier", // LEXTOK_IDENT
    "number", // LEXTOK_NUMBER
    "string", // LEXTOK_STRING

    "symbol plus", // LEXTOK_PLUS
    "symbol minus", // LEXTOK_MINUS
    "symbol multiply", // LEXTOK_MULT
    "keyword div", // LEXTOK_DIV
    "keyword mod", // LEXTOK_MOD

    "symbol equal", // LEXTOK_EQ
    "symbol not-equal", // LEXTOK_NEQ
    "symbol less-than", // LEXTOK_LT
    "symbol less-or-equal", // LEXTOK_LEQ
    "symbol greater-than", // LEXTOK_GT
    "symbol greater-or-equal", // LEXTOK_GEQ

    "keyword or", // LEXTOK_OR
    "keyword and", // LEXTOK_AND
    "symbol not", // LEXTOK_NOT

    "symbol left-paren", // LEXTOK_LPAR
    "symbol right-paren", // LEXTOK_RPAR
    "symbol left-bracket", // LEXTOK_LBRA
    "symbol right-bracket", // LEXTOK_RBRA

    "symbol dot", // LEXTOK_DOT
    "symbol double-dot", // LEXTOK_DDOT
    "symbol comma", // LEXTOK_COMMA
    "symbol colon", // LEXTOK_COL
    "symbol semicolon", // LEXTOK_SCOL
    "symbol assign", // LEXTOK_ASSGN

    "keyword var", // LEXTOK_KW_VAR
    "keyword const", // LEXTOK_KW_CONST
    "keyword program", // LEXTOK_KW_PROG
    "keyword function", // LEXTOK_KW_FUNC
    "keyword procedure", // LEXTOK_KW_PROC
    "keyword forward", // LEXTOK_KW_FWD

    "keyword integer", // LEXTOK_KW_INT
    "keyword array", // LEXTOK_KW_ARR
    "keyword of", // LEXTOK_KW_OF

    "keyword begin", // LEXTOK_KW_BEGIN
    "keyword end", // LEXTOK_KW_END
    "keyword if", // LEXTOK_KW_IF
    "keyword then", // LEXTOK_KW_THEN
    "keyword else", // LEXTOK_KW_ELSE
    "keyword while", // LEXTOK_KW_WHILE
    "keyword do", // LEXTOK_KW_DO
    "keyword for", // LEXTOK_KW_FOR
    "keyword to", // LEXTOK_KW_TO
    "keyword downto", // LEXTOK_KW_DWNTO
    "keyword exit", // LEXTOK_KW_EXIT

    "keyword inc", // LEXTOK_KW_INC
    "keyword dec", // LEXTOK_KW_DEC
    "keyword readln", // LEXTOK_KW_READLN
    "keyword write", // LEXTOK_KW_WRITE
    "keyword writeln", // LEXTOK_KW_WRITELN
};

// Must be sorted alphabetically!
const LexKeyword LEX_KEYWORDS[] =
{
    { "and",       LEXTOK_AND },
    { "array",     LEXTOK_KW_ARR },
    { "begin",     LEXTOK_KW_BEGIN },
    { "const",     LEXTOK_KW_CONST },
    { "dec",       LEXTOK_KW_DEC },
    { "div",       LEXTOK_DIV },
    { "do",        LEXTOK_KW_DO },
    { "downto",    LEXTOK_KW_DWNTO },
    { "else",      LEXTOK_KW_ELSE },
    { "end",       LEXTOK_KW_END },
    { "exit",      LEXTOK_KW_EXIT },
    { "for",       LEXTOK_KW_FOR },
    { "forward",   LEXTOK_KW_FWD },
    { "function",  LEXTOK_KW_FUNC },
    { "if",        LEXTOK_KW_IF },
    { "inc",       LEXTOK_KW_INC },
    { "integer",   LEXTOK_KW_INT },
    { "mod",       LEXTOK_MOD },
    { "of",        LEXTOK_KW_OF },
    { "or",        LEXTOK_OR },
    { "procedure", LEXTOK_KW_PROC },
    { "program",   LEXTOK_KW_PROG },
    { "readln",    LEXTOK_KW_READLN },
    { "then",      LEXTOK_KW_THEN },
    { "to",        LEXTOK_KW_TO },
    { "var",       LEXTOK_KW_VAR },
    { "while",     LEXTOK_KW_WHILE },
    { "write",     LEXTOK_KW_WRITE },
    { "writeln",   LEXTOK_KW_WRITELN },
};

const unsigned int LEX_KEYWORD_COUNT =
    sizeof(LEX_KEYWORDS) / sizeof(*LEX_KEYWORDS);

LexToken
lexKeywordLookup(
    const char* str
    )
{
    int res;
    unsigned int l, r, m;

    l = 0;
    r = LEX_KEYWORD_COUNT;
    while( l < r )
    {
        m = (l + r) / 2;
        res = strcmp(
            str, LEX_KEYWORDS[m].keyword );

        if( res > 0 )
            l = m + 1;
        else if( res < 0 )
            r = m;
        else /* res == 0 */
            return LEX_KEYWORDS[m].token;
    }

    return LEXTOK_IDENT;
}

/*************************************************************************/
/* LexAnalyzer                                                           */
/*************************************************************************/
LexAnalyzer::LexAnalyzer(
    FILE* file
    )
: mFile( file ),
  mNext( '\0' )
{
    // Load the next character immediately.
    getc();
}

LexAnalyzer::~LexAnalyzer()
{
    fclose( mFile );
}

bool
LexAnalyzer::read(
    LexElem& elem
    )
{
    while( isspace( peekc() ) )
        getc();

    if( isalpha( peekc() ) || '_' == peekc() )
        return readIdent( elem );
    else if( isdigit( peekc() ) )
        return readNumberDec( elem );

    switch( peekc() )
    {
        case EOF: elem.token = LEXTOK_EOI; return true;

        case '&': getc(); return readNumberOct( elem );
        case '$': getc(); return readNumberHex( elem );
        case '\'': getc(); return readString( elem );

        case '+': getc(); elem.token = LEXTOK_PLUS;  return true;
        case '-': getc(); elem.token = LEXTOK_MINUS; return true;
        case '*': getc(); elem.token = LEXTOK_MULT;  return true;
        case '/': getc(); elem.token = LEXTOK_DIV;   return true;

        case '=': getc(); elem.token = LEXTOK_EQ; return true;
        case '<': getc();
            if( '>' == peekc() )
                getc(), elem.token = LEXTOK_NEQ;
            else if( '=' == peekc() )
                getc(), elem.token = LEXTOK_LEQ;
            else
                elem.token = LEXTOK_LT;
            return true;
        case '>': getc();
            if( '=' == peekc() )
                getc(), elem.token = LEXTOK_GEQ;
            else
                elem.token = LEXTOK_GT;
            return true;

        case '(': getc(); elem.token = LEXTOK_LPAR; return true;
        case ')': getc(); elem.token = LEXTOK_RPAR; return true;
        case '[': getc(); elem.token = LEXTOK_LBRA; return true;
        case ']': getc(); elem.token = LEXTOK_RBRA; return true;

        case ',': getc(); elem.token = LEXTOK_COMMA; return true;
        case ';': getc(); elem.token = LEXTOK_SCOL;  return true;
        case '.': getc();
            if( '.' == peekc() )
                getc(), elem.token = LEXTOK_DDOT;
            else
                elem.token = LEXTOK_DOT;
            return true;
        case ':': getc();
            if( '=' == peekc() )
                getc(), elem.token = LEXTOK_ASSGN;
            else
                elem.token = LEXTOK_COL;
            return true;
    }

    fprintf( stderr, "Unknown character '%c' encountered\n",
             peekc() );
    return false;
}

int
LexAnalyzer::peekc() const
{
    return mNext;
}

int
LexAnalyzer::getc()
{
    int c = mNext;
    mNext = fgetc( mFile );
    return c;
}

bool
LexAnalyzer::readIdent(
    LexElem& elem
    )
{
    elem.strval.clear();

    do
    {
        elem.strval.push_back( getc() );
    } while( isalnum( peekc() ) || '_' == peekc() );

    elem.token = lexKeywordLookup(
        elem.strval.c_str() );
    return true;
}

bool
LexAnalyzer::readNumberOct(
    LexElem& elem
    )
{
    elem.token = LEXTOK_NUMBER;
    elem.intval = 0;

    if( !('0' <= peekc() && peekc() <= '7') )
    {
        fprintf( stderr, "Invalid octal digit '%c'\n",
                 peekc() );
        return false;
    }

    do
    {
        elem.intval = elem.intval * 8 + getc() - '0';
    } while( '0' <= peekc() && peekc() <= '7' );

    return true;
}

bool
LexAnalyzer::readNumberDec(
    LexElem& elem
    )
{
    elem.token = LEXTOK_NUMBER;
    elem.intval = 0;

    if( !isdigit( peekc() ) )
    {
        fprintf( stderr, "Invalid decimal digit '%c'\n",
                 peekc() );
        return false;
    }

    do
    {
        elem.intval = elem.intval * 10 + getc() - '0';
    } while( isdigit( peekc() ) );

    return true;
}

bool
LexAnalyzer::readNumberHex(
    LexElem& elem
    )
{
    elem.token = LEXTOK_NUMBER;
    elem.intval = 0;

    if( !isxdigit( peekc() ) )
    {
        fprintf( stderr, "Invalid hexadecimal digit '%c'\n",
                 peekc() );
        return false;
    }

    do
    {
        elem.intval = elem.intval * 16 +
            (isdigit( peekc() )
             ? getc() - '0'
             : tolower( getc() ) - 'a' + 10);
    } while( isxdigit( peekc() ) );

    return true;
}

bool
LexAnalyzer::readString(
    LexElem& elem
    )
{
    elem.token = LEXTOK_STRING;
    elem.strval.clear();

    while( '\'' != peekc() )
    {
        if( EOF == peekc() )
        {
            fprintf( stderr, "Encountered unterminated string literal\n" );
            return false;
        }

        elem.strval.push_back( getc() );
    }

    // Eat terminal quote
    getc();
    return true;
}
