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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const char KeywordHashHeader[] = 
"/*\n"
"**  CardinalDB - KeywordHash\n"
"**  BSD 3-Clause License\n"
"**  \n"
"**  Copyright (c) 2022, Santhosh K\n"
"**  All rights reserved.\n"
"**  \n"
"**  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following\n"
"**  conditions are met:\n"
"**  \n"
"**  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following\n"
"**     disclaimer.\n"
"**  \n"
"**  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following\n"
"**     disclaimer in the documentation and/or other materials provided with the distribution.\n"
"**  \n"
"**  3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote\n"
"**     products derived from this software without specific prior written permission.\n"
"**  \n"
"**  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES,\n"
"**  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE\n"
"**  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,\n"
"**  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF\n"
"**  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT\n"
"**  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED\n"
"**  OF THE POSSIBILITY OF SUCH DAMAGE.\n"
"*/\n"
"///    This file is automatically generated using the file \n"
"///    \n"
"///        cardinal-engine/tools/generate-Word-hash.c\n"
"///    \n"
"///    The code in this standalone file provides a function `is_Word` which determines the given literal, represented as \n"
"///    stream of characters, is a CardinalDB Word or an identifier. This method is based on the sqlite's mkWordhash.c"
"///    tool.\n"
"#include <CardinalDB/Configuration.h>\n"
"#include <CardinalDB/TypeDeclaration.h>\n"
"#include <Internal/SQL/Token.h>\n\n";

static const char KeywordHashCFooter[] =
"/// Check to see whether the given string is a keyword. If it is then write\n"
"/// the respective sql_token_type_t valude for that keyword into `token_type`.\n"
"/// Always return the length of the keyword\n"
"static uint32_t DeduceKeyword(const char* Iterator, const uint32_t Length, SQLTokenType* TokenType) {\n"
"    int32_t Hash, j;\n"
"    const char* KeywordIterator;\n"
"    if( Length >= 2 ) {\n";

static const char KeywordHashCFooter2[] = 
"        for(Hash = ((int32_t)KeywordHash[Hash]) - 1; Hash >= 0; Hash = ((int32_t)KeywordNext[Hash]) - 1){\n"
"            if(KeywordLength[Hash] != Length) continue;\n"
"            KeywordIterator = &KeywordsEncoded[KeywordOffset[Hash]];\n"
"            if((Iterator[0] & ~0x20) != KeywordIterator[0]) continue;\n"
"            if((Iterator[1] & ~0x20) != KeywordIterator[1]) continue;\n"
"            j = 2;\n"
"            while( j < Length && (Iterator[j] & ~0x20) == KeywordIterator[j] ) j++;\n"
"            if( j < Length ) continue;\n"
"            *TokenType = KeywordToken[Hash];\n"
"            break;\n"
"        }\n"
"    }\n"
"    return Length;\n"
"};\n"
"\n"
"SQLTokenType DeduceLiteralType(const char* Iterator, uint32_t Length) {\n"
"    SQLTokenType Type = SQL_TOKEN_TYPE__LITERAL;\n"
"    DeduceKeyword(Iterator, Length, &Type);\n"
"    return Type;\n"
"}\n"
"\n"
"bool IsSQLKeyword(const char* Iterator, uint32_t Length) {\n"
"    return SQL_TOKEN_TYPE__LITERAL != DeduceLiteralType(Iterator, Length);\n"
"}\n";

static const char* KeywordHashCHashFormat = 
"        Hash = (((0x20 | *Iterator) * %u) %c                       \n\
                ((0x20 | *(Iterator + Length - 1)) * %u) %c         \n\
                Length * %u) %% %u;                                 \n";

typedef struct __cardinal_db_Word {
    char*       Name;
    char*       TokenType;
    uint32_t    Priority;
    uint32_t    Identifier;
    uint32_t    Hash;
    int32_t     Offset;
    uint32_t    Length;
    uint32_t    Prefix;
    uint32_t    LongestSuffix;
    uint32_t    Next;
    uint32_t    SubstringID;
    uint32_t    SubstringOffset;
    char        OriginalName[ 32 ];
} Keyword;

