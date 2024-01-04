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
   Būdvardis - прилагательное, {"Type":"budvardis"}, может склоняться
   в мужском и женском роде:
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
     - Linksniuotė
         Vyriškoji 1 = -is
         Vyriųkoji 3 = -is
     - Atvejis, падеж
        Verdininkas, koks? kokia? / kokie? kokios? (какой? какая? / какие? какие?)
        Kilmininkas, kokio? kokios? kokių? (какого? какой? / каких?)
        Naudininkas, kokiam? kokiai? kokiems? kokioms? (какому? какой? / каким? каким?)
        Galininkas, kokį? kokia?
        Įnagininkas, kokiu? kokia?
        Vietininkas, kokiame? kokioje?
*/

BudvardisGeneric::BudvardisGeneric(AttributeType *cfg)
    : WordGeneric(cfg),
    gimine_(Gimine_nezinoma),
    paradigma_(Paradigma_nezinoma)
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
    int linksniuote = Linksniuote_nezinoma;
    if ((*cfg).has_key(L"Linksniuote")) {
        AttributeType &Linksniuote = (*cfg)[L"Linksniuote"];
        linksniuote = Linksniuote.to_int();
    }

    AttributeType &ru = (*cfg)[L"Ru"];
    if (ru.is_list()) {
        ru_[Vienaskaita][Vyriskoji][Vardininkas] = std::wstring(ru[0u].to_string());
    } else if (ru.is_string()) {
        ru_[Vienaskaita][Vyriskoji][Vardininkas] = std::wstring(ru.to_string());
    }

    nustatyti_paradigma(linksniuote);
    atnaujinti();
}

