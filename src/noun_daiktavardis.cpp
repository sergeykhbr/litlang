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

#include "noun_daiktavardis.h"
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
        Linksniuotė - склонение, {"Linksniuote":1 or ..}
            1 .. 5 в зависимости от окончания
        Модификаторы
            Sangrąžinis daiktarvadis - Возвратное существительное -si (+Šauksmininkas): -as => -asis; -e => -esi
            Deminutyvas - Уменьшительная форма -(uk) (+Šauksmininkas): as => -ukas; -e => -uk

*/

DaiktavardisGeneric::DaiktavardisGeneric(AttributeType *cfg)
    : WordGeneric(cfg),
    gimine_(Gimine_nezinoma),
    paradigma_(Paradigma_nezinoma)
{
    int linksniuote = Linksniuote_nezinoma;
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
    if ((*cfg).has_key(L"Linksniuote")) {
        linksniuote = (*cfg)[L"Linksniuote"].to_int();
    }

    AttributeType &ru = (*cfg)[L"Ru"];
    if (ru.is_string()) {
        ru_[Vienaskaita][Vardininkas] = std::wstring(ru.to_string());
    } else if (ru.is_list() && ru.size() == 2) {
        AttributeType &viena = ru[0u];
        AttributeType &daug = ru[1];
        if (viena.is_list() && viena.size() == Atvejis_Total
            && daug.is_list() && daug.size() == Atvejis_Total) {
            for (unsigned i = 0; i < Atvejis_Total; i++) {
                ru_[Vienaskaita][i] = std::wstring(viena[i].to_string());
                ru_[Daugiskaita][i] = std::wstring(daug[i].to_string());
            }
        } else {
            printf_error("Neteisinga atvejis format %d", viena.size());
        }
    } else {
        printf_error("Neteisinga [[..],[..]] format %d", ru.size());
    }

    nustatyti_paradigma(linksniuote);
    atnaujinti();
}

