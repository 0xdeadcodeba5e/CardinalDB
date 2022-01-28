##
##  BSD 3-Clause License
##  
##  Copyright (c) 2022, Santhosh K
##  All rights reserved.
##  
##  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
##  conditions are met:
##  
##  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
##     disclaimer.
##  
##  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
##     disclaimer in the documentation and/or other materials provided with the distribution.
##  
##  3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
##     products derived from this software without specific prior written permission.
##  
##  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
##  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
##  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
##  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
##  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
##  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
##  OF THE POSSIBILITY OF SUCH DAMAGE.
##
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

if(NOT DEFINED CARDINALDB_PACKAGE_VERSION)
    message(FATAL_ERROR "This file should not be directly used for configuration as this is part of the CardinalDB Project")
endif()

set(CARDINALDBSQL_HEADERS
    CardinalDB/Configuration.h
    CardinalDB/TypeDeclaration.h
)

set(CARDINALDBSQL_SOURCES
    Internal/SQL/Token.h
    Internal/SQL/Lexer.h
    Internal/SQL/Parser.h
    Internal/SQL/Query/Query.h
    Internal/SQL/Query/List.h
    Internal/SQL/Query/DDL.h
    Internal/SQL/Query/DML.h
    Internal/SQL/Query/TCL.h

    SQL/Lexer.c
    SQL/Parser.c
    SQL/Query/Query.c
    SQL/Query/List.c
    SQL/Query/TCL.c
    SQL/Query/DDL.c
)

set(CARDINALDBSQL_GENERATED_SOURCES
    "SQL/KeywordHash.c"
)


set(KEYWORD_HASH_C ${CARDINALDB_SOURCE_DIR}/SQL/KeywordHash.c)
add_custom_command(
    PRE_BUILD
    OUTPUT ${KEYWORD_HASH_C}
    COMMENT "Generating KeywordHash.c [REQUIRED by CardinalDB::SQL]"
    COMMAND ${CARDINALDB_TOOLS}/GenerateKeywordHash
    WORKING_DIRECTORY ${CARDINALDB_SOURCE_DIR}/SQL
    VERBATIM
)

add_library(CardinalDBSQLLib)
add_library(CardinalDB::SQL ALIAS CardinalDBSQLLib)
add_dependencies(CardinalDBSQLLib GenerateKeywordHash)

foreach(HEADER_FILE IN LISTS CARDINALDBSQL_HEADERS)
    target_sources(
        CardinalDBSQLLib PUBLIC
        $<BUILD_INTERFACE:${CARDINALDB_INCLUDE_DIR}/${HEADER_FILE}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/CardinalDB/${HEADER_FILE}>
    )
endforeach()

foreach(SOURCE_FILE IN LISTS CARDINALDBSQL_SOURCES)
    target_sources(
        CardinalDBSQLLib PRIVATE
        $<BUILD_INTERFACE:${CARDINALDB_SOURCE_DIR}/${SOURCE_FILE}>
    )
endforeach()

set_source_files_properties(${CARDINALDBSQL_GENERATED_SOURCES} PROPERTIES GENERATED TRUE)
foreach(GENERATED_SOURCE_FILE IN LISTS CARDINALDBSQL_GENERATED_SOURCES)
    target_sources(
        CardinalDBSQLLib PRIVATE
        $<BUILD_INTERFACE:${CARDINALDB_SOURCE_DIR}/${GENERATED_SOURCE_FILE}>
    )
endforeach()
target_include_directories(CardinalDBSQLLib PUBLIC ${CARDINALDB_INCLUDE_DIR} PRIVATE ${CARDINALDB_SOURCE_DIR})