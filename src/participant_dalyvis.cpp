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

#include "participant_dalyvis.h"
#include "utils.h"

/** 
    Dalyvis - причастие, {"Type":"dalyvis"}
        - Formas:
            - not specified
            - Ivardis - Местоименная форма как у прилашательных, 
                -usis (-ysis), -ioji в активном залоге
                -asis, -oji в страдательном залоге
            - Padaiyvinis - деепричастный -ant, -int в акт. залоге наст времени: видя, делая; -(i)us - пр.вр.: увидев
            - Vienalaikas - одноврменности, переводится как деепричастие
                -dam- суффикс
        - Giminė - род, {"Gimine":"Vyriskoji", "Moteriskoji"}
            Vyriškoji
            Moteriškoji
        - Skaičius - число, {"Skacius":"Vienaskaita","Daugiskaita"}
            Vienaskaita
            Daugiskaita
        - Atvejis - падеж, {"Atvejis":"Vardninkas",...}
            Vardninkas
            ..
            Šauksmininkas
        - Laikas - Время, {"Laikas":"Esamasis","Butasis",..}
            настоящее
            прошедшее
            прошедшее многократное
            будущее
        - Rūšis - Залог, {"rusis":"Veikiamoji", "Neveikiamoji"}
            активный (идущий, прошедший, шедший, тот кто пойдет)
            страдательный (просматриваемый, просмотренный, !нет многокр формы!, тот который будет)

        - {"Sangrazynis":"-si"} - возвратный
        - {"Formas":"Vienalaikis"} - одноврменный суффикс + окончание прилаг. 1 группы

*/

DalyvisGeneric::DalyvisGeneric(AttributeType *cfg, VeiksmazodisGeneric *verb)
    : WordGeneric(cfg),
    veiksmazodis_(verb)
{
    atnaujinti();
}

DalyvisGeneric::ERusis DalyvisGeneric::str2rusis(const wchar_t *s) {
    ERusis ret = Veikiamoji;
    if (wcscmp(s, L"Veikiamoji") == 0) {
        ret = Veikiamoji;
    } else if (wcscmp(s, L"Neveikiamoji") == 0) {
        ret = Neveikiamoji;
    }
    return ret;
}

