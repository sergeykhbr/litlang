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
        lentele_[Skaicus_Vienaskaita][Vardininkas] = saknis + std::wstring(L"as");  // именительный. namas = дом
        lentele_[Skaicus_Vienaskaita][Kilmininkas] = saknis + std::wstring(L"o");   // родительный. namo = дома
        lentele_[Skaicus_Vienaskaita][Naudininkas] = saknis + std::wstring(L"ui");  // дательный/полезный. namui = дому
        lentele_[Skaicus_Vienaskaita][Galininkas] = saknis + std::wstring(L"ą");    // винительный/могущий. namą = дома
        lentele_[Skaicus_Vienaskaita][Inagininkas] = saknis + std::wstring(L"u");   // творительный. namu = домом
        lentele_[Skaicus_Vienaskaita][Vietininkas] = saknis + std::wstring(L"e");   // местный. name = в доме
        lentele_[Skaicus_Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"e"); // звательный. name = дооооом!

        lentele_[Skaicus_Daugiskaita][Vardininkas] = saknis + std::wstring(L"ai");  // namai = дома
        lentele_[Skaicus_Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"ų");// namų = домов
        lentele_[Skaicus_Daugiskaita][Naudininkas] = saknis + std::wstring(L"ams"); // namams = домам
        lentele_[Skaicus_Daugiskaita][Galininkas] = saknis + std::wstring(L"us");   // namus = дома
        lentele_[Skaicus_Daugiskaita][Inagininkas] = saknis + std::wstring(L"ais"); // nomais = домами
        lentele_[Skaicus_Daugiskaita][Vietininkas] = saknis + std::wstring(L"uose");// namuose = в домах
        lentele_[Skaicus_Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"ai");// namai = домаааа!
        break;

    case Paradigma_1_ias:
        lentele_[Skaicus_Vienaskaita][Vardininkas] = saknis + std::wstring(L"ias");  // именительный. kelias = дорога
        lentele_[Skaicus_Vienaskaita][Kilmininkas] = saknis + std::wstring(L"io");   // родительный. kelio = дорог
        lentele_[Skaicus_Vienaskaita][Naudininkas] = saknis + std::wstring(L"iui");  // дательный/полезный. keliui = дороге
        lentele_[Skaicus_Vienaskaita][Galininkas] = saknis + std::wstring(L"ią");    // винительный/могущий. kelią = дорогу
        lentele_[Skaicus_Vienaskaita][Inagininkas] = saknis + std::wstring(L"iu");   // творительный. keliu = дорогой
        lentele_[Skaicus_Vienaskaita][Vietininkas] = saknis + std::wstring(L"yje");  // местный. kelyje = в дороге
        lentele_[Skaicus_Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"e");  // звательный. kele

        lentele_[Skaicus_Daugiskaita][Vardininkas] = saknis + std::wstring(L"iai");  // keliai = дороги
        lentele_[Skaicus_Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"ių");// kelių = дорог
        lentele_[Skaicus_Daugiskaita][Naudininkas] = saknis + std::wstring(L"iams"); // keliams = дорогам
        lentele_[Skaicus_Daugiskaita][Galininkas] = saknis + std::wstring(L"ius");   // kelius = дороги
        lentele_[Skaicus_Daugiskaita][Inagininkas] = saknis + std::wstring(L"iais"); // keliais = дорогами
        lentele_[Skaicus_Daugiskaita][Vietininkas] = saknis + std::wstring(L"iuose");// keliuose в дорогах
        lentele_[Skaicus_Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"iai");// keliai = дорогиии!
        break;

    case Paradigma_1_is:
        lentele_[Skaicus_Vienaskaita][Vardininkas] = saknis + std::wstring(L"is");  // именительный. brolis
        lentele_[Skaicus_Vienaskaita][Kilmininkas] = saknis + std::wstring(L"io");   // родительный. brolio
        lentele_[Skaicus_Vienaskaita][Naudininkas] = saknis + std::wstring(L"iui");  // дательный/полезный. broliui
        lentele_[Skaicus_Vienaskaita][Galininkas] = saknis + std::wstring(L"į");    // винительный/могущий. brolį
        lentele_[Skaicus_Vienaskaita][Inagininkas] = saknis + std::wstring(L"iu");   // творительный. broliu
        lentele_[Skaicus_Vienaskaita][Vietininkas] = saknis + std::wstring(L"yje");  // местный. brolyje
        lentele_[Skaicus_Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"i");  // звательный. broli

        lentele_[Skaicus_Daugiskaita][Vardininkas] = saknis + std::wstring(L"iai");  // broliai
        lentele_[Skaicus_Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"ių");// brolių
        lentele_[Skaicus_Daugiskaita][Naudininkas] = saknis + std::wstring(L"iams"); // broliams
        lentele_[Skaicus_Daugiskaita][Galininkas] = saknis + std::wstring(L"ius");   // brolius
        lentele_[Skaicus_Daugiskaita][Inagininkas] = saknis + std::wstring(L"iais"); // broliais
        lentele_[Skaicus_Daugiskaita][Vietininkas] = saknis + std::wstring(L"iuose");// broliuose
        lentele_[Skaicus_Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"iai");// broliai
        break;

    case Paradigma_1_ys:
        lentele_[Skaicus_Vienaskaita][Vardininkas] = saknis + std::wstring(L"ias");  // именительный. kambarys = команата
        lentele_[Skaicus_Vienaskaita][Kilmininkas] = saknis + std::wstring(L"io");   // родительный. kambario = комнаты
        lentele_[Skaicus_Vienaskaita][Naudininkas] = saknis + std::wstring(L"iui");  // дательный/полезный. kambariui = комнате
        lentele_[Skaicus_Vienaskaita][Galininkas] = saknis + std::wstring(L"į");    // винительный/могущий. kambarį = комнату
        lentele_[Skaicus_Vienaskaita][Inagininkas] = saknis + std::wstring(L"iu");   // творительный. kambariu = комнатой
        lentele_[Skaicus_Vienaskaita][Vietininkas] = saknis + std::wstring(L"yje");  // местный. kambaryje в комнате
        lentele_[Skaicus_Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"y");  // звательный. kambary комнатааа!

        lentele_[Skaicus_Daugiskaita][Vardininkas] = saknis + std::wstring(L"iai");  // kambariai
        lentele_[Skaicus_Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"ių");// kambarių
        lentele_[Skaicus_Daugiskaita][Naudininkas] = saknis + std::wstring(L"iams"); // kambariams
        lentele_[Skaicus_Daugiskaita][Galininkas] = saknis + std::wstring(L"ius");   // kambarius
        lentele_[Skaicus_Daugiskaita][Inagininkas] = saknis + std::wstring(L"iais"); // kambariais
        lentele_[Skaicus_Daugiskaita][Vietininkas] = saknis + std::wstring(L"iuose");// kambariuose
        lentele_[Skaicus_Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"iai");// kambariai
        break;

    case Paradigma_2_a:
        lentele_[Skaicus_Vienaskaita][Vardininkas] = saknis + std::wstring(L"a");    // именительный. knyga
        lentele_[Skaicus_Vienaskaita][Kilmininkas] = saknis + std::wstring(L"os");   // родительный. knygos
        lentele_[Skaicus_Vienaskaita][Naudininkas] = saknis + std::wstring(L"ai");   // дательный/полезный. knygai
        lentele_[Skaicus_Vienaskaita][Galininkas] = saknis + std::wstring(L"ą");     // винительный/могущий. knygą
        lentele_[Skaicus_Vienaskaita][Inagininkas] = saknis + std::wstring(L"a");    // творительный. kniga
        lentele_[Skaicus_Vienaskaita][Vietininkas] = saknis + std::wstring(L"oje");  // местный. knigoje
        lentele_[Skaicus_Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"a");  // звательный. kniga

        lentele_[Skaicus_Daugiskaita][Vardininkas] = saknis + std::wstring(L"os");   // knygos
        lentele_[Skaicus_Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"ų");// knigų
        lentele_[Skaicus_Daugiskaita][Naudininkas] = saknis + std::wstring(L"oms");  // knygoms
        lentele_[Skaicus_Daugiskaita][Galininkas] = saknis + std::wstring(L"as");    // knygas
        lentele_[Skaicus_Daugiskaita][Inagininkas] = saknis + std::wstring(L"omis"); // knygomis
        lentele_[Skaicus_Daugiskaita][Vietininkas] = saknis + std::wstring(L"ose");  // knygose
        lentele_[Skaicus_Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"os"); // knygos
        break;

    case Paradigma_2_ia:
        lentele_[Skaicus_Vienaskaita][Vardininkas] = saknis + std::wstring(L"ia");  // именительный. višnia
        lentele_[Skaicus_Vienaskaita][Kilmininkas] = saknis + std::wstring(L"ios");   // родительный. vyšnios
        lentele_[Skaicus_Vienaskaita][Naudininkas] = saknis + std::wstring(L"iai");  // дательный/полезный. vyšniai
        lentele_[Skaicus_Vienaskaita][Galininkas] = saknis + std::wstring(L"ią");    // винительный/могущий. višnią
        lentele_[Skaicus_Vienaskaita][Inagininkas] = saknis + std::wstring(L"ia");   // творительный. višnia
        lentele_[Skaicus_Vienaskaita][Vietininkas] = saknis + std::wstring(L"ioje");  // местный. višnioje
        lentele_[Skaicus_Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"ia");  // звательный. višnia

        lentele_[Skaicus_Daugiskaita][Vardininkas] = saknis + std::wstring(L"ios");  // višnios
        lentele_[Skaicus_Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"ių");// višnių
        lentele_[Skaicus_Daugiskaita][Naudininkas] = saknis + std::wstring(L"ioms"); // vyšnioms
        lentele_[Skaicus_Daugiskaita][Galininkas] = saknis + std::wstring(L"ias");   // vyšnias
        lentele_[Skaicus_Daugiskaita][Inagininkas] = saknis + std::wstring(L"iomis"); // vyšniomis
        lentele_[Skaicus_Daugiskaita][Vietininkas] = saknis + std::wstring(L"iuose");// vyšniose
        lentele_[Skaicus_Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"iai");// vyšnios
        break;

    case Paradigma_2_e:
        lentele_[Skaicus_Vienaskaita][Vardininkas] = saknis + std::wstring(L"ė");  // именительный. gatvė
        lentele_[Skaicus_Vienaskaita][Kilmininkas] = saknis + std::wstring(L"ės");   // родительный. gatvės
        lentele_[Skaicus_Vienaskaita][Naudininkas] = saknis + std::wstring(L"ei");  // дательный/полезный. gatvei
        lentele_[Skaicus_Vienaskaita][Galininkas] = saknis + std::wstring(L"ę");    // винительный/могущий. gatvę
        lentele_[Skaicus_Vienaskaita][Inagininkas] = saknis + std::wstring(L"e");   // творительный. gatve
        lentele_[Skaicus_Vienaskaita][Vietininkas] = saknis + std::wstring(L"ėje");  // местный. gatvėje
        lentele_[Skaicus_Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"e");  // звательный. gatve

        lentele_[Skaicus_Daugiskaita][Vardininkas] = saknis + std::wstring(L"ės");  // gatvės
        lentele_[Skaicus_Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"ių");// gatvių
        lentele_[Skaicus_Daugiskaita][Naudininkas] = saknis + std::wstring(L"ėms"); // gatvėms
        lentele_[Skaicus_Daugiskaita][Galininkas] = saknis + std::wstring(L"es");   // gatves
        lentele_[Skaicus_Daugiskaita][Inagininkas] = saknis + std::wstring(L"ėmis"); // gatvėmis
        lentele_[Skaicus_Daugiskaita][Vietininkas] = saknis + std::wstring(L"ėse");// gatvėse
        lentele_[Skaicus_Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"ės");// gatvės
        break;

    case Paradigma_3_is:
        lentele_[Skaicus_Vienaskaita][Vardininkas] = saknis + std::wstring(L"is");      // именительный. pilis = замок; dantis = зубы
        lentele_[Skaicus_Vienaskaita][Kilmininkas] = saknis + std::wstring(L"ies");     // родительный. pilies; danties
        if (saknis[saknis.size() - 1] == L't') {
            // t -> č
            lentele_[Skaicus_Vienaskaita][Naudininkas] = saknis.substr(0, saknis.size() - 1);
            lentele_[Skaicus_Vienaskaita][Naudininkas] += std::wstring(L"č");
            lentele_[Skaicus_Vienaskaita][Naudininkas] += std::wstring(L"iui");         // dančiui
        } else {
            lentele_[Skaicus_Vienaskaita][Naudininkas] = saknis + std::wstring(L"iai"); // дательный/полезный. piliai
        }
        lentele_[Skaicus_Vienaskaita][Galininkas] = saknis + std::wstring(L"į");        // винительный/могущий. pilį; dantį
        lentele_[Skaicus_Vienaskaita][Inagininkas] = saknis + std::wstring(L"imi");     // творительный. pilimi; dantimi
        lentele_[Skaicus_Vienaskaita][Vietininkas] = saknis + std::wstring(L"yje");     // местный. pilyje; dantyje
        lentele_[Skaicus_Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"ie");    // звательный. pilie; dantie

        lentele_[Skaicus_Daugiskaita][Vardininkas] = saknis + std::wstring(L"ys");      // pilys; dantys
        if (saknis[saknis.size() - 1] == L't') {
            lentele_[Skaicus_Daugiskaita][Kilmininkas] = saknis + std::wstring(L"ų");   // dantų
        } else {
            lentele_[Skaicus_Daugiskaita][Kilmininkas] = saknis + std::wstring(L"ių");  // pilių
        }
        lentele_[Skaicus_Daugiskaita][Naudininkas] = saknis + std::wstring(L"ims");     // pilims; dantims
        lentele_[Skaicus_Daugiskaita][Galininkas] = saknis + std::wstring(L"is");       // pilis; dantis
        lentele_[Skaicus_Daugiskaita][Inagininkas] = saknis + std::wstring(L"imis");    // pilimis; dantimis
        lentele_[Skaicus_Daugiskaita][Vietininkas] = saknis + std::wstring(L"yse");     // pilyse; dantyse
        lentele_[Skaicus_Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"ys");    // pilys; dantys
        break;

    case Paradigma_4_us:
        lentele_[Skaicus_Vienaskaita][Vardininkas] = saknis + std::wstring(L"us");  // именительный. sūnus = сын
        lentele_[Skaicus_Vienaskaita][Kilmininkas] = saknis + std::wstring(L"aus");   // родительный. sūnaus
        lentele_[Skaicus_Vienaskaita][Naudininkas] = saknis + std::wstring(L"ui");  // дательный/полезный. sūnui
        lentele_[Skaicus_Vienaskaita][Galininkas] = saknis + std::wstring(L"ų");    // винительный/могущий. sūnų
        lentele_[Skaicus_Vienaskaita][Inagininkas] = saknis + std::wstring(L"umi");   // творительный. sūnumi
        lentele_[Skaicus_Vienaskaita][Vietininkas] = saknis + std::wstring(L"uje");  // местный. sūnuje
        lentele_[Skaicus_Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"au");  // звательный. sūnau

        lentele_[Skaicus_Daugiskaita][Vardininkas] = saknis + std::wstring(L"ūs");  // sūnūs
        lentele_[Skaicus_Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"ų");// sunų
        lentele_[Skaicus_Daugiskaita][Naudininkas] = saknis + std::wstring(L"ums"); // sūnums
        lentele_[Skaicus_Daugiskaita][Galininkas] = saknis + std::wstring(L"us");   // sūnus
        lentele_[Skaicus_Daugiskaita][Inagininkas] = saknis + std::wstring(L"umis"); // sūnumis
        lentele_[Skaicus_Daugiskaita][Vietininkas] = saknis + std::wstring(L"uose");// sūnuose
        lentele_[Skaicus_Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"ūs");// sūnūs
        break;

    case Paradigma_4_ius:
        lentele_[Skaicus_Vienaskaita][Vardininkas] = saknis + std::wstring(L"ias");  // именительный. vaisius = фрукт
        lentele_[Skaicus_Vienaskaita][Kilmininkas] = saknis + std::wstring(L"iaus");   // родительный. vaisiaus
        lentele_[Skaicus_Vienaskaita][Naudininkas] = saknis + std::wstring(L"iui");  // дательный/полезный. vaisiui
        lentele_[Skaicus_Vienaskaita][Galininkas] = saknis + std::wstring(L"ių");    // винительный/могущий. vaisių
        lentele_[Skaicus_Vienaskaita][Inagininkas] = saknis + std::wstring(L"iumi");   // творительный. vaisiumi
        lentele_[Skaicus_Vienaskaita][Vietininkas] = saknis + std::wstring(L"iuje");  // местный. vaisiuje
        lentele_[Skaicus_Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"iau");  // звательный. vaisiau

        lentele_[Skaicus_Daugiskaita][Vardininkas] = saknis + std::wstring(L"iai");  // vaisiai
        lentele_[Skaicus_Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"ių");// vaisių
        lentele_[Skaicus_Daugiskaita][Naudininkas] = saknis + std::wstring(L"iams"); // vaisiams
        lentele_[Skaicus_Daugiskaita][Galininkas] = saknis + std::wstring(L"ius");   // vaisius
        lentele_[Skaicus_Daugiskaita][Inagininkas] = saknis + std::wstring(L"iais"); // vaisiais
        lentele_[Skaicus_Daugiskaita][Vietininkas] = saknis + std::wstring(L"iuose");// vaisiuose
        lentele_[Skaicus_Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"iai");// vaisiai
        break;

    case Paradigma_5_uo:
        if (gimine_ == Gimine_viriskoji) {
            lentele_[Skaicus_Vienaskaita][Vardininkas] = saknis + std::wstring(L"uo");  // именительный. akmuo = камень
            lentele_[Skaicus_Vienaskaita][Kilmininkas] = saknis + std::wstring(L"ens");   // родительный. akmens
            lentele_[Skaicus_Vienaskaita][Naudininkas] = saknis + std::wstring(L"eniui");  // дательный/полезный.  akmeniui
            lentele_[Skaicus_Vienaskaita][Galininkas] = saknis + std::wstring(L"enį");    // винительный/могущий. akmenį
            lentele_[Skaicus_Vienaskaita][Inagininkas] = saknis + std::wstring(L"eniu");   // творительный. akmeniu
            lentele_[Skaicus_Vienaskaita][Vietininkas] = saknis + std::wstring(L"enyje");  // местный. akmenyje
            lentele_[Skaicus_Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"enie");  // звательный. akmenie

            lentele_[Skaicus_Daugiskaita][Vardininkas] = saknis + std::wstring(L"enys");  // akmenys
            lentele_[Skaicus_Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"enų");// akmenų
            lentele_[Skaicus_Daugiskaita][Naudininkas] = saknis + std::wstring(L"enims"); // akmenims
            lentele_[Skaicus_Daugiskaita][Galininkas] = saknis + std::wstring(L"enis");   // akmenis
            lentele_[Skaicus_Daugiskaita][Inagininkas] = saknis + std::wstring(L"enimis"); // akmenimis
            lentele_[Skaicus_Daugiskaita][Vietininkas] = saknis + std::wstring(L"enyse");// akmenyse
            lentele_[Skaicus_Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"enys");// akmenys
        } else if (gimine_ == Gimine_moteriskoji) {
            lentele_[Skaicus_Vienaskaita][Vardininkas] = saknis + std::wstring(L"uo");  // именительный. sesuo = сестра
            lentele_[Skaicus_Vienaskaita][Kilmininkas] = saknis + std::wstring(L"ers");   // родительный. sesers
            lentele_[Skaicus_Vienaskaita][Naudininkas] = saknis + std::wstring(L"eriai");  // дательный/полезный.  seseriai
            lentele_[Skaicus_Vienaskaita][Galininkas] = saknis + std::wstring(L"erį");    // винительный/могущий. seserį
            lentele_[Skaicus_Vienaskaita][Inagininkas] = saknis + std::wstring(L"eria");   // творительный. seseria
            lentele_[Skaicus_Vienaskaita][Vietininkas] = saknis + std::wstring(L"eryje");  // местный. seseryje
            lentele_[Skaicus_Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"erie");  // звательный. seserie

            lentele_[Skaicus_Daugiskaita][Vardininkas] = saknis + std::wstring(L"erys");  // seserys
            lentele_[Skaicus_Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"erų");// seserų
            lentele_[Skaicus_Daugiskaita][Naudininkas] = saknis + std::wstring(L"erims"); // seserims
            lentele_[Skaicus_Daugiskaita][Galininkas] = saknis + std::wstring(L"eris");   // seseris
            lentele_[Skaicus_Daugiskaita][Inagininkas] = saknis + std::wstring(L"erimis"); // seserimis
            lentele_[Skaicus_Daugiskaita][Vietininkas] = saknis + std::wstring(L"eryse");// seseryse
            lentele_[Skaicus_Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"erys");// seserys
        }
        break;

    case Paradigma_5_e:
        if (gimine_ == Gimine_viriskoji) {
            lentele_[Skaicus_Vienaskaita][Vardininkas] = saknis + std::wstring(L"ė");  // именительный. tete
            lentele_[Skaicus_Vienaskaita][Kilmininkas] = saknis + std::wstring(L"ens");   // родительный. akmens
            lentele_[Skaicus_Vienaskaita][Naudininkas] = saknis + std::wstring(L"eniui");  // дательный/полезный.  
            lentele_[Skaicus_Vienaskaita][Galininkas] = saknis + std::wstring(L"enį");    // винительный/могущий. 
            lentele_[Skaicus_Vienaskaita][Inagininkas] = saknis + std::wstring(L"eniu");   // творительный. 
            lentele_[Skaicus_Vienaskaita][Vietininkas] = saknis + std::wstring(L"enyje");  // местный. 
            lentele_[Skaicus_Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"enie");  // звательный. 

            lentele_[Skaicus_Daugiskaita][Vardininkas] = saknis + std::wstring(L"enys");  // 
            lentele_[Skaicus_Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"enų");// 
            lentele_[Skaicus_Daugiskaita][Naudininkas] = saknis + std::wstring(L"enims"); // 
            lentele_[Skaicus_Daugiskaita][Galininkas] = saknis + std::wstring(L"enis");   // 
            lentele_[Skaicus_Daugiskaita][Inagininkas] = saknis + std::wstring(L"enimis"); // 
            lentele_[Skaicus_Daugiskaita][Vietininkas] = saknis + std::wstring(L"enyse");// 
            lentele_[Skaicus_Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"enys");//
        } else if (gimine_ == Gimine_moteriskoji) {
            lentele_[Skaicus_Vienaskaita][Vardininkas] = saknis + std::wstring(L"ė");  // именительный. duktė = дочь
            lentele_[Skaicus_Vienaskaita][Kilmininkas] = saknis + std::wstring(L"ers");   // родительный. dukters
            lentele_[Skaicus_Vienaskaita][Naudininkas] = saknis + std::wstring(L"eriai");  // дательный/полезный.  dukteriai
            lentele_[Skaicus_Vienaskaita][Galininkas] = saknis + std::wstring(L"erį");    // винительный/могущий. dukterį
            lentele_[Skaicus_Vienaskaita][Inagininkas] = saknis + std::wstring(L"eria");   // творительный. dukteria
            lentele_[Skaicus_Vienaskaita][Vietininkas] = saknis + std::wstring(L"eryje");  // местный. dukteryje
            lentele_[Skaicus_Vienaskaita][Sauksmininkas] = saknis + std::wstring(L"erie");  // звательный. dukterie

            lentele_[Skaicus_Daugiskaita][Vardininkas] = saknis + std::wstring(L"erys");  // dukterys
            lentele_[Skaicus_Daugiskaita][Kilmininkas] = daugiskaita_kilmininkas + std::wstring(L"erų");// dukterų
            lentele_[Skaicus_Daugiskaita][Naudininkas] = saknis + std::wstring(L"erims"); // dukterims
            lentele_[Skaicus_Daugiskaita][Galininkas] = saknis + std::wstring(L"eris");   // dukteris
            lentele_[Skaicus_Daugiskaita][Inagininkas] = saknis + std::wstring(L"erimis"); // dukterimis
            lentele_[Skaicus_Daugiskaita][Vietininkas] = saknis + std::wstring(L"eryse");// dukteryse
            lentele_[Skaicus_Daugiskaita][Sauksmininkas] = saknis + std::wstring(L"erys");// dukterys
        }
        break;
    default:;
    }
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