// Užpildykite deklinacijos lentelę - Заполнить таблицу склонений (склонятельную таблицу)
void BudvardisGeneric::atnaujinti() {
    std::wstring saknis = imkSaknis();
    std::wstring *l;

    switch (paradigma_) {
    case Paradigma_V1_as:
        l = lentele_[Vienaskaita][Vyriskoji];
        l[Vardininkas] = saknis + std::wstring(L"as");  // именительный. geras = хороший
        l[Kilmininkas] = saknis + std::wstring(L"o");   // родительный. gero = хорошего
        l[Naudininkas] = saknis + std::wstring(L"am");  // дательный/полезный. geram = хорошему
        l[Galininkas] = saknis + std::wstring(L"ą");    // винительный/могущий. gerą = хорошего
        l[Inagininkas] = saknis + std::wstring(L"u");   // творительный. geru = хорошим
        l[Vietininkas] = saknis + std::wstring(L"ame"); // местный. gerame = в хорошем

        l = lentele_[Daugiskaita][Vyriskoji];
        l[Vardininkas] = saknis + std::wstring(L"i");   // geri = хорошие
        l[Kilmininkas] = saknis + std::wstring(L"ų");   // gerų = хороших
        l[Naudininkas] = saknis + std::wstring(L"iems");// geriems = хорошим
        l[Galininkas] = saknis + std::wstring(L"us");   // gerus = хорошие
        l[Inagininkas] = saknis + std::wstring(L"ais"); // gerais = хорошими
        l[Vietininkas] = saknis + std::wstring(L"uose");// geruose = в хороших

        l = lentele_[Vienaskaita][Moteriskoji];
        l[Vardininkas] = saknis + std::wstring(L"a");   // именительный. gera = хорошая
        l[Kilmininkas] = saknis + std::wstring(L"os");  // родительный. geros = хорошего
        l[Naudininkas] = saknis + std::wstring(L"ai");  // дательный/полезный. gerai = хорошей
        l[Galininkas] = saknis + std::wstring(L"ą");    // винительный/могущий. gerą = хорошую
        l[Inagininkas] = saknis + std::wstring(L"a");   // творительный. gera = хорошей
        l[Vietininkas] = saknis + std::wstring(L"oje"); // местный. geroje = в хорошей

        l = lentele_[Daugiskaita][Moteriskoji];
        l[Vardininkas] = saknis + std::wstring(L"os");   // geros = хорошие ж.р
        l[Kilmininkas] = saknis + std::wstring(L"ų");    // gerų = хороших ж.р
        l[Naudininkas] = saknis + std::wstring(L"oms");  // geriems = хорошим ж.р
        l[Galininkas] = saknis + std::wstring(L"as");    // gerus = хорошие ж.р.
        l[Inagininkas] = saknis + std::wstring(L"omis"); // gerais = хорошими ж.р
        l[Vietininkas] = saknis + std::wstring(L"ose");  // geruose = в хороших ж.р
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
void BudvardisGeneric::nustatyti_paradigma(int linksniuote) {
    const wchar_t *p = value_.c_str();
    int sz = static_cast<int>(value_.size());
    int tcnt = 0;
    if (p[sz-1] == L's') {
        if (p[sz-3] == L'i' && p[sz-2] == L'a') {
            paradigma_ = Paradigma_V1_ias;
        } else if (p[sz-2] == L'a') {
            paradigma_ = Paradigma_V1_as;
        } else if (p[sz-2] == L'i'
            && (linksniuote == Linksniuote_nezinoma || linksniuote == 1)) {
            paradigma_ = Paradigma_V1_is;
        } else if (p[sz-2] == L'i' && linksniuote == 3) {
            paradigma_ = Paradigma_V3_is;
        } else if (p[sz-2] == L'u') {
            paradigma_ = Paradigma_V2_us;
        } else {
            printf_error(L"Negaliu nustatyti Vyriskoji paradigma: %s", p);
        }
        if (gimine_ == Gimine_nezinoma) {
            gimine_ = Vyriskoji;
        } else if (gimine_ == Moteriskoji) {
            printf_error(L"Neteisinga gimine: %d", gimine_);
        }
    } else {
        if (p[sz-1] == L'a') {
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
            printf_error(L"Negaliu nustatyti Moteriskoji paradigma: %s", p);
        }

        if (gimine_ == Gimine_nezinoma) {
            gimine_ = Moteriskoji;
        } else if (gimine_ == Vyriskoji) {
            printf_error(L"Neteisinga gimine: %d", gimine_);
        }
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
            ru_[Vienaskaita][Vyriskoji][Vardininkas].c_str());

    tcnt = add2wline(tstr, 0, L"Vardininkas: ", 16);
    tcnt = add2wline(tstr, tcnt, L"koks? ", 10);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Vyriskoji][Vardininkas].c_str(), 12);
    tcnt = add2wline(tstr, tcnt, L"kokia? ", 10);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Moteriskoji][Vardininkas].c_str(), 12);
    tcnt = add2wline(tstr, tcnt, L"kokie? ", 10);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Vyriskoji][Vardininkas].c_str(), 12);
    tcnt = add2wline(tstr, tcnt, L"kokios? ", 10);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Moteriskoji][Vardininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);


    tcnt = add2wline(tstr, 0, L"Kilmininkas: ", 16);
    tcnt = add2wline(tstr, tcnt, L"kokio? ", 10);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Vyriskoji][Kilmininkas].c_str(), 12);
    tcnt = add2wline(tstr, tcnt, L"kokios? ", 10);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Moteriskoji][Kilmininkas].c_str(), 12);
    tcnt = add2wline(tstr, tcnt, L"kokių? ", 10);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Vyriskoji][Kilmininkas].c_str(), 12);
    tcnt = add2wline(tstr, tcnt, L"kokių? ", 10);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Moteriskoji][Kilmininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);


    tcnt = add2wline(tstr, 0, L"Naudininkas: ", 16);
    tcnt = add2wline(tstr, tcnt, L"kokiam? ", 10);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Vyriskoji][Naudininkas].c_str(), 12);
    tcnt = add2wline(tstr, tcnt, L"kokiai? ", 10);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Moteriskoji][Naudininkas].c_str(), 12);
    tcnt = add2wline(tstr, tcnt, L"kokiems? ", 10);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Vyriskoji][Naudininkas].c_str(), 12);
    tcnt = add2wline(tstr, tcnt, L"kokioms? ", 10);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Moteriskoji][Naudininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);


    tcnt = add2wline(tstr, 0, L"Galininkas: ", 16);
    tcnt = add2wline(tstr, tcnt, L"kokį? ", 10);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Vyriskoji][Galininkas].c_str(), 12);
    tcnt = add2wline(tstr, tcnt, L"kokią? ", 10);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Moteriskoji][Galininkas].c_str(), 12);
    tcnt = add2wline(tstr, tcnt, L"kokius? ", 10);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Vyriskoji][Galininkas].c_str(), 12);
    tcnt = add2wline(tstr, tcnt, L"kokias? ", 10);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Moteriskoji][Galininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);


    tcnt = add2wline(tstr, 0, L"Inagininkas: ", 16);
    tcnt = add2wline(tstr, tcnt, L"kokiu? ", 10);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Vyriskoji][Inagininkas].c_str(), 12);
    tcnt = add2wline(tstr, tcnt, L"kokia? ", 10);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Moteriskoji][Inagininkas].c_str(), 12);
    tcnt = add2wline(tstr, tcnt, L"kokiais? ", 10);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Vyriskoji][Inagininkas].c_str(), 12);
    tcnt = add2wline(tstr, tcnt, L"kokiomis? ", 10);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Moteriskoji][Inagininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);


    wstr = L"Vietininkas ";
    if (gimine_ == Vyriskoji) {
        wstr += L"(kokiame?): ";
    } else {
        wstr += L"(kokioje?): ";
    }
    tcnt = add2wline(tstr, 0, wstr.c_str(), 30);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Vyriskoji][Vietininkas].c_str(), 24);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Vyriskoji][Vietininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);
}


