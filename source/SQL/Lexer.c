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
#include <CardinalDB/Configuration.h>
#include <CardinalDB/TypeDeclaration.h>
#include <Internal/SQL/Token.h>
#include <Internal/SQL/Lexer.h>
#include <stdio.h>

//  The following functions are defined in KeywordHash.c
extern SQLTokenType DeduceLiteralType(const char* Iterator, uint32_t Length);
extern bool IsSQLKeyword(const char* Iterator, uint32_t Length);

static const SQLCharacterClass CharacterClass[] = {
/*      x0     x1     x2     x3     x4     x5     x6     x7     x8     x9     xA     xB     xC     xD     XE     XF  */

/*x0*/  0x112, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x040, 0x040, 0x000, 0x040, 0x040, 0x000, 0x000,
/*x1*/  0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
/*x2*/  0x040, 0x105, 0x080, 0x010, 0x008, 0x10C, 0x10B, 0x080, 0x107, 0x108, 0x10B, 0x10A, 0x10D, 0x101, 0x111, 0x106,
/*x3*/  0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x010, 0x109, 0x102, 0x104, 0x103, 0x020,
/*x4*/  0x010, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001,
/*x5*/  0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x002, 0x002, 0x000, 0x000, 0x000, 0x000, 0x002,
/*x6*/  0x080, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001,
/*x7*/  0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x002, 0x002, 0x000, 0x100, 0x000, 0x110, 0x000,
/*x8*/  0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
/*x9*/  0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
/*xA*/  0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
/*xB*/  0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
/*xC*/  0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
/*xD*/  0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
/*xE*/  0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
/*xF*/  0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
};

void Throw_LexerError(SQLLexer* Lexer, const char* LexerError, ...) {
    va_list argument_pack;
    va_start(argument_pack, LexerError);
    fprintf(stderr, "[CardinalDB.SQLLexer] Syntax error thrown\n");
    fprintf(stderr, "                      ");
    vfprintf(stderr, LexerError, argument_pack);
    fprintf(stderr, "\n\n                      In line %zu, column %zu\n\n", Lexer->Line + 1, Lexer->Column);
    va_end(argument_pack);
    Lexer->ShouldContinue = false;
}

#define SET_TOKEN(_Type, _Begin, _End)                                                                                         \
    Token->Type = _Type;                                                                                                       \
    Token->Begin = _Begin;                                                                                                     \
    Token->End = _End;                                                                                                         \
    return CARDINALDB_CAST(size_t, (_End - _Begin));

//  The following are the utility functions that are required by the TryConsume_* routines
bool IsSpace(const char* Iterator) {
    return *Iterator == ' ' || *Iterator == '\t' || *Iterator == '\n';
}

bool IsNumerical(const char* Iterator, const char* Begin, const char* End) {
    if(Iterator == Begin && *Iterator >= '0' && *Iterator <= '9') return true;
    else if(Iterator != Begin && *Iterator == '.' && (*(Iterator - 1) >= '0' && *(Iterator - 1) <= '9')) return true;
    else return (*Begin >= '0' && *Begin <= '9' && *Iterator >= '0' && *Iterator <= '9');
}

//  The following are the individual routines that consume tokens of definite class
size_t TryConsume_StringToken(SQLLexer* Lexer, const char* Begin, const char* End, SQLToken* Token) {
    const char Quote = *Begin;
    const char* Iterator = Begin + 1;

    while(*Iterator != Quote) {
        if(*Iterator == '\\' && ( CharacterClass[ *Iterator ] == SQL_CHARACTER_CLASS__QUOTE )) Iterator += 2;
        if(Iterator == End)
            Throw_LexerError(Lexer, "Unterminated quoted string literal >> %.3s", Begin);
        Iterator += 1;
    }

    SET_TOKEN(SQL_TOKEN_TYPE__STRING, Begin, Iterator + 1);
}

size_t TryConsume_NumericToken(SQLLexer* Lexer, const char* Begin, const char* End, SQLToken* Token) {
    const char* Iterator = Begin;
    while(IsNumerical(Iterator, Begin, End)) Iterator += 1;

    SET_TOKEN(SQL_TOKEN_TYPE__NUMERIC, Begin, Iterator);
}

