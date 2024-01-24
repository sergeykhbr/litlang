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

#include "verb_veiksmazodis.h"
#include <utils.h>

/** 
    Veiksmažodis - глагол, {"Type":"veiksmazodis"}
        Giminė - Род
            Vyriškoji - мужской
            Moteriškoji - женский
        nUosaka - Наклонение {"Nuosaka":"Tiesiogine"}
            TiesiOginė - изъявительное (indicative)
            TariamOji - сослагательное (subjunctive)
            Liepiamoji - повелительное (imperative)
            GiedžiamOji - побудительное
        Asmenuotė - Спряжение, парадигма {"Asmenuote":1}
            Pirmoji - I (dirbti)
            Antroji - II (mylėti)
            Trečioji - III (skaityti)
        Laikas - Время {"Laikas":"ButasisDazninis"}
            Esamasis laikas - настоящее
            Būtasis laikas - прошедшее
            Būtasis dažninis laikas - прошедшее многократное
            Būsimasis laikas - будующее
        Asmuo - Лицо {"Asmo":"jis"}
            1 ед.ч aš
            2 ед.ч tu
            3 ед.ч jis, ji
            1 мн.ч mes
            2 мн.ч jūs
            3 мн.ч jie, jos
        Sangrazinis - Возвратная форма -ться 
            -tis инфинитив

*/

VeiksmazodisGeneric::VeiksmazodisGeneric(AttributeType *cfg)
    : WordGeneric(cfg)
{
    lt_[Esamasis][jis] = std::wstring((*cfg)[L"Jis"][0u].to_string());
    lt_[Esamasis][ji] = std::wstring((*cfg)[L"Jis"][0u].to_string());
    lt_[Butasis][jis] = std::wstring((*cfg)[L"Jis"][1].to_string());
    lt_[Butasis][ji] = std::wstring((*cfg)[L"Jis"][1].to_string());

    if ((*cfg).has_key(L"Lt")) {
        AttributeType &lt = (*cfg)[L"Lt"];
        if (lt.is_dict()) {
            if (lt.has_key(L"Esamasis")) {
                AttributeType &esa = lt[L"Esamasis"];
                lt_[Esamasis][as] = std::wstring(esa[0u].to_string());
                lt_[Esamasis][tu] = std::wstring(esa[1].to_string());
                lt_[Esamasis][mes] = std::wstring(esa[3].to_string());
                lt_[Esamasis][jus] = std::wstring(esa[4].to_string());
                lt_[Esamasis][jie] = std::wstring(esa[5].to_string());
                lt_[Esamasis][jos] = std::wstring(esa[5].to_string());
            }
        }
    }

    AttributeType &ru = (*cfg)[L"Ru"];
    if (ru.is_string()) {
        ru_ = std::wstring(ru.to_string());
    }

    atnaujinti();
}

std::wstring VeiksmazodisGeneric::imkSaknis(int idx) {
    std::wstring ret = L"";
    if (idx == 1) {
        // -ti
        ret = value_.substr(0, value_.size() - 2);
    } else if (idx == 2) {
        // -(i)a; -i; -o
        const wchar_t *wzodis = lt_[Esamasis][jis].c_str();
        int vsz = static_cast<int>(lt_[Esamasis][jis].size());
        if (wzodis[vsz-2] == L'i' && wzodis[vsz-1] == L'a') {
            ret = lt_[Esamasis][jis].substr(0, vsz - 2);
        } else {
            ret = lt_[Esamasis][jis].substr(0, vsz - 1);
        }
    } else if (idx == 3) {
        // -o; -ė
        int vsz = static_cast<int>(lt_[Butasis][jis].size());
        ret = lt_[Butasis][jis].substr(0, vsz - 1);
    }
    return ret;
}

std::wstring VeiksmazodisGeneric::imkZodis(int idx) {
    if (idx == 2) {
        return lt_[Esamasis][jis];
    } else if (idx == 3) {
        return lt_[Butasis][jis];
    }
    return value_;
}

