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

#include "wordgen.h"
#include "api_wrd.h"

WordGeneric::WordGeneric(AttributeType *cfg) {
    wchar_t tbuf[256];
    const wchar_t *twstr = (*cfg)[L"Value"].to_string();
    int stressed = -1; // ударная, kirčiuotas
    int cnt = 0;
    while (*twstr) {
        if (*twstr >= L'A' && *twstr <= L'Z') {
            stressed = cnt;
            tbuf[cnt++] = *twstr - L'A' + L'a';
        } else if (*twstr == L'Ą') {
            stressed = cnt;
            tbuf[cnt++] = L'ą';
        } else if (*twstr == L'Č') {
            tbuf[cnt++] = L'č';
        } else if (*twstr == L'Ę') {
            stressed = cnt;
            tbuf[cnt++] = L'ę';
        } else if (*twstr == L'Ė') {
            stressed = cnt;
            tbuf[cnt++] = L'ė';
        } else if (*twstr == L'Į') {
            tbuf[cnt++] = L'į';
        } else if (*twstr == L'Š') {
            tbuf[cnt++] = L'š';
        } else if (*twstr == L'Ų') {
            stressed = cnt;
            tbuf[cnt++] = L'ų';
        } else if (*twstr == L'Ū') {
            stressed = cnt;
            tbuf[cnt++] = L'Ū';
        } else if (*twstr == L'Ž') {
            tbuf[cnt++] = L'ž';
        } else {
            tbuf[cnt++] = *twstr;
        }
        twstr++;
        tbuf[cnt] = 0;
    }

    value_ = std::wstring(tbuf);
    id_ = WRD_unique_id();
}

int WordGeneric::add2wline(wchar_t *buf, int pos, const wchar_t *s, int align) {
    wchar_t *p = &buf[pos];
    while (*s) {
        *p++ = *s++;
        *p = 0;
    }
   for (size_t i = p - &buf[pos]; i < align; i++) {
        *p++ = L' ';
        *p = 0;
   }
   return static_cast<int>(pos + (p - &buf[pos]));
}

