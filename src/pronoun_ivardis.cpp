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

#include "pronoun_ivardis.h"
#include <utils.h>

/** 
    Įvardis - местоимение, {"Type":"ivardis"}
        Skaičus - число
            Vienaskaita
            Daugiskaita
        Linksniuote - склонение по типу и роду
            Vyriškoji:
                1. tas, šitas, anas, kas..
                2. kitas, visas, tam tikras..
                3. toks, joks, visoks, kitoks
                4. jis, šis
                5. pats
            Moteriškoji
                6. ta, šita, kita, visa
                7. tokia, viskokia
                8. ji, ši, pati
        Atvejis - падеж
            Vardininkas .. Vietininkas
        - разряд
            asmeninis įvardis - личное местоимение, я, ты, он
                              - притяжательные
            sangražinis įvardis - возвратное местоимение
                              - неопределенные: нЕкто, нЕкоторый, нЕсколько
            parodomasis įvardis - указательное местоимение: столько, этот, эта, это, эти, тот, та, то, те, такие, таковы, сии
                              - вопросительные
                              - относительные: кто, что, какой, каков, чей, который
                              - взаимные: друг другу, каждый за
                              - определенные: все, весь, всяк, любой, сам, самый, другой, иной
                              - отрицательные: никто, некого. нисколько, никакой, ничей
            

*/

IvardisGeneric::IvardisGeneric(AttributeType *cfg)
    : WordGeneric(cfg),
    gimine_(Gimine_nezinoma),
    linksniuote_(Paradigma_nezinoma)
{
    if ((*cfg).has_key(L"Linksniuote")) {
        nustatyti_paradigma((*cfg)[L"Linksniuote"].to_string());
    } else if ((*cfg).has_key(L"Lt")) {
        AttributeType &lt = (*cfg)[L"Lt"];
        AttributeType &viena = lt[0u];
        AttributeType &daug = lt[1];
        for (unsigned i = 0; i < viena.size(); i++) {
            lt_[Vienaskaita][i] = std::wstring(viena[i].to_string());
            lt_[Daugiskaita][i] = std::wstring(daug[i].to_string());
        }
    } else {
        // Tik viena forma galima "Только одна форма возможна"
        lt_[Vienaskaita][Vardininkas] = value_;
    }

    AttributeType &ru = (*cfg)[L"Ru"];
    if (ru.is_string()) {
        ru_[Vienaskaita][Vardininkas] = std::wstring(ru.to_string());
    } else if (ru.is_list()) {
        AttributeType &viena = (*cfg)[L"Ru"][0u];
        AttributeType &daug = (*cfg)[L"Ru"][1];
        for (unsigned i = 0; i < viena.size(); i++) {
            ru_[Vienaskaita][i] = std::wstring(viena[i].to_string());
            ru_[Daugiskaita][i] = std::wstring(daug[i].to_string());
        }
    }
}

