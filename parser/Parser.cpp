/** @file
 * @brief Implementation of the parser.
 *
 * @author Jan Bobek
 * @since 31st May 2014
 */

#include "parser/Parser.hpp"

/*************************************************************************/
/* Parser                                                                */
/*************************************************************************/
Parser::Parser(
    LexAnalyzer& lexan
    )
: mLexan( lexan )
{
}

bool
Parser::parse(
    AstNode*& ast
    )
{
    AstProgDeclNode* prog;

    if( !next() ||
        !parseProgram( prog ) )
        return false;

    ast = prog;
    return true;
}

LexToken
Parser::peek() const
{
    return mElem.token;
}

const std::string&
Parser::peekStr() const
{
    return mElem.strval;
}

int
Parser::peekInt() const
{
    return mElem.intval;
}

bool
Parser::match(
    LexToken token
    )
{
    if( peek() != token )
    {
        fprintf( stderr, "Token mismatch: expected `%s', got `%s' instead\n",
                 LEXTOK_STR[token], LEXTOK_STR[peek()] );
        return false;
    }

    return next();
}

bool
Parser::next()
{
    return mLexan.read( mElem );
}

bool
Parser::parseProgram(
    AstProgDeclNode*& prog
    )
{
    AstFunDeclNode* fun;
    AstBlkStmtNode* blk;

#ifdef DEBUG_PARSER
    fprintf( stderr, "parseProgram\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
    case LEXTOK_KW_PROG:
        if( !match( LEXTOK_KW_PROG ) )
            return false;

        prog = new AstProgDeclNode(
            peekStr().c_str() );
        blk = new AstBlkStmtNode();

        if( !match( LEXTOK_IDENT ) ||
            !match( LEXTOK_SCOL ) ||
            !parseDeclsGlob( prog ) ||
            !parseBlock( blk ) ||
            !match( LEXTOK_DOT ) ||
            !match( LEXTOK_EOI ) )
            return false;

        fun = new AstFunDeclNode( "main" );
        fun->setResType( new AstIntTypeNode() );
        fun->setBody( blk );

        prog->addFun( fun );
        return true;

    default:
        return error();
    }
}

bool
Parser::parseDeclsGlob(
    AstProgDeclNode* prog
    )
{
    AstFunDeclNode* fun;
    AstTypeNode* resType;

#ifdef DEBUG_PARSER
    fprintf( stderr, "parseDeclsGlob\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
    case LEXTOK_KW_FUNC:
        if( !match( LEXTOK_KW_FUNC ) )
            return false;

        fun = new AstFunDeclNode(
            peekStr().c_str() );

        if( !match( LEXTOK_IDENT ) ||
            !match( LEXTOK_LPAR ) ||
            !parseFunDeclArgs( fun ) ||
            !match( LEXTOK_RPAR ) ||
            !match( LEXTOK_COL ) ||
            !parseTypeIdent( resType ) ||
            !match( LEXTOK_SCOL ) ||
            !parseFunDeclBody( fun ) ||
            !match( LEXTOK_SCOL ) )
            return false;

        fun->setResType( resType );
        prog->addFun( fun );

        return parseDeclsGlob( prog );

    case LEXTOK_KW_PROC:
        if( !match( LEXTOK_KW_PROC ) )
            return false;

        fun = new AstFunDeclNode(
            peekStr().c_str() );

        if( !match( LEXTOK_IDENT ) ||
            !match( LEXTOK_LPAR ) ||
            !parseFunDeclArgs( fun ) ||
            !match( LEXTOK_RPAR ) ||
            !match( LEXTOK_SCOL ) ||
            !parseFunDeclBody( fun ) ||
            !match( LEXTOK_SCOL ) )
            return false;

        fun->setResType( NULL );
        prog->addFun( fun );

        return parseDeclsGlob( prog );

    case LEXTOK_KW_CONST:
    case LEXTOK_KW_VAR:
    case LEXTOK_KW_BEGIN:
        return true;

    default:
        return error();
    }
}

bool
Parser::parseFunDeclArgs(
    AstFunDeclNode* fun
    )
{
    std::string name;
    AstTypeNode* type;

#ifdef DEBUG_PARSER
    fprintf( stderr, "parseFunDeclArgs\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
    case LEXTOK_IDENT:
        name = peekStr();

        if( !match( LEXTOK_IDENT ) ||
            !match( LEXTOK_COL ) ||
            !parseTypeIdent( type ) )
            return false;

        fun->addArg( name.c_str(), type );
        return parseFunDeclArgsRest( fun );

    case LEXTOK_RPAR:
        return true;

    default:
        return error();
    }
}