size_t TryConsume_LiteralToken(SQLLexer* Lexer, const char* Begin, const char* End, SQLToken* Token) {
    const char* Iterator = Begin;
    while(
        CharacterClass[ *Iterator ] == SQL_CHARACTER_CLASS__KEYWORD             ||
        CharacterClass[ *Iterator ] == SQL_CHARACTER_CLASS__KEYWORD_BEGIN       ||
        CharacterClass[ *Iterator ] == SQL_CHARACTER_CLASS__DIGIT
    ) {
        if(Iterator == End || IsSpace(Iterator)) break;
        Iterator += 1;
    }

    uint32_t LiteralLength = CARDINALDB_CAST(uint32_t, (Iterator - Begin));
    if(IsSQLKeyword(Begin, LiteralLength)) {
        SET_TOKEN(DeduceLiteralType(Begin, LiteralLength), Begin, Iterator);
    } else {
        SET_TOKEN(SQL_TOKEN_TYPE__LITERAL, Begin, Iterator);
    }
}

size_t TryConsume_SymbolToken(SQLLexer* Lexer, const char* Begin, const char* End, SQLToken* Token) {
    const char* Iterator = Begin;
    SQLTokenType Type;

    switch(*Iterator) {
        case '.':   Type = SQL_TOKEN_TYPE__PERIOD;  break;
        case '=':   Type = SQL_TOKEN_TYPE__EQUAL; break;
        case ';':   Type = SQL_TOKEN_TYPE__SEMICOLON; break;
        case '+':   Type = SQL_TOKEN_TYPE__PLUS; break;
        case '-':   Type = SQL_TOKEN_TYPE__MINUS; break;
        case '*':   Type = SQL_TOKEN_TYPE__ASTERISK; break;
        case '/':   Type = SQL_TOKEN_TYPE__SLASH; break;
        case '(':   Type = SQL_TOKEN_TYPE__LEFT_PARENTHESIS; break;
        case ')':   Type = SQL_TOKEN_TYPE__RIGHT_PARENTHESIS; break;
        case ',':   Type = SQL_TOKEN_TYPE__COMMA; break;
        case '!':   if(*(Iterator + 1) == '=')
                        (Type = SQL_TOKEN_TYPE__NOT_EQUAL, ++Iterator);
                    else
                        Type = SQL_TOKEN_TYPE__BANG;
                    break;
        case '>':   if(*(Iterator + 1) == '=')
                        (Type = SQL_TOKEN_TYPE__GREATER_THAN_EQUAL, ++Iterator);
                    else
                        Type = SQL_TOKEN_TYPE__GREATER_THAN;
                    break;
        case '<':   if(*(Iterator + 1) == '>')
                        (Type = SQL_TOKEN_TYPE__NOT_EQUAL_NULL_SAFE, ++Iterator);
                    else if(*(Iterator + 1) == '=')
                        (Type = SQL_TOKEN_TYPE__LESS_THAN_EQUAL, ++Iterator);
                    else
                        Type = SQL_TOKEN_TYPE__LESS_THAN;
        default:    break;
    }

    SET_TOKEN(Type, Begin, Iterator + 1);
}

