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

#include "adjective.h"
#include <utils.h>

/** 
   Būdvardis - прилагательное, {"Type":"budvardis"}
     - форма
         местоименная
         не местоименная
         сравнительная местоименная
         сравнительная не местоименная
         превосходящая местоименнаая
         превосходящая не местоименная
     - Geminė, род
         Vyreškoji мужской
         Moteriškoji женский
     - Skaičiu
         Vienaskaita 
         Daugiskaita
     - Atvejis, падеж
        Verdininkas, koks? kokia? какой? какая?
        Kilmininkas, kokio? kokios?
        Naudininkas, kokiam? kokiai?
        Galininkas, kokį? kokia?
        Įnagininkas, kokiu? kokia?
        Vietininkas, kokiame? kokioje?
     - Linksniuotė - склонение {"Linksniuote":1 or ..}
        1 .. 3 в зависимости от окончания
*/

BudvardisGeneric::BudvardisGeneric(AttributeType *cfg)
    : WordGeneric(cfg),
    gimine_(Gimine_nezinoma),
    paradigma_(Paradigma_nezinoma),
    linksniuote_(Linksniuote_nezinoma)
{
    if ((*cfg).has_key(L"Gimine")) {
        AttributeType &gimine = (*cfg)[L"Gimine"];
        if (gimine.is_equal(L"V") || gimine.is_equal(L"Vyriskoji")) {
            gimine_ = Vyriskoji;
        } else if (gimine.is_equal(L"M") || gimine.is_equal(L"Moteriskoji")) {
            gimine_ = Moteriskoji;
        } else {
            printf_error(L"Undefined Gimine %s", gimine.to_string());
        }
    }
    // Linksniuote turi būti nustatyta su gimine
    if ((*cfg).has_key(L"Linksniuote")) {
        AttributeType &linksniuote = (*cfg)[L"Linksniuote"];
        if (gimine_ == Vyriskoji) {
            if (linksniuote.to_int() == 1) {
                linksniuote_ = Linksniuote_V1;
            } else if (linksniuote.to_int() == 2) {
                linksniuote_ = Linksniuote_V2;
            } else if (linksniuote.to_int() == 3) {
                linksniuote_ = Linksniuote_V3;
            }
        } else if (gimine_ == Moteriskoji) {
            if (linksniuote.to_int() == 1) {
                linksniuote_ = Linksniuote_M1;
            } else if (linksniuote.to_int() == 2) {
                linksniuote_ = Linksniuote_M2;
            } else if (linksniuote.to_int() == 3) {
                linksniuote_ = Linksniuote_M3;
            }
        } else {
            printf_error(L"Neteisingas budvardžio linksnio formatas %d",
                        linksniuote.to_int());
        }
    }

    AttributeType &ru = (*cfg)[L"Ru"];
    if (ru.is_list()) {
        ru_[Vienaskaita][Vardininkas] = std::wstring(ru[0u].to_string());
    } else if (ru.is_string()) {
        ru_[Vienaskaita][Vardininkas] = std::wstring(ru.to_string());
    }

    nustatyti_paradigma();
    atnaujinti();
}

// Užpildykite deklinacijos lentelę - Заполнить таблицу склонений (склонятельную таблицу)
void BudvardisGeneric::atnaujinti() {
    std::wstring saknis = imkSaknis();

    switch (paradigma_) {
    case Paradigma_V1_as:
        lentele_[Vienaskaita][Vardininkas] = saknis + std::wstring(L"as");  // именительный. geras = хороший
        lentele_[Vienaskaita][Kilmininkas] = saknis + std::wstring(L"o");   // родительный. gero = хорошего
        lentele_[Vienaskaita][Naudininkas] = saknis + std::wstring(L"am");  // дательный/полезный. geram = хорошему
        lentele_[Vienaskaita][Galininkas] = saknis + std::wstring(L"ą");    // винительный/могущий. gerą = хорошего
        lentele_[Vienaskaita][Inagininkas] = saknis + std::wstring(L"u");   // творительный. geru = хорошим
        lentele_[Vienaskaita][Vietininkas] = saknis + std::wstring(L"ame"); // местный. gerame = в хорошем

        lentele_[Daugiskaita][Vardininkas] = saknis + std::wstring(L"i");   // geri = хорошие
        lentele_[Daugiskaita][Kilmininkas] = saknis + std::wstring(L"ų");   // gerų = хороших
        lentele_[Daugiskaita][Naudininkas] = saknis + std::wstring(L"iems");// geriems = хорошим
        lentele_[Daugiskaita][Galininkas] = saknis + std::wstring(L"us");   // gerus = хорошие
        lentele_[Daugiskaita][Inagininkas] = saknis + std::wstring(L"ais"); // gerais = хорошими
        lentele_[Daugiskaita][Vietininkas] = saknis + std::wstring(L"uose");// geruose = в хороших
        break;
    }
}