bool
Parser::parseFunDeclArgsRest(
    AstFunDeclNode* fun
    )
{
    std::string name;
    AstTypeNode* type;

#ifdef DEBUG_PARSER
    fprintf( stderr, "parseFunDeclArgsRest\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
    case LEXTOK_SCOL:
        if( !match( LEXTOK_SCOL ) )
            return false;

        name = peekStr();
        if( !match( LEXTOK_IDENT ) ||
            !match( LEXTOK_COL ) ||
            !parseTypeIdent( type ) )
            return false;

        fun->addArg( name.c_str(), type );
        return parseFunDeclArgsRest( fun );

    case LEXTOK_RPAR:
        return true;

    default:
        return error();
    }
}

bool
Parser::parseFunDeclBody(
    AstFunDeclNode* fun
    )
{
    AstBlkStmtNode* blk;

#ifdef DEBUG_PARSER
    fprintf( stderr, "parseFunDeclBody\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
    case LEXTOK_KW_CONST:
    case LEXTOK_KW_VAR:
    case LEXTOK_KW_BEGIN:
        blk = new AstBlkStmtNode();
        if( !parseBlock( blk ) )
            return false;

        fun->setBody( blk );
        return true;

    case LEXTOK_KW_FWD:
        return match( LEXTOK_KW_FWD );

    default:
        return error();
    }
}

bool
Parser::parseDeclsLoc(
    AstBlkStmtNode* blk
    )
{
    int val;
    AstTypeNode* type;
    std::vector< std::string > idents;
    std::vector< std::string >::const_iterator cur, end;

#ifdef DEBUG_PARSER
    fprintf( stderr, "parseDeclsLoc\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
    case LEXTOK_KW_CONST:
        if( !match( LEXTOK_KW_CONST ) )
            return false;

        idents.push_back( peekStr() );

        if( !match( LEXTOK_IDENT ) ||
            !parseIdentListRest( idents ) ||
            !match( LEXTOK_EQ ) ||
            !parseMinusNumber( val ) ||
            !match( LEXTOK_SCOL ) )
            return false;

        cur = idents.begin();
        end = idents.end();
        for(; cur != end; ++cur )
            blk->addDecl(
                new AstConstDeclNode(
                    cur->c_str(), val ) );

        return parseConstDeclRest( blk )
            && parseDeclsLoc( blk );

    case LEXTOK_KW_VAR:
        if( !match( LEXTOK_KW_VAR ) )
            return false;

        idents.push_back( peekStr() );

        if( !match( LEXTOK_IDENT ) ||
            !parseIdentListRest( idents ) ||
            !match( LEXTOK_COL ) ||
            !parseTypeIdent( type ) ||
            !match( LEXTOK_SCOL ) )
            return false;

        cur = idents.begin();
        end = idents.end();

        blk->addDecl(
            new AstVarDeclNode(
                (cur++)->c_str(), type ) );

        for(; cur != end; ++cur )
            blk->addDecl(
                new AstVarDeclNode(
                    cur->c_str(),
                    type->clone() ) );

        return parseVarDeclRest( blk )
            && parseDeclsLoc( blk );

    case LEXTOK_KW_BEGIN:
        return true;

    default:
        return error();
    }
}

bool
Parser::parseConstDeclRest(
    AstBlkStmtNode* blk
    )
{
    int val;
    std::vector< std::string > idents;
    std::vector< std::string >::const_iterator cur, end;

#ifdef DEBUG_PARSER
    fprintf( stderr, "parseConstRest\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
    case LEXTOK_IDENT:
        idents.push_back( peekStr() );

        if( !match( LEXTOK_IDENT ) ||
            !parseIdentListRest( idents ) ||
            !match( LEXTOK_EQ ) ||
            !parseMinusNumber( val ) ||
            !match( LEXTOK_SCOL ) )
            return false;

        cur = idents.begin();
        end = idents.end();
        for(; cur != end; ++cur )
            blk->addDecl(
                new AstConstDeclNode(
                    cur->c_str(), val ) );

        return parseConstDeclRest( blk );

    case LEXTOK_KW_CONST:
    case LEXTOK_KW_VAR:
    case LEXTOK_KW_BEGIN:
        return true;

    default:
        return error();
    }
}