// Определить парадигму
void DaiktavardisGeneric::nustatyti_paradigma(int linksniuote) {
    const wchar_t *p = value_.c_str();
    int sz = static_cast<int>(value_.size());
    int tcnt = 0;
    if (p[sz-1] == L's') {
        if (p[sz-3] == L'i' && p[sz-2] == L'a') {
            paradigma_ = Paradigma_1_ias;
        } else if (p[sz-2] == L'a') {
            paradigma_ = Paradigma_1_as;
        } else if (p[sz-2] == L'y') {
            paradigma_ = Paradigma_1_ys;
        } else if (p[sz-2] == L'i'
            && (linksniuote == Linksniuote_nezinoma || linksniuote == 1)) {
            paradigma_ = Paradigma_1_is;
        } else if (p[sz-2] == L'i' && linksniuote == 3) {
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
        if (p[sz-2] == L'i') {
            paradigma_ = Paradigma_2_ia;
        } else {
            paradigma_ = Paradigma_2_a;
        }
    } else if (p[sz-1] == L'ė') {
        if (linksniuote == Linksniuote_nezinoma || linksniuote == 2) {
            paradigma_ = Paradigma_2_e;
        } else if (linksniuote == 5) {
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
    std::wstring saknis = imkSaknis(0);
    std::wstring daugiskaita_kilmininkas = saknis;

    // Множестенное число, родительное падеж, если число круглое или превышает десяток
    // используется модфицированный корень слова:
    if (saknis[saknis.size() - 1] == L'd') {
        // d -> dž
        daugiskaita_kilmininkas = saknis.substr(0, saknis.size() - 1);
        daugiskaita_kilmininkas += std::wstring(L"dž"); //
    } else if (saknis[saknis.size() - 1] == L't') {
        // t -> č
        daugiskaita_kilmininkas = saknis.substr(0, saknis.size() - 1);
        daugiskaita_kilmininkas += std::wstring(L"č");  //
    }


    switch (paradigma_) {
    case Paradigma_1_as:
        lentele_[Vienaskaita][Vardininkas] = saknis + std::wstring(L"as");  // именительный. namas = дом
        lentele_[Vienaskaita][Kilmininkas] = saknis + std::wstring(L"o");   // родительный. namo = дома
        lentele_[Vienaskaita][Naudininkas] = saknis + std::wstring(L"ui");  // дательный/полезный. namui = дому
        lentele_[Vienaskaita][Galininkas] = saknis + std::wstring(L"ą");    // винительный/могущий. namą = дома
        lentele_[Vienaskaita][Inagininkas] = saknis + std::wstring(L"u");   // творительный. namu = домом
        lentele_[Vienaskaita][Vietininkas] = saknis + std::wstring(L"e");   // местный. name = в доме
        lentele_[Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"e"); // звательный. name = дооооом!

        lentele_[Daugiskaita][Vardininkas] = saknis + std::wstring(L"ai");  // namai = дома
        lentele_[Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"ų");// namų = домов
        lentele_[Daugiskaita][Naudininkas] = saknis + std::wstring(L"ams"); // namams = домам
        lentele_[Daugiskaita][Galininkas] = saknis + std::wstring(L"us");   // namus = дома
        lentele_[Daugiskaita][Inagininkas] = saknis + std::wstring(L"ais"); // nomais = домами
        lentele_[Daugiskaita][Vietininkas] = saknis + std::wstring(L"uose");// namuose = в домах
        lentele_[Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"ai");// namai = домаааа!
        break;

    case Paradigma_1_ias:
        lentele_[Vienaskaita][Vardininkas] = saknis + std::wstring(L"ias");  // именительный. kelias = дорога
        lentele_[Vienaskaita][Kilmininkas] = saknis + std::wstring(L"io");   // родительный. kelio = дорог
        lentele_[Vienaskaita][Naudininkas] = saknis + std::wstring(L"iui");  // дательный/полезный. keliui = дороге
        lentele_[Vienaskaita][Galininkas] = saknis + std::wstring(L"ią");    // винительный/могущий. kelią = дорогу
        lentele_[Vienaskaita][Inagininkas] = saknis + std::wstring(L"iu");   // творительный. keliu = дорогой
        lentele_[Vienaskaita][Vietininkas] = saknis + std::wstring(L"yje");  // местный. kelyje = в дороге
        lentele_[Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"e");  // звательный. kele

        lentele_[Daugiskaita][Vardininkas] = saknis + std::wstring(L"iai");  // keliai = дороги
        lentele_[Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"ių");// kelių = дорог
        lentele_[Daugiskaita][Naudininkas] = saknis + std::wstring(L"iams"); // keliams = дорогам
        lentele_[Daugiskaita][Galininkas] = saknis + std::wstring(L"ius");   // kelius = дороги
        lentele_[Daugiskaita][Inagininkas] = saknis + std::wstring(L"iais"); // keliais = дорогами
        lentele_[Daugiskaita][Vietininkas] = saknis + std::wstring(L"iuose");// keliuose в дорогах
        lentele_[Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"iai");// keliai = дорогиии!
        break;

    case Paradigma_1_is:
        lentele_[Vienaskaita][Vardininkas] = saknis + std::wstring(L"is");  // именительный. brolis
        lentele_[Vienaskaita][Kilmininkas] = saknis + std::wstring(L"io");   // родительный. brolio
        lentele_[Vienaskaita][Naudininkas] = saknis + std::wstring(L"iui");  // дательный/полезный. broliui
        lentele_[Vienaskaita][Galininkas] = saknis + std::wstring(L"į");    // винительный/могущий. brolį
        lentele_[Vienaskaita][Inagininkas] = saknis + std::wstring(L"iu");   // творительный. broliu
        lentele_[Vienaskaita][Vietininkas] = saknis + std::wstring(L"yje");  // местный. brolyje
        lentele_[Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"i");  // звательный. broli

        lentele_[Daugiskaita][Vardininkas] = saknis + std::wstring(L"iai");  // broliai
        lentele_[Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"ių");// brolių
        lentele_[Daugiskaita][Naudininkas] = saknis + std::wstring(L"iams"); // broliams
        lentele_[Daugiskaita][Galininkas] = saknis + std::wstring(L"ius");   // brolius
        lentele_[Daugiskaita][Inagininkas] = saknis + std::wstring(L"iais"); // broliais
        lentele_[Daugiskaita][Vietininkas] = saknis + std::wstring(L"iuose");// broliuose
        lentele_[Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"iai");// broliai
        break;

    case Paradigma_1_ys:
        lentele_[Vienaskaita][Vardininkas] = saknis + std::wstring(L"ias");  // именительный. kambarys = команата
        lentele_[Vienaskaita][Kilmininkas] = saknis + std::wstring(L"io");   // родительный. kambario = комнаты
        lentele_[Vienaskaita][Naudininkas] = saknis + std::wstring(L"iui");  // дательный/полезный. kambariui = комнате
        lentele_[Vienaskaita][Galininkas] = saknis + std::wstring(L"į");    // винительный/могущий. kambarį = комнату
        lentele_[Vienaskaita][Inagininkas] = saknis + std::wstring(L"iu");   // творительный. kambariu = комнатой
        lentele_[Vienaskaita][Vietininkas] = saknis + std::wstring(L"yje");  // местный. kambaryje в комнате
        lentele_[Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"y");  // звательный. kambary комнатааа!

        lentele_[Daugiskaita][Vardininkas] = saknis + std::wstring(L"iai");  // kambariai
        lentele_[Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"ių");// kambarių
        lentele_[Daugiskaita][Naudininkas] = saknis + std::wstring(L"iams"); // kambariams
        lentele_[Daugiskaita][Galininkas] = saknis + std::wstring(L"ius");   // kambarius
        lentele_[Daugiskaita][Inagininkas] = saknis + std::wstring(L"iais"); // kambariais
        lentele_[Daugiskaita][Vietininkas] = saknis + std::wstring(L"iuose");// kambariuose
        lentele_[Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"iai");// kambariai
        break;

    case Paradigma_2_a:
        lentele_[Vienaskaita][Vardininkas] = saknis + std::wstring(L"a");    // именительный. knyga
        lentele_[Vienaskaita][Kilmininkas] = saknis + std::wstring(L"os");   // родительный. knygos
        lentele_[Vienaskaita][Naudininkas] = saknis + std::wstring(L"ai");   // дательный/полезный. knygai
        lentele_[Vienaskaita][Galininkas] = saknis + std::wstring(L"ą");     // винительный/могущий. knygą
        lentele_[Vienaskaita][Inagininkas] = saknis + std::wstring(L"a");    // творительный. kniga
        lentele_[Vienaskaita][Vietininkas] = saknis + std::wstring(L"oje");  // местный. knigoje
        lentele_[Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"a");  // звательный. kniga

        lentele_[Daugiskaita][Vardininkas] = saknis + std::wstring(L"os");   // knygos
        lentele_[Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"ų");// knigų
        lentele_[Daugiskaita][Naudininkas] = saknis + std::wstring(L"oms");  // knygoms
        lentele_[Daugiskaita][Galininkas] = saknis + std::wstring(L"as");    // knygas
        lentele_[Daugiskaita][Inagininkas] = saknis + std::wstring(L"omis"); // knygomis
        lentele_[Daugiskaita][Vietininkas] = saknis + std::wstring(L"ose");  // knygose
        lentele_[Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"os"); // knygos
        break;

    case Paradigma_2_ia:
        lentele_[Vienaskaita][Vardininkas] = saknis + std::wstring(L"ia");  // именительный. vyšnia
        lentele_[Vienaskaita][Kilmininkas] = saknis + std::wstring(L"ios");   // родительный. vyšnios
        lentele_[Vienaskaita][Naudininkas] = saknis + std::wstring(L"iai");  // дательный/полезный. vyšniai
        lentele_[Vienaskaita][Galininkas] = saknis + std::wstring(L"ią");    // винительный/могущий. višnią
        lentele_[Vienaskaita][Inagininkas] = saknis + std::wstring(L"ia");   // творительный. vyšnia
        lentele_[Vienaskaita][Vietininkas] = saknis + std::wstring(L"ioje");  // местный. vyšnioje
        lentele_[Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"ia");  // звательный. vyšnia

        lentele_[Daugiskaita][Vardininkas] = saknis + std::wstring(L"ios");  // vyšnios
        lentele_[Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"ių");// vyšnių
        lentele_[Daugiskaita][Naudininkas] = saknis + std::wstring(L"ioms"); // vyšnioms
        lentele_[Daugiskaita][Galininkas] = saknis + std::wstring(L"ias");   // vyšnias
        lentele_[Daugiskaita][Inagininkas] = saknis + std::wstring(L"iomis"); // vyšniomis
        lentele_[Daugiskaita][Vietininkas] = saknis + std::wstring(L"iuose");// vyšniose
        lentele_[Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"iai");// vyšnios
        break;

    case Paradigma_2_e:
        lentele_[Vienaskaita][Vardininkas] = saknis + std::wstring(L"ė");  // именительный. gatvė
        lentele_[Vienaskaita][Kilmininkas] = saknis + std::wstring(L"ės");   // родительный. gatvės
        lentele_[Vienaskaita][Naudininkas] = saknis + std::wstring(L"ei");  // дательный/полезный. gatvei
        lentele_[Vienaskaita][Galininkas] = saknis + std::wstring(L"ę");    // винительный/могущий. gatvę
        lentele_[Vienaskaita][Inagininkas] = saknis + std::wstring(L"e");   // творительный. gatve
        lentele_[Vienaskaita][Vietininkas] = saknis + std::wstring(L"ėje");  // местный. gatvėje
        lentele_[Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"e");  // звательный. gatve

        lentele_[Daugiskaita][Vardininkas] = saknis + std::wstring(L"ės");  // gatvės
        lentele_[Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"ių");// gatvių
        lentele_[Daugiskaita][Naudininkas] = saknis + std::wstring(L"ėms"); // gatvėms
        lentele_[Daugiskaita][Galininkas] = saknis + std::wstring(L"es");   // gatves
        lentele_[Daugiskaita][Inagininkas] = saknis + std::wstring(L"ėmis"); // gatvėmis
        lentele_[Daugiskaita][Vietininkas] = saknis + std::wstring(L"ėse");// gatvėse
        lentele_[Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"ės");// gatvės
        break;

    case Paradigma_3_is:
        lentele_[Vienaskaita][Vardininkas] = saknis + std::wstring(L"is");      // именительный. pilis = замок; dantis = зубы
        lentele_[Vienaskaita][Kilmininkas] = saknis + std::wstring(L"ies");     // родительный. pilies; danties
        if (saknis[saknis.size() - 1] == L't') {
            // t -> č
            lentele_[Vienaskaita][Naudininkas] = saknis.substr(0, saknis.size() - 1);
            lentele_[Vienaskaita][Naudininkas] += std::wstring(L"č");
            lentele_[Vienaskaita][Naudininkas] += std::wstring(L"iui");         // dančiui, (todo: patirčiai)
        } else {
            lentele_[Vienaskaita][Naudininkas] = saknis + std::wstring(L"iai"); // дательный/полезный. piliai
        }
        lentele_[Vienaskaita][Galininkas] = saknis + std::wstring(L"į");        // винительный/могущий. pilį; dantį
        lentele_[Vienaskaita][Inagininkas] = saknis + std::wstring(L"imi");     // творительный. pilimi; dantimi
        lentele_[Vienaskaita][Vietininkas] = saknis + std::wstring(L"yje");     // местный. pilyje; dantyje
        lentele_[Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"ie");    // звательный. pilie; dantie

        lentele_[Daugiskaita][Vardininkas] = saknis + std::wstring(L"ys");      // pilys; dantys
        if (saknis[saknis.size() - 1] == L't') {
            lentele_[Daugiskaita][Kilmininkas] = saknis + std::wstring(L"ų");   // dantų
        } else {
            lentele_[Daugiskaita][Kilmininkas] = saknis + std::wstring(L"ių");  // pilių, (todo: patirčiu)
        }
        lentele_[Daugiskaita][Naudininkas] = saknis + std::wstring(L"ims");     // pilims; dantims
        lentele_[Daugiskaita][Galininkas] = saknis + std::wstring(L"is");       // pilis; dantis
        lentele_[Daugiskaita][Inagininkas] = saknis + std::wstring(L"imis");    // pilimis; dantimis
        lentele_[Daugiskaita][Vietininkas] = saknis + std::wstring(L"yse");     // pilyse; dantyse
        lentele_[Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"ys");    // pilys; dantys
        break;

    case Paradigma_4_us:
        lentele_[Vienaskaita][Vardininkas] = saknis + std::wstring(L"us");  // именительный. sūnus = сын
        lentele_[Vienaskaita][Kilmininkas] = saknis + std::wstring(L"aus");   // родительный. sūnaus
        lentele_[Vienaskaita][Naudininkas] = saknis + std::wstring(L"ui");  // дательный/полезный. sūnui
        lentele_[Vienaskaita][Galininkas] = saknis + std::wstring(L"ų");    // винительный/могущий. sūnų
        lentele_[Vienaskaita][Inagininkas] = saknis + std::wstring(L"umi");   // творительный. sūnumi
        lentele_[Vienaskaita][Vietininkas] = saknis + std::wstring(L"uje");  // местный. sūnuje
        lentele_[Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"au");  // звательный. sūnau

        lentele_[Daugiskaita][Vardininkas] = saknis + std::wstring(L"ūs");  // sūnūs
        lentele_[Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"ų");// sunų
        lentele_[Daugiskaita][Naudininkas] = saknis + std::wstring(L"ums"); // sūnums
        lentele_[Daugiskaita][Galininkas] = saknis + std::wstring(L"us");   // sūnus
        lentele_[Daugiskaita][Inagininkas] = saknis + std::wstring(L"umis"); // sūnumis
        lentele_[Daugiskaita][Vietininkas] = saknis + std::wstring(L"uose");// sūnuose
        lentele_[Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"ūs");// sūnūs
        break;

    case Paradigma_4_ius:
        lentele_[Vienaskaita][Vardininkas] = saknis + std::wstring(L"ias");  // именительный. vaisius = фрукт
        lentele_[Vienaskaita][Kilmininkas] = saknis + std::wstring(L"iaus");   // родительный. vaisiaus
        lentele_[Vienaskaita][Naudininkas] = saknis + std::wstring(L"iui");  // дательный/полезный. vaisiui
        lentele_[Vienaskaita][Galininkas] = saknis + std::wstring(L"ių");    // винительный/могущий. vaisių
        lentele_[Vienaskaita][Inagininkas] = saknis + std::wstring(L"iumi");   // творительный. vaisiumi
        lentele_[Vienaskaita][Vietininkas] = saknis + std::wstring(L"iuje");  // местный. vaisiuje
        lentele_[Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"iau");  // звательный. vaisiau

        lentele_[Daugiskaita][Vardininkas] = saknis + std::wstring(L"iai");  // vaisiai
        lentele_[Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"ių");// vaisių
        lentele_[Daugiskaita][Naudininkas] = saknis + std::wstring(L"iams"); // vaisiams
        lentele_[Daugiskaita][Galininkas] = saknis + std::wstring(L"ius");   // vaisius
        lentele_[Daugiskaita][Inagininkas] = saknis + std::wstring(L"iais"); // vaisiais
        lentele_[Daugiskaita][Vietininkas] = saknis + std::wstring(L"iuose");// vaisiuose
        lentele_[Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"iai");// vaisiai
        break;

    case Paradigma_5_uo:
        if (gimine_ == Vyriskoji) {
            lentele_[Vienaskaita][Vardininkas] = saknis + std::wstring(L"uo");  // именительный. akmuo = камень
            lentele_[Vienaskaita][Kilmininkas] = saknis + std::wstring(L"ens");   // родительный. akmens
            lentele_[Vienaskaita][Naudininkas] = saknis + std::wstring(L"eniui");  // дательный/полезный.  akmeniui
            lentele_[Vienaskaita][Galininkas] = saknis + std::wstring(L"enį");    // винительный/могущий. akmenį
            lentele_[Vienaskaita][Inagininkas] = saknis + std::wstring(L"eniu");   // творительный. akmeniu
            lentele_[Vienaskaita][Vietininkas] = saknis + std::wstring(L"enyje");  // местный. akmenyje
            lentele_[Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"enie");  // звательный. akmenie

            lentele_[Daugiskaita][Vardininkas] = saknis + std::wstring(L"enys");  // akmenys
            lentele_[Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"enų");// akmenų
            lentele_[Daugiskaita][Naudininkas] = saknis + std::wstring(L"enims"); // akmenims
            lentele_[Daugiskaita][Galininkas] = saknis + std::wstring(L"enis");   // akmenis
            lentele_[Daugiskaita][Inagininkas] = saknis + std::wstring(L"enimis"); // akmenimis
            lentele_[Daugiskaita][Vietininkas] = saknis + std::wstring(L"enyse");// akmenyse
            lentele_[Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"enys");// akmenys
        } else if (gimine_ == Moteriskoji) {
            lentele_[Vienaskaita][Vardininkas] = saknis + std::wstring(L"uo");  // именительный. sesuo = сестра
            lentele_[Vienaskaita][Kilmininkas] = saknis + std::wstring(L"ers");   // родительный. sesers
            lentele_[Vienaskaita][Naudininkas] = saknis + std::wstring(L"eriai");  // дательный/полезный.  seseriai
            lentele_[Vienaskaita][Galininkas] = saknis + std::wstring(L"erį");    // винительный/могущий. seserį
            lentele_[Vienaskaita][Inagininkas] = saknis + std::wstring(L"eria");   // творительный. seseria
            lentele_[Vienaskaita][Vietininkas] = saknis + std::wstring(L"eryje");  // местный. seseryje
            lentele_[Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"erie");  // звательный. seserie

            lentele_[Daugiskaita][Vardininkas] = saknis + std::wstring(L"erys");  // seserys
            lentele_[Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"erų");// seserų
            lentele_[Daugiskaita][Naudininkas] = saknis + std::wstring(L"erims"); // seserims
            lentele_[Daugiskaita][Galininkas] = saknis + std::wstring(L"eris");   // seseris
            lentele_[Daugiskaita][Inagininkas] = saknis + std::wstring(L"erimis"); // seserimis
            lentele_[Daugiskaita][Vietininkas] = saknis + std::wstring(L"eryse");// seseryse
            lentele_[Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"erys");// seserys
        }
        break;

    case Paradigma_5_e:
        if (gimine_ == Vyriskoji) {
            lentele_[Vienaskaita][Vardininkas] = saknis + std::wstring(L"ė");  // именительный. tete
            lentele_[Vienaskaita][Kilmininkas] = saknis + std::wstring(L"ens");   // родительный. akmens
            lentele_[Vienaskaita][Naudininkas] = saknis + std::wstring(L"eniui");  // дательный/полезный.  
            lentele_[Vienaskaita][Galininkas] = saknis + std::wstring(L"enį");    // винительный/могущий. 
            lentele_[Vienaskaita][Inagininkas] = saknis + std::wstring(L"eniu");   // творительный. 
            lentele_[Vienaskaita][Vietininkas] = saknis + std::wstring(L"enyje");  // местный. 
            lentele_[Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"enie");  // звательный. 

            lentele_[Daugiskaita][Vardininkas] = saknis + std::wstring(L"enys");  // 
            lentele_[Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"enų");// 
            lentele_[Daugiskaita][Naudininkas] = saknis + std::wstring(L"enims"); // 
            lentele_[Daugiskaita][Galininkas] = saknis + std::wstring(L"enis");   // 
            lentele_[Daugiskaita][Inagininkas] = saknis + std::wstring(L"enimis"); // 
            lentele_[Daugiskaita][Vietininkas] = saknis + std::wstring(L"enyse");// 
            lentele_[Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"enys");//
        } else if (gimine_ == Moteriskoji) {
            lentele_[Vienaskaita][Vardininkas] = saknis + std::wstring(L"ė");  // именительный. duktė = дочь
            lentele_[Vienaskaita][Kilmininkas] = saknis + std::wstring(L"ers");   // родительный. dukters
            lentele_[Vienaskaita][Naudininkas] = saknis + std::wstring(L"eriai");  // дательный/полезный.  dukteriai
            lentele_[Vienaskaita][Galininkas] = saknis + std::wstring(L"erį");    // винительный/могущий. dukterį
            lentele_[Vienaskaita][Inagininkas] = saknis + std::wstring(L"eria");   // творительный. dukteria
            lentele_[Vienaskaita][Vietininkas] = saknis + std::wstring(L"eryje");  // местный. dukteryje
            lentele_[Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"erie");  // звательный. dukterie

            lentele_[Daugiskaita][Vardininkas] = saknis + std::wstring(L"erys");  // dukterys
            lentele_[Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"erų");// dukterų
            lentele_[Daugiskaita][Naudininkas] = saknis + std::wstring(L"erims"); // dukterims
            lentele_[Daugiskaita][Galininkas] = saknis + std::wstring(L"eris");   // dukteris
            lentele_[Daugiskaita][Inagininkas] = saknis + std::wstring(L"erimis"); // dukterimis
            lentele_[Daugiskaita][Vietininkas] = saknis + std::wstring(L"eryse");// dukteryse
            lentele_[Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"erys");// dukterys
        }
        break;
    default:;
    }
}

// root of the word (common part without ending):
std::wstring DaiktavardisGeneric::imkSaknis(int idx) {
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

int DaiktavardisGeneric::imkLinksniuote() {
    int ret = Linksniuote_nezinoma;
    switch (paradigma_) {
    // 1 linksniuotė
    case Paradigma_1_as:
    case Paradigma_1_ias:
    case Paradigma_1_is:
    case Paradigma_1_ys:
        ret = 1;
        break;
    // 2 linksniuotė
    case Paradigma_2_a:
    case Paradigma_2_ia:
    case Paradigma_2_e:  // ė
        ret = 2;
        break;
    // 3 linksniuotė
    case Paradigma_3_is:
        ret = 3;
        break;
    // 4 linksniuotė
    case Paradigma_4_us:
    case Paradigma_4_ius:
        ret = 4;
        break;
    // 5 linksniuotė
    case Paradigma_5_uo:
    case Paradigma_5_e:  // ė
        ret = 5;
       break; 
    }
    return ret;
}

void DaiktavardisGeneric::info() {
    wchar_t tstr[1024];
    int wsz = static_cast<int>(value_.size()) + 8; // ending + spaces (pabaiga + įtrauka)
    int tcnt = 0;
    printf_log(L"\nDaiktavardis: %s, linksniuotė: %d, (%s)\n",
            value_.c_str(),
            imkLinksniuote(),
            ru_[Vienaskaita][Vardininkas].c_str());

    tcnt = add2wline(tstr, 0, L"Vardininkas (kas?): ", 30);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Vardininkas].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Vardininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"Kilmininkas (ko?): ", 30);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Kilmininkas].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Kilmininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"Naudininkas (kam?): ", 30);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Naudininkas].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Naudininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"Galininkas (ką?): ", 30);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Galininkas].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Galininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"Inagininkas (kuo?): ", 30);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Inagininkas].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Inagininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"Vietininkas (kur? kame?): ", 30);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Vietininkas].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Vietininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"Šauksmininkas: ", 30);
    tcnt = add2wline(tstr, tcnt, lentele_[Vienaskaita][Sauksmininkas].c_str(), wsz);
    tcnt = add2wline(tstr, tcnt, lentele_[Daugiskaita][Sauksmininkas].c_str(), 0);
    printf_log(L"%s\n", tstr);
}