// root of the word (common part without ending):
std::wstring BudvardisGeneric::imkSaknis() {
    std::wstring ret = L"";
    wchar_t tstr[256] = {0};
    int wsz = static_cast<int>(value_.size());
    if ((wsz + 2) >= sizeof(tstr)/sizeof(wchar_t)) {
        printf_error("Buferio perpildymas %d", wsz);
    }
    memcpy(tstr, value_.c_str(), (wsz + 1) * sizeof(wchar_t));
    
    switch (paradigma_) {
    case Paradigma_M1_a:
    case Paradigma_M2_i:
    case Paradigma_M3_e:
        tstr[wsz - 1] = L'\0';
        break;
    case Paradigma_V1_as:
    case Paradigma_V1_is:
    case Paradigma_V2_us:
    case Paradigma_V3_is:
    case Paradigma_M1_ia:
        tstr[wsz - 2] = L'\0';
        break;
    case Paradigma_V1_ias:
        tstr[wsz - 3] = L'\0';
        break;
    }
    ret = std::wstring(tstr);
    return ret;
}

// Определить парадигму
void BudvardisGeneric::nustatyti_paradigma() {
    const wchar_t *p = value_.c_str();
    int sz = static_cast<int>(value_.size());
    int tcnt = 0;
    if (p[sz-1] == L's') {
        if (p[sz-3] == L'i' && p[sz-2] == L'a') {
            paradigma_ = Paradigma_V1_ias;
        } else if (p[sz-2] == L'a') {
            paradigma_ = Paradigma_V1_as;
        } else if (p[sz-2] == L'i'
            && (linksniuote_ == Linksniuote_nezinoma || linksniuote_ == Linksniuote_V1)) {
            paradigma_ = Paradigma_V1_is;
        } else if (p[sz-2] == L'i' && linksniuote_ == Linksniuote_V3) {
            paradigma_ = Paradigma_V3_is;
        } else if (p[sz-2] == L'u') {
            paradigma_ = Paradigma_V2_us;
        } else {
            printf_error(L"Negaliu nustatyti paradigma s: %s", p);
        }
    } else if (p[sz-1] == L'a') {
        if (p[sz-2]) {
            paradigma_ = Paradigma_M1_ia;
        } else {
            paradigma_ = Paradigma_M1_a;
        }
    } else if (p[sz-1] == L'i') {
        paradigma_ = Paradigma_M2_i;
    } else if (p[sz-1] == L'ė') {
        paradigma_ = Paradigma_M3_e;
    } else {
        printf_error(L"Negaliu nustatyti paradigma: %s", p);
    }
}

int BudvardisGeneric::imkLinksniuote() {
    int ret = Linksniuote_nezinoma;
    switch (paradigma_) {
    // 1 linksniuotė
    case Paradigma_V1_as:
    case Paradigma_V1_ias:
    case Paradigma_V1_is:
    case Paradigma_M1_ia:
    case Paradigma_M1_a:
        ret = 1;
        break;
    // 2 linksniuotė
    case Paradigma_V2_us:
    case Paradigma_M2_i:
        ret = 2;
        break;
    // 3 linksniuotė
    case Paradigma_V3_is:
    case Paradigma_M3_e:
        ret = 3;
        break;
    }
    return ret;
}

void BudvardisGeneric::info() {
    wchar_t tstr[1024];
    std::wstring wstr;
    int tcnt = 0;
    printf_log(L"\nDaiktavardis: %s, linksniuotė: %d, (%s)\n",
            value_.c_str(),
            imkLinksniuote(),
            ru_[Vienaskaita][Vardininkas].c_str());

    wstr = L"Kilmininkas ";
    if (gimine_ == Vyriskoji) {
        wstr += L"(koks?): ";
    } else {
        wstr += L"(kokia?): ";
    }
    tcnt = add2wline(tstr, 0, wstr.c_str(), 30);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Kilmininkas].c_str(), 24);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Kilmininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);

    wstr = L"Naudininkas ";
    if (gimine_ == Vyriskoji) {
        wstr += L"(kokio?): ";
    } else {
        wstr += L"(kokios?): ";
    }
    tcnt = add2wline(tstr, 0, wstr.c_str(), 30);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Naudininkas].c_str(), 24);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Naudininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);

    wstr = L"Galininkas ";
    if (gimine_ == Vyriskoji) {
        wstr += L"(kokiam?): ";
    } else {
        wstr += L"(kokiai?): ";
    }
    tcnt = add2wline(tstr, 0, wstr.c_str(), 30);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Galininkas].c_str(), 24);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Galininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);

    wstr = L"Inagininkas ";
    if (gimine_ == Vyriskoji) {
        wstr += L"(kokiu?): ";
    } else {
        wstr += L"(kokia?): ";
    }
    tcnt = add2wline(tstr, 0, wstr.c_str(), 30);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Inagininkas].c_str(), 24);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Inagininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);

    wstr = L"Vietininkas ";
    if (gimine_ == Vyriskoji) {
        wstr += L"(kokiame?): ";
    } else {
        wstr += L"(kokioje?): ";
    }
    tcnt = add2wline(tstr, 0, wstr.c_str(), 30);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Vietininkas].c_str(), 24);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Vietininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);
}


