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

#include "api_wrd.h"
#include <attribute.h>
#include "noun_daiktavardis.h"
#include "adjective_budvardis.h"
#include "pronoun_ivardis.h"
#include "particle_dalelyte.h"
#include <utils.h>

static int idcnt_ = 0;
static std::list<WordGeneric *> zodynas_;

int WRD_unique_id() {
    return ++idcnt_;
}

// Kalbos dalys, части речи:
//      daiktavardis, noun, существительное (daikta = вещь, vardis = имя)
//      būdvardis, adjective, прилагательное (būd = способ)
//      veiksmazodis, verb, глагол (veiksma = действие)
//      įvardis, pronoun, местоимение
void WRD_pridelioti_zodis(AttributeType *cfg) {
    WordGeneric *p = 0;
    if (!cfg->is_dict()) {
        return;
    }
    AttributeType &zodiai = (*cfg)[L"Items"];
    for (unsigned i = 0; i < zodiai.size(); i++) {
        AttributeType &zodis = zodiai[i];
        if (!zodis.is_dict()) {
            continue;
        }

        if (zodis[L"Type"].is_equal(L"daiktavardis")) {
            p = new DaiktavardisGeneric(cfg);
        } else if (zodis[L"Type"].is_equal(L"budvardis")) {
            p = new BudvardisGeneric(cfg);
        } else if (zodis[L"Type"].is_equal(L"ivardis")) {
            p = new IvardisGeneric(cfg);
        } else if (zodis[L"Type"].is_equal(L"dalelyte")) {
            p = new DalelyteGeneric(cfg);
        } else {
            printf_error(L"Undefined Type %s", zodis[L"Type"].to_string());
        }

        if (p) {
            zodynas_.push_back(p);
        }
    }
}

// gauti = get
WordGeneric *WRD_gauti_zodis(const wchar_t *s) {
    for (auto &p : zodynas_) {
        if (wcscmp(p->getValue(), s) == 0) {
            return p;
        }
    }
    return 0;
}

void WRD_info() {
    for (auto &p : zodynas_) {
        p->info();
    }
}
