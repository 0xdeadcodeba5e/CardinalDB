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
#ifndef CARDINALDB__INTERNAL_SQL_QUERY__LIST_H_
#define CARDINALDB__INTERNAL_SQL_QUERY__LIST_H_

#include <CardinalDB/Configuration.h>
#include <CardinalDB/TypeDeclaration.h>
#include <Internal/SQL/Token.h>

#define CARDINALDB_MAX_LIST_ELEMENT 64

typedef struct __cardinaldb_sql_literal_alias {
    SQLToken    Token;
    const char* Begin;
    const char* End;
} SQLAlias;

struct SQLListEntry {
    bool        IsAlias;
    union {
        SQLToken    Literal;
        SQLAlias    Alias;
    };
};

typedef struct __cardinaldb_sql_query_list {
    struct SQLListEntry     Elements[ CARDINALDB_MAX_LIST_ELEMENT ];
    size_t                  Length;
} SQLList;

typedef SQLList SQLColumnList;
typedef SQLList SQLTableList;

struct SQLColumnDefinition {
    SQLToken    ColumnName;
    SQLToken    ColumnDomain;
    bool        IsNotNull;
    bool        IsPrimaryKey;
    bool        IsUnique;
};
typedef struct {
    struct SQLColumnDefinition  Columns[ CARDINALDB_MAX_LIST_ELEMENT ];
    size_t                      Length;
} SQLColumnDefinitionList;

SQLParserError TryParse_SQLListEntry(struct SQLListEntry* Entry, SQLLexer* Lexer);
SQLParserError TryParse_SQLList(SQLList* List, SQLLexer* Lexer);
SQLParserError TryParse_ColumnDefinition(struct SQLColumnDefinition* Definition, SQLLexer* Lexer);
SQLParserError TryParse_ColumnDefinitionList(SQLColumnDefinitionList* List, SQLLexer* Lexer);

#endif