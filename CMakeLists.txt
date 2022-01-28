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
    set(CARDINALDB_VERSION_MAJOR 0)
    set(CARDINALDB_VERSION_MINOR 1)
    set(CARDINALDB_VERSION_PATCH 0)
    set(CARDINALDB_PACKAGE_VERSION ${CARDINALDB_VERSION_MAJOR}.${CARDINALDB_VERSION_MINOR}.${CARDINALDB_VERSION_PATCH})
endif()

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(CARDINALDB_STANDALONE_BUILD YES)
    project(
        CardinalDB
        VERSION ${CARDINALDB_PACKAGE_VERSION}
        DESCRIPTION "A extendable database engine written in C11"
        LANGUAGES C
    )
endif()

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(
        FATAL_ERROR
        "
        In-source builds are strongly discouraged by the CardinalEngine project as they tend to pollute the source directory.
        Please create a dedicated build directory such as `build` or `build-cardinal` to store the build and the related
        configuration files. This process should have created `CMakeFiles.txt` and `CMakeCache` in the project root directory.
        Please remove them before proceeding further
        "
    )
endif()

##
##  Global Definitions
set(CARDINALDB_ROOT_DIR     ${CMAKE_CURRENT_SOURCE_DIR})
set(CARDINALDB_BUILD_DIR    ${CMAKE_CURRENT_BINARY_DIR})
set(CARDINALDB_INCLUDE_DIR  ${CARDINALDB_ROOT_DIR}/include)
set(CARDINALDB_SOURCE_DIR   ${CARDINALDB_ROOT_DIR}/source)

##
##  Build options

##
##  Configure the Configuration.h.in file for storing the configuration and build options
configure_file(${CARDINALDB_INCLUDE_DIR}/CardinalDB/Configuration.h.in ${CARDINALDB_INCLUDE_DIR}/CardinalDB/Configuration.h)

##
##  Targets
##  Build the tools that are required to build the library
add_subdirectory(${CARDINALDB_ROOT_DIR}/tools)
add_subdirectory(${CARDINALDB_ROOT_DIR}/source)
add_subdirectory(${CARDINALDB_ROOT_DIR}/testing)