std::wstring DaiktavardisGeneric::gautiForma(AttributeType &arg) {
    std::wstring ret = L"";
    EAtvejis atvejis = str2atvejis(arg[L"Atvejis"].to_string());
    EGimine gimine = str2gimine(arg[L"Gimine"].to_string());
    ESkaicus skaicus = str2skaicus(arg[L"Skaicius"].to_string());
    std::wstring sangrazinis = L""; // возвратная форма (sangrąžynis) -si
    std::wstring deminutyvas = L"";

    if (arg.has_key(L"Sangrazynis")) {
        // возвратная форма (sangrąžynis) -si
        sangrazinis = std::wstring(arg[L"Sangrazinis"].to_string());
    }

    if (arg.has_key(L"Deminutyvas")) {
        // уменьшительная форма (deminutyvas) -uk
        deminutyvas = std::wstring(arg[L"Deminutyvas"].to_string());
    }

    if (sangrazinis != L""
        && atvejis == Sauksmininkas
        && gimine == Vyriskoji) {
        // Возвратное существительное мужского рода (праткически не употребляется)
        if (paradigma_ == Paradigma_2_e || paradigma_ == Paradigma_5_e) {
            ret = imkSaknis(0) + L"esi";
        } else if (paradigma_ == Paradigma_1_is || paradigma_ == Paradigma_3_is
             || paradigma_ == Paradigma_1_as) {
            ret = imkSaknis(0) + L"asis";
        } else {
            ret = imkSaknis(0) + L"is";
            // unsupported = nepalaikomas
            printf_error(L"nepalaikomas sangrąžynis daiktavardis (-si): %s", value_.c_str());
        }
    } else {
        ret = lentele_[skaicus][atvejis];
    }
    return ret;
}