size_t SQLLexer_ConsumeToken(SQLLexer* Lexer, const char* Begin, const char* End, SQLToken* Token) {


    Token->Line     = Lexer->Line;
    Token->Column   = Lexer->Column;
    size_t Offset = 0;

lexer_routine:
    if(Begin >= End) {
        Token->Type = SQL_TOKEN_TYPE__EOF;
        Token->Begin = Begin;
        Token->End = End;

        return 0;
    }

    switch( CharacterClass[ *Begin ] ) {
        case SQL_CHARACTER_CLASS__KEYWORD_BEGIN:
        case SQL_CHARACTER_CLASS__KEYWORD:
                                            Offset += TryConsume_LiteralToken(Lexer, Begin, End, Token);
                                            break;
        case SQL_CHARACTER_CLASS__DIGIT:
                                            Offset += TryConsume_NumericToken(Lexer, Begin, End, Token);
                                            break;
        case SQL_CHARACTER_CLASS__QUOTE:
                                            Offset += TryConsume_StringToken(Lexer, Begin, End, Token);
                                            break;
        case SQL_CHARACTER_CLASS__SPACE:
                                            while( CharacterClass[ *Begin ] == SQL_CHARACTER_CLASS__SPACE && Begin != End ) {
                                                if(*Begin == '\n') {
                                                    Lexer->Line     +=  1;
                                                    Lexer->Column   =   0;
                                                }
                                                Begin += 1;
                                                Offset += 1;
                                                Lexer->Column += 1;
                                            }
                                            if(Begin >= End) {
                                                Token->Type = SQL_TOKEN_TYPE__EOF;
                                                Token->Begin = Begin;
                                                Token->End = End;
                                                Lexer->ShouldContinue = false;
                                                break;
                                            }
                                            return Offset + SQLLexer_ConsumeToken(Lexer, Begin, End, Token);
        case SQL_CHARACTER_CLASS__INVALID:
                                            Throw_LexerError(Lexer, "Invalid token %c; Ignoring rest of query", *Begin);
                                            Token->Begin = Token->End = NULL;
                                            Token->Type = SQL_TOKEN_TYPE__INVALID;
                                            return 1;
        default:
                                            if( CharacterClass[*Begin] & SQL_CHARACTER_CLASS__SYMBOL ){
                                                Offset += TryConsume_SymbolToken(Lexer, Begin, End, Token);
                                                break;
                                            } else {
                                                Throw_LexerError(Lexer, "Unexpected literal \'%c\' >> %.5s|%c|%.8s...", *Begin,
                                                Begin - 5, *Begin, Begin + 1);
                                                *Token = INVALID_TOKEN;
                                            }
    }
    Lexer->Column += Offset;
    return Offset;
}

static const SQLLexerError LEXER_ERROR_NONE = {
    .Type       =   SQL_LEXER_ERROR_TYPE__NONE,
    .Iterator   =   NULL,
    .End        =   NULL,
    .Line       =   SIZE_MAX,
    .Column     =   SIZE_MAX
};

//  SQLLexer Interface
void SQLLexer_Initialize(SQLLexer* Lexer) {
    Lexer->ShouldContinue   =   true;
    Lexer->Line             =   0;
    Lexer->Column           =   0;
    Lexer->CurrentToken     =   INVALID_TOKEN;
    Lexer->NextToken        =   INVALID_TOKEN;
    Lexer->Begin            =   NULL;
    Lexer->End              =   NULL;
    Lexer->LastError        =   LEXER_ERROR_NONE;
}

void SQLLexer_SetSource(SQLLexer* Lexer, const char* Begin, const char* End) {
    Lexer->Begin    =   Begin;
    Lexer->End      =   End;

    SQLLexer_Advance(Lexer);
    SQLLexer_Advance(Lexer);
}

SQLLexerError SQLLexer_NextToken(SQLLexer* Lexer, SQLToken* Token) {
    if(Lexer->Begin < Lexer->End && Lexer->ShouldContinue) {
        Lexer->Begin += SQLLexer_ConsumeToken(Lexer, Lexer->Begin, Lexer->End, Token);

        Lexer->CurrentToken = Lexer->NextToken;
        Lexer->NextToken = *Token;

        return Lexer->LastError;
    } else {
        *Token = (SQLToken){
            .Type = SQL_TOKEN_TYPE__EOF,
            .Begin = Lexer->Begin,
            .End = Lexer->Begin,
            .Line = Lexer->Line,
            .Column = Lexer->Column
        };
        Lexer->ShouldContinue = false;
    }
}

SQLLexerError SQLLexer_Advance(SQLLexer* Lexer) {
    SQLToken Token;

    /*if(Lexer->Begin < Lexer->End && Lexer->ShouldContinue)
        Lexer->Begin += SQLLexer_ConsumeToken(Lexer, Lexer->Begin, Lexer->End, &Token);
    else
        Lexer->ShouldContinue = false;*/
    Lexer->Begin += SQLLexer_ConsumeToken(Lexer, Lexer->Begin, Lexer->End, &Token);
    
    Lexer->CurrentToken = Lexer->NextToken;
    Lexer->NextToken = Token;

    return Lexer->LastError;
}