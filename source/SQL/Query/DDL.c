#include <Internal/SQL/Token.h>
#include <Internal/SQL/Lexer.h>
#include <Internal/SQL/Parser.h>
#include <Internal/SQL/Query/List.h>
#include <Internal/SQL/Query/DDL.h>

SQLParserError TryParse_CreateTable(CreateTableStatement* Statement, SQLLexer* Lexer) {
    SQLLexer_Advance(Lexer);
    
    if(Lexer->CurrentToken.Type != SQL_TOKEN_TYPE__KEYWORD_TABLE)
        return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__MISSING_KEYWORD, &Lexer->CurrentToken);
    SQLLexer_Advance(Lexer);

    if(Lexer->CurrentToken.Type == SQL_TOKEN_TYPE__LITERAL) {
        Statement->Table = Lexer->CurrentToken;
        SQLLexer_Advance(Lexer);
    } else
        return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__EXPECTED_LITERAL, &Lexer->CurrentToken);
    
    if(Lexer->CurrentToken.Type != SQL_TOKEN_TYPE__LEFT_PARENTHESIS)
        return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__EXPECTED_LEFT_PARENTHESIS, &Lexer->CurrentToken);
    SQLLexer_Advance(Lexer);

    SQLParserError Error = TryParse_ColumnDefinitionList(&Statement->Columns, Lexer);

    if(Lexer->CurrentToken.Type != SQL_TOKEN_TYPE__RIGHT_PARENTHESIS)
        return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__MISSING_RIGHT_PARENTHESIS, &Lexer->CurrentToken);
    SQLLexer_Advance(Lexer);

    return SQL_PARSER_SUCCESS;
}

SQLParserError TryParse_AlterTable(AlterTableStatement* Statement, SQLLexer* Lexer) {
    SQLLexer_Advance(Lexer);
    
    if(Lexer->CurrentToken.Type != SQL_TOKEN_TYPE__KEYWORD_TABLE)
        return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__MISSING_KEYWORD, &Lexer->CurrentToken);
    SQLLexer_Advance(Lexer);

    if(Lexer->CurrentToken.Type == SQL_TOKEN_TYPE__LITERAL) {
        Statement->Table = Lexer->CurrentToken;
        SQLLexer_Advance(Lexer);
    } else
        return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__EXPECTED_LITERAL, &Lexer->CurrentToken);
    
    Statement->ActionType = Lexer->CurrentToken.Type;
    switch(Statement->ActionType) {
        case SQL_TOKEN_TYPE__KEYWORD_ADD:  
        case SQL_TOKEN_TYPE__KEYWORD_MODIFY: 
                                            SQLLexer_Advance(Lexer);
                                            if(Lexer->CurrentToken.Type != SQL_TOKEN_TYPE__KEYWORD_COLUMN)
                                                return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__MISSING_KEYWORD, &Lexer->CurrentToken);
                                            SQLLexer_Advance(Lexer);
                                            return TryParse_ColumnDefinition(&Statement->ColumnDefinition, Lexer);

        case SQL_TOKEN_TYPE__KEYWORD_DROP:
                                            SQLLexer_Advance(Lexer);
                                            if(Lexer->CurrentToken.Type != SQL_TOKEN_TYPE__KEYWORD_COLUMN)
                                                return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__MISSING_KEYWORD, &Lexer->CurrentToken);
                                            SQLLexer_Advance(Lexer);

                                            if(Lexer->CurrentToken.Type != SQL_TOKEN_TYPE__LITERAL)
                                                return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__EXPECTED_LITERAL, &Lexer->CurrentToken);
                                            Statement->ColumnName = Lexer->CurrentToken;

        default:                            return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__INCOMPLETE_STATEMENT, &Lexer->CurrentToken);
    }

    return SQL_PARSER_SUCCESS;
}

SQLParserError TryParse_DropTable(DropTableStatement* Statment, SQLLexer* Lexer) {
    if(Lexer->CurrentToken.Type != SQL_TOKEN_TYPE__KEYWORD_DROP)
        return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__INVALID_QUERY_STATEMENT, &Lexer->CurrentToken);
    SQLLexer_Advance(Lexer);
    
    if(Lexer->CurrentToken.Type != SQL_TOKEN_TYPE__KEYWORD_TABLE)
        return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__MISSING_KEYWORD, &Lexer->CurrentToken);
    SQLLexer_Advance(Lexer);

    if(Lexer->CurrentToken.Type == SQL_TOKEN_TYPE__LITERAL) {
        Statment->Table = Lexer->CurrentToken;
        SQLLexer_Advance(Lexer);
    } else
        return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__EXPECTED_LITERAL, &Lexer->CurrentToken);
    
    return SQL_PARSER_SUCCESS;
}

SQLParserError TryParse_TruncateTable(TruncateTableStatement* Statment, SQLLexer* Lexer) {
    if(Lexer->CurrentToken.Type != SQL_TOKEN_TYPE__KEYWORD_TRUNCATE)
        return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__INVALID_QUERY_STATEMENT, &Lexer->CurrentToken);
    SQLLexer_Advance(Lexer);
    
    if(Lexer->CurrentToken.Type != SQL_TOKEN_TYPE__KEYWORD_TABLE)
        return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__MISSING_KEYWORD, &Lexer->CurrentToken);
    SQLLexer_Advance(Lexer);
    
    if(Lexer->CurrentToken.Type == SQL_TOKEN_TYPE__LITERAL) {
        Statment->Table = Lexer->CurrentToken;
        SQLLexer_Advance(Lexer);
    } else
        return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__EXPECTED_LITERAL, &Lexer->CurrentToken);
    
    return SQL_PARSER_SUCCESS;
}

SQLParserError TryParse_DDLStatement(DDLStatement* Statement, SQLLexer* Lexer) {
    SQLParserError Error;

    Statement->Type = Lexer->CurrentToken.Type;
    switch(Statement->Type) {
        case SQL_TOKEN_TYPE__KEYWORD_CREATE:    return TryParse_CreateTable(&Statement->CreateTable, Lexer);
        case SQL_TOKEN_TYPE__KEYWORD_ALTER:     return TryParse_AlterTable(&Statement->AlterTable, Lexer);
        case SQL_TOKEN_TYPE__KEYWORD_DROP:      return TryParse_DropTable(&Statement->DropTable, Lexer);
        case SQL_TOKEN_TYPE__KEYWORD_TRUNCATE:  return TryParse_TruncateTable(&Statement->TruncateTable, Lexer);
        default:                                return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__INVALID_QUERY_STATEMENT, &Lexer->CurrentToken);
    }
}