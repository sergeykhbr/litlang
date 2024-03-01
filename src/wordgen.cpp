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
            tbuf[cnt++] = L'ū';
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

EAtvejis WordGeneric::str2atvejis(const wchar_t *s) {
    EAtvejis ret = Vardininkas;
    if (wcscmp(s, L"Kilmininkas") == 0
        || wcscmp(s, L"Kil") == 0 || wcscmp(s, L"K") == 0) {
        ret = Kilmininkas;
    } else if (wcscmp(s, L"Naudininkas") == 0
        || wcscmp(s, L"Nau") == 0 || wcscmp(s, L"N") == 0) {
        ret = Naudininkas;
    } else if (wcscmp(s, L"Galininkas") == 0
        || wcscmp(s, L"Gal") == 0 || wcscmp(s, L"G") == 0) {
        ret = Galininkas;
    } else if (wcscmp(s, L"Inagininkas") == 0
        || wcscmp(s, L"In") == 0 || wcscmp(s, L"I") == 0) {
        ret = Inagininkas;
    } else if (wcscmp(s, L"Vietininkas") == 0
        || wcscmp(s, L"Vt") == 0) {
        ret = Vietininkas;
    } else if (wcscmp(s, L"Sauksmininkas") == 0
        || wcscmp(s, L"Sau") == 0 || wcscmp(s, L"S") == 0) {
        ret = Sauksmininkas;
    }
    return ret;
}

EGimine WordGeneric::str2gimine(const wchar_t *s) {
    EGimine ret = Vyriskoji;
    if (wcscmp(s, L"Moteriskoji") == 0
        || wcscmp(s, L"M") == 0) {
        ret = Moteriskoji;
    } else if (wcscmp(s, L"Bevarde") == 0) {
        ret = Bevarde;
    }
    return ret;
}

ESkaicus WordGeneric::str2skaicus(const wchar_t *s) {
    ESkaicus ret = Vienaskaita;
    if (wcscmp(s, L"Daugiskaita") == 0
        || wcscmp(s, L"D") == 0) {
        ret = Daugiskaita;
    }
    return ret;
}

ELaikas WordGeneric::str2laikas(const wchar_t *asmuo) {
    ELaikas ret = Esamasis;
    if (wcscmp(asmuo, L"Esamasis") == 0) {
        ret = Esamasis;
    } else if (wcscmp(asmuo, L"Butasis") == 0) {
        ret = Butasis;
    } else if (wcscmp(asmuo, L"ButasisDazninis") == 0) {
        ret = ButasisDazninis;
    } else if (wcscmp(asmuo, L"Busimasis") == 0) {
        ret = Busimasis;
    }
    return ret;
}


EAsmuo WordGeneric::str2asmuo(const wchar_t *asmuo) {
    EAsmuo ret = as;
    if (wcscmp(asmuo, L"as") == 0) {
        ret = as;
    } else if (wcscmp(asmuo, L"tu") == 0) {
        ret = tu;
    } else if (wcscmp(asmuo, L"jis") == 0) {
        ret = jis;
    } else if (wcscmp(asmuo, L"ji") == 0) {
        ret = ji;
    } else if (wcscmp(asmuo, L"mes") == 0) {
        ret = mes;
    } else if (wcscmp(asmuo, L"jus") == 0) {
        ret = jus;
    } else if (wcscmp(asmuo, L"jie") == 0) {
        ret = jie;
    } else if (wcscmp(asmuo, L"jos") == 0) {
        ret = jos;
    }
    return ret;
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

