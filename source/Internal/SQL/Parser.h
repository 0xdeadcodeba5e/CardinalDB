/*
**  CardinalDB
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
#ifndef CARDINALDB__INTERNAL_SQL__PARSER_H_
#define CARDINALDB__INTERNAL_SQL__PARSER_H_

#include <CardinalDB/Configuration.h>
#include <CardinalDB/TypeDeclaration.h>
#include <Internal/SQL/Token.h>
#include <Internal/SQL/Lexer.h>

typedef struct __cardinaldb_sql_query SQLQuery;

typedef enum __cardinaldb_sql_parser_error_type {
    SQL_PARSER_ERROR_TYPE__NONE,

    SQL_PARSER_ERROR_TYPE__EXPECTED_LITERAL,
    SQL_PARSER_ERROR_TYPE__EXPECTED_DOMAIN,
    SQL_PARSER_ERROR_TYPE__EXPECTED_LEFT_PARENTHESIS,
    SQL_PARSER_ERROR_TYPE__MISSING_RIGHT_PARENTHESIS,
    SQL_PARSER_ERROR_TYPE__INVALID_QUERY_STATEMENT,
    
    SQL_PARSER_ERROR_TYPE__MISSING_KEYWORD,
    SQL_PARSER_ERROR_TYPE__INCOMPLETE_STATEMENT,
} SQLParserErrorType;

typedef struct __cardinaldb_sql_parser_error {
    SQLParserErrorType  Type;
    size_t              Line;
    size_t              Column;
    const char*         Begin;
    const char*         End;
} SQLParserError;

SQLParserError Throw_SQLParserError(SQLParserErrorType ErrorType, SQLToken* Token);
extern SQLParserError SQL_PARSER_SUCCESS;

#   if defined(PARSER_C)
#include <Internal/SQL/Query/Query.h>

typedef struct __cardinaldb_sql_parser {
    SQLQuery            Head;
    SQLQuery*           Current;
} SQLParser;

void SQLParser_Initialize(SQLParser* Parser);
SQLParserError SQLParser_Parse(SQLParser* Parser, SQLLexer* Lexer);
#   endif


#endif