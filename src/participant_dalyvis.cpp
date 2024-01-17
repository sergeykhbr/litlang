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
        Giminė
            Vyriškoji
            Moteriškoji
        Skaičius
            Vienaskaita
            Daugiskaita
        Atvejis - падеж
        - Время
            настоящее
            прошедшее
            прошедшее многократное
            будущее
        - Залог
            активный (идущий, прошедший, шедший, тот кто пойдет)
            страдательный (просматриваемый, просмотренный, !нет многокр формы!, тот который будет)

        - Местоименная форма как у прилашательных
            -usis (-ysis), -ioji в активном залоге
            -asis, -oji в страдательном залоге

        - {"Sangrazynis":"-si"} - возвратный
        - {"Formas":"Vienalaikis"} - одноврменный суффикс + окончание прилаг. 1 группы

*/

DalyvisGeneric::DalyvisGeneric(AttributeType *cfg, VeiksmazodisGeneric *verb)
    : WordGeneric(cfg),
    veiksmazodis_(verb)
{
    atnaujinti();
}

void DalyvisGeneric::atnaujinti() {
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

    // Vienaskaita
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


    // Одновременности. -dam- + окончание прилагательных первой группы Им.падежа.
    // Употребляется только в иментильном падеже как деепричастие: делая, идя, спя...
    std::wstring saknis1 = veiksmazodis_->imkSaknis(1);
    lt_vienalaikis_[Vienaskaita][Vyriskoji] = saknis1 + L"dam" + L"as";
    lt_vienalaikis_[Vienaskaita][Moteriskoji] = saknis1 + L"dam" + L"a";
    lt_vienalaikis_[Daugiskaita][Vyriskoji] = saknis1 + L"dam" + L"i";
    lt_vienalaikis_[Daugiskaita][Moteriskoji] = saknis1 + L"dam" + L"os";
}

void DalyvisGeneric::info() {
    wchar_t tstr[1024];
    int wsz = static_cast<int>(value_.size()) + 8; // ending + spaces (pabaiga + įtrauka)
    int tcnt = 0;
    int aligncnt = wsz;
    if (aligncnt < 20) {
        aligncnt = 20;
    }
    printf_log(L"\nDalyvis: %s, (%s)\n",
            value_.c_str(),
            L"-щий");

    printf_log(L"Tiesioginė nuosaka, Esamasis laikas (%s):\n",
               L"Активный залог, н.вр.");
    // 19 = aligncnt - 1
    printf_log(L"%26s %19s %19s %19s\n",
        L"ед.ч.,м.р.", L"ед.ч.,ж.р.", L"мн.ч.,м.р.", L"мн.ч.,ж.р.");

    tcnt = add2wline(tstr, 0, L"Vardininkas: ", 16);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Vardininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Vardininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Vardininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Vardininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"Kilmininkas: ", 16);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Kilmininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Kilmininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Kilmininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Kilmininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"Naudininkas: ", 16);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Naudininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Naudininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Naudininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Naudininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"Galininkas: ", 16);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Galininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Galininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Galininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Galininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"Inagininkas: ", 16);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Inagininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Inagininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Inagininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Inagininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"Vietininkas: ", 16);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Vietininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Vietininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Vietininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Vietininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);

    tcnt = add2wline(tstr, 0, L"Sauksmininkas: ", 16);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Sauksmininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Vienaskaita][Sauksmininkas][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Sauksmininkas][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_[Veikiamoji][Esamasis][Daugiskaita][Sauksmininkas][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);


    printf_log(L"Vienalaikis formas, (%s):\n", L"форма одновременности, только в Им.п.");

    tcnt = add2wline(tstr, 0, L"Vardininkas: ", 16);
    tcnt = add2wline(tstr, tcnt, lt_vienalaikis_[Vienaskaita][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_vienalaikis_[Vienaskaita][Moteriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_vienalaikis_[Daugiskaita][Vyriskoji].c_str(), aligncnt);
    tcnt = add2wline(tstr, tcnt, lt_vienalaikis_[Daugiskaita][Moteriskoji].c_str(), aligncnt);
    printf_log(L"%s\n", tstr);
}

std::wstring DalyvisGeneric::gautiForma(AttributeType &arg) {
    std::wstring ret = L"";
    ESkaicus skaicus = str2skaicus(arg[L"Skaicius"].to_string());
    EGimine gimine = str2gimine(arg[L"Gimine"].to_string());
    if (!arg.has_key(L"Formas")) {
        ELaikas laikas = str2laikas(arg[L"Laikas"].to_string());
        EAsmuo asmuo = str2asmuo(arg[L"Asmuo"].to_string());
        //ret = lt_[laikas][asmuo];
    } else if (arg[L"Formas"].is_equal(L"Vienalaikis")) {
        ret = lt_vienalaikis_[skaicus][gimine];
    }

    return ret;
}