bool
Parser::parseVarDeclRest(
    AstBlkStmtNode* blk
    )
{
    AstTypeNode* type;
    std::vector< std::string > idents;
    std::vector< std::string >::const_iterator cur, end;

#ifdef DEBUG_PARSER
    fprintf( stderr, "parseVarDeclRest\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
    case LEXTOK_IDENT:
        idents.push_back( peekStr() );

        if( !match( LEXTOK_IDENT ) ||
            !parseIdentListRest( idents ) ||
            !match( LEXTOK_COL ) ||
            !parseTypeIdent( type ) ||
            !match( LEXTOK_SCOL ) )
            return false;

        cur = idents.begin();
        end = idents.end();

        blk->addDecl(
            new AstVarDeclNode(
                (cur++)->c_str(), type ) );

        for(; cur != end; ++cur )
            blk->addDecl(
                new AstVarDeclNode(
                    cur->c_str(),
                    type->clone() ) );

        return parseVarDeclRest( blk );

    case LEXTOK_KW_CONST:
    case LEXTOK_KW_VAR:
    case LEXTOK_KW_BEGIN:
        return true;

    default:
        return error();
    }
}

bool
Parser::parseBlock(
    AstBlkStmtNode* blk
    )
{
#ifdef DEBUG_PARSER
    fprintf( stderr, "parseBlock\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
    case LEXTOK_KW_CONST:
    case LEXTOK_KW_VAR:
    case LEXTOK_KW_BEGIN:
        return parseDeclsLoc( blk )
            && match( LEXTOK_KW_BEGIN )
            && parseStmt( blk )
            && parseBlockRest( blk )
            && match( LEXTOK_KW_END );

    default:
        return error();
    }
}

bool
Parser::parseBlockRest(
    AstBlkStmtNode* blk
    )
{
#ifdef DEBUG_PARSER
    fprintf( stderr, "parseBlockRest\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
    case LEXTOK_SCOL:
        return match( LEXTOK_SCOL )
            && parseStmt( blk )
            && parseBlockRest( blk );

    case LEXTOK_KW_END:
        return true;

    default:
        return error();
    }
}

