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

#include "halfparticipant_podalyvis.h"
#include "utils.h"

/** 
    https://www.lituanistica.ru/materials_grammar.html#5

    Podalyvis - полупричастие, {"Type":"podalyvis"}, переводится как деепричастие
        - Giminė - род, {"Gimine":"Vyriskoji", "Moteriskoji"}
            Vyriškoji
            Moteriškoji
        - Skaičius - число, {"Skacius":"Vienaskaita","Daugiskaita"}
            Vienaskaita
            Daugiskaita
        - {"Sangrazynis":"-si"} - возвратный
*/

PodalyvisGeneric::PodalyvisGeneric(AttributeType *cfg, VeiksmazodisGeneric *verb)
    : WordGeneric(cfg),
    veiksmazodis_(verb)
{
    atnaujinti();
}

void PodalyvisGeneric::atnaujinti() {
    std::wstring saknis1 = veiksmazodis_->imkSaknis(1);

    lt_[Vienaskaita][Vyriskoji] = saknis1 + L"damas";
    lt_[Vienaskaita][Moteriskoji] = saknis1 + L"dama";
    lt_[Daugiskaita][Vyriskoji] = saknis1 + L"dami";
    lt_[Daugiskaita][Moteriskoji] = saknis1 + L"damios";

    lt_si_[Vienaskaita][Vyriskoji] = saknis1 + L"damas" + L"is";
    lt_si_[Vienaskaita][Moteriskoji] = saknis1 + L"dama" + L"si";
    lt_si_[Daugiskaita][Vyriskoji] = saknis1 + L"dam" + L"iesi";
    lt_si_[Daugiskaita][Moteriskoji] = saknis1 + L"dam" + L"osi";
}

void PodalyvisGeneric::info() {
    wchar_t tstr[1024];
    int wsz = static_cast<int>(value_.size()) + 8; // ending + spaces (pabaiga + įtrauka)
    int tcnt = 0;
    int aligncnt = wsz;
    if (aligncnt < 20) {
        aligncnt = 20;
    }
    printf_log(L"\nPodalyvis: %s\n", value_.c_str());

    // 19 = aligncnt - 1
    printf_log(L"%30s %19s %19s %19s\n",
        L"ед.ч.,м.р.", L"ед.ч.,ж.р.", L"мн.ч.,м.р.", L"мн.ч.,ж.р.");

    tcnt = add2wline(tstr, 0, L" ", 20);
    tcnt = add2wline(tstr, tcnt, lt_[Vienaskaita][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Vienaskaita][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Daugiskaita][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Daugiskaita][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);
}

std::wstring PodalyvisGeneric::gautiForma(AttributeType &arg) {
    std::wstring ret = L"";
    ESkaicus skaicus = str2skaicus(arg[L"Skaicius"].to_string());
    EGimine gimine = str2gimine(arg[L"Gimine"].to_string());

    if (arg.has_key(L"Sangrazinis")) {
        ret = lt_si_[skaicus][gimine];
    } else {
        ret = lt_[skaicus][gimine];
    }

    return ret;
}
