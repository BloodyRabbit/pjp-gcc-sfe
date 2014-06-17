/** @file
 * @brief Test driver for Simple FrontEnd.
 *
 * @author Jan Bobek
 * @since 11th June 2014
 */

#include <cstdio>
#include <cstdlib>

#include "sfe-lang.h"

int
main(
    int argc,
    char* argv[]
    )
{
    FILE* fp;

    if( argc < 2 )
    {
        fprintf( stderr, "Usage: %s <source.p> [...]\n", *argv );
        return EXIT_FAILURE;
    }

    for( int i = 1; i < argc; ++i )
    {
        printf( "Parsing `%s'\n", argv[i] );

        if( !(fp = fopen( argv[i], "r" )) )
        {
            perror( "fopen" );
            continue;
        }

        LexAnalyzer lexan( fp );
        Parser parser( lexan );

        AstNode* ast;
        if( !parser.parse( ast ) )
        {
            printf( "Failed to parse file `%s'\n", argv[i] );
            continue;
        }

        printf( "File `%s' parsed successfully\n", argv[i] );
#ifdef DEBUG_AST
        ast->print( 1, stdout );
#endif /* DEBUG_AST */

        tree t;
        SymTable symTable;
        if( !ast->translate( t, NULL_TREE, symTable ) )
        {
            printf( "Failed to translate file `%s'\n", argv[i] );
            delete ast;
            continue;
        }

        printf( "File `%s' translated successfully\n", argv[i] );
        delete ast;
    }
}
