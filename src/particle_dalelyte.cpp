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

#include "particle_dalelyte.h"
#include <utils.h>

/** 
    Dalelytė - частица, {"Type":"dalelyte"}
            

*/

DalelyteGeneric::DalelyteGeneric(AttributeType *cfg)
    : WordGeneric(cfg),
    gimine_(Gimine_nezinoma)
{
    if ((*cfg).has_key(L"Gimine")) {
        AttributeType &gimine = (*cfg)[L"Gimine"];
        if (gimine.is_equal(L"V") || gimine.is_equal(L"Vyriskoji")) {
            gimine_ = Vyriskoji;
        } else if (gimine.is_equal(L"M") || gimine.is_equal(L"Moteriskoji")) {
            gimine_ = Moteriskoji;
        } else {
            printf_error(L"Neteisinga Gimine %s", gimine.to_string());
        }
    }

    AttributeType &ru = (*cfg)[L"Ru"];
    AttributeType tattr;
    ru_.make_list(0);
    if (ru.is_string()) {
        ru_.add_to_list(&ru);
    } else if (ru.is_list()) {
        for (unsigned i = 0; i < ru.size(); i++) {
            AttributeType &t1 = ru[i];
            ru_.add_to_list(&ru[i]);
        }
    }
}

void DalelyteGeneric::info() {
    wchar_t tstr[1024];
    int tcnt = 0;
    tcnt = add2wline(tstr, tcnt, L"\nDalelytė: ", 0);
    tcnt = add2wline(tstr, tcnt, value_.c_str(), 0);
    tcnt = add2wline(tstr, tcnt, L", (", 0);
    for (unsigned i = 0; i < ru_.size(); i++) {
        AttributeType &ruitem = ru_[i];
        if (i > 0) {
            tcnt = add2wline(tstr, tcnt, L"; ", 0);
        }
        tcnt = add2wline(tstr, tcnt, ruitem.to_string(), 0);
    }
    tcnt = add2wline(tstr, tcnt, L")", 0);
    printf_log(L"%s\n", tstr);
}