bool
Parser::parseStmt(
    AstBlkStmtNode* blk
    )
{
    bool inc;
    std::string str;
    AstExprNode *expr1, *expr2;
    AstBlkStmtNode *bodyBlk, *elseBlk;

#ifdef DEBUG_PARSER
    fprintf( stderr, "parseStmt\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
    case LEXTOK_IDENT:
        str = peekStr();

        return match( LEXTOK_IDENT )
            && parseStmtIdent( str, blk );

    case LEXTOK_KW_IF:
        bodyBlk = new AstBlkStmtNode();
        elseBlk = NULL;

        if( !match( LEXTOK_KW_IF ) ||
            !parseExpr0( expr1 ) ||
            !match( LEXTOK_KW_THEN ) ||
            !parseStmt( bodyBlk ) ||
            !parseElseBranch( elseBlk ) )
            return false;

        blk->addStmt(
            new AstIfStmtNode(
                expr1, bodyBlk, elseBlk ) );
        return true;

    case LEXTOK_KW_WHILE:
        if( !match( LEXTOK_KW_WHILE ) ||
            !parseExpr0( expr1 ) ||
            !match( LEXTOK_KW_DO ) )
            return false;

        bodyBlk = new AstBlkStmtNode();
        bodyBlk->addStmt(
            new AstBreakStmtNode(
                new AstUnopExprNode(
                    LEXTOK_NOT, expr1 ) ) );

        if( !parseStmt( bodyBlk ) )
            return false;

        blk->addStmt(
            new AstLoopStmtNode(
                bodyBlk ) );

        return true;

    case LEXTOK_KW_FOR:
        if( !match( LEXTOK_KW_FOR ) )
            return false;

        str = peekStr();

        if( !match( LEXTOK_IDENT ) ||
            !match( LEXTOK_ASSGN ) ||
            !parseExpr0( expr1 ) ||
            !parseForDirection( inc ) ||
            !parseExpr0( expr2 ) ||
            !match( LEXTOK_KW_DO ) )
            return false;

        bodyBlk = new AstBlkStmtNode();
        bodyBlk->addStmt(
            new AstBreakStmtNode(
                new AstBinopExprNode(
                    (inc ? LEXTOK_GT : LEXTOK_LT),
                    new AstVarExprNode(
                        str.c_str() ),
                    expr2 ) ) );

        if( !parseStmt( bodyBlk ) )
            return false;

        bodyBlk->addStmt(
            new AstBinopStmtNode(
                (inc ? LEXTOK_KW_INC : LEXTOK_KW_DEC),
                new AstVarExprNode(
                    str.c_str() ),
                new AstIntExprNode(
                    1 ) ) );

        blk->addStmt(
            new AstBinopStmtNode(
                LEXTOK_ASSGN,
                new AstVarExprNode(
                    str.c_str() ),
                expr1 ) );
        blk->addStmt(
            new AstLoopStmtNode(
                bodyBlk ) );

        return true;

    case LEXTOK_KW_EXIT:
        blk->addStmt(
            new AstExitStmtNode() );

        return match( LEXTOK_KW_EXIT );

    case LEXTOK_KW_CONST:
    case LEXTOK_KW_VAR:
    case LEXTOK_KW_BEGIN:
        return parseBlock( blk );

    case LEXTOK_KW_INC:
        if( !match( LEXTOK_KW_INC ) ||
            !match( LEXTOK_LPAR ) )
            return false;

        str = peekStr();

        if( !match( LEXTOK_IDENT ) ||
            !parseStmtLvalIdent( str, expr1 ) ||
            !match( LEXTOK_RPAR ) )
            return false;

        blk->addStmt(
            new AstBinopStmtNode(
                LEXTOK_KW_INC,
                expr1,
                new AstIntExprNode(
                    1 ) ) );
        return true;

    case LEXTOK_KW_DEC:
        if( !match( LEXTOK_KW_DEC ) ||
            !match( LEXTOK_LPAR ) )
            return false;

        str = peekStr();

        if( !match( LEXTOK_IDENT ) ||
            !parseStmtLvalIdent( str, expr1 ) ||
            !match( LEXTOK_RPAR ) )
            return false;

        blk->addStmt(
            new AstBinopStmtNode(
                LEXTOK_KW_DEC,
                expr1,
                new AstIntExprNode(
                    1 ) ) );
        return true;

    case LEXTOK_KW_READLN:
        if( !match( LEXTOK_KW_READLN ) ||
            !match( LEXTOK_LPAR ) )
            return false;

        str = peekStr();

        if( !match( LEXTOK_IDENT ) ||
            !parseStmtLvalIdent( str, expr1 ) ||
            !match( LEXTOK_RPAR ) )
            return false;

        blk->addStmt(
            new AstReadStmtNode(
                "%d", expr1 ) );
        return true;

    case LEXTOK_KW_WRITE:
        if( !match( LEXTOK_KW_WRITE ) ||
            !match( LEXTOK_LPAR ) )
            return false;

        str = peekStr();

        if( !match( LEXTOK_STRING ) ||
            !match( LEXTOK_RPAR ) )
            return false;

        blk->addStmt(
            new AstWriteStmtNode(
                "%s", new AstStrExprNode(
                    str.c_str() ) ) );
        return true;

    case LEXTOK_KW_WRITELN:
        if( !match( LEXTOK_KW_WRITELN ) ||
            !match( LEXTOK_LPAR ) ||
            !parseExpr0( expr1 ) ||
            !match( LEXTOK_RPAR ) )
            return false;

        blk->addStmt(
            new AstWriteStmtNode(
                "%d\n", expr1 ) );
        return true;

    case LEXTOK_SCOL:
    case LEXTOK_KW_END:
        return true;

    default:
        return error();
    }
}

bool
Parser::parseStmtIdent(
    const std::string& name,
    AstBlkStmtNode* blk
    )
{
    AstExprNode *expr, *val;
    AstFunExprNode* fun;

#ifdef DEBUG_PARSER
    fprintf( stderr, "parseStmtIdent\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
    case LEXTOK_ASSGN:
    case LEXTOK_LBRA:
        if( !parseStmtLvalIdent( name, expr ) ||
            !match( LEXTOK_ASSGN ) ||
            !parseExpr0( val ) )
            return false;

        blk->addStmt(
            new AstBinopStmtNode(
                LEXTOK_ASSGN,
                expr, val ) );
        return true;

    case LEXTOK_LPAR:
        fun = new AstFunExprNode(
            name.c_str() );

        if( !match( LEXTOK_LPAR ) ||
            !parseFunExprArgs( fun ) ||
            !match( LEXTOK_RPAR ) )
            return false;

        blk->addStmt(
            new AstFunStmtNode(
                fun ) );
        return true;

    default:
        return error();
    }
}