static Keyword KeywordTable[] = {
    { "ABORT",            "ABORT",                  0      },
    { "ACTION",           "ACTION",                 0      },
    { "ADD",              "ADD",                    1      },
    { "AFTER",            "AFTER",                  0      },
    { "ALL",              "ALL",                    0      },
    { "ALTER",            "ALTER",                  0      },
    { "ALWAYS",           "ALWAYS",                 0      },
    { "ANALYZE",          "ANALYZE",                0      },
    { "AND",              "AND",                    10     },
    { "AS",               "AS",                     10     },
    { "ASC",              "ASC",                    0      },
    { "ATTACH",           "ATTACH",                 1      },
    { "AUTOINCREMENT",    "AUTOINCREMENT",          0      },
    { "BEFORE",           "BEFORE",                 0      },
    { "BEGIN",            "BEGIN",                  1      },
    { "BETWEEN",          "BETWEEN",                5      },
    { "BY",               "BY",                     10     },
    { "CASCADE",          "CASCADE",                1      },
    { "CASE",             "CASE",                   5      },
    { "CAST",             "CAST",                   5      },
    { "CHECK",            "CHECK",                  1      },
    { "COLLATE",          "COLLATE",                1      },
    { "COLUMN",           "COLUMN",                 1      },
    { "COMMIT",           "COMMIT",                 1      },
    { "CONFLICT",         "CONFLICT",               0      },
    { "CONSTRAINT",       "CONSTRAINT",             1      },
    { "CREATE",           "CREATE",                 2      },
    { "CROSS",            "CROSS",                  3      },
    { "CURRENT",          "CURRENT",                1      },
    { "CURRENT_TIME",     "CURRENT_TIME",           1      },
    { "CURRENT_TIMESTAMP","CURRENT_TIMESTAMP",      1      },
    { "DATABASE",         "DATABASE",               0      },
    { "DATE",             "DATE",                   1,     },
    { "DEFAULT",          "DEFAULT",                1      },
    { "DEFERRED",         "DEFERRED",               1      },
    { "DEFERRABLE",       "DEFERRABLE",             1      },
    { "DELETE",           "DELETE",                 10     },
    { "DESC",             "DESC",                   3      },
    { "DETACH",           "DETACH",                 0      },
    { "DISTINCT",         "DISTINCT",               5      },
    { "DO",               "DO",                     2      },
    { "DROP",             "DROP",                   1      },
    { "END",              "END",                    1      },
    { "EACH",             "EACH",                   1      },
    { "ELSE",             "ELSE",                   2      },
    { "ESCAPE",           "ESCAPE",                 4      },
    { "EXCEPT",           "EXCEPT",                 4      },
    { "EXCLUSIVE",        "EXCLUSIVE",              1      },
    { "EXCLUDE",          "EXCLUDE",                1      },
    { "EXISTS",           "EXISTS",                 4      },
    { "EXPLAIN",          "EXPLAIN",                1      },
    { "FAIL",             "FAIL",                   1      },
    { "FILTER",           "FILTER",                 4      },
    { "FIRST",            "FIRST",                  4      },
    { "FOLLOWING",        "FOLLOWING",              4      },
    { "FOR",              "FOR",                    2      },
    { "FOREIGN",          "FOREIGN",                1      },
    { "FROM",             "FROM",                   10     },
    { "FULL",             "FULL",                   3      },
    { "GENERATED",        "GENERATED",              1      },
    { "GLOB",             "GLOB",                   3      },
    { "GROUP",            "GROUP",                  5      },
    { "GROUPS",           "GROUPS",                 2      },
    { "HAVING",           "HAVING",                 5      },
    { "IF",               "IF",                     2      },
    { "IGNORE",           "IGNORE",                 1      },
    { "IMMEDIATE",        "IMMEDIATE",              1      },
    { "IN",               "IN",                     10     },
    { "INDEX",            "INDEX",                  1      },
    { "INDEXED",          "INDEXED",                0      },
    { "INITIALLY",        "INITIALLY",              1      },
    { "INNER",            "INNER",                  1      },
    { "INSERT",           "INSERT",                 10     },
    { "INSTEAD",          "INSTEAD",                1      },
    { "INTERSECT",        "INTERSECT",              5      },
    { "INTO",             "INTO",                   10     },
    { "IS",               "IS",                     5      },
    { "ISNULL",           "ISNULL",                 5      },
    { "JOIN",             "JOIN",                   5      },
    { "KEY",              "KEY",                    1      },
    { "LAST",             "LAST",                   4      },
    { "LEFT",             "LEFT",                   5      },
    { "LIKE",             "LIKE",                   5      },
    { "LIMIT",            "LIMIT",                  3      },
    { "MATCH",            "MATCH",                  2      },
    { "MATERIALIZED",     "MATERIALIZED",           12     },
    { "MODIFY",           "MODIFY",                 10     },
    { "NATURAL",          "NATURAL",                3      },
    { "NO",               "NO",                     2      },
    { "NOT",              "NOT",                    10     },
    { "NOTHING",          "NOTHING",                1      },
    { "NOTNULL",          "NOTNULL",                3      },
    { "NULL",             "NULL",                   10     },
    { "NULLS",            "NULLS",                  3      },
    { "NUMBER",           "NUMBER",                 10     },
    { "OF",               "OF",                     3      },
    { "OFFSET",           "OFFSET",                 1      },
    { "ON",               "ON",                     1      },
    { "OR",               "OR",                     9      },
    { "ORDER",            "ORDER",                  10     },
    { "OTHERS",           "OTHERS",                 3      },
    { "OUTER",            "OUTER",                  5      },
    { "OVER",             "OVER",                   3      },
    { "PARTITION",        "PARTITION",              3      },
    { "PLAN",             "PLAN",                   0      },
    { "PRAGMA",           "PRAGMA",                 0      },
    { "PRECEDING",        "PRECEDING",              3      },
    { "PRIMARY",          "PRIMARY",                1      },
    { "QUERY",            "QUERY",                  0      },
    { "RAISE",            "RAISE",                  1      },
    { "RANGE",            "RANGE",                  3      },
    { "RECURSIVE",        "RECURSIVE",              3      },
    { "REFERENCES",       "REFERENCES",             1      },
    { "REGEXP",           "REGEXP",                 3      },
    { "REINDEX",          "REINDEX",                1      },
    { "RELEASE",          "RELEASE",                1      },
    { "RENAME",           "RENAME",                 1      },
    { "REPLACE",          "REPLACE",                10     },
    { "RESTRICT",         "RESTRICT",               1      },
    { "RETURNING",        "RETURNING",              10     },
    { "RIGHT",            "RIGHT",                  0      },
    { "ROLLBACK",         "ROLLBACK",               1      },
    { "ROW",              "ROW",                    1      },
    { "ROWS",             "ROWS",                   1      },
    { "SAVEPOINT",        "SAVEPOINT",              1      },
    { "SELECT",           "SELECT",                 10     },
    { "SET",              "SET",                    10     },
    { "STRING",           "STRING",                 10     },
    { "TABLE",            "TABLE",                  1      },
    { "TEMP",             "TEMP",                   1      },
    { "TEMPORARY",        "TEMPORARY",              1      },
    { "THEN",             "THEN",                   3      },
    { "TIES",             "TIES",                   3      },
    { "TIME",             "TIME",                   1      },
    { "TIMESTAMP",        "TIMESTAMP",              1      },
    { "TO",               "TO",                     3      },
    { "TRANSACTION",      "TRANSACTION",            1      },
    { "TRIGGER",          "TRIGGER",                1      },
    { "TRUNCATE",         "TRUNCATE",               1      },
    { "UNBOUNDED",        "UNBOUNDED",              3      },
    { "UNION",            "UNION",                  3      },
    { "UNIQUE",           "UNIQUE",                 1      },
    { "UPDATE",           "UPDATE",                 10     },
    { "USING",            "USING",                  8      },
    { "VACUUM",           "VACUUM",                 1      },
    { "VALUES",           "VALUES",                 10     },
    { "VIEW",             "VIEW",                   1      },
    { "VIRTUAL",          "VIRTUAL",                1      },
    { "WHEN",             "WHEN",                   1      },
    { "WHERE",            "WHERE",                  10     },
    { "WINDOW",           "WINDOW",                 3      },
    { "WITH",             "WITH",                   4      },
    { "WITHOUT",          "WITHOUT",                1      },
};
static const unsigned NumberOfKeywords = sizeof(KeywordTable) / sizeof(Keyword);

