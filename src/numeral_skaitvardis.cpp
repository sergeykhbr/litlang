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

#include "numeral_skaitvardis.h"
#include <utils.h>

/** 
    Skaitvardis - числительное {"Type":"skaitvardis"}
        - Род
             Vyriskoji
             Moteriskoji
        - число
             Vienaskaita
             Daugiskaita
        - разряд
*/

SkaitvardisGeneric::SkaitvardisGeneric(AttributeType *cfg) : WordGeneric(cfg) {
    AttributeType &lt = (*cfg)[L"Lt"];
    if (lt.is_string()) {
    } else if (lt.is_list()) {
        for (unsigned i = 0; i < lt.size(); i++) {
            AttributeType &v = lt[i];
            if (v.is_list()) {
                lt_[i][Vyriskoji] = std::wstring(v[0u].to_string());
                lt_[i][Moteriskoji] = std::wstring(v[0u].to_string());
            }
        }
    }

    AttributeType &ru = (*cfg)[L"Ru"];
    if (ru.is_string()) {
        ru_[Vardininkas][Vyriskoji] = std::wstring(ru.to_string());
    } else if (ru.is_list()) {
        for (unsigned i = 0; i < ru.size(); i++) {
            AttributeType &v = ru[i];
            if (v.is_list()) {
                ru_[i][Vyriskoji] = std::wstring(v[0u].to_string());
                ru_[i][Moteriskoji] = std::wstring(v[0u].to_string());
            }
        }
    }
}

void SkaitvardisGeneric::info() {
    wchar_t tstr[1024];
    int tcnt = 0;
    tcnt = add2wline(tstr, tcnt, L"\nSkaitvardis: ", 0);
    tcnt = add2wline(tstr, tcnt, value_.c_str(), 0);
    tcnt = add2wline(tstr, tcnt, L", (", 0);
    tcnt = add2wline(tstr, tcnt, ru_[Vardininkas][Vyriskoji].c_str(), 0);
    tcnt = add2wline(tstr, tcnt, L")", 0);
    printf_log(L"%s\n", tstr);
}

std::wstring SkaitvardisGeneric::gautiForma(AttributeType &arg) {
    EGimine gimine = str2gimine(arg[L"Gimine"].to_string());
    EAtvejis atvejis = str2atvejis(arg[L"Atvejis"].to_string());
    return lt_[atvejis][gimine];
}