bool
Parser::parseStmtLvalIdent(
    const std::string& name,
    AstExprNode*& expr
    )
{
#ifdef DEBUG_PARSER
    fprintf( stderr, "parseStmtLvalIdent\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
    case LEXTOK_LBRA:
        if( !match( LEXTOK_LBRA ) ||
            !parseExpr0( expr ) ||
            !match( LEXTOK_RBRA ) )
            return false;

        expr = new AstArrExprNode(
            name.c_str(), expr );
        return true;

    case LEXTOK_ASSGN:
    case LEXTOK_RPAR:
        expr = new AstVarExprNode(
            name.c_str() );
        return true;

    default:
        return error();
    }
}

bool
Parser::parseElseBranch(
    AstBlkStmtNode*& blk
    )
{
#ifdef DEBUG_PARSER
    fprintf( stderr, "parseElseBranch\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
    case LEXTOK_KW_ELSE:
        blk = new AstBlkStmtNode();

        return match( LEXTOK_KW_ELSE )
            && parseStmt( blk );

    case LEXTOK_SCOL:
    case LEXTOK_KW_END:
        blk = NULL;
        return true;

    default:
        return error();
    }
}

bool
Parser::parseForDirection(
    bool& inc
    )
{
#ifdef DEBUG_PARSER
    fprintf( stderr, "parseForDirection\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
    case LEXTOK_KW_TO:
        inc = true;
        return match( LEXTOK_KW_TO );

    case LEXTOK_KW_DWNTO:
        inc = false;
        return match( LEXTOK_KW_DWNTO );

    default:
        return error();
    }
}

bool
Parser::parseExpr0(
    AstExprNode*& expr
    )
{
#ifdef DEBUG_PARSER
    fprintf( stderr, "parseExpr0\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
        // Expr3:
    case LEXTOK_MINUS:
        // Expr5:
    case LEXTOK_IDENT:
    case LEXTOK_NUMBER:
    case LEXTOK_LPAR:
        return parseExpr1( expr )
            && parseExpr0Rest( expr );

    default:
        return error();
    }
}

bool
Parser::parseExpr0Rest(
    AstExprNode*& expr
    )
{
    AstExprNode* rop;

#ifdef DEBUG_PARSER
    fprintf( stderr, "parseExpr0Rest\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
        // Expr0:
    case LEXTOK_OR:
        if( !match( LEXTOK_OR ) ||
            !parseExpr1( rop ) )
            return true;

        expr = new AstBinopExprNode(
            LEXTOK_OR, expr, rop );

        return parseExpr0Rest( expr );

        // Expr0 follow:
    case LEXTOK_RPAR:
    case LEXTOK_RBRA:
    case LEXTOK_COMMA:
    case LEXTOK_SCOL:
    case LEXTOK_KW_THEN:
    case LEXTOK_KW_ELSE:
    case LEXTOK_KW_DO:
    case LEXTOK_KW_TO:
    case LEXTOK_KW_DWNTO:
    case LEXTOK_KW_END:
        return true;

    default:
        return error();
    }
}

bool
Parser::parseExpr1(
    AstExprNode*& expr
    )
{
#ifdef DEBUG_PARSER
    fprintf( stderr, "parseExpr1\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
        // Expr3:
    case LEXTOK_MINUS:
        // Expr5:
    case LEXTOK_IDENT:
    case LEXTOK_NUMBER:
    case LEXTOK_LPAR:
        return parseExpr2( expr )
            && parseExpr1Rest( expr );

    default:
        return error();
    }
}

bool
Parser::parseExpr1Rest(
    AstExprNode*& expr
    )
{
    AstExprNode* rop;

#ifdef DEBUG_PARSER
    fprintf( stderr, "parseExpr1Rest\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
        // Expr1:
    case LEXTOK_AND:
        if( !match( LEXTOK_AND ) ||
            !parseExpr2( rop ) )
            return false;

        expr = new AstBinopExprNode(
            LEXTOK_AND, expr, rop );

        return parseExpr1Rest( expr );

        // Expr0:
    case LEXTOK_OR:
        // Expr0 follow:
    case LEXTOK_RPAR:
    case LEXTOK_RBRA:
    case LEXTOK_COMMA:
    case LEXTOK_SCOL:
    case LEXTOK_KW_THEN:
    case LEXTOK_KW_ELSE:
    case LEXTOK_KW_DO:
    case LEXTOK_KW_TO:
    case LEXTOK_KW_DWNTO:
    case LEXTOK_KW_END:
        return true;

    default:
        return error();
    }
}