// Užpildykite deklinacijos lentelę - Заполнить таблицу склонений (склонятельную таблицу)
void VeiksmazodisGeneric::atnaujinti() {
    int vsz = static_cast<int>(value_.size());
    const wchar_t *wzodis = value_.c_str();
    std::wstring saknis1;   // infinitive
    std::wstring saknis2;   // jis, esamasis laikas
    std::wstring saknis3;   // jus, butasis laikas
    std::wstring galunes[Asmuo_total];
    std::wstring galunes_si[Asmuo_total]; // возвратная форма
    std::wstring galunes1;  // -ti
    std::wstring galunes2;  // -(i)a; -i; -o
    std::wstring galunes3;  // -o; -ė
    std::wstring _dz = L"";   // d->dž; t->č

    std::wstring _ia = L"";
    std::wstring _ai_ei = L"";
    std::wstring _o_e = L"";

    // Infinitive:
    if (wzodis[vsz-2] == L't' && wzodis[vsz-1] == L'i') {
        saknis1 = value_.substr(0, vsz - 2);
        galunes1 = L"ti";
    } else {
        // unsupported format
        wprintf(L"warning: nepalaikomas veikmažodiai formatas %s\n", L"-ti");
    }

    // 3 asmuo, vietininkas, esamasis laikas
    wzodis = lt_[Esamasis][jis].c_str();
    vsz = static_cast<int>(lt_[Esamasis][jis].size());
    saknis2 = imkSaknis(2);
    if (wzodis[vsz-2] == L'i' && wzodis[vsz-1] == L'a') {
        asmenuote_ = 1;
        galunes2 = L"ia";
        galunes[as] = L"iu";
        galunes[tu] = L"i";
        galunes[mes] = L"iame";
        galunes[jus] = L"iate";
        galunes[jie] = L"ia";
        galunes[jos] = L"ia";

        galunes_si[as] = L"iuosi";
        galunes_si[tu] = L"iesi";
        galunes_si[jis] = L"iasi";
        galunes_si[ji] = L"iasi";
        galunes_si[mes] = L"iamės";
        galunes_si[jus] = L"iatės";
        galunes_si[jie] = L"iasi";
        galunes_si[jos] = L"iasi";
    } else if (wzodis[vsz-1] == L'a') {
        asmenuote_ = 1;
        galunes2 = L"a";
        galunes[as] = L"u";
        galunes[tu] = L"i";
        galunes[mes] = L"ame";
        galunes[jus] = L"ate";
        galunes[jie] = L"a";
        galunes[jos] = L"a";

        galunes_si[as] = L"uosi";
        galunes_si[tu] = L"iesi";
        galunes_si[jis] = L"asi";
        galunes_si[ji] = L"asi";
        galunes_si[mes] = L"amės";
        galunes_si[jus] = L"atės";
        galunes_si[jie] = L"asi";
        galunes_si[jos] = L"asi";
    } else if (wzodis[vsz-1] == L'i') {
        asmenuote_ = 2;
        galunes2 = L"i";
        galunes[as] = L"iu";
        galunes[tu] = L"i";
        galunes[mes] = L"ime";
        galunes[jus] = L"ite";
        galunes[jie] = L"i";
        galunes[jos] = L"i";

        galunes_si[as] = L"iuosi";
        galunes_si[tu] = L"iesi";
        galunes_si[jis] = L"isi";
        galunes_si[ji] = L"isi";
        galunes_si[mes] = L"imės";
        galunes_si[jus] = L"itės";
        galunes_si[jie] = L"isi";
        galunes_si[jos] = L"isi";
    } else if (wzodis[vsz-1] == L'o') {
        asmenuote_ = 3;
        galunes2 = L"o";
        galunes[as] = L"au";
        galunes[tu] = L"ai";
        galunes[mes] = L"ome";
        galunes[jus] = L"ote";
        galunes[jie] = L"o";
        galunes[jos] = L"o";

        galunes_si[as] = L"ausi";
        galunes_si[tu] = L"aisi";
        galunes_si[jis] = L"osi";
        galunes_si[ji] = L"osi";
        galunes_si[mes] = L"omės";
        galunes_si[jus] = L"otės";
        galunes_si[jie] = L"osi";
        galunes_si[jos] = L"osi";
    } else {
        // unsupported (private) ending = nepalaikomas galūnės
        wprintf(L"warning: nepalaikomas asmenų galūnės asmenuote: %s\n", wzodis);
    }

    // patikrinti d->dž (patikrinti = check)
    vsz = static_cast<int>(saknis2.size());
    if (saknis2.c_str()[vsz - 2] == L'd' && saknis2.c_str()[vsz - 1] == L'ž') {
        saknis2 = saknis2.substr(0, vsz - 1);
        _dz = L"ž";
    }

    if (lt_[Esamasis][as].size() == 0) {
        // was not defined as a list
        lt_[Esamasis][as] = saknis2 + _dz + galunes[as];
        lt_[Esamasis][tu] = saknis2 + galunes[tu];
        lt_[Esamasis][mes] = saknis2 + _dz + galunes[mes];
        lt_[Esamasis][jus] = saknis2 + _dz + galunes[jus];
        lt_[Esamasis][jie] = saknis2 + _dz + galunes[jie];
        lt_[Esamasis][jos] = saknis2 + _dz + galunes[jos];

        lt_si_[Esamasis][as] = saknis2 + _dz + galunes_si[as];
        lt_si_[Esamasis][tu] = saknis2 + galunes_si[tu];
        lt_si_[Esamasis][jis] = saknis2 + galunes_si[jis];
        lt_si_[Esamasis][ji] = saknis2 + galunes_si[ji];
        lt_si_[Esamasis][mes] = saknis2 + _dz + galunes_si[mes];
        lt_si_[Esamasis][jus] = saknis2 + _dz + galunes_si[jus];
        lt_si_[Esamasis][jie] = saknis2 + _dz + galunes_si[jie];
        lt_si_[Esamasis][jos] = saknis2 + _dz + galunes_si[jos];
    }


    // 3 asmuo, vietininkas, būtasis laikas
    wzodis = lt_[Butasis][jis].c_str();
    vsz = static_cast<int>(lt_[Butasis][jis].size());
    saknis3 = imkSaknis(3);
    std::wstring _i = L"";
    if (value_[value_.size() - 3] == L'y' || value_[value_.size() - 3] == L'i') {
        // -iti or -yti
        _i = L"i";
    }
    if (wzodis[vsz-1] == L'o') {
        galunes3 = L"o";
        galunes[as] = _i + L"au";
        galunes[tu] = L"ai";
        galunes[mes] = L"ome";
        galunes[jus] = L"ote";
        galunes[jie] = L"o";
        galunes[jos] = L"o";
    } else if (wzodis[vsz-1] == L'ė') {
        galunes3 = L"ė";
        galunes[as] = _i + L"au";
        galunes[tu] = L"ei";
        galunes[mes] = L"ėme";
        galunes[jus] = L"ėte";
        galunes[jie] = L"ė";
        galunes[jos] = L"ė";
    } else {
        // unsupported (private) ending = nepalaikomas galūnės
        wprintf(L"warning: nepalaikomas asmenų galūnės asmenuote: %s\n", wzodis);
    }

    vsz = static_cast<int>(saknis3.size());
    if (saknis3.c_str()[vsz - 1] == L'd') {
        lt_[Butasis][as] = saknis3.substr(0, vsz - 1) + L"ži" + galunes[as];
    } else if (saknis3.c_str()[vsz - 1] == L't') {
        lt_[Butasis][as] = saknis3.substr(0, vsz - 1) + L"či" + galunes[as];
    } else {
        lt_[Butasis][as] = saknis3 + galunes[as];
    }
    lt_[Butasis][tu] = saknis3 + galunes[tu];
    lt_[Butasis][mes] = saknis3 + galunes[mes];
    lt_[Butasis][jus] = saknis3 + galunes[jus];
    lt_[Butasis][jie] = saknis3 + galunes[jie];
    lt_[Butasis][jos] = saknis3 + galunes[jos];

    // Būtasis dažninis laikas
    lt_[ButasisDazninis][as] = saknis1 + L"davau";
    lt_[ButasisDazninis][tu] = saknis1 + L"davai";
    lt_[ButasisDazninis][jis] = saknis1 + L"davo";
    lt_[ButasisDazninis][ji] = saknis1 + L"davo";
    lt_[ButasisDazninis][mes] = saknis1 + L"davome";
    lt_[ButasisDazninis][jus] = saknis1 + L"davote";
    lt_[ButasisDazninis][jie] = saknis1 + L"davo";
    lt_[ButasisDazninis][jos] = saknis1 + L"davo";

    // Būsimasis laikas
    std::wstring saknis4 = imkSaknis(1);
    wzodis = saknis4.c_str();
    vsz = static_cast<int>(saknis4.size());
    std::wstring _s = L"s";
    if (wzodis[vsz - 1] == L's' || wzodis[vsz - 1] == L'z') {
        saknis4 = saknis4.substr(0, vsz - 1);
    } else if (wzodis[vsz - 1] == L'š' || wzodis[vsz - 1] == L'ž') {
        saknis4 = saknis4.substr(0, vsz - 1);
        _s = L"š";
    }
    lt_[Busimasis][as] = saknis4 + _s + L"iu";
    lt_[Busimasis][tu] = saknis4 + _s + L"i";
    lt_[Busimasis][jis] = saknis4 + _s;
    lt_[Busimasis][ji] = saknis4 + _s;
    lt_[Busimasis][mes] = saknis4 + _s + L"ime";
    lt_[Busimasis][jus] = saknis4 + _s + L"ite";
    lt_[Busimasis][jie] = saknis4 + _s;
    lt_[Busimasis][jos] = saknis4 + _s;


    // Liepiamoji nuosaka, Повелительное наклонение
    std::wstring saknis5 = saknis1;
    wzodis = saknis5.c_str();
    vsz = static_cast<int>(saknis5.size());
    std::wstring _k = L"k";     // TODO: -ki
    if (wzodis[vsz - 1] == L'g') {
        saknis5 = saknis5.substr(0, vsz - 1) + L"k";
    }
    lt_liepiamoji_[as] = L"";
    lt_liepiamoji_[tu] = saknis5 + _k;
    lt_liepiamoji_[jis] = L"te" + lt_[Esamasis][jis];
    lt_liepiamoji_[ji] = L"te" + lt_[Esamasis][ji];
    lt_liepiamoji_[mes] = saknis5 + _k + L"ime";
    lt_liepiamoji_[jus] = saknis5 + _k + L"ite";
    lt_liepiamoji_[jie] = L"te" + lt_[Esamasis][jie];
    lt_liepiamoji_[jos] = L"te" + lt_[Esamasis][jos];

    // Tariamoji nuosaka, Условное наклонение
    lt_tariamoji_[as] = saknis1 + L"čiau";
    lt_tariamoji_[tu] = saknis1 + L"tum";
    lt_tariamoji_[jis] = saknis1 + L"tų";
    lt_tariamoji_[ji] = saknis1 + L"tų";
    lt_tariamoji_[mes] = saknis1 + L"tume";
    lt_tariamoji_[jus] = saknis1 + L"tute";   // tu(mė)te
    lt_tariamoji_[jie] = saknis1 + L"tų";
    lt_tariamoji_[jos] = saknis1 + L"tų";
}

