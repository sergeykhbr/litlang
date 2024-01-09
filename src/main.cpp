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

#include "api_wrd.h"
#include <iostream>
#include <string.h>
#include <utils.h>
#include <io.h>
#include <fcntl.h>

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
std::wstring sakinys1 = 
L"["
L"[\"tai\", \"ivardis\"],"
L"[\"ne\", \"dalelyte\"],"
L"[\"vienpusis\", \"budvardis\", \"Vyriskoji\", \"Vienaskaita\", \"Vardininkas\"]," // односторонний
L"[\"dalytis\", \"veikmazodis\", \"Vyriskoji\", \"Vienaskaita\", \"Vardininkas\"],"  // делится,  страдательный залог местоименного причастия -asis
L"[\"patirtis\", \"daiktavardis\", \"Vyriskoji\", \"Vienaskaita\", \"Inagininkas\"]," // опытом
L"[\"ar\", \"dalelyte\"]," // или
L"[\"patarimas\", \"daiktavardis\", \"Vyriskoji\", \"Vienaskaita\", \"Vardininkas\"]," // совет
L"[\"a\", \"dalelyte\"]," // а
L"[\"mainai\", \"daiktavardis\"]," // обмен
L"]";

int main(int argc, const char *argv[]) {
    AttributeType databuf;
    AttributeType config;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-z") == 0) {
            utils_read_json_file(argv[++i], &databuf);
        }
    }
    config.from_config(databuf.to_string());

    AttributeType &zodis = config[L"Items"];
    for (unsigned i = 0; i < zodis.size(); i++) {
        WRD_pridelioti_zodis(&zodis[i]);
    }

    _setmode(_fileno(stdout), _O_U8TEXT);

    //WRD_info();
    AttributeType example1;
    example1.from_config(sakinys1.c_str());
    if (example1.is_list()) {
        for (unsigned i = 0; i < example1.size(); i++) {
            AttributeType &word = example1[i];
            if (!word.is_list()) {
                continue;
            }
            WordGeneric *p = WRD_gauti_zodis(word[0u].to_string());
            if (p) {
                p->info();
            } else {
                printf_error(L"Zodis nerastas '%s'\n", word[0u].to_string());
            }
        }
    }

    WRD_gauti_zodis(L"pastaba")->info();

    return 0;
}