bool
Parser::parseExpr2(
    AstExprNode*& expr
    )
{
#ifdef DEBUG_PARSER
    fprintf( stderr, "parseExpr2\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
        // Expr3:
    case LEXTOK_MINUS:
        // Expr5:
    case LEXTOK_IDENT:
    case LEXTOK_NUMBER:
    case LEXTOK_LPAR:
        return parseExpr3( expr )
            && parseExpr2Rest( expr );

    default:
        return error();
    }
}

bool
Parser::parseExpr2Rest(
    AstExprNode*& expr
    )
{
    AstExprNode* rop;

#ifdef DEBUG_PARSER
    fprintf( stderr, "parseExpr2Rest\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
        // Expr2:
    case LEXTOK_EQ:
        if( !match( LEXTOK_EQ ) ||
            !parseExpr3( rop ) )
            return false;

        expr = new AstBinopExprNode(
            LEXTOK_EQ, expr, rop );
        return true;

    case LEXTOK_NEQ:
        if( !match( LEXTOK_NEQ ) ||
            !parseExpr3( rop ) )
            return false;

        expr = new AstBinopExprNode(
            LEXTOK_NEQ, expr, rop );
        return true;

    case LEXTOK_LT:
        if( !match( LEXTOK_LT ) ||
            !parseExpr3( rop ) )
            return false;

        expr = new AstBinopExprNode(
            LEXTOK_LT, expr, rop );
        return true;

    case LEXTOK_LEQ:
        if( !match( LEXTOK_LEQ ) ||
            !parseExpr3( rop ) )
            return false;

        expr = new AstBinopExprNode(
            LEXTOK_LEQ, expr, rop );
        return true;

    case LEXTOK_GT:
        if( !match( LEXTOK_GT ) ||
            !parseExpr3( rop ) )
            return false;

        expr = new AstBinopExprNode(
            LEXTOK_GT, expr, rop );
        return true;

    case LEXTOK_GEQ:
        if( !match( LEXTOK_GEQ ) ||
            !parseExpr3( rop ) )
            return false;

        expr = new AstBinopExprNode(
            LEXTOK_GEQ, expr, rop );
        return true;

        // Expr0:
    case LEXTOK_OR:
        // Expr1:
    case LEXTOK_AND:
        // Expr0 follow:
    case LEXTOK_RPAR:
    case LEXTOK_RBRA:
    case LEXTOK_COMMA:
    case LEXTOK_SCOL:
    case LEXTOK_KW_THEN:
    case LEXTOK_KW_ELSE:
    case LEXTOK_KW_DO:
    case LEXTOK_KW_TO:
    case LEXTOK_KW_DWNTO:
    case LEXTOK_KW_END:
        return true;

    default:
        return error();
    }
}

bool
Parser::parseExpr3(
    AstExprNode*& expr
    )
{
#ifdef DEBUG_PARSER
    fprintf( stderr, "parseExpr3\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
        // Expr5:
    case LEXTOK_IDENT:
    case LEXTOK_NUMBER:
    case LEXTOK_LPAR:
        return parseExpr4( expr )
            && parseExpr3Rest( expr );

        // Expr3:
    case LEXTOK_MINUS:
        if( !match( LEXTOK_MINUS ) ||
            !parseExpr4( expr ) )
            return false;

        expr = new AstUnopExprNode(
            LEXTOK_MINUS, expr );

        return parseExpr3Rest( expr );

    default:
        return error();
    }
}

bool
Parser::parseExpr3Rest(
    AstExprNode*& expr
    )
{
    AstExprNode* rop;

#ifdef DEBUG_PARSER
    fprintf( stderr, "parseExpr3Rest\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
        // Expr3:
    case LEXTOK_PLUS:
        if( !match( LEXTOK_PLUS ) ||
            !parseExpr4( rop ) )
            return false;

        expr = new AstBinopExprNode(
            LEXTOK_PLUS, expr, rop );

        return parseExpr3Rest( expr );

    case LEXTOK_MINUS:
        if( !match( LEXTOK_MINUS ) ||
            !parseExpr4( rop ) )
            return false;

        expr = new AstBinopExprNode(
            LEXTOK_MINUS, expr, rop );

        return parseExpr3Rest( expr );

        // Expr2:
    case LEXTOK_EQ:
    case LEXTOK_NEQ:
    case LEXTOK_LT:
    case LEXTOK_LEQ:
    case LEXTOK_GT:
    case LEXTOK_GEQ:
        // Expr0:
    case LEXTOK_OR:
        // Expr1:
    case LEXTOK_AND:
        // Expr0 follow:
    case LEXTOK_RPAR:
    case LEXTOK_RBRA:
    case LEXTOK_COMMA:
    case LEXTOK_SCOL:
    case LEXTOK_KW_THEN:
    case LEXTOK_KW_ELSE:
    case LEXTOK_KW_DO:
    case LEXTOK_KW_TO:
    case LEXTOK_KW_DWNTO:
    case LEXTOK_KW_END:
        return true;

    default:
        return error();
    }
}

