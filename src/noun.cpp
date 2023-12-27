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

#include "noun.h"
#include <utils.h>

/** 
    Dayktavardis - существительное, {"Type":"daiktavardis"}
        Giminė - род, {"Gimine":"V", "Vyriskoji" or "M", "Moteriskoji"}
            Vyriškoji (V) - мужской
            Moteriškoji (M) - женский
        Skaičius - число, {"Skaicius":"V" or "D'}
            Vienaskaita (V) - единственное
            Daugiskaita (D) - множественное
        Atvejis - падеж, {"Atvejis":"V" or ..}
            Vardininkas, kas? (V) именительный, кто, что?
            Kilmininkas, ko? (K)  родительный, кого, чего?
            Naudininkas, kam? (N) дательный, кому, чему?
            Galininkas, ką? (G) винительный, кого, что?
            Inagininkas, kuo? (In) инструментальный (творительный), кем, чем?
            Vietininkas, kur? kame? (Vi) местный, где? в ком? в чем?
            Šauksmininkas, (S) звательный
        Linksniuotė - склонение, {"Linksninuote":1 or ..}
            1 .. 5 в зависимости от окончания
*/

DaiktavardisGeneric::DaiktavardisGeneric(AttributeType *cfg)
    : WordGeneric(cfg),
    gimine_(Gimine_nezinoma),
    paradigma_(Paradigma_nezinoma),
    linksniuote_(Linksniuote_nezinoma)
{
    if ((*cfg).has_key(L"Gimine")) {
        AttributeType &gimine = (*cfg)[L"Gimine"];
        if (gimine.is_equal(L"V") || gimine.is_equal(L"Vyriskoji")) {
            gimine_ = Gimine_viriskoji;
        } else if (gimine.is_equal(L"M") || gimine.is_equal(L"Moteriskoji")) {
            gimine_ = Gimine_moteriskoji;
        } else {
            printf_error(L"Undefined Gimine %s", gimine.to_string());
        }
    }
    if ((*cfg).has_key(L"Linksniuote")) {
        AttributeType &linksniuote = (*cfg)[L"Linksniuote"];
        if (linksniuote.to_int() == 1) {
            linksniuote_ = Linksniuote_1;
        } else if (linksniuote.to_int() == 2) {
            linksniuote_ = Linksniuote_2;
        } else if (linksniuote.to_int() == 3) {
            linksniuote_ = Linksniuote_3;
        } else if (linksniuote.to_int() == 4) {
            linksniuote_ = Linksniuote_4;
        } else if (linksniuote.to_int() == 5) {
            linksniuote_ = Linksniuote_5;
        } else {
            printf_error(L"Neteisingas daiktavardžio linksnio formatas %d",
                        linksniuote.to_int());
        }
    }

    nustatyti_paradigma();
    atnaujinti();
}

// Определить парадигму
void DaiktavardisGeneric::nustatyti_paradigma() {
    const wchar_t *p = value_.c_str();
    int sz = static_cast<int>(value_.size());
    int tcnt = 0;
    /*
    // 1 linksniuotė
    Paradigma_1_as,
    Paradigma_1_ias,
    Paradigma_1_is,
    Paradigma_1_ys,
    // 2 linksniuotė
    Paradigma_2_a,
    Paradigma_2_ia,
    Paradigma_2_ė,
    // 3 linksniuotė
    Paradigma_3_is,
    // 4 linksniuotė
    Paradigma_4_us,
    Paradigma_4_ius,
    // 5 linksniuotė
    Paradigma_5_uo,
    Paradigma_5_ė,*/
    if (p[sz-1] == L's') {
        if (p[sz-3] == L'i' && p[sz-2] == L'a') {
            paradigma_ = Paradigma_1_ias;
        } else if (p[sz-2] == L'a') {
            paradigma_ = Paradigma_1_as;
        } else if (p[sz-2] == L'y') {
            paradigma_ = Paradigma_1_ys;
        } else if (p[sz-2] == L'i'
            && (linksniuote_ == Linksniuote_nezinoma || linksniuote_ == Linksniuote_1)) {
            paradigma_ = Paradigma_1_is;
        } else if (p[sz-2] == L'i' && linksniuote_ == Linksniuote_3) {
            paradigma_ = Paradigma_3_is;
        } else if (p[sz-2] == L'u') {
            if (p[sz-3] == L'i') {
                paradigma_ = Paradigma_4_ius;
            } else {
                paradigma_ = Paradigma_4_us;
            }
        } else {
            printf_error(L"Negaliu nustatyti paradigma s: %s", p);
        }
    } else if (p[sz-1] == L'a') {
        if (p[sz-2]) {
            paradigma_ = Paradigma_2_ia;
        } else {
            paradigma_ = Paradigma_2_a;
        }
    } else if (p[sz-1] == L'ė') {
        if (linksniuote_ == Linksniuote_nezinoma || linksniuote_ == Linksniuote_2) {
            paradigma_ = Paradigma_2_e;
        } else if (linksniuote_ == Linksniuote_5) {
            paradigma_ = Paradigma_5_e;
        } else {
            printf_error(L"Negaliu nustatyti paradigma ė: %s", p);
        }
    } else if (p[sz-2] == L'u' && p[sz-1] == L'o') {
        paradigma_ = Paradigma_5_uo;
    } else {
        printf_error(L"Negaliu nustatyti paradigma: %s", p);
    }
}

// Užpildykite deklinacijos lentelę - Заполнить таблицу склонений (склонятельную таблицу)
void DaiktavardisGeneric::atnaujinti() {
    std::wstring saknis = imkSaknis();
    bool st = true;
}

// root of the word (common part without ending):
std::wstring DaiktavardisGeneric::imkSaknis() {
    std::wstring ret = L"";
    wchar_t tstr[256] = {0};
    int wsz = static_cast<int>(value_.size());
    if ((wsz + 2) >= sizeof(tstr)/sizeof(wchar_t)) {
        printf_error("Buferio perpildymas %d", wsz);
    }
    memcpy(tstr, value_.c_str(), (wsz + 1) * sizeof(wchar_t));
    
    switch (paradigma_) {
    case Paradigma_2_a:
    case Paradigma_2_e:
    case Paradigma_5_e:
        tstr[wsz - 1] = L'\0';
        break;
    case Paradigma_1_as:
    case Paradigma_1_is:
    case Paradigma_1_ys:
    case Paradigma_2_ia:
    case Paradigma_3_is:
    case Paradigma_4_us:
    case Paradigma_5_uo:
        tstr[wsz - 2] = L'\0';
        break;
    case Paradigma_1_ias:
    case Paradigma_4_ius:
        tstr[wsz - 3] = L'\0';
        break;
    }
    ret = std::wstring(tstr);
    return ret;
}
