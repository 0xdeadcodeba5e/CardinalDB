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
#include <CardinalDB/Configuration.h>
#include <CardinalDB/TypeDeclaration.h>
#include <Internal/SQL/Token.h>
#include <Internal/SQL/Lexer.h>

#include <stdio.h>
#include <string.h>

extern bool IsSQLKeyword(const char* Iterator, uint32_t Length);

const char* HumanBoolean[] = {"NO", "YES"};
const char* SQLTokenTypeString[] = {
    "INVALID",

    "LITERAL",
    "NUMERIC",
    "STRING",

    "KEYWORD::CURRENT",
    "KEYWORD::CURRENT_TIME",
    "KEYWORD::GROUP",
    "KEYWORD::IN",
    "KEYWORD::INDEX",
    "KEYWORD::NOT",
    "KEYWORD::NULL",
    "KEYWORD::OF",
    "KEYWORD::ROW",
    "KEYWORD::TEMP",
    "KEYWORD::WITH",
    "KEYWORD::ISNULL",
    "KEYWORD::AS",
    "KEYWORD::ASC",
    "KEYWORD::DATE",
    "KEYWORD::FOR",
    "KEYWORD::IS",
    "KEYWORD::NO",
    "KEYWORD::NULLS",
    "KEYWORD::OR",
    "KEYWORD::TO",
    "KEYWORD::DO",
    "KEYWORD::ON",
    "KEYWORD::SET",
    "KEYWORD::ACTION",
    "KEYWORD::ALL",
    "KEYWORD::SAVEPOINT",
    "KEYWORD::TIME",
    "KEYWORD::TIMESTAMP",
    "KEYWORD::INTERSECT",
    "KEYWORD::TABLE",
    "KEYWORD::LEFT",
    "KEYWORD::THEN",
    "KEYWORD::END",
    "KEYWORD::DELETE",
    "KEYWORD::TEMPORARY",
    "KEYWORD::NOTNULL",
    "KEYWORD::LAST",
    "KEYWORD::TIES",
    "KEYWORD::ESCAPE",
    "KEYWORD::EXCLUDE",
    "KEYWORD::DEFAULT",
    "KEYWORD::TRIGGER",
    "KEYWORD::RANGE",
    "KEYWORD::GENERATED",
    "KEYWORD::DATABASE",
    "KEYWORD::SELECT",
    "KEYWORD::TRANSACTION",
    "KEYWORD::NATURAL",
    "KEYWORD::ALTER",
    "KEYWORD::REFERENCES",
    "KEYWORD::BEFORE",
    "KEYWORD::FOREIGN",
    "KEYWORD::IGNORE",
    "KEYWORD::REINDEX",
    "KEYWORD::INDEXED",
    "KEYWORD::DEFERRABLE",
    "KEYWORD::EXPLAIN",
    "KEYWORD::INNER",
    "KEYWORD::RAISE",
    "KEYWORD::EACH",
    "KEYWORD::CHECK",
    "KEYWORD::KEY",
    "KEYWORD::BEGIN",
    "KEYWORD::INSERT",
    "KEYWORD::CASCADE",
    "KEYWORD::DEFERRED",
    "KEYWORD::DESC",
    "KEYWORD::COLLATE",
    "KEYWORD::ELSE",
    "KEYWORD::EXCEPT",
    "KEYWORD::CREATE",
    "KEYWORD::EXISTS",
    "KEYWORD::IMMEDIATE",
    "KEYWORD::EXCLUSIVE",
    "KEYWORD::JOIN",
    "KEYWORD::INSTEAD",
    "KEYWORD::ADD",
    "KEYWORD::DETACH",
    "KEYWORD::HAVING",
    "KEYWORD::GLOB",
    "KEYWORD::BETWEEN",
    "KEYWORD::NOTHING",
    "KEYWORD::GROUPS",
    "KEYWORD::UPDATE",
    "KEYWORD::VIRTUAL",
    "KEYWORD::ALWAYS",
    "KEYWORD::WHERE",
    "KEYWORD::RECURSIVE",
    "KEYWORD::ABORT",
    "KEYWORD::AFTER",
    "KEYWORD::REGEXP",
    "KEYWORD::PLAN",
    "KEYWORD::ANALYZE",
    "KEYWORD::AND",
    "KEYWORD::DISTINCT",
    "KEYWORD::ATTACH",
    "KEYWORD::AUTOINCREMENT",
    "KEYWORD::CASE",
    "KEYWORD::CAST",
    "KEYWORD::COLUMN",
    "KEYWORD::COMMIT",
    "KEYWORD::CONFLICT",
    "KEYWORD::CONSTRAINT",
    "KEYWORD::INTO",
    "KEYWORD::OFFSET",
    "KEYWORD::CROSS",
    "KEYWORD::CURRENT_TIMESTAMP",
    "KEYWORD::PRAGMA",
    "KEYWORD::MATCH",
    "KEYWORD::DROP",
    "KEYWORD::PARTITION",
    "KEYWORD::FAIL",
    "KEYWORD::LIKE",
    "KEYWORD::FILTER",
    "KEYWORD::RELEASE",
    "KEYWORD::FIRST",
    "KEYWORD::FOLLOWING",
    "KEYWORD::FROM",
    "KEYWORD::MATERIALIZED",
    "KEYWORD::FULL",
    "KEYWORD::LIMIT",
    "KEYWORD::IF",
    "KEYWORD::ORDER",
    "KEYWORD::RENAME",
    "KEYWORD::OTHERS",
    "KEYWORD::OUTER",
    "KEYWORD::REPLACE",
    "KEYWORD::OVER",
    "KEYWORD::RESTRICT",
    "KEYWORD::RIGHT",
    "KEYWORD::ROLLBACK",
    "KEYWORD::ROWS",
    "KEYWORD::UNBOUNDED",
    "KEYWORD::UNION",
    "KEYWORD::UNIQUE",
    "KEYWORD::QUERY",
    "KEYWORD::USING",
    "KEYWORD::VACUUM",
    "KEYWORD::VALUES",
    "KEYWORD::VIEW",
    "KEYWORD::WHEN",
    "KEYWORD::WINDOW",
    "KEYWORD::WITHOUT",
    "KEYWORD::BY",
    "KEYWORD::INITIALLY",
    "KEYWORD::PRECEDING",
    "KEYWORD::PRIMARY",
    "KEYWORD::RETURNING",

    "EQUAL",
    "LESS_THAN",
    "GREATER_THAN",
    "LESS_THAN_EQUAL",
    "GREATER_THAN_EQUAL",
    "BANG",
    "NOT_EQUAL",
    "NOT_EQUAL_NULL_SAFE",
    "SEMICOLON",
    "COMMA",
    "PERIOD",
    "TILDE",
    "LEFT_PARENTHESIS",
    "RIGHT_PARENTHESIS",
    "PLUS",
    "MINUS",
    "ASTERISK",
    "SLASH",
    "MODULO",
    "AMPERSAND",
    "PIPE",
    "EOF",
};