bool
Parser::parseExpr4(
    AstExprNode*& expr
    )
{
#ifdef DEBUG_PARSER
    fprintf( stderr, "parseExpr4\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
        // Expr5:
    case LEXTOK_IDENT:
    case LEXTOK_NUMBER:
    case LEXTOK_LPAR:
        return parseExpr5( expr )
            && parseExpr4Rest( expr );

    default:
        return error();
    }
}

bool
Parser::parseExpr4Rest(
    AstExprNode*& expr
    )
{
    AstExprNode* rop;

#ifdef DEBUG_PARSER
    fprintf( stderr, "parseExpr4Rest\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
        // Expr4:
    case LEXTOK_MULT:
        if( !match( LEXTOK_MULT ) ||
            !parseExpr5( rop ) )
            return false;

        expr = new AstBinopExprNode(
            LEXTOK_MULT, expr, rop );

        return parseExpr4Rest( expr );

    case LEXTOK_DIV:
        if( !match( LEXTOK_DIV ) ||
            !parseExpr5( rop ) )
            return false;

        expr = new AstBinopExprNode(
            LEXTOK_DIV, expr, rop );

        return parseExpr4Rest( expr );

    case LEXTOK_MOD:
        if( !match( LEXTOK_MOD ) ||
            !parseExpr5( rop ) )
            return false;

        expr = new AstBinopExprNode(
            LEXTOK_MOD, expr, rop );

        return parseExpr4Rest( expr );

        // Expr3:
    case LEXTOK_PLUS:
    case LEXTOK_MINUS:
        // Expr2:
    case LEXTOK_EQ:
    case LEXTOK_NEQ:
    case LEXTOK_LT:
    case LEXTOK_LEQ:
    case LEXTOK_GT:
    case LEXTOK_GEQ:
        // Expr0:
    case LEXTOK_OR:
        // Expr1:
    case LEXTOK_AND:
        // Expr0 follow:
    case LEXTOK_RPAR:
    case LEXTOK_RBRA:
    case LEXTOK_COMMA:
    case LEXTOK_SCOL:
    case LEXTOK_KW_THEN:
    case LEXTOK_KW_ELSE:
    case LEXTOK_KW_DO:
    case LEXTOK_KW_TO:
    case LEXTOK_KW_DWNTO:
    case LEXTOK_KW_END:
        return true;

    default:
        return error();
    }
}

bool
Parser::parseExpr5(
    AstExprNode*& expr
    )
{
    std::string name;

#ifdef DEBUG_PARSER
    fprintf( stderr, "parseExpr5\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
        // Expr5:
    case LEXTOK_IDENT:
        name = peekStr();

        return match( LEXTOK_IDENT )
            && parseExpr5Ident( name, expr );

    case LEXTOK_NUMBER:
        expr = new AstIntExprNode( peekInt() );

        return match( LEXTOK_NUMBER );

    case LEXTOK_LPAR:
        return match( LEXTOK_LPAR )
            && parseExpr0( expr )
            && match( LEXTOK_RPAR );

    default:
        return error();
    }
}

