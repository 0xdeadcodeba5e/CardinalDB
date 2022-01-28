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
#ifndef CARDINALDB__INTERNAL_SQL_QUERY__DDL_H
#define CARDINALDB__INTERNAL_SQL_QUERY__DDL_H_

#include <Internal/SQL/Token.h>
#include <Internal/SQL/Lexer.h>
#include <Internal/SQL/Query/List.h>

typedef struct __cardinaldb_sql_query_create_table {
    SQLToken                Table;
    SQLColumnDefinitionList Columns;
} CreateTableStatement;

typedef struct __cardinaldb_sql_query_drop_table {
    SQLToken                Table;
} DropTableStatement;

typedef struct __cardinaldb_sql_query_truncate_table {
    SQLToken                Table;
} TruncateTableStatement;

typedef enum {
    ALTER_TABLE_ACTION__INVALID,

    ALTER_TABLE_ACTION__ADD_COLUMN,
    ALTER_TABLE_ACTION__MODIFY_COLUMN,
    ALTER_TABLE_ACTION__DROP_COLUMN,
    ALTER_TABLE_ACTION__RENAME_COLUMN
} AlterTableAction;

typedef struct __cardinaldb_sql_query_alter_table {
    SQLToken            Table;
    AlterTableAction    ActionType;
    union {
        struct SQLColumnDefinition  ColumnDefinition;
        SQLToken                    ColumnName;
        SQLToken                    ColumnNewName;
    };
} AlterTableStatement;

typedef struct {
    SQLTokenType            Type;
    union {
        CreateTableStatement    CreateTable;
        DropTableStatement      DropTable;
        TruncateTableStatement  TruncateTable;
        AlterTableStatement     AlterTable;
    };
} DDLStatement;

SQLParserError TryParse_CreateTable(CreateTableStatement* Statement, SQLLexer* Lexer);
SQLParserError TryParse_DropTable(DropTableStatement* Statment, SQLLexer* Lexer);
SQLParserError TryParse_TruncateTable(TruncateTableStatement* Statement, SQLLexer* Lexer);
SQLParserError TryParse_AlterTable(AlterTableStatement* Statement, SQLLexer* Lexer);

SQLParserError TryParse_DDLStatement(DDLStatement* Statement, SQLLexer* Lexer);

#endif