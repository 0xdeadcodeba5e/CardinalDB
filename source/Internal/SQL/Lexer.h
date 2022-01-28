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
#ifndef CARDINALDB__INTERNAL_SQL__LEXER_H_
#define CARDINALDB__INTERNAL_SQL__LEXER_H_

#include <CardinalDB/Configuration.h>
#include <CardinalDB/TypeDeclaration.h>
#include <Internal/SQL/Token.h>

typedef enum __cardinaldb_sql_character_class {
    SQL_CHARACTER_CLASS__INVALID            =   0x000,
    SQL_CHARACTER_CLASS__KEYWORD_BEGIN      =   0x001,
    SQL_CHARACTER_CLASS__KEYWORD            =   0x002,
    SQL_CHARACTER_CLASS__DIGIT              =   0x004,
    SQL_CHARACTER_CLASS__DOLAR              =   0x008,
    SQL_CHARACTER_CLASS__VAR_ALPHABETIC     =   0x010,
    SQL_CHARACTER_CLASS__VAR_NUMERIC        =   0x020,
    SQL_CHARACTER_CLASS__SPACE              =   0x040,
    SQL_CHARACTER_CLASS__QUOTE              =   0x080,
    SQL_CHARACTER_CLASS__SYMBOL             =   0x100
} SQLCharacterClass;

typedef enum __cardinaldb_sql_symbol_class {
    SQL_SYMBOL_TYPE___PIPE                  =   0x100,
    SQL_SYMBOL_TYPE___MINUS                 =   0x101,
    SQL_SYMBOL_TYPE___LESS_THAN             =   0x102,
    SQL_SYMBOL_TYPE___GREATER_THAN          =   0x103,
    SQL_SYMBOL_TYPE___EQUAL                 =   0x104,
    SQL_SYMBOL_TYPE___BANG                  =   0x105,
    SQL_SYMBOL_TYPE___SLASH                 =   0x106,
    SQL_SYMBOL_TYPE___LEFT_PARENTHESIS      =   0x107,
    SQL_SYMBOL_TYPE___RIGHT_PARENTHESIS     =   0x108,
    SQL_SYMBOL_TYPE___SEMICOLON             =   0x109,
    SQL_SYMBOL_TYPE___PLUS                  =   0x10A,
    SQL_SYMBOL_TYPE___ASTERISK              =   0x10B,
    SQL_SYMBOL_TYPE___MODULO                =   0x10C,
    SQL_SYMBOL_TYPE___COMMA                 =   0x10D,
    SQL_SYMBOL_TYPE___AMPERSAND             =   0x10E,
    SQL_SYMBOL_TYPE___TILDE                 =   0x110,
    SQL_SYMBOL_TYPE___PERIOD                =   0x111,
    SQL_SYMBOL_TYPE___NULL                  =   0x112,
} SQLSymbolClass;

typedef enum __cardinaldb_sql_lexer_error_type {
    SQL_LEXER_ERROR_TYPE__NONE,

    SQL_LEXER_ERROR_TYPE__UNTERMINATED_STRING,
    SQL_LEXER_ERROR_TYPE__UNEXPECTED_LITERAL,
    SQL_LEXER_ERROR_TYPE__INVALID_TOKEN_CHARACTER,
    SQL_LEXER_ERROR_TYPE__UNDEFINED_ERROR,
} SQLLexerErrorType;

typedef struct __cardinaldb_sql_lexer_error {
    SQLLexerErrorType   Type;
    const char*         Iterator;
    const char*         End;
    size_t              Line;
    size_t              Column;
} SQLLexerError;

typedef struct __cardinaldb_sql_lexer {
    bool            ShouldContinue;
    const char*     Begin;
    const char*     End;
    size_t          Line;
    size_t          Column;
    SQLToken        CurrentToken;
    SQLToken        NextToken;
    SQLLexerError   LastError;
} SQLLexer;


void SQLLexer_Initialize(SQLLexer* Lexer);
void SQLLexer_SetSource(SQLLexer* Lexer, const char* Begin, const char* End);
SQLLexerError SQLLexer_CurrentToken(SQLLexer* Lexer, SQLToken* Token);
SQLLexerError SQLLexer_NextToken(SQLLexer* Lexer, SQLToken* Token);
SQLLexerError SQLLexer_Advance(SQLLexer* Lexer);
SQLLexerError SQLLexer_LastError(const SQLLexer* Lexer);

#endif