void DalyvisGeneric::atnaujinti() {
    std::wstring saknis1 = veiksmazodis_->imkSaknis(1);
    std::wstring saknis2 = veiksmazodis_->imkSaknis(2);
    std::wstring _a_i = L"";
    std::wstring _a_i_nosine = L"";
    if (veiksmazodis_->imkAsmenuote() == 1
        || veiksmazodis_->imkAsmenuote() == 3) {
        _a_i = L"a";
        _a_i_nosine = L"ą";
    } else {
        _a_i = L"i";
        _a_i_nosine = L"į";
    }

    // Активная форма, наст.вр. Vienaskaita
    // я идущий, ты идущий, он идущий
    // я идущая, ты идущая, она идущая
    lt_[Veikiamoji][Esamasis][Vienaskaita][Vardininkas][Vyriskoji] = saknis2 + _a_i_nosine + L"s"; // -antis/-intis are possible too
    lt_[Veikiamoji][Esamasis][Vienaskaita][Vardininkas][Moteriskoji] = saknis2 + _a_i + L"nti";
    lt_[Veikiamoji][Esamasis][Vienaskaita][Kilmininkas][Vyriskoji] = saknis2 + _a_i + L"nčio";
    lt_[Veikiamoji][Esamasis][Vienaskaita][Kilmininkas][Moteriskoji] = saknis2 + _a_i + L"nčios";
    lt_[Veikiamoji][Esamasis][Vienaskaita][Naudininkas][Vyriskoji] = saknis2 + _a_i + L"nčiam";
    lt_[Veikiamoji][Esamasis][Vienaskaita][Naudininkas][Moteriskoji] = saknis2 + _a_i + L"nčiai";
    lt_[Veikiamoji][Esamasis][Vienaskaita][Galininkas][Vyriskoji] = saknis2 + _a_i + L"ntį";
    lt_[Veikiamoji][Esamasis][Vienaskaita][Galininkas][Moteriskoji] = saknis2 + _a_i + L"nčią";
    lt_[Veikiamoji][Esamasis][Vienaskaita][Inagininkas][Vyriskoji] = saknis2 + _a_i + L"nčiu";
    lt_[Veikiamoji][Esamasis][Vienaskaita][Inagininkas][Moteriskoji] = saknis2 + _a_i + L"nčia";
    lt_[Veikiamoji][Esamasis][Vienaskaita][Vietininkas][Vyriskoji] = saknis2 + _a_i + L"nčiame";
    lt_[Veikiamoji][Esamasis][Vienaskaita][Vietininkas][Moteriskoji] = saknis2 + _a_i + L"nčioje";
    // same as vardininkas (taip pat kaip vardininkas)
    lt_[Veikiamoji][Esamasis][Vienaskaita][Sauksmininkas][Vyriskoji] = saknis2 + _a_i_nosine + L"s"; // -antis/-intis are possible too
    lt_[Veikiamoji][Esamasis][Vienaskaita][Sauksmininkas][Moteriskoji] = saknis2 + _a_i + L"nti";
    // Daugiskaita
    // мы идущие, вы идущие, они идущие (м.р.)
    // мы идущие, вы идущие, они идущие (ж.р.)
    lt_[Veikiamoji][Esamasis][Daugiskaita][Vardininkas][Vyriskoji] = saknis2 + _a_i_nosine;       // -antys/-intys are possible too
    lt_[Veikiamoji][Esamasis][Daugiskaita][Vardininkas][Moteriskoji] = saknis2 + _a_i + L"nčios";
    lt_[Veikiamoji][Esamasis][Daugiskaita][Kilmininkas][Vyriskoji] = saknis2 + _a_i + L"nčių";
    lt_[Veikiamoji][Esamasis][Daugiskaita][Kilmininkas][Moteriskoji] = saknis2 + _a_i + L"nčių";
    lt_[Veikiamoji][Esamasis][Daugiskaita][Naudininkas][Vyriskoji] = saknis2 + _a_i + L"ntiems";
    lt_[Veikiamoji][Esamasis][Daugiskaita][Naudininkas][Moteriskoji] = saknis2 + _a_i + L"nčioms";
    lt_[Veikiamoji][Esamasis][Daugiskaita][Galininkas][Vyriskoji] = saknis2 + _a_i + L"nčius";
    lt_[Veikiamoji][Esamasis][Daugiskaita][Galininkas][Moteriskoji] = saknis2 + _a_i + L"nčias";
    lt_[Veikiamoji][Esamasis][Daugiskaita][Inagininkas][Vyriskoji] = saknis2 + _a_i + L"nčiais";
    lt_[Veikiamoji][Esamasis][Daugiskaita][Inagininkas][Moteriskoji] = saknis2 + _a_i + L"nčiomis";
    lt_[Veikiamoji][Esamasis][Daugiskaita][Vietininkas][Vyriskoji] = saknis2 + _a_i + L"nčiuose";
    lt_[Veikiamoji][Esamasis][Daugiskaita][Vietininkas][Moteriskoji] = saknis2 + _a_i + L"nčiose";
    // same as vardininkas (taip pat kaip vardininkas)
    lt_[Veikiamoji][Esamasis][Daugiskaita][Sauksmininkas][Vyriskoji] = saknis2 + _a_i_nosine;       // -antys/-intys are possible too
    lt_[Veikiamoji][Esamasis][Daugiskaita][Sauksmininkas][Moteriskoji] = saknis2 + _a_i + L"nčios";
    // Герундная форма: увидя
    lt_padaiyvinis_[Esamasis] = saknis2 + _a_i + L"nt";



    // Пассивная форма, наст. вр., Vienaskaita (-m + окончание прилагательных первой группы)
    // я видимый, ты видимый, он видимый
    // я видимая, ты видимая, она видимая
    std::wstring zodis2 = veiksmazodis_->imkZodis(2);
    lt_[Neveikiamoji][Esamasis][Vienaskaita][Vardininkas][Vyriskoji] = zodis2 + L"m" + L"as";
    lt_[Neveikiamoji][Esamasis][Vienaskaita][Vardininkas][Moteriskoji] = zodis2 + L"m" + L"a";
    lt_[Neveikiamoji][Esamasis][Vienaskaita][Kilmininkas][Vyriskoji] = zodis2 + L"m" + L"o";
    lt_[Neveikiamoji][Esamasis][Vienaskaita][Kilmininkas][Moteriskoji] = zodis2 + L"m" + L"os";
    lt_[Neveikiamoji][Esamasis][Vienaskaita][Naudininkas][Vyriskoji] = zodis2 + L"m" + L"am";
    lt_[Neveikiamoji][Esamasis][Vienaskaita][Naudininkas][Moteriskoji] = zodis2 + L"m" + L"ai";
    lt_[Neveikiamoji][Esamasis][Vienaskaita][Galininkas][Vyriskoji] = zodis2 + L"m" + L"ą";
    lt_[Neveikiamoji][Esamasis][Vienaskaita][Galininkas][Moteriskoji] = zodis2 + L"m" + L"ą";
    lt_[Neveikiamoji][Esamasis][Vienaskaita][Inagininkas][Vyriskoji] = zodis2 + L"m" + L"u";
    lt_[Neveikiamoji][Esamasis][Vienaskaita][Inagininkas][Moteriskoji] = zodis2 + L"m" + L"a";
    lt_[Neveikiamoji][Esamasis][Vienaskaita][Vietininkas][Vyriskoji] = zodis2 + L"m" + L"ame";
    lt_[Neveikiamoji][Esamasis][Vienaskaita][Vietininkas][Moteriskoji] = zodis2 + L"m" + L"oje";
    // same as vardininkas (taip pat kaip vardininkas)
    lt_[Neveikiamoji][Esamasis][Vienaskaita][Sauksmininkas][Vyriskoji] = zodis2 + L"m" + L"as";
    lt_[Neveikiamoji][Esamasis][Vienaskaita][Sauksmininkas][Moteriskoji] = zodis2 + L"m" + L"a";
    // Daugiskaita
    // мы видимые, вы видимые, они видимые(м.р.)
    // мы видимые, вы видимые, они видимые (ж.р.)
    lt_[Neveikiamoji][Esamasis][Daugiskaita][Vardininkas][Vyriskoji] = zodis2 + L"m" + L"i";
    lt_[Neveikiamoji][Esamasis][Daugiskaita][Vardininkas][Moteriskoji] = zodis2 + L"m" + L"os";
    lt_[Neveikiamoji][Esamasis][Daugiskaita][Kilmininkas][Vyriskoji] = zodis2 + L"m" + L"ų";
    lt_[Neveikiamoji][Esamasis][Daugiskaita][Kilmininkas][Moteriskoji] = zodis2 + L"m" + L"ų";
    lt_[Neveikiamoji][Esamasis][Daugiskaita][Naudininkas][Vyriskoji] = zodis2 + L"m" + L"iems";
    lt_[Neveikiamoji][Esamasis][Daugiskaita][Naudininkas][Moteriskoji] = zodis2 + L"m" + L"oms";
    lt_[Neveikiamoji][Esamasis][Daugiskaita][Galininkas][Vyriskoji] = zodis2 + L"m" + L"us";
    lt_[Neveikiamoji][Esamasis][Daugiskaita][Galininkas][Moteriskoji] = zodis2 + L"m" + L"as";
    lt_[Neveikiamoji][Esamasis][Daugiskaita][Inagininkas][Vyriskoji] = zodis2 + L"m" + L"ais";
    lt_[Neveikiamoji][Esamasis][Daugiskaita][Inagininkas][Moteriskoji] = zodis2 + L"m" + L"omis";
    lt_[Neveikiamoji][Esamasis][Daugiskaita][Vietininkas][Vyriskoji] = zodis2 + L"m" + L"uose";
    lt_[Neveikiamoji][Esamasis][Daugiskaita][Vietininkas][Moteriskoji] = zodis2 + L"m" + L"ose";
    // same as vardininkas (taip pat kaip vardininkas)
    lt_[Neveikiamoji][Esamasis][Daugiskaita][Sauksmininkas][Vyriskoji] = zodis2 + L"m" + L"i";
    lt_[Neveikiamoji][Esamasis][Daugiskaita][Sauksmininkas][Moteriskoji] = zodis2 + L"m" + L"os";
    // ---
    // Местоименная форма:
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Vienaskaita][Vardininkas][Vyriskoji] = zodis2 + L"m" + L"asis";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Vienaskaita][Vardininkas][Moteriskoji] = zodis2 + L"m" + L"oji";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Vienaskaita][Kilmininkas][Vyriskoji] = zodis2 + L"m" + L"ojo";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Vienaskaita][Kilmininkas][Moteriskoji] = zodis2 + L"m" + L"osios";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Vienaskaita][Naudininkas][Vyriskoji] = zodis2 + L"m" + L"ajam";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Vienaskaita][Naudininkas][Moteriskoji] = zodis2 + L"m" + L"ajai";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Vienaskaita][Galininkas][Vyriskoji] = zodis2 + L"m" + L"ąji";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Vienaskaita][Galininkas][Moteriskoji] = zodis2 + L"m" + L"ąja";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Vienaskaita][Inagininkas][Vyriskoji] = zodis2 + L"m" + L"uoju";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Vienaskaita][Inagininkas][Moteriskoji] = zodis2 + L"m" + L"ąja";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Vienaskaita][Vietininkas][Vyriskoji] = zodis2 + L"m" + L"ajame";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Vienaskaita][Vietininkas][Moteriskoji] = zodis2 + L"m" + L"ojoje";
    // same as vardininkas (taip pat kaip vardininkas)
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Vienaskaita][Sauksmininkas][Vyriskoji] = zodis2 + L"m" + L"asis";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Vienaskaita][Sauksmininkas][Moteriskoji] = zodis2 + L"m" + L"oji";
    // Daugiskaita
    // мы видимые, вы видимые, они видимые(м.р.)
    // мы видимые, вы видимые, они видимые (ж.р.)
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Daugiskaita][Vardininkas][Vyriskoji] = zodis2 + L"m" + L"ieji";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Daugiskaita][Vardininkas][Moteriskoji] = zodis2 + L"m" + L"osios";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Daugiskaita][Kilmininkas][Vyriskoji] = zodis2 + L"m" + L"ųjų";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Daugiskaita][Kilmininkas][Moteriskoji] = zodis2 + L"m" + L"ųjų";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Daugiskaita][Naudininkas][Vyriskoji] = zodis2 + L"m" + L"iesiems";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Daugiskaita][Naudininkas][Moteriskoji] = zodis2 + L"m" + L"osioms";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Daugiskaita][Galininkas][Vyriskoji] = zodis2 + L"m" + L"uosius";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Daugiskaita][Galininkas][Moteriskoji] = zodis2 + L"m" + L"ąsias";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Daugiskaita][Inagininkas][Vyriskoji] = zodis2 + L"m" + L"aisiais";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Daugiskaita][Inagininkas][Moteriskoji] = zodis2 + L"m" + L"osiomis";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Daugiskaita][Vietininkas][Vyriskoji] = zodis2 + L"m" + L"uosiuose";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Daugiskaita][Vietininkas][Moteriskoji] = zodis2 + L"m" + L"osiose";
    // same as vardininkas (taip pat kaip vardininkas)
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Daugiskaita][Sauksmininkas][Vyriskoji] = zodis2 + L"m" + L"ieji";
    lt_ivardziuotinis[Neveikiamoji][Esamasis][Daugiskaita][Sauksmininkas][Moteriskoji] = zodis2 + L"m" + L"osios";
}