// определить парадигму
void IvardisGeneric::nustatyti_paradigma(const wchar_t *linksniuote) {
    std::wstring saknis;
    std::wstring saknis_c;
    if (wcscmp(linksniuote, L"tas") == 0) {
        gimine_ = Vyriskoji;
        linksniuote_ = Paradigma_V_tas;
        saknis = value_.substr(0, value_.size() - 2);
        lt_[Vienaskaita][Vardininkas] = saknis + L"as";     // kas? tas = тот
        lt_[Vienaskaita][Kilmininkas] = saknis + L"o";      // ko? to = того
        lt_[Vienaskaita][Naudininkas] = saknis + L"am";     // kam? tam = тому
        lt_[Vienaskaita][Galininkas] = saknis + L"ą";       // ką? tą = того
        lt_[Vienaskaita][Inagininkas] = saknis + L"uo";     // kuo? tuo = тем
        lt_[Vienaskaita][Vietininkas] = saknis + L"ame";    // kur? kame? tame = в том

        lt_[Daugiskaita][Vardininkas] = saknis + L"ie";     // kas? tie = те
        lt_[Daugiskaita][Kilmininkas] = saknis + L"ų";      // ko? tų = тех
        lt_[Daugiskaita][Naudininkas] = saknis + L"iems";   // kam? tiems = тем
        lt_[Daugiskaita][Galininkas] = saknis + L"uos";     // ką? tuos = тех
        lt_[Daugiskaita][Inagininkas] = saknis + L"ais";    // kuo? tais = теми
        lt_[Daugiskaita][Vietininkas] = saknis + L"uose";   // kur? kame? tuose = в тех
    } else if (wcscmp(linksniuote, L"kitas") == 0) {
        gimine_ = Vyriskoji;
        linksniuote_ = Paradigma_V_kitas;
        saknis = value_.substr(0, value_.size() - 2);
        lt_[Vienaskaita][Vardininkas] = saknis + L"as";     // kas? kitas = другой
        lt_[Vienaskaita][Kilmininkas] = saknis + L"o";      // ko? kito = другого
        lt_[Vienaskaita][Naudininkas] = saknis + L"am";     // kam? kitam = другому
        lt_[Vienaskaita][Galininkas] = saknis + L"ą";       // ką? kitą = другого
        lt_[Vienaskaita][Inagininkas] = saknis + L"u";      // kuo? kitu = другим
        lt_[Vienaskaita][Vietininkas] = saknis + L"ame";    // kur? kitame = в другом

        lt_[Daugiskaita][Vardininkas] = saknis + L"i";      // kas? kiti = другие
        lt_[Daugiskaita][Kilmininkas] = saknis + L"ų";      // ko? kitų = других
        lt_[Daugiskaita][Naudininkas] = saknis + L"iems";   // kam? kitiems = другим
        lt_[Daugiskaita][Galininkas] = saknis + L"us";      // ką? kitus = других
        lt_[Daugiskaita][Inagininkas] = saknis + L"ais";    // kuo? kitais = другими
        lt_[Daugiskaita][Vietininkas] = saknis + L"uose";   // kur? kame? kitouse = в других
    } else if (wcscmp(linksniuote, L"toks") == 0) {
        gimine_ = Vyriskoji;
        linksniuote_ = Paradigma_V_toks;
        saknis = value_.substr(0, value_.size() - 1);
        lt_[Vienaskaita][Vardininkas] = saknis + L"s";      // kas? toks = такой
        lt_[Vienaskaita][Kilmininkas] = saknis + L"io";     // ko? tokio = такого
        lt_[Vienaskaita][Naudininkas] = saknis + L"iam";    // kam? tokiam = такому
        lt_[Vienaskaita][Galininkas] = saknis + L"į";       // ką? tokį = такого
        lt_[Vienaskaita][Inagininkas] = saknis + L"iu";     // kuo? tokiu = таким
        lt_[Vienaskaita][Vietininkas] = saknis + L"iame";   // kur? tokiame = в таком

        lt_[Daugiskaita][Vardininkas] = saknis + L"ie";     // kas? tokie = такие
        lt_[Daugiskaita][Kilmininkas] = saknis + L"ių";     // ko? tokių = таких
        lt_[Daugiskaita][Naudininkas] = saknis + L"iems";   // kam? tokiems = таким
        lt_[Daugiskaita][Galininkas] = saknis + L"ius";     // ką? tokius = таких
        lt_[Daugiskaita][Inagininkas] = saknis + L"iais";   // kuo? tokiais = такими
        lt_[Daugiskaita][Vietininkas] = saknis + L"iuose";  // kur? kame? tokiuose = в таких
    } else if (wcscmp(linksniuote, L"jis") == 0) {
        gimine_ = Vyriskoji;
        linksniuote_ = Paradigma_V_jis;
        saknis = value_.substr(0, value_.size() - 2);
        lt_[Vienaskaita][Vardininkas] = saknis + L"is";     // kas? kuris = который
        lt_[Vienaskaita][Kilmininkas] = saknis + L"io";     // ko? kurio = которого
        lt_[Vienaskaita][Naudininkas] = saknis + L"iam";    // kam? kuriam = которму
        lt_[Vienaskaita][Galininkas] = saknis + L"į";       // ką? kurį = которого
        lt_[Vienaskaita][Inagininkas] = saknis + L"iuo";    // kuo? kuriuo = которым
        lt_[Vienaskaita][Vietininkas] = saknis + L"iame";   // kur? kuriame = в котором

        lt_[Daugiskaita][Vardininkas] = saknis + L"ie";     // kas? kurie = которые
        lt_[Daugiskaita][Kilmininkas] = saknis + L"ių";     // ko? kurių = которых
        lt_[Daugiskaita][Naudininkas] = saknis + L"iems";   // kam? kuriems = которым
        lt_[Daugiskaita][Galininkas] = saknis + L"iuos";    // ką? kuriuos = которых
        lt_[Daugiskaita][Inagininkas] = saknis + L"iais";   // kuo? kuriais = которыми
        lt_[Daugiskaita][Vietininkas] = saknis + L"iuose";  // kur? kame? kuriuose = в которых
    } else if (wcscmp(linksniuote, L"pats") == 0) {
        gimine_ = Vyriskoji;
        linksniuote_ = Paradigma_V_kitas;
        saknis = value_.substr(0, value_.size() - 1);  // t -> č
        saknis_c = saknis;
        if (saknis[saknis.size() - 1] == L't') {
            saknis_c = value_.substr(0, value_.size() - 2) + L"č";  // t -> č
        }
        lt_[Vienaskaita][Vardininkas] = saknis + L"s";      // kas? pats
        lt_[Vienaskaita][Kilmininkas] = saknis + L"ies";    // ko? paties
        lt_[Vienaskaita][Naudininkas] = saknis_c + L"iam";  // kam? pačiam
        lt_[Vienaskaita][Galininkas] = saknis + L"į";       // ką? patį
        lt_[Vienaskaita][Inagininkas] = saknis_c + L"iu";   // kuo? pačiu
        lt_[Vienaskaita][Vietininkas] = saknis_c + L"iame"; // kur? pačiame

        lt_[Daugiskaita][Vardininkas] = saknis + L"ys";     // kas? patys
        lt_[Daugiskaita][Kilmininkas] = saknis_c + L"ių";   // ko? pačių
        lt_[Daugiskaita][Naudininkas] = saknis + L"iems";   // kam? patiems
        lt_[Daugiskaita][Galininkas] = saknis_c + L"ius";   // ką? pačius
        lt_[Daugiskaita][Inagininkas] = saknis_c + L"iais"; // kuo? pačiais
        lt_[Daugiskaita][Vietininkas] = saknis_c + L"iuose";// kur? kame? pačiuose
    } else if (wcscmp(linksniuote, L"ta") == 0) {
        gimine_ = Moteriskoji;
        linksniuote_ = Paradigma_M_ta;
        saknis = value_.substr(0, value_.size() - 1);
        lt_[Vienaskaita][Vardininkas] = saknis + L"a";      // kas? ta = та
        lt_[Vienaskaita][Kilmininkas] = saknis + L"os";     // ko? tos = ту
        lt_[Vienaskaita][Naudininkas] = saknis + L"ai";     // kam? tai = той
        lt_[Vienaskaita][Galininkas] = saknis + L"ą";       // ką? tą = ту
        lt_[Vienaskaita][Inagininkas] = saknis + L"a";      // kuo? ta = той
        lt_[Vienaskaita][Vietininkas] = saknis + L"oje";    // kur? toje = в той

        lt_[Daugiskaita][Vardininkas] = saknis + L"os";     // kas? tos = те
        lt_[Daugiskaita][Kilmininkas] = saknis + L"ų";      // ko? tų = тех
        lt_[Daugiskaita][Naudininkas] = saknis + L"oms";    // kam? toms = тем
        lt_[Daugiskaita][Galininkas] = saknis + L"as";      // ką? tas = тех
        lt_[Daugiskaita][Inagininkas] = saknis + L"omis";   // kuo? tomis = теми
        lt_[Daugiskaita][Vietininkas] = saknis + L"ose";    // kur? kame? tose = в тех
    } else if (wcscmp(linksniuote, L"tokia") == 0) {
        gimine_ = Moteriskoji;
        linksniuote_ = Paradigma_M_tokia;
        saknis = value_.substr(0, value_.size() - 2);
        lt_[Vienaskaita][Vardininkas] = saknis + L"ia";     // kas? kokia = какая
        lt_[Vienaskaita][Kilmininkas] = saknis + L"ios";    // ko? kokios = какую
        lt_[Vienaskaita][Naudininkas] = saknis + L"iai";    // kam? kokiai = какой
        lt_[Vienaskaita][Galininkas] = saknis + L"ią";      // ką? kokią = какую
        lt_[Vienaskaita][Inagininkas] = saknis + L"ia";     // kuo? kokia = какой
        lt_[Vienaskaita][Vietininkas] = saknis + L"ioje";   // kur? kokioje = в какой

        lt_[Daugiskaita][Vardininkas] = saknis + L"ios";    // kas? kokios = какие
        lt_[Daugiskaita][Kilmininkas] = saknis + L"ių";     // ko? kokių = каких
        lt_[Daugiskaita][Naudininkas] = saknis + L"ioms";   // kam? kokioms = каким
        lt_[Daugiskaita][Galininkas] = saknis + L"ias";     // ką? kokias = каких
        lt_[Daugiskaita][Inagininkas] = saknis + L"iomis";  // kuo? kokiomis = какими
        lt_[Daugiskaita][Vietininkas] = saknis + L"iose";   // kur? kame? kokiose = в каких
    } else if (wcscmp(linksniuote, L"ji") == 0) {
        gimine_ = Moteriskoji;
        linksniuote_ = Paradigma_M_ji;
        saknis = value_.substr(0, value_.size() - 1);
        lt_[Vienaskaita][Vardininkas] = saknis + L"i";      // kas? kuri = которая
        lt_[Vienaskaita][Kilmininkas] = saknis + L"ios";    // ko? kurios = которую
        lt_[Vienaskaita][Naudininkas] = saknis + L"iai";    // kam? kuriai = которой
        lt_[Vienaskaita][Galininkas] = saknis + L"ią";      // ką? kurią = которую
        lt_[Vienaskaita][Inagininkas] = saknis + L"ia";     // kuo? kuria = которой
        lt_[Vienaskaita][Vietininkas] = saknis + L"ioje";   // kur? kurioje = в которой

        lt_[Daugiskaita][Vardininkas] = saknis + L"ios";    // kas? kurios = которые
        lt_[Daugiskaita][Kilmininkas] = saknis + L"ių";     // ko? kurių = которых
        lt_[Daugiskaita][Naudininkas] = saknis + L"ioms";   // kam? kurioms = которым
        lt_[Daugiskaita][Galininkas] = saknis + L"ias";     // ką? kurias = которых
        lt_[Daugiskaita][Inagininkas] = saknis + L"iomis";  // kuo? kuriomis = которыми
        lt_[Daugiskaita][Vietininkas] = saknis + L"iose";   // kur? kame? kuriose = в которых
    } else {
        wprintf(L"Nezinoma Įvardio linksniuotė: %s\n", linksniuote);
    }
}