void VeiksmazodisGeneric::info() {
    wchar_t tstr[1024];
    int wsz = static_cast<int>(value_.size()) + 8; // ending + spaces (pabaiga + įtrauka)
    int tcnt = 0;
    printf_log(L"\nVeiksmažodis: %s, asmenuotė: %d, (%s)\n",
            value_.c_str(),
            asmenuote_,
            ru_.c_str());

    printf_log(L"  Tiesioginė nuosaka, Esamasis laikas (%s):\n",
               L"Изявительное наклонение, настоящее время");

    tcnt = add2wline(tstr, 0, L"    aš: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[Esamasis][as].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"mes: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Esamasis][mes].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    tu: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[Esamasis][tu].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"jūs: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Esamasis][jus].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    jis,ji: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[Esamasis][jis].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"jie,jos: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Esamasis][jie].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    printf_log(L"  Tiesioginė nuosaka, Būtasis laikas (%s):\n",
               L"Изявительное наклонение, прошедшее время");

    tcnt = add2wline(tstr, 0, L"    aš: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[Butasis][as].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"mes: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Butasis][mes].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    tu: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[Butasis][tu].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"jūs: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Butasis][jus].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    jis,ji: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[Butasis][jis].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"jie,jos: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Butasis][jie].c_str(), wsz);
    printf_log(L"%s\n", tstr);


    printf_log(L"  Tiesioginė nuosaka, Būtasis dažninis laikas (%s):\n",
               L"Изявительное наклонение, прошедшее повторяющееся время");

    tcnt = add2wline(tstr, 0, L"    aš: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[ButasisDazninis][as].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"mes: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[ButasisDazninis][mes].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    tu: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[ButasisDazninis][tu].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"jūs: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[ButasisDazninis][jus].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    jis,ji: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[ButasisDazninis][jis].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"jie,jos: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[ButasisDazninis][jie].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    printf_log(L"  Tiesioginė nuosaka, Būsimasis laikas (%s):\n",
               L"Изявительное наклонение, будущее время");

    tcnt = add2wline(tstr, 0, L"    aš: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[Busimasis][as].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"mes: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Busimasis][mes].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    tu: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[Busimasis][tu].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"jūs: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Busimasis][jus].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    jis,ji: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[Busimasis][jis].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"jie,jos: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Busimasis][jie].c_str(), wsz);
    printf_log(L"%s\n", tstr);


    printf_log(L"  Liepiamoji nuosaka (%s):\n",
               L"Повелительное наклонение");

    tcnt = add2wline(tstr, 0, L"    aš: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_liepiamoji_[as].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"mes: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_liepiamoji_[mes].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    tu: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_liepiamoji_[tu].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"jūs: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_liepiamoji_[jus].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    jis,ji: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_liepiamoji_[jis].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"jie,jos: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_liepiamoji_[jie].c_str(), wsz);
    printf_log(L"%s\n", tstr);


    printf_log(L"  Tariamoji nuosaka (%s):\n",
               L"Условное наклонение");

    tcnt = add2wline(tstr, 0, L"    aš: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_tariamoji_[as].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"mes: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_tariamoji_[mes].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    tu: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_tariamoji_[tu].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"jūs: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_tariamoji_[jus].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    jis,ji: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_tariamoji_[jis].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"jie,jos: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_tariamoji_[jie].c_str(), wsz);
    printf_log(L"%s\n", tstr);
}

std::wstring VeiksmazodisGeneric::gautiForma(AttributeType &arg) {
    std::wstring ret = L"";
    if (arg.has_key(L"Asmuo") == 0) {
        // infinitive
        ret = value_;
        if (arg.has_key(L"Sangrazinis")) {
            ret += L"s";
        }
    } else {
        EAsmuo asmuo = str2asmuo(arg[L"Asmuo"].to_string());
        if (!arg.has_key(L"Nuosaka") || arg.is_equal(L"Tiesiogine")) {
            ELaikas laikas = str2laikas(arg[L"Laikas"].to_string());
            if (arg.has_key(L"Sangrazinis")) {
                ret = lt_si_[laikas][asmuo];
            } else {
                ret = lt_[laikas][asmuo];
            }
        } else if (arg[L"Nuosaka"].is_equal(L"Liepiamoji")) {
            ret = lt_liepiamoji_[asmuo];
        } else if (arg[L"Nuosaka"].is_equal(L"Tariamoji")) {
            ret = lt_tariamoji_[asmuo];
        }
    }

    return ret;
}