void LexerTest(const char* Query) {
    SQLLexer Lexer;

    SQLLexer_Initialize(&Lexer);
    SQLLexer_SetSource(&Lexer, Query, Query + strlen(Query));

    printf("%-32s %-32s %-16s\n\n", "TOKEN TYPE", "TOKEN STRING", "KEYWORD");
    while(Lexer.ShouldContinue) {
        SQLToken        Token;
        SQLLexer_NextToken(&Lexer, &Token);
        const size_t    TokenLength =   ((size_t)(Token.End - Token.Begin));

        if(Token.Type == SQL_TOKEN_TYPE__EOF) break;
        printf("%-32s %-32.*s %-16s\n", SQLTokenTypeString[Token.Type], (int)TokenLength, Token.Begin, HumanBoolean[IsSQLKeyword(Token.Begin, TokenLength)]);
        if(Token.Type == SQL_TOKEN_TYPE__EOF) break;
    }
}

int main(int argc, char** argv) {
    
    /*
    **  SELECT * FROM student
    **  INNER JOIN assessment ON assessment.as_student = student.s_id
    **  INNER JOIN address ON student.s_address = address.add_id
    **  WHERE as_grade = 'O'
    */
    const char* Query =
    "SELECT * FROM student\n\
    INNER JOIN assessment ON assessment.as_student = student.s_id\n\
    INNER JOIN address ON student.s_address = address.add_id\n\
    WHERE as_grade = 'O';\n";

    LexerTest(Query);
}