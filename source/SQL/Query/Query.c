#include <CardinalDB/Configuration.h>
#include <CardinalDB/TypeDeclaration.h>
#include <Internal/SQL/Token.h>
#include <Internal/SQL/Lexer.h>
#include <Internal/SQL/Query/Query.h>

SQLParserError TryParse_SQLQuery(SQLQuery* Query, SQLLexer* Lexer) {
    SQLParserError Error = SQL_PARSER_SUCCESS;
    bool ShouldContinue = true;

    Error = TryParse_DDLStatement(&Query->Query.DDL, Lexer);
    if(
        Error.Type == SQL_PARSER_ERROR_TYPE__NONE ||
        Error.Type != SQL_PARSER_ERROR_TYPE__INVALID_QUERY_STATEMENT
    ) {
        ShouldContinue = false;
        Query->Type = SQL_QUERY_TYPE__DDL;
    }
    
    if(ShouldContinue) Error = TryParse_TCLStatement(&Query->Query.TCL, Lexer);
    if(
        Error.Type == SQL_PARSER_ERROR_TYPE__NONE ||
        Error.Type != SQL_PARSER_ERROR_TYPE__INVALID_QUERY_STATEMENT
    ) {
        ShouldContinue = false;
        Query->Type = SQL_QUERY_TYPE__TCL;
    }

    if(!ShouldContinue) return Error;
    if(Lexer->CurrentToken.Type != SQL_TOKEN_TYPE__EOF || Lexer->CurrentToken.Type != SQL_TOKEN_TYPE__SEMICOLON)
        return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__INCOMPLETE_STATEMENT, &Lexer->CurrentToken);
    if(Lexer->CurrentToken.Type == SQL_TOKEN_TYPE__SEMICOLON)
        SQLLexer_Advance(Lexer);
    
    return SQL_PARSER_SUCCESS;
}