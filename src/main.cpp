﻿// 
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

#include "api_wrd.h"
#include <iostream>
#include <string.h>
#include <utils.h>
#include <io.h>
#include <fcntl.h>

#include <iostream>
#include <sstream>

std::wstring l1 = 
L"Mentorystė. Tai ne vienpusis dalinimasis patirtimi ar patarimais, o Mainai. "
L"Mainai kur dalindamasis gauni daug įžvalgų apie save, daug pozityvios energijos "
L"matant, kaip dėmesingas kito žmogaus klausymasis ar gebėjimas paklausti jo "
L"kryptingų klausimų padeda augti abiems. Tą tvirtinu iš savo patyrimo, nes jau 9 "
L"metai esu LT Big Brother mentorė ir kviečiu jus jungtis prie šios programos. "
L"Ir žinoma - tai puiki vieta Networkinti - sutikti labai įdomių žmonių, "
L"lietuvių profesionalų iš viso pasaulio!"
L"Bravo LT Big Brother komandai ir vadovui Vytautas Jankauskas, PhD už įkvepiantį "
L"mentorystės forumą!";

// https://www.lituanistica.ru/materials_grammar.html#5

/* https://lingvoforum.net/index.php?topic=14603.0
8. Другие причастные формы

Причастие одновременности образуется от основы инфинитива с помощью суффикса "-dam-" и окончаний имен прилагательных именительного падежа первой группы: miegoti -> miegodamas, miegodama, miegodami, miegodamos. Употребляется только в именительном падеже как русское деепричастие:

jis miegodamas nieko negridėjo – он, спя, ничего не слышал



Причастие долженствования образуется присоединением суффикса -n- и окончаний прилагательного первой группы к инфинитиву: matyti -> matytinas; norėti -> norėtinas

Обозначает оно действие, которое стоит или нужно выполнить: tas dalykas yra minėtinas – эту вещь стоит упомянуть.
*/

std::wstring lesson2text(AttributeType &lesson) {
    std::wstring ret = L"";
    std::wstring generated;
    std::wstring original;
    AttributeType &Zodiai = lesson[L"Zodiai"];
    AttributeType &Original = lesson[L"Original"];
    WordGeneric *p;

    if (!Original.is_list()) {
        return ret;
    }

    for (unsigned i = 0; i < Original.size(); i++) {
        AttributeType &orig_fragment = Original[i];
        ret += L"Original:  " + std::wstring(orig_fragment.to_string()) + L"\n";
        if (i >= Zodiai.size()) {
            continue;
        }
        AttributeType &gen_fragment = Zodiai[i];
        if (!gen_fragment.is_list()) {
            continue;
        }
        ret += L"Generated: ";
        for (unsigned n = 0; n < gen_fragment.size(); n++) {
            AttributeType &zodis = gen_fragment[n];
            p = WRD_gauti_zodis(zodis[0u].to_string(), zodis[1].to_string());

            if (p) {
                if (n && !zodis[1].is_equal(L"Skirtukas")) {
                    ret += L" ";
                }
                ret += p->gautiForma(zodis[2]);
                
            } else {
                printf_error(L"Zodis nerastas '%s'\n", zodis[0u].to_string());
            }
        }
        if (!gen_fragment[gen_fragment.size() - 1][1].is_equal(L"Skirtukas")) {
            ret += L".";
        }
        ret += L"\n\n";
    }
    return ret;
}

std::wstring to_wlower_lt(std::wstring str) {
    std::wstring ret = L"";
    for (std::wstring::iterator it = str.begin(); it != str.end(); it++) {
        if (*it >= L'A' && *it <= L'Z') {
            ret += *it - L'A' + L'a';
        } else if (*it == L'Ą' || *it == L'ą') {
            ret += L'a';
        } else if (*it == L'Č' || *it == L'č') {
            ret += L'c';
        } else if (*it == L'Ę' || *it == L'ę') {
            ret += L'e';
        } else if (*it == L'Ė' || *it == L'ė') {
            ret += L'e';
        } else if (*it == L'Į' || *it == L'į') {
            ret += L'i';
        } else if (*it == L'Š' || *it == L'š') {
            ret += L's';
        } else if (*it == L'Ų' || *it == L'ų') {
            ret += L'u';
        } else if (*it == L'Ū' || *it == L'ū') {
            ret += L'u';
        } else if (*it == L'Ž' || *it == L'ž') {
            ret += L'z';
        } else {
            ret += *it;
        }
    }
    return ret;
}

void ask_words(AttributeType &zodiai, const wchar_t *type) {
    AttributeType &words = zodiai[type];
    AttributeType trial(Attr_Dict);
    std::wstring lt, ref;

    int i = 0;
    while (trial.size() != words.size()) {
        AttributeType &w = words[i];

        ref = to_wlower_lt(w[L"Value"].to_string());
        if (trial.has_key(ref.c_str())) {
            i = (i + 1) % words.size();
            continue;
        }

        // Ask question:
        wprintf(L"%s: ", w[L"Ru"].to_string());
        // get response
        std::getline(std::wcin, lt);
        lt = to_wlower_lt(lt);

        if (lt == ref) {
            trial[lt.c_str()].make_int64(0);
            wprintf(L"+ %s, %d to go\n",
                    w[L"Value"].to_string(),
                    words.size() - trial.size());
        } else {
            wprintf(L"- %s\n", w[L"Value"].to_string());
        }

        i = (i + 1) % words.size();
    }
}

int main(int argc, const char *argv[]) {
    AttributeType databuf;
    AttributeType zodiai;
    AttributeType lesson;

    _setmode(_fileno(stdout), _O_U8TEXT);

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-z") == 0) {
            utils_read_json_file(argv[++i], &databuf);
            zodiai.from_config(databuf.to_string());
            WRD_pridelioti_zodis(&zodiai);
        } else if (strcmp(argv[i], "-t") == 0) {
            utils_read_json_file(argv[++i], &databuf);
            lesson.from_config(databuf.to_string());
        }
    }


    if (lesson.is_dict()) {
        wprintf(L"%s.\n", lesson2text(lesson).c_str());
    }

    ask_words(zodiai, L"Veiksmazodis");

    //WRD_gauti_zodis(L"dirbti", L"Dalyvis")->info();

    return 0;
}