bool
Parser::parseExpr5Ident(
    const std::string& name,
    AstExprNode*& expr
    )
{
    AstFunExprNode* fun;

#ifdef DEBUG_PARSER
    fprintf( stderr, "parseExpr5Ident\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
        // Expr5:
    case LEXTOK_LPAR:
        expr = fun = new AstFunExprNode(
            name.c_str() );

        return match( LEXTOK_LPAR )
            && parseFunExprArgs( fun )
            && match( LEXTOK_RPAR );

    case LEXTOK_LBRA:
        if( !match( LEXTOK_LBRA ) ||
            !parseExpr0( expr ) ||
            !match( LEXTOK_RBRA ) )
            return false;

        expr = new AstArrExprNode(
            name.c_str(), expr );
        return true;

        // Expr3:
    case LEXTOK_PLUS:
    case LEXTOK_MINUS:
        // Expr4:
    case LEXTOK_MULT:
    case LEXTOK_DIV:
    case LEXTOK_MOD:
        // Expr2:
    case LEXTOK_EQ:
    case LEXTOK_NEQ:
    case LEXTOK_LT:
    case LEXTOK_LEQ:
    case LEXTOK_GT:
    case LEXTOK_GEQ:
        // Expr0:
    case LEXTOK_OR:
        // Expr1:
    case LEXTOK_AND:
        // Expr0 follow:
    case LEXTOK_RPAR:
    case LEXTOK_RBRA:
    case LEXTOK_COMMA:
    case LEXTOK_SCOL:
    case LEXTOK_KW_THEN:
    case LEXTOK_KW_ELSE:
    case LEXTOK_KW_DO:
    case LEXTOK_KW_TO:
    case LEXTOK_KW_DWNTO:
    case LEXTOK_KW_END:
        expr = new AstVarExprNode(
            name.c_str() );
        return true;

    default:
        return error();
    }
}

bool
Parser::parseFunExprArgs(
    AstFunExprNode* fun
    )
{
    AstExprNode* arg;

#ifdef DEBUG_PARSER
    fprintf( stderr, "parseFunExprArgs\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
    case LEXTOK_IDENT:
    case LEXTOK_NUMBER:
    case LEXTOK_MINUS:
    case LEXTOK_LPAR:
        if( !parseExpr0( arg ) )
            return false;

        fun->addArg( arg );
        return parseFunExprArgsRest( fun );

    case LEXTOK_RPAR:
        return true;

    default:
        return error();
    }
}

bool
Parser::parseFunExprArgsRest(
    AstFunExprNode* fun
    )
{
    AstExprNode* arg;

#ifdef DEBUG_PARSER
    fprintf( stderr, "parseFunExprArgsRest\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
    case LEXTOK_COMMA:
        if( !match( LEXTOK_COMMA ) ||
            !parseExpr0( arg ) )
            return false;

        fun->addArg( arg );
        return parseFunExprArgsRest( fun );

    case LEXTOK_RPAR:
        return true;

    default:
        return error();
    }
}

bool
Parser::parseMinusNumber(
    int& num
    )
{
#ifdef DEBUG_PARSER
    fprintf( stderr, "parseMinusNumber\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
    case LEXTOK_MINUS:
        if( !match( LEXTOK_MINUS ) )
            return false;

        num = -peekInt();
        return match( LEXTOK_NUMBER );

    case LEXTOK_NUMBER:
        num = peekInt();
        return match( LEXTOK_NUMBER );

    default:
        return error();
    }
}

bool
Parser::parseTypeIdent(
    AstTypeNode*& type
    )
{
    int begin, end;

#ifdef DEBUG_PARSER
    fprintf( stderr, "parseTypeIdent\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
    case LEXTOK_KW_INT:
        if( !match( LEXTOK_KW_INT ) )
            return false;

        type = new AstIntTypeNode();
        return true;

    case LEXTOK_KW_ARR:
        if( !match( LEXTOK_KW_ARR ) ||
            !match( LEXTOK_LBRA ) ||
            !parseMinusNumber( begin ) ||
            !match( LEXTOK_DDOT ) ||
            !parseMinusNumber( end ) ||
            !match( LEXTOK_RBRA ) ||
            !match( LEXTOK_KW_OF ) ||
            !parseTypeIdent( type ) )
            return false;

        if( end <= begin )
        {
            fprintf( stderr, "Invalid array range [%d;%d]\n",
                     begin, end );
            return false;
        }

        type = new AstArrTypeNode(
            begin, end, type );
        return true;

    default:
        return error();
    }
}

bool
Parser::parseIdentListRest(
    std::vector< std::string >& idents
    )
{
#ifdef DEBUG_PARSER
    fprintf( stderr, "parseIdentListRest\n" );
#endif /* DEBUG_PARSER */

    switch( peek() )
    {
    case LEXTOK_COMMA:
        if( !match( LEXTOK_COMMA ) )
            return false;

        idents.push_back( peekStr() );

        return match( LEXTOK_IDENT )
            && parseIdentListRest( idents );

    case LEXTOK_EQ:
    case LEXTOK_COL:
        return true;

    default:
        return error();
    }
}

bool
Parser::error()
{
    fprintf( stderr, "Unexpected `%s' encountered\n",
             LEXTOK_STR[peek()] );
    return false;
}
