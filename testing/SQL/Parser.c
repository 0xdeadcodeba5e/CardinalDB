/*
**  CardinalDB - KeywordHash
**  BSD 3-Clause License
**  
**  Copyright (c) 2022, Santhosh K
**  All rights reserved.
**  
**  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
**  conditions are met:
**  
**  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
**     disclaimer.
**  
**  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
**     disclaimer in the documentation and/or other materials provided with the distribution.
**  
**  3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written permission.
**  
**  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
**  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
**  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
**  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
**  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
**  OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#define PARSER_C
#include <CardinalDB/Configuration.h>
#include <CardinalDB/TypeDeclaration.h>
#include <Internal/SQL/Token.h>
#include <Internal/SQL/Lexer.h>
#include <Internal/SQL/Parser.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void PRINT_PARSER_ERROR(const char* Query, SQLParserError Error) {
    printf("[FAILURE]   %s\n", Query);
    printf("[CardinalDB.SQLParser]  Syntax Error: \n");
    printf("                        ");

    switch(Error.Type) {
        case SQL_PARSER_ERROR_TYPE__EXPECTED_LITERAL:   printf("Expected a literal before the keyword, `%.*s`",
                                                            CARDINALDB_CAST(size_t, (Error.End - Error.Begin)), Error.Begin
                                                        );
                                                        break;
        case SQL_PARSER_ERROR_TYPE__MISSING_KEYWORD:    printf("Expected a keyword before `%.*s`", CARDINALDB_CAST(size_t, (Error.End - Error.Begin)), Error.Begin);
                                                        break;
        case SQL_PARSER_ERROR_TYPE__INVALID_QUERY_STATEMENT:
                                                        printf("The given statement is not a valid SQL Query");
                                                        break;
        case SQL_PARSER_ERROR_TYPE__EXPECTED_LEFT_PARENTHESIS:
                                                        printf("Expected left / open parenthesis before `%.*s`",
                                                            CARDINALDB_CAST(size_t, (Error.End - Error.Begin)), Error.Begin
                                                        );
                                                        break;
        case SQL_PARSER_ERROR_TYPE__MISSING_RIGHT_PARENTHESIS:
                                                        printf("Missing right / parenthesis parenthesis before `%.*s`",
                                                            CARDINALDB_CAST(size_t, (Error.End - Error.Begin)), Error.Begin
                                                        );
                                                        break;
    }
    printf("\n                        In Line %zu, Column %zu\n", Error.Line, Error.Column);
}

void PARSER_TEST(const char* Query) {
    SQLParser Parser; SQLLexer Lexer;

    SQLLexer_Initialize(&Lexer);
    SQLParser_Initialize(&Parser);
    SQLLexer_SetSource(&Lexer, Query, Query + strlen(Query));

    SQLParserError Error = SQLParser_Parse(&Parser, &Lexer);
    if(Error.Type != SQL_PARSER_ERROR_TYPE__NONE)
        PRINT_PARSER_ERROR(Query, Error);
    else
        printf("[SUCCESS]   %s\n", Query);
}

void PARSER_SHOULD_SUCCESS(const char* Query) {
    SQLParser Parser; SQLLexer Lexer;

    SQLLexer_Initialize(&Lexer);
    SQLParser_Initialize(&Parser);
    SQLLexer_SetSource(&Lexer, Query, Query + strlen(Query));

    SQLParserError Error = SQLParser_Parse(&Parser, &Lexer);
    if(Error.Type != SQL_PARSER_ERROR_TYPE__NONE) {
        PRINT_PARSER_ERROR(Query, Error);
        exit(-1);
    }
    else
        printf("[SUCCESS]   %s\n", Query);
}

void PARSER_SHOULD_FAIL(const char* Query) {
    SQLParser Parser; SQLLexer Lexer;

    SQLLexer_Initialize(&Lexer);
    SQLParser_Initialize(&Parser);
    SQLLexer_SetSource(&Lexer, Query, Query + strlen(Query));

    SQLParserError Error = SQLParser_Parse(&Parser, &Lexer);
    if(Error.Type == SQL_PARSER_ERROR_TYPE__NONE) {
        printf("[FAILURE]   This query `%s` should have failed due to an intentional syntax error\n", Query);
        exit(-1);
    }
    else
        printf("[SUCCESS]   %s\n", Query);
}

int main(int argc, char** argv) {
    PARSER_SHOULD_SUCCESS("COMMIT;");
    PARSER_SHOULD_SUCCESS("ROLLBACK;");
    PARSER_SHOULD_SUCCESS("ABORT");
    PARSER_SHOULD_SUCCESS("SAVEPOINT savepoint_a");

    PARSER_SHOULD_SUCCESS("DROP TABLE student;");
    PARSER_SHOULD_SUCCESS("TRUNCATE TABLE student;");
    PARSER_SHOULD_SUCCESS("ALTER TABLE santhosh MODIFY COLUMN student_name STRING");
    PARSER_SHOULD_SUCCESS("CREATE TABLE student (s_id NUMBER NOT NULL, s_name STRING NOT NULL);");
    return 0;
}