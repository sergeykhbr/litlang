// 
//  Copyright 2023 Sergey Khabarov, sergeykhbr@gmail.com
// 
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
// 
//      http://www.apache.org/licenses/LICENSE-2.0
// 
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
// 

#pragma once

#include <inttypes.h>
#include <stdarg.h>

#if defined(_WIN32) || defined(__CYGWIN__)
    #define RV_PRI64 L"I64"
#else
    # if defined(__WORDSIZE) && (__WORDSIZE == 64)
    #  define RV_PRI64 L"l"
    # else
    #  define RV_PRI64 L"ll"
    # endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

int printf_log(const wchar_t *fmt, ...);
int sprintf_log(wchar_t *s, size_t len, const wchar_t *fmt, ...);

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define printf_error(fmt, ...) \
    printf_log(L"\n%s:%d " fmt, WIDEN(__FILE__), __LINE__, __VA_ARGS__)

int utils_read_json_file(const char *filename, void *outattr);
void utils_set_current_dir();

#ifdef __cplusplus
}
#endif