void DalyvisGeneric::info() {
    wchar_t tstr[1024];
    int wsz = static_cast<int>(value_.size()) + 8; // ending + spaces (pabaiga + įtrauka)
    int tcnt = 0;
    int aligncnt = wsz;
    if (aligncnt < 20) {
        aligncnt = 20;
    }
    printf_log(L"\nDalyvis: %s\n", value_.c_str());

    printf_log(L"  Tiesioginė nuosaka, Veikiamoji rūšis, Esamasis laikas (%s):\n",
               L"-щий, Активный залог, н.вр.");
    // 19 = aligncnt - 1
    printf_log(L"%30s %19s %19s %19s\n",
        L"ед.ч.,м.р.", L"ед.ч.,ж.р.", L"мн.ч.,м.р.", L"мн.ч.,ж.р.");

    tcnt = add2wline(tstr, 0, L"    Vardininkas: ", 20);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Vardininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Vardininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Vardininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Vardininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    Kilmininkas: ", 20);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Kilmininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Kilmininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Kilmininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Kilmininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    Naudininkas: ", 20);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Naudininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Naudininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Naudininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Naudininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    Galininkas: ", 20);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Galininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Galininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Galininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Galininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    Inagininkas: ", 20);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Inagininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Inagininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Inagininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Inagininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    Vietininkas: ", 20);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Vietininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Vietininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Vietininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Vietininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    Sauksmininkas: ", 20);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Sauksmininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Sauksmininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Sauksmininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Sauksmininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);


    printf_log(L"  Tiesioginė nuosaka, Neveikiamoji rūšis, Esamasis laikas (%s):\n",
               L"-мый, Пассивный залог, н.вр.");
    // 19 = aligncnt - 1
    printf_log(L"%30s %19s %19s %19s\n",
        L"ед.ч.,м.р.", L"ед.ч.,ж.р.", L"мн.ч.,м.р.", L"мн.ч.,ж.р.");

    tcnt = add2wline(tstr, 0, L"    Vardininkas: ", 20);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Vienaskaita][Vardininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Vienaskaita][Vardininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Daugiskaita][Vardininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Daugiskaita][Vardininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    Kilmininkas: ", 20);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Vienaskaita][Kilmininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Vienaskaita][Kilmininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Daugiskaita][Kilmininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Daugiskaita][Kilmininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    Naudininkas: ", 20);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Vienaskaita][Naudininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Vienaskaita][Naudininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Daugiskaita][Naudininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Daugiskaita][Naudininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    Galininkas: ", 20);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Vienaskaita][Galininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Vienaskaita][Galininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Daugiskaita][Galininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Daugiskaita][Galininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    Inagininkas: ", 20);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Vienaskaita][Inagininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Vienaskaita][Inagininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Daugiskaita][Inagininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Daugiskaita][Inagininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    Vietininkas: ", 20);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Vienaskaita][Vietininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Vienaskaita][Vietininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Daugiskaita][Vietininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Daugiskaita][Vietininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"    Sauksmininkas: ", 20);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Vienaskaita][Sauksmininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Vienaskaita][Sauksmininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Daugiskaita][Sauksmininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Neveikiamoji][Esamasis][Daugiskaita][Sauksmininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);

}

