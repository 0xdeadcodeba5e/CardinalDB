#include <Internal/SQL/Token.h>
#include <Internal/SQL/Lexer.h>
#include <Internal/SQL/Parser.h>
#include <Internal/SQL/Query/List.h>

SQLParserError TryParse_SQLListEntry(struct SQLListEntry* Entry, SQLLexer* Lexer) {
    if(Lexer->CurrentToken.Type == SQL_TOKEN_TYPE__LITERAL) {
        if(Lexer->NextToken.Type == SQL_TOKEN_TYPE__KEYWORD_AS) {
            Entry->IsAlias = true;
            Entry->Alias.Token = Lexer->CurrentToken;
            SQLLexer_Advance(Lexer);
            SQLLexer_Advance(Lexer);
            if(Lexer->CurrentToken.Type == SQL_TOKEN_TYPE__LITERAL) {
                Entry->Alias.Begin = Lexer->CurrentToken.Begin;
                Entry->Alias.End = Lexer->CurrentToken.End;

                SQLLexer_Advance(Lexer);
            } else
                return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__EXPECTED_LITERAL, &Lexer->CurrentToken);
        } else {
            Entry->IsAlias = false;
            Entry->Literal = Lexer->CurrentToken;

            SQLLexer_Advance(Lexer);
        }
    } else
        return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__EXPECTED_LITERAL, &Lexer->CurrentToken);
}


SQLParserError TryParse_SQLList(SQLList* List, SQLLexer* Lexer) {
    List->Length = 0;
    SQLParserError Error;
    while(Lexer->CurrentToken.Type == SQL_TOKEN_TYPE__LITERAL) {
        if(
            (Error = TryParse_SQLListEntry(&List->Elements[List->Length], Lexer))
            .Type != SQL_PARSER_ERROR_TYPE__NONE
        )
            return Error;
        List->Length += 1;
        if(Lexer->CurrentToken.Type != SQL_TOKEN_TYPE__COMMA) break;
        SQLLexer_Advance(Lexer);
    }

    return SQL_PARSER_SUCCESS;
}


SQLParserError TryParse_ColumnDefinition(struct SQLColumnDefinition* Definition, SQLLexer* Lexer) {
    if(Lexer->CurrentToken.Type == SQL_TOKEN_TYPE__LITERAL) {
        Definition->ColumnName = Lexer->CurrentToken;
        SQLLexer_Advance(Lexer);

        if(
            Lexer->CurrentToken.Type == SQL_TOKEN_TYPE__KEYWORD_STRING ||
            Lexer->CurrentToken.Type == SQL_TOKEN_TYPE__KEYWORD_NUMBER
        ) {
            Definition->ColumnDomain = Lexer->CurrentToken;
            SQLLexer_Advance(Lexer);

            if(
                Lexer->CurrentToken.Type == SQL_TOKEN_TYPE__KEYWORD_NOT &&
                Lexer->NextToken.Type == SQL_TOKEN_TYPE__KEYWORD_NULL
            ) {
                Definition->IsNotNull = true;
                SQLLexer_Advance(Lexer);
                SQLLexer_Advance(Lexer);
            } else
                Definition->IsNotNull = false;
            
            if(
                Lexer->CurrentToken.Type == SQL_TOKEN_TYPE__KEYWORD_PRIMARY &&
                Lexer->NextToken.Type == SQL_TOKEN_TYPE__KEYWORD_KEY
            ) {
                Definition->IsPrimaryKey = true;
                SQLLexer_Advance(Lexer);
                SQLLexer_Advance(Lexer);
            } else
                Definition->IsPrimaryKey = false;

            if(
                Lexer->CurrentToken.Type == SQL_TOKEN_TYPE__KEYWORD_UNIQUE
            ) {
                Definition->IsUnique = true;
                SQLLexer_Advance(Lexer);
            } else
                Definition->IsUnique = false;
        } else
            return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__EXPECTED_DOMAIN, &Lexer->CurrentToken);
    } else
        return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__EXPECTED_LITERAL, &Lexer->CurrentToken);
}

SQLParserError TryParse_ColumnDefinitionList(SQLColumnDefinitionList* List, SQLLexer* Lexer) {
    List->Length = 0;
    SQLParserError Error = SQL_PARSER_SUCCESS;

    while(Lexer->CurrentToken.Type == SQL_TOKEN_TYPE__LITERAL) {
        if(
            (Error = TryParse_ColumnDefinition(&List->Columns[List->Length], Lexer))
            .Type != SQL_PARSER_ERROR_TYPE__NONE
        )
            return Error;
        List->Length += 1;
        if(Lexer->CurrentToken.Type != SQL_TOKEN_TYPE__COMMA) break;
        SQLLexer_Advance(Lexer);
    }
}