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
#include "verb_veiksmazodis.h"
#include "noun_daiktavardis.h"
#include "adjective_budvardis.h"
#include "pronoun_ivardis.h"
#include "particle_dalelyte.h"
#include "adverb_prieveiksmis.h"
#include "participant_dalyvis.h"
#include "halfparticipant_podalyvis.h"
#include "preposition_prielinksnis.h"
#include "conjuction_jungtukas.h"
#include "numeral_skaitvardis.h"
#include <utils.h>
#include <map>

static int idcnt_ = 0;
static std::map<std::wstring, std::list<WordGeneric *>> zodynas_;

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
    VeiksmazodisGeneric *pv = 0;
    if (!cfg->is_dict()) {
        return;
    }

    AttributeType &Veiksmazodziai = (*cfg)[L"Veiksmazodis"];
    for (unsigned i = 0; i < Veiksmazodziai.size(); i++) {
        AttributeType &zodis = Veiksmazodziai[i];
        if (!zodis.is_dict()) {
            continue;
        }
        pv = new VeiksmazodisGeneric(&zodis);
        zodynas_[L"Veiksmazodis"].push_back(pv);
        p = new DalyvisGeneric(&zodis, pv);
        zodynas_[L"Dalyvis"].push_back(p);
        p = new PodalyvisGeneric(&zodis, pv);
        zodynas_[L"Podalyvis"].push_back(p);
    }

    AttributeType &Daiktavardziai = (*cfg)[L"Daiktavardis"];
    for (unsigned i = 0; i < Daiktavardziai.size(); i++) {
        AttributeType &zodis = Daiktavardziai[i];
        if (!zodis.is_dict()) {
            continue;
        }
        p = new DaiktavardisGeneric(&zodis);
        zodynas_[L"Daiktavardis"].push_back(p);
    }

    AttributeType &Budvardziai = (*cfg)[L"Budvardis"];
    for (unsigned i = 0; i < Budvardziai.size(); i++) {
        AttributeType &zodis = Budvardziai[i];
        if (!zodis.is_dict()) {
            continue;
        }
        p = new BudvardisGeneric(&zodis);
        zodynas_[L"Budvardis"].push_back(p);
    }

    AttributeType &Ivardziai = (*cfg)[L"Ivardis"];
    for (unsigned i = 0; i < Ivardziai.size(); i++) {
        AttributeType &zodis = Ivardziai[i];
        if (!zodis.is_dict()) {
            continue;
        }
        p = new IvardisGeneric(&zodis);
        zodynas_[L"Ivardis"].push_back(p);
    }

    AttributeType &Skiatvardzis = (*cfg)[L"Skaitvardis"];
    for (unsigned i = 0; i < Skiatvardzis.size(); i++) {
        AttributeType &zodis = Skiatvardzis[i];
        if (!zodis.is_dict()) {
            continue;
        }
        p = new SkaitvardisGeneric(&zodis);
        zodynas_[L"Skaitvardis"].push_back(p);
    }

    AttributeType &Dalelytes = (*cfg)[L"Dalelyte"];
    for (unsigned i = 0; i < Dalelytes.size(); i++) {
        AttributeType &zodis = Dalelytes[i];
        if (!zodis.is_dict()) {
            continue;
        }
        p = new DalelyteGeneric(&zodis);
        zodynas_[L"Dalelyte"].push_back(p);
    }

    // наречие
    AttributeType &Prieveiksmiai = (*cfg)[L"Prieveiksmis"];
    for (unsigned i = 0; i < Prieveiksmiai.size(); i++) {
        AttributeType &zodis = Prieveiksmiai[i];
        if (!zodis.is_dict()) {
            continue;
        }
        p = new PrieveiksmisGeneric(&zodis);
        zodynas_[L"Prieveiksmis"].push_back(p);
    }

    // предлоги
    AttributeType &Prielinksniai = (*cfg)[L"Prielinksnis"];
    for (unsigned i = 0; i < Prielinksniai.size(); i++) {
        AttributeType &zodis = Prielinksniai[i];
        if (!zodis.is_dict()) {
            continue;
        }
        p = new PrielinksnisGeneric(&zodis);
        zodynas_[L"Prielinksnis"].push_back(p);
    }

    // союзы
    AttributeType &Jungtukai = (*cfg)[L"Jungtukas"];
    for (unsigned i = 0; i < Jungtukai.size(); i++) {
        AttributeType &zodis = Jungtukai[i];
        if (!zodis.is_dict()) {
            continue;
        }
        p = new JungtukasGeneric(&zodis);
        zodynas_[L"Jungtukas"].push_back(p);
    }

    // Знаки препинания
    AttributeType &Skirtukiai = (*cfg)[L"Skirtukas"];
    for (unsigned i = 0; i < Skirtukiai.size(); i++) {
        AttributeType &zodis = Skirtukiai[i];
        if (!zodis.is_dict()) {
            continue;
        }
        p = new WordGeneric(&zodis);
        zodynas_[L"Skirtukas"].push_back(p);
    }
}

// gauti = get
WordGeneric *WRD_gauti_zodis(const wchar_t *s, const wchar_t *type) {
    if (wcscmp(type, L"AsIs") == 0) {
        AttributeType cfg(Attr_Dict);
        cfg[L"Value"].make_string(s);
        return new WordGeneric(&cfg);
    }
    for (auto &p : zodynas_[type]) {
        if (wcscmp(p->getValue(), s) == 0) {
            return p;
        }
    }
    return 0;
}