std::wstring DalyvisGeneric::gautiForma(AttributeType &arg) {
    std::wstring ret = L"";
    if (!arg.has_key(L"Formas")) {
        // forma is not specified
        ERusis rusis = str2rusis(arg[L"Rusis"].to_string());
        ELaikas laikas = str2laikas(arg[L"Laikas"].to_string());
        ESkaicus skaicus = str2skaicus(arg[L"Skaicius"].to_string());
        EAtvejis atvejis = str2atvejis(arg[L"Atvejis"].to_string());
        EGimine gimine = str2gimine(arg[L"Gimine"].to_string());
        ret = lt_[rusis][laikas][skaicus][atvejis][gimine];
    } else if (arg[L"Formas"].is_equal(L"Ivardis")) {
        ERusis rusis = str2rusis(arg[L"Rusis"].to_string());
        ELaikas laikas = str2laikas(arg[L"Laikas"].to_string());
        ESkaicus skaicus = str2skaicus(arg[L"Skaicius"].to_string());
        EAtvejis atvejis = str2atvejis(arg[L"Atvejis"].to_string());
        EGimine gimine = str2gimine(arg[L"Gimine"].to_string());
        ret = lt_ivardziuotinis[rusis][laikas][skaicus][atvejis][gimine];
    } else if (arg[L"Formas"].is_equal(L"Padaiyvinis")) {
        ELaikas laikas = str2laikas(arg[L"Laikas"].to_string());
        ret = lt_padaiyvinis_[laikas];
    }

    if (arg.has_key(L"Sangrazinis")) {
        // dalyti [dalija, dalijo] - делить
        // dalytis [dalijasi, dalijosi] - делиться
        if (arg[L"Sangrazinis"].is_equal(L"-is")) {
            ret += L"is";
        }
    }

    return ret;
}