void IvardisGeneric::info() {
    wchar_t tstr[1024];
    int tcnt = 0;
    int wsz = static_cast<int>(value_.size()) + 8; // ending + spaces (pabaiga + įtrauka)

    tcnt = add2wline(tstr, tcnt, L"\nĮvardis: ", 0);
    tcnt = add2wline(tstr, tcnt, value_.c_str(), 0);
    tcnt = add2wline(tstr, tcnt, L", (", 0);
    tcnt = add2wline(tstr, tcnt, ru_[Vienaskaita][Vardininkas].c_str(), 0);
    tcnt = add2wline(tstr, tcnt, L")", 0);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"Vardininkas (kas?): ", 30);
    tcnt = add2wline(tstr, tcnt, lt_[Vienaskaita][Vardininkas].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, lt_[Daugiskaita][Vardininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"Kilmininkas (ko?): ", 30);
    tcnt = add2wline(tstr, tcnt, lt_[Vienaskaita][Kilmininkas].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, lt_[Daugiskaita][Kilmininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"Naudininkas (kam?): ", 30);
    tcnt = add2wline(tstr, tcnt, lt_[Vienaskaita][Naudininkas].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, lt_[Daugiskaita][Naudininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"Galininkas (ką?): ", 30);
    tcnt = add2wline(tstr, tcnt, lt_[Vienaskaita][Galininkas].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, lt_[Daugiskaita][Galininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"Inagininkas (kuo?): ", 30);
    tcnt = add2wline(tstr, tcnt, lt_[Vienaskaita][Inagininkas].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, lt_[Daugiskaita][Inagininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"Vietininkas (kur? kame?): ", 30);
    tcnt = add2wline(tstr, tcnt, lt_[Vienaskaita][Vietininkas].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, lt_[Daugiskaita][Vietininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);
}

std::wstring IvardisGeneric::gautiForma(AttributeType &arg) {
    std::wstring ret = L"";
    if (arg.is_dict() && arg.size()) {
        EAtvejis atvejis = str2atvejis(arg[L"Atvejis"].to_string());
        EGimine gimine = str2gimine(arg[L"Gimine"].to_string());
        ESkaicus skaicus = str2skaicus(arg[L"Skaicius"].to_string());
        if (gimine_ != Gimine_nezinoma && gimine_ != gimine) {
            // sutApti - совпадать, "рода не совпали".
            printf_error(L"Giminenys nesutapo %d != %d", gimine_, gimine);
        }

        ret = lt_[skaicus][atvejis];
    } else {
        ret = lt_[Vienaskaita][Vardininkas];
    }
    return ret;
}

