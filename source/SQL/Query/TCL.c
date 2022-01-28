#include <Internal/SQL/Token.h>
#include <Internal/SQL/Lexer.h>
#include <Internal/SQL/Parser.h>
#include <Internal/SQL/Query/TCL.h>

//
//  tcl_statement       :=  commit_statment | rollback_statment | savepoint_statement | abort_statement
//  commit_statement    :=  COMMIT
//  rollback_statement  :=  ROLLBACK ${SAVEPOINT_NAME}?
//  savepoint_statement :=  SAVEPOINT ${SAVEPOINT_NAME}
//  abort_statement     :=  ABORT
//

SQLParserError TryParse_TCLStatement(TCLStatement* Statement, SQLLexer* Lexer) {
    
    switch(Lexer->CurrentToken.Type) {
        case SQL_TOKEN_TYPE__KEYWORD_ABORT:
        case SQL_TOKEN_TYPE__KEYWORD_COMMIT:    
                                                Statement->Token = Lexer->CurrentToken;
                                                SQLLexer_Advance(Lexer);
                                                break;
        case SQL_TOKEN_TYPE__KEYWORD_ROLLBACK:
                                                Statement->Token = Lexer->CurrentToken;
                                                SQLLexer_Advance(Lexer);
                                                if(Lexer->NextToken.Type == SQL_TOKEN_TYPE__LITERAL) {
                                                    Statement->Argument = Lexer->NextToken;
                                                    SQLLexer_Advance(Lexer);
                                                }
                                                
                                                break;
        case SQL_TOKEN_TYPE__KEYWORD_SAVEPOINT:
                                                if(Lexer->NextToken.Type != SQL_TOKEN_TYPE__LITERAL)
                                                    return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__EXPECTED_LITERAL, &Lexer->NextToken);
                                                Statement->Token = Lexer->CurrentToken;
                                                Statement->Argument = Lexer->NextToken;

                                                SQLLexer_Advance(Lexer);
                                                SQLLexer_Advance(Lexer);
                                                
                                                break;
        default:
                                                return Throw_SQLParserError(SQL_PARSER_ERROR_TYPE__INVALID_QUERY_STATEMENT, &Lexer->CurrentToken);
                                                break;
    }

    return SQL_PARSER_SUCCESS;
}