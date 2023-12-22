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

#if defined(_WIN32) || defined(__CYGWIN__)
#include <windows.h>
#endif
#include <wchar.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <attribute.h>
#include "utils.h"

static wchar_t tstr[1<<16];
FILE *logfile_ = 0;
const char *LOG_FILENAME = "_can.log";

extern "C" {

int printf_log(const wchar_t *fmt, ...) {
    int ret = 0;
    va_list arg;
    va_start(arg, fmt);
#if defined(_WIN32) || defined(__CYGWIN__)
    ret += vswprintf_s(&tstr[ret], sizeof(tstr) / sizeof(wchar_t), fmt, arg);
#else
    ret += vswprintf(&buf[ret], fmt, arg);
#endif
    va_end(arg);

    wprintf(L"%s", tstr);

    if (logfile_ == 0) {
        logfile_ = fopen(LOG_FILENAME, "wb");
        if (logfile_ == 0) {
            printf("Can't open file %s\n", LOG_FILENAME);
            return ret;
        }
    }
    fwrite(tstr, sizeof(wchar_t), ret, logfile_);
    fflush(logfile_);
    return ret;
}

int sprintf_log(wchar_t *s, size_t len, const wchar_t *fmt, ...) {
    int ret;
    va_list arg;
    va_start(arg, fmt);
#if defined(_WIN32) || defined(__CYGWIN__)
    ret = vswprintf_s(s, len / sizeof(wchar_t), fmt, arg);
#else
    ret = vswprintf(s, fmt, arg);
#endif
    va_end(arg);
    return ret;
}

int json_file_readline(FILE *f, wchar_t *buf, int &sz) {
    int c = getc(f);
    int ret = sz;
    while (c != EOF) {
        if (ret == sz && (c == L'\n' || c == L'\r')) {
            // empty line
            c = getc(f);
            continue;
        } else if ((c & 0xE0) == 0xC0) {
            // 2-bytes unicode sequence
            c = c & 0x1F;
            c = (c << 6) | (getc(f) & 0x3F);

        }
        buf[ret++] = static_cast<wchar_t>(c);
        buf[ret] = L'\0';
        if (c == L'\n') {
            break;
        }
        c = getc(f);
    }
    sz = ret;
    return c;
}

int json_file_readall(const char *filename, wchar_t **pout) {
    FILE *f = fopen(filename, "rb");
    wchar_t *incdata;
    int textcnt;
    int incsz;
    int fsz;
    int ret = 0;
    if (!f) {
        return 0;
    }
    fseek(f, 0, SEEK_END);
    fsz = ftell(f);
    fseek(f, 0, SEEK_SET);

    (*pout) = new wchar_t[fsz + 1];
    memset((*pout), 0, (fsz + 1) * sizeof(wchar_t));

    textcnt = 0;
    while (json_file_readline(f, (*pout), textcnt) != EOF) {
        wchar_t *psub1 = wcsstr(&(*pout)[ret], L"#include");
        if (psub1 != 0) {
            char fname[4096];
            char *pf = fname;
            psub1 += 8;     // skip #include
            while (*psub1 == L' ') {
                psub1++;
            }
            if (*psub1 == L'"' || *psub1 == L'\'') {
                psub1++;
            }
            while (*psub1 && *psub1 != L'"' && *psub1 != L'\'') {
                *pf++ = *psub1++;
                *pf = L'\0';
            }
            // todo:  read file
            incsz = json_file_readall(fname, &incdata);
            if (incsz) {
                // realloc buffer:
                wchar_t *t1 = new wchar_t [(fsz + incsz) / sizeof(wchar_t) + 1];
                memset(t1, 0, fsz + incsz + sizeof(wchar_t));
                memcpy(t1, (*pout), ret);
                memcpy(&t1[ret], incdata, incsz);
                delete [] incdata;
                delete [] (*pout);
                (*pout) = t1;
                fsz += incsz;
                textcnt = ret + incsz;
            }
        }
        ret = textcnt;
    }
    fclose(f);
    return ret;
}

int utils_read_json_file(const char *filename, void *outattr) {
    AttributeType *out = reinterpret_cast<AttributeType *>(outattr);
    wchar_t *tbuf;
    int sz = json_file_readall(filename, &tbuf);
    if (sz) {
        out->make_data((sz + 1) * sizeof(wchar_t), tbuf);
        delete [] tbuf;
    }
    return sz;
}

void utils_set_current_dir() {
#if defined(_WIN32) || defined(__CYGWIN__)
    HMODULE hMod = GetModuleHandle(NULL);
    char path[MAX_PATH] = "";
    GetModuleFileNameA(hMod, path, MAX_PATH);
#else         // Linux
    // Get path of executable.
    char path[1024];
    ssize_t n = readlink("/proc/self/exe", path,
                         sizeof(path)/sizeof(path[0]) - 1);
    if (n == -1) {
        return;
    }
    path[n] = 0;
#endif

    size_t i;
    for(i = strlen(path) - 1; i > 0 && path[i] != '/' && path[i] != '\\'; --i);
    path[i] = '\0';

#if defined(_WIN32) || defined(__CYGWIN__)
    SetCurrentDirectoryA(path);
#else         // Linux
    chdir(path);
#endif
}

}  // extern "C"