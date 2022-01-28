#define PARSER_C
#include <CardinalDB/Configuration.h>
#include <CardinalDB/TypeDeclaration.h>
#include <Internal/SQL/Token.h>
#include <Internal/SQL/Parser.h>
#include <Internal/SQL/Query/Query.h>

SQLParserError SQL_PARSER_SUCCESS = {
    .Type   =   SQL_PARSER_ERROR_TYPE__NONE,
    .Line   =   SIZE_MAX,
    .Column =   SIZE_MAX,
    .Begin  =   NULL,
    .End    =   NULL
};

SQLParserError Throw_SQLParserError(SQLParserErrorType ErrorType, SQLToken* Token) {
    return (SQLParserError){
        .Type   =   ErrorType,
        .Begin  =   Token->Begin,
        .End    =   Token->End,
        .Line   =   Token->Line,
        .Column =   Token->Column
    };
}

void SQLParser_Initialize(SQLParser* Parser) {
    Parser->Current = &Parser->Head;
}
SQLParserError SQLParser_Parse(SQLParser* Parser, SQLLexer* Lexer) {
    return TryParse_SQLQuery(Parser->Current, Lexer);
}