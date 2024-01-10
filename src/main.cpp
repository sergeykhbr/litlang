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
L"['tai', 'ivardis', {}],"
L"['ne', 'dalelyte', {}],"
L"['vienpusis', 'budvardis', {'Gimine':'Vyriskoji','Skaicius':'Vienaskaita','Atvejis':'Vardininkas'}]," // односторонний
// (призыв) деление собственным опытом.
// Взвратные имена существительные мужского рода (практически не употребляются) -as -> -asis; -e -> -esi 
L"['dalijimas', 'daiktavardis', {'Gimine':'Vyriskoji', 'Skaicius':'Vienaskaita', 'Atvejis':'Sauksmininkas', 'Sangrazynis':'-si'}],"
L"['patirtis', 'daiktavardis', {'Gimine':'Vyriskoji', 'Skaicius':'Vienaskaita', 'Atvejis':'Inagininkas'}]," // опытом
L"['ar', 'dalelyte', {}]," // или
L"['patarimas', 'daiktavardis', {'Gimine':'Vyriskoji', 'Skaicius':'Daugiskaita', 'Atvejis':'Inagininkas'}]," // совет
L"['o', 'dalelyte', {}]," // а
L"['mainas', 'daiktavardis', {'Gimine':'Vyriskoji', 'Skaicius':'Daugiskaita', 'Atvejis':'Vardininkas'}]," // обмен
L"]";

int main(int argc, const char *argv[]) {
    AttributeType databuf;
    AttributeType zodiai;
    AttributeType lesson;
    WordGeneric *p;

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


    //WRD_info();
    AttributeType example1;
    example1.from_config(sakinys1.c_str());
    std::wstring wout = L"";
    if (example1.is_list()) {
        for (unsigned i = 0; i < example1.size(); i++) {
            AttributeType &word = example1[i];
            if (!word.is_list()) {
                continue;
            }
            p = WRD_gauti_zodis(word[0u].to_string());
            if (p) {
                if (wout.size()) {
                    wout += L" ";
                }
                wout += p->gautiForma(word[2]);
                
            } else {
                printf_error(L"Zodis nerastas '%s'\n", word[0u].to_string());
            }
        }
    }

    //WRD_gauti_zodis(L"pastaba")->info();
    wprintf(L"%s.\n", wout.c_str());

    return 0;
}