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
#ifndef CARDINALDB__INCLUDE__CONFIGURATION_H_
#define CARDINALDB__INCLUDE__CONFIGURATION_H_

#if defined(__clang__) && !defined(__ibmxl__)
#   define CARDINALDB_COMPILER_CLANG (__clang_major__ * 100) + __clang_minor__
#else
#   define CARDINALDB_COMPILER_CLANG 0
#endif

#if defined(__GNUC__) && !defined(__clang__)
#   define CARDINALDB_COMPILER_GCC (__GNUC__ * 100) + __GNUC_MINOR__
#else
#   define CARDINALDB_COMPILER_GCC 0
#endif

#if defined(_MSC_VER)
#   define CARDINALDB_COMPILER_MSVC _MSC_VER
#else
#   define CARDINALDB_COMPILER_MSVC 0
#endif

#if !(CARDINALDB_COMPILER_CLANG || CARDINALDB_COMPILER_GCC || CARDINALDB_COMPILER_MSVC)
#   error "CardinalEngine supports GCC, Clang and MSVC compiler at the time of releasing the file"
#endif

#if CARDINALDB_COMPILER_MSVC
#   define CARDINALDB_EXPORT __declspec(dllexport)
#   define CARDINALDB_IMPORT __declspec(dllimport)
#else
#   define CARDINALDB_EXPORT __attribute__((visibility("default")))
#   define CARDINALDB_IMPORT
#endif

#if !defined(CARDINALDB_INLINE)
#   if CARDINALDB_COMPILER_GCC | CARDINALDB_COMPILER_CLANG
#       define CARDINALDB_INLINE __attribute__((always_inline))
#   elif CARDINALDB_COMPILER_MSVC
#       define CARDINALDB_INLINE
#   endif
#endif

#if defined(CARDINALDB_SHARED_BUILD)
#   if defined(CARDINALDB_BUILD_LIBRARY)
#       define CARDINALDB_API CARDINALDB_EXPORT
#   else
#       define CARDINALDB_API CARDINALDB_IMPORT
#   endif
#else
#   define CARDINALDB_API
#endif

#define CARDINALDB_VERSION_MAJOR 0
#define CARDINALDB_VERSION_MINOR 1
#define CARDINALDB_VERSION_PATCH 0

#define CARDINALDB_PACKAGE_VERSION "v0.1.0"

#endif