static int Keyword_CompareLength(const void* LHS, const void* RHS) {
    const Keyword* LHSKeyword = (Keyword *)LHS;
    const Keyword* RHSKeyword = (Keyword *)RHS;
    
    int difference = LHSKeyword->Length - RHSKeyword->Length;
    if(difference == 0)
        difference = strcmp(LHSKeyword->Name, RHSKeyword->Name);
    return difference;
}

static int Keyword_CompareSuffixLength(const void* LHS, const void* RHS) {
    const Keyword* LHSKeyword = (Keyword *)LHS;
    const Keyword* RHSKeyword = (Keyword *)RHS;
    
    int difference = RHSKeyword->LongestSuffix - LHSKeyword->LongestSuffix;
    if(difference == 0)
        difference = strcmp(LHSKeyword->Name, RHSKeyword->Name);
    return difference;
}

static int Keyword_CompareOffset(const void* LHS, const void* RHS) {
    const Keyword* LHSKeyword = (Keyword *)LHS;
    const Keyword* RHSKeyword = (Keyword *)RHS;
    
    int difference = LHSKeyword->Offset - RHSKeyword->Offset;
    if(difference == 0)
        difference = LHSKeyword->Identifier - RHSKeyword->Identifier;
    return difference;
}

static Keyword* GetKeyword_ByID(const uint32_t ID) {
    for(uint32_t i = 0; i < NumberOfKeywords; ++i)
        if(KeywordTable[i].Identifier == ID)
            return &KeywordTable[i];
    return NULL;
}

