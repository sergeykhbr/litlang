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
        Laikas - Время {"Laikas":"Butasis dazninis"}
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

*/

VeiksmazodisGeneric::VeiksmazodisGeneric(AttributeType *cfg)
    : WordGeneric(cfg)
{
    asmenuote_ = (*cfg)[L"Asmenuote"].to_int();
    lt_[Tiesiogine_Esamasis_laikas][jis_ji] = std::wstring((*cfg)[L"Jis"][0u].to_string());
    lt_[Tiesiogine_Butasis_laikas][jis_ji] = std::wstring((*cfg)[L"Jis"][1].to_string());

    AttributeType &ru = (*cfg)[L"Ru"];
    if (ru.is_string()) {
        ru_ = std::wstring(ru.to_string());
    }

    atnaujinti();
}

// Užpildykite deklinacijos lentelę - Заполнить таблицу склонений (склонятельную таблицу)
void VeiksmazodisGeneric::atnaujinti() {
    int vsz = static_cast<int>(value_.size());
    const wchar_t *wzodis = value_.c_str();
    std::wstring saknis1;   // infinitive
    std::wstring saknis2;   // jis, esamasis laikas
    std::wstring saknis3;   // jus, butasis laikas
    std::wstring galunes[Asmuo_total];
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
    wzodis = lt_[Tiesiogine_Esamasis_laikas][jis_ji].c_str();
    vsz = static_cast<int>(lt_[Tiesiogine_Esamasis_laikas][jis_ji].size());
    if (wzodis[vsz-2] == L'i' && wzodis[vsz-1] == L'a') {
        asmenuote_ = 1;
        saknis2 = lt_[Tiesiogine_Esamasis_laikas][jis_ji].substr(0, vsz - 2);
        galunes2 = L"ia";
        galunes[as] = L"iu";
        galunes[tu] = L"i";
        galunes[mes] = L"iame";
        galunes[jus] = L"iate";
        galunes[jie_jos] = L"ia";
    } else if (wzodis[vsz-1] == L'a') {
        asmenuote_ = 1;
        saknis2 = lt_[Tiesiogine_Esamasis_laikas][jis_ji].substr(0, vsz - 1);
        galunes2 = L"a";
        galunes[as] = L"u";
        galunes[tu] = L"i";
        galunes[mes] = L"ame";
        galunes[jus] = L"ate";
        galunes[jie_jos] = L"a";
    } else if (wzodis[vsz-1] == L'i') {
        asmenuote_ = 2;
        saknis2 = lt_[Tiesiogine_Esamasis_laikas][jis_ji].substr(0, vsz - 1);
        galunes2 = L"i";
        galunes[as] = L"iu";
        galunes[tu] = L"i";
        galunes[mes] = L"ime";
        galunes[jus] = L"ite";
        galunes[jie_jos] = L"i";
    } else if (wzodis[vsz-1] == L'o') {
        asmenuote_ = 3;
        saknis2 = lt_[Tiesiogine_Esamasis_laikas][jis_ji].substr(0, vsz - 1);
        galunes2 = L"o";
        galunes[as] = L"au";
        galunes[tu] = L"ai";
        galunes[mes] = L"ome";
        galunes[jus] = L"ote";
        galunes[jie_jos] = L"o";
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

    lt_[Tiesiogine_Esamasis_laikas][as] = saknis2 + _dz + galunes[as];
    lt_[Tiesiogine_Esamasis_laikas][tu] = saknis2 + galunes[tu];
    lt_[Tiesiogine_Esamasis_laikas][mes] = saknis2 + _dz + galunes[mes];
    lt_[Tiesiogine_Esamasis_laikas][jus] = saknis2 + _dz + galunes[jus];
    lt_[Tiesiogine_Esamasis_laikas][jie_jos] = saknis2 + _dz + galunes[jie_jos];


    // 3 asmuo, vietininkas, būtasis laikas
    wzodis = lt_[Tiesiogine_Butasis_laikas][jis_ji].c_str();
    vsz = static_cast<int>(lt_[Tiesiogine_Butasis_laikas][jis_ji].size());
    if (wzodis[vsz-1] == L'o') {
        saknis3 = lt_[Tiesiogine_Butasis_laikas][jis_ji].substr(0, vsz - 1);
        galunes3 = L"o";
        galunes[as] = L"au";
        galunes[tu] = L"ai";
        galunes[mes] = L"ome";
        galunes[jus] = L"ote";
        galunes[jie_jos] = L"o";
    } else if (wzodis[vsz-1] == L'ė') {
        saknis3 = lt_[Tiesiogine_Butasis_laikas][jis_ji].substr(0, vsz - 1);
        galunes3 = L"ė";
        galunes[as] = L"au";
        galunes[tu] = L"ei";
        galunes[mes] = L"ėme";
        galunes[jus] = L"ėte";
        galunes[jie_jos] = L"ė";
    } else {
        // unsupported (private) ending = nepalaikomas galūnės
        wprintf(L"warning: nepalaikomas asmenų galūnės asmenuote: %s\n", wzodis);
    }

    vsz = static_cast<int>(saknis3.size());
    if (saknis3.c_str()[vsz - 1] == L'd') {
        lt_[Tiesiogine_Butasis_laikas][as] = saknis3.substr(0, vsz - 1) + L"ži" + galunes[as];
    } else if (saknis3.c_str()[vsz - 1] == L't') {
        lt_[Tiesiogine_Butasis_laikas][as] = saknis3.substr(0, vsz - 1) + L"či" + galunes[as];
    } else {
        lt_[Tiesiogine_Butasis_laikas][as] = saknis3 + galunes[as];
    }
    lt_[Tiesiogine_Butasis_laikas][tu] = saknis3 + galunes[tu];
    lt_[Tiesiogine_Butasis_laikas][mes] = saknis3 + galunes[mes];
    lt_[Tiesiogine_Butasis_laikas][jus] = saknis3 + galunes[jus];
    lt_[Tiesiogine_Butasis_laikas][jie_jos] = saknis3 + galunes[jie_jos];

    // Būtasis dažninis laikas
    lt_[Tiesiogine_ButasisDazninis_laikas][as] = saknis1 + L"davau";
    lt_[Tiesiogine_ButasisDazninis_laikas][tu] = saknis1 + L"davai";
    lt_[Tiesiogine_ButasisDazninis_laikas][jis_ji] = saknis1 + L"davo";
    lt_[Tiesiogine_ButasisDazninis_laikas][mes] = saknis1 + L"davome";
    lt_[Tiesiogine_ButasisDazninis_laikas][jus] = saknis1 + L"davote";
    lt_[Tiesiogine_ButasisDazninis_laikas][jie_jos] = saknis1 + L"davo";

    // Būsimasis laikas
    std::wstring saknis4 = saknis1;
    wzodis = saknis4.c_str();
    vsz = static_cast<int>(saknis4.size());
    std::wstring _s = L"s";
    if (wzodis[vsz - 1] == L's' || wzodis[vsz - 1] == L'z') {
        saknis4 = saknis4.substr(0, vsz - 1);
    } else if (wzodis[vsz - 1] == L'š' || wzodis[vsz - 1] == L'ž') {
        saknis4 = saknis4.substr(0, vsz - 1);
        _s = L"š";
    }
    lt_[Tiesiogine_Busimasis_laikas][as] = saknis4 + _s + L"iu";
    lt_[Tiesiogine_Busimasis_laikas][tu] = saknis4 + _s + L"i";
    lt_[Tiesiogine_Busimasis_laikas][jis_ji] = saknis4 + _s;
    lt_[Tiesiogine_Busimasis_laikas][mes] = saknis4 + _s + L"ime";
    lt_[Tiesiogine_Busimasis_laikas][jus] = saknis4 + _s + L"ite";
    lt_[Tiesiogine_Busimasis_laikas][jie_jos] = saknis4 + _s;
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
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_Esamasis_laikas][as].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"mes: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_Esamasis_laikas][mes].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    tu: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_Esamasis_laikas][tu].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"jūs: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_Esamasis_laikas][jus].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    jis,ji: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_Esamasis_laikas][jis_ji].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"jie,jos: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_Esamasis_laikas][jie_jos].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    printf_log(L"  Tiesioginė nuosaka, Būtasis laikas (%s):\n",
               L"Изявительное наклонение, прошедшее время");

    tcnt = add2wline(tstr, 0, L"    aš: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_Butasis_laikas][as].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"mes: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_Butasis_laikas][mes].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    tu: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_Butasis_laikas][tu].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"jūs: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_Butasis_laikas][jus].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    jis,ji: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_Butasis_laikas][jis_ji].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"jie,jos: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_Butasis_laikas][jie_jos].c_str(), wsz);
    printf_log(L"%s\n", tstr);


    printf_log(L"  Tiesioginė nuosaka, Būtasis dažninis laikas (%s):\n",
               L"Изявительное наклонение, прошедшее повторяющееся время");

    tcnt = add2wline(tstr, 0, L"    aš: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_ButasisDazninis_laikas][as].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"mes: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_ButasisDazninis_laikas][mes].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    tu: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_ButasisDazninis_laikas][tu].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"jūs: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_ButasisDazninis_laikas][jus].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    jis,ji: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_ButasisDazninis_laikas][jis_ji].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"jie,jos: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_ButasisDazninis_laikas][jie_jos].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    printf_log(L"  Tiesioginė nuosaka, Būsimasis laikas (%s):\n",
               L"Изявительное наклонение, будущее время");

    tcnt = add2wline(tstr, 0, L"    aš: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_Busimasis_laikas][as].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"mes: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_Busimasis_laikas][mes].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    tu: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_Busimasis_laikas][tu].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"jūs: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_Busimasis_laikas][jus].c_str(), wsz);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    jis,ji: ", 14);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_Busimasis_laikas][jis_ji].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, L"jie,jos: ", 10);
    tcnt = add2wline(tstr, tcnt, lt_[Tiesiogine_Busimasis_laikas][jie_jos].c_str(), wsz);
    printf_log(L"%s\n", tstr);
}

std::wstring VeiksmazodisGeneric::gautiForma(AttributeType &arg) {
    std::wstring ret = L"";
    //EAtvejis atvejis = str2atvejis(arg[L"Atvejis"].to_string());
    //EGimine gimine = str2gimine(arg[L"Gimine"].to_string());
    //ESkaicus skaicus = str2skaicus(arg[L"Skaicius"].to_string());
    return ret;
}