static void Reorder(uint32_t* KeywordFrom) {
    if(*KeywordFrom <= 0) return;
    uint32_t Current = *KeywordFrom - 1;
    uint32_t Next = KeywordTable[ Current ].Next;
    if(Next == 0) return;
    Next -= 1;

    if(KeywordTable[Current].Priority >= KeywordTable[Next].Priority) return;

    KeywordTable[Current].Next = KeywordTable[Next].Next;
    KeywordTable[Next].Next = Current + 1;
    *KeywordFrom = Next + 1;

    Reorder(&KeywordTable[Current].Next);
}

#define HASH_OP ^
#define HASH_CC '^'
#define HASH_C0 4
#define HASH_C1 3
#define HASH_C2 1
#define WRITE(...) fprintf(KeywordHashC, __VA_ARGS__)

int main(int argc, char** argv) {
    uint32_t    KeywordHash[ 1024 ];
    char        KeywordEncoded[ 2048 ];

    //  Fill in the lengths of the strings and hashes for all the entries
    uint32_t TotalLength = 0;
    for(uint32_t i = 0; i < NumberOfKeywords; ++i) {
        Keyword* Word = &KeywordTable[ i ];
        Word->Length = (uint32_t)strlen(Word->Name);
        memcpy(Word->OriginalName, Word->Name, Word->Length + 1);
        TotalLength += Word->Length;
        Word->Hash =    ((0x20 | Word->Name[0]) * HASH_C0)                  HASH_OP
                        ((0x20 | Word->Name[ Word->Length - 1 ]) * HASH_C1) HASH_OP
                        (Word->Length * HASH_C2);
        Word->Identifier = i + 1;
    }

    //  Sort the KeywordTable from shortest to longest
    qsort(KeywordTable, NumberOfKeywords, sizeof(Keyword), Keyword_CompareLength);

    //  Look for the shortest Words that can be embedded into longer Words
    for(int32_t i = NumberOfKeywords - 2; i >= 0; --i) {
        Keyword* LHS = &KeywordTable[i];
        for(int32_t j = NumberOfKeywords - 1; j >= 0; --j) {
            Keyword* RHS = &KeywordTable[j];

            if(RHS->SubstringID) continue;
            if(RHS->Length <= LHS->Length) continue;
            for(uint32_t k = 0; k <= RHS->Length - LHS->Length; ++k)
                if(memcmp(LHS->Name, &RHS->Name[k], LHS->Length) == 0) {
                    LHS->SubstringID = RHS->Identifier;
                    LHS->SubstringOffset = k;
                    break;
                }
            
        }
    }

    //  Compute the LongestSuffix valud for every Word
    for(uint32_t i = 0; i < NumberOfKeywords; ++i) {  
        Keyword* LHS = &KeywordTable[i];
        if(LHS->SubstringID) continue;

        for(uint32_t j = 0; j < NumberOfKeywords; ++j) {
            if(j == i) continue;

            Keyword* RHS = &KeywordTable[j];
            if(RHS->SubstringID) continue;
            
            for(uint32_t k = RHS->LongestSuffix + 1; k < LHS->Length && k < RHS->Length; ++k)
                if(memcmp(&LHS->Name[LHS->Length - k], RHS->Name, k) == 0) {
                    LHS->LongestSuffix = k;
                }
            
        }
    }

    //  Sort the KeywordTable in the reverse order of length
    qsort(KeywordTable, NumberOfKeywords, sizeof (Keyword), Keyword_CompareSuffixLength);

    // Fill in the offset for all the entries
    uint32_t NumberOfCharacters = 0;
    for(uint32_t i = 0; i < NumberOfKeywords; ++i) {
        Keyword* LHS = &KeywordTable[i];
        if(LHS->Offset > 0 || LHS->SubstringID) continue;

        LHS->Offset = NumberOfCharacters;
        NumberOfCharacters += LHS->Length;

        for(uint32_t k = LHS->Length - 1; k >= 1; --k) {
            for(uint32_t j = i + 1; j < NumberOfKeywords; ++j) {
                Keyword* RHS = &KeywordTable[j];

                if(RHS->Offset > 0 || RHS->SubstringID || RHS->Length <= k) continue;
                if( memcmp(&LHS->Name[LHS->Length - k], RHS->Name, k) == 0 ) {
                    LHS = RHS;
                    LHS->Offset = NumberOfCharacters - k;
                    NumberOfCharacters = LHS->Offset + LHS->Length;
                    LHS->Name += k;
                    LHS->Length -= k;
                    LHS->Prefix = k;
                    j = i;
                    k = LHS->Length;
                }
            }
        }
    }
    for(uint32_t i = 0; i < NumberOfKeywords; ++i) {
        Keyword* Word = &KeywordTable[i];
        if(Word->SubstringID)
            Word->Offset =   GetKeyword_ByID(Word->SubstringID)->Offset + Word->SubstringOffset;
    }

    //  Sort the KeywordTable by offset
    qsort(KeywordTable, NumberOfKeywords, sizeof (Keyword), Keyword_CompareOffset);

    //  Calculate the best hash table size in order to minimize the number of collisions
    uint32_t BestSize = NumberOfKeywords;
    uint32_t BestCount = NumberOfKeywords * NumberOfKeywords;

    for(uint32_t i = NumberOfKeywords / 2; i <= NumberOfKeywords* 2; ++i) {
        if(i <= 0) continue;
        for(uint32_t j = 0; j < i; ++j) KeywordHash[i] = 0;
        for(uint32_t j = 0; j < NumberOfKeywords; ++j) {
            uint32_t hash = KeywordTable[j].Hash % i;
            KeywordHash[hash] *= 2;
            KeywordHash[hash]++;
        }
        uint32_t Count = 0;
        for(uint32_t j = 0; j < i; ++j) Count += KeywordHash[j];
        
        if(Count < BestCount) {
            BestCount   = Count;
            BestCount   = i;
        }
    }

    //  Compute the hash
    for(uint32_t i = 0; i < BestSize; ++i) KeywordHash[i] = 0;
    for(uint32_t i = 0; i < NumberOfKeywords; ++i) {
        uint32_t Hash = KeywordTable[i].Hash % BestSize;
        KeywordTable[i].Next = KeywordHash[Hash];
        KeywordHash[Hash] = i + 1;
        Reorder(&KeywordHash[Hash]);
    }

    //  Generate the required code
    FILE* KeywordHashC = fopen("KeywordHash.c", "w+");
    WRITE("%s", KeywordHashHeader);
    WRITE("//   Hash Score : %u\n", BestCount);
    WRITE("//   KeywordsEncoded[] has encoded %uBytes of Word string into %uBytes of string\n",
        TotalLength + NumberOfKeywords, NumberOfCharacters + 1
    );

    uint32_t i, j, k;
    for(i = 0, j = 0, k = 0; i < NumberOfKeywords; ++i) {
        Keyword* Word = &KeywordTable[i];

        if(Word->SubstringID) continue;

        memcpy(&KeywordEncoded[k], Word->Name, Word->Length);
        k += Word->Length;

        if( j + Word->Length > 114) {
            WRITE("%*s\n", 121 - j, "");
            j = 0;
        }
        if(j == 0) {
            WRITE("/// ");
            j = 8;
        }
        WRITE("%s", Word->Name);
        j += Word->Length;
    }
    if(j > 0)
        WRITE("%*s ///\n\n", 121 - j, "");

    WRITE("static const char KeywordsEncoded[ %u ] = { \n", NumberOfCharacters);
    KeywordEncoded[ NumberOfCharacters ] = 0;
    for(i = j = 0; i < k; ++i) {
        if(j == 0)
            WRITE("    ");
        if( KeywordEncoded[i] == 0 )
            WRITE("0");
        else
            WRITE("'%c',", KeywordEncoded[i]);
        j += 4;
        if( j > 111 ) {
            WRITE("\n");
            j = 0;
        }
    }
    if(j > 0) WRITE("\n");
    WRITE("};\n\n");

    WRITE("/// KeywordHash[i] is the hash value for the i-th keyword\n");
    WRITE("static const uint32_t KeywordHash[ %u ] = {\n", BestSize);
    for(i = j = 0; i < BestSize; ++i) {
        if(j == 0) WRITE("    ");
        WRITE(" %3u,", KeywordHash[i]);
        j++;
        if(j > 16) {
            WRITE("\n");
            j  = 0;
        }
    }
    WRITE("\n};\n\n");

    WRITE("/// KeywordNext[] forms the hash collision chain. If KeywordHash[i] == 0\n");
    WRITE("/// then the i-th keyword hash no more hash no more collisions. Otherwise, \n");
    WRITE("/// the next keyword with the same hash is KeywordNext[i] - 1\n");
    WRITE("static const uint32_t KeywordNext[ %u ] = {\n", NumberOfKeywords);
    for(i = j = 0; i < NumberOfKeywords; ++i) {
        if( j == 0) WRITE("    ");
        WRITE(" %3u,", KeywordTable[i].Next);
        j += 1;
        if( j > 12 ) {
            WRITE("\n");
            j = 0;
        }
    }
    WRITE("\n};\n\n");

    WRITE("/// Length[] is the length of the ith-th keyword (in bytes)\n");
    WRITE("static const uint32_t KeywordLength[ %u ] = {\n", NumberOfKeywords);
    for(i = j = 0; i < NumberOfKeywords; ++i) {
        if( j == 0) WRITE("    ");
        WRITE(" %3u,", KeywordTable[i].Length + KeywordTable[i].Prefix);
        j += 1;
        if( j > 12 ) {
            WRITE("\n");
            j = 0;
        }
    }
    WRITE("\n};\n\n");

    WRITE("/// Offset[] is the indexinto KeywordEncoded[] which points to the\n");
    WRITE("/// start of the i-th keyword\n");
    WRITE("static const uint32_t KeywordOffset[ %u ] = {\n", NumberOfKeywords);
    for(i = j = 0; i < NumberOfKeywords; ++i) {
        if( j == 0) WRITE("    ");
        WRITE(" %3u,", KeywordTable[i].Offset);
        j += 1;
        if( j > 12 ) {
            WRITE("\n");
            j = 0;
        }
    }
    WRITE("\n};\n\n");

    WRITE("/// KeywordToken[i] is the token_type for the i-th keyword\n");
    WRITE("static const SQLTokenType KeywordToken[ %u ] = {\n", NumberOfKeywords);
    for(i = j = 0; i < NumberOfKeywords; ++i) {
        char* token_type = KeywordTable[i].TokenType;
        WRITE("    SQL_TOKEN_TYPE__KEYWORD_%s,\n", token_type);
    }
    WRITE("\n};\n\n");

    WRITE(KeywordHashCFooter);
    WRITE(KeywordHashCHashFormat, HASH_C0, HASH_CC, HASH_C1, HASH_CC, HASH_C2, BestSize);
    WRITE(KeywordHashCFooter2);

    fclose(KeywordHashC);
    return 0;
}