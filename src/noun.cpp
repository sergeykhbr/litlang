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

/** 
    Dayktavardis - существительное, {"Type":"daiktavardis"}
        Giminė - род, {"Gimine":"V" or "M"}
            Vyriškoji (V) - мужской
            Moteriškoji (M) - женский
        Skaičius - число, {"Skaicius":"V" or "D'}
            Vienaskaita (V) - единственное
            Daugiskaita (D) - множественное
        Linksnis - падеж, {"Linksnis":"V" or ..}
            Vardininkas, kas? (V) именительный, кто, что?
            Kilmininkas, ko? (K)  родительный, кого, чего?
            Naudininkas, kam? (N) дательный, кому, чему?
            Galininkas, ką? (G) винительный, кого, что?
            Inagininkas, kuo? (In) инструментальный (творительный), кем, чем?
            Vietininkas, kur? kame? (Vi) местный, где? в ком? в чем?
            Šauksmininkas, (S) звательный
        Linksninuotė - склонение, {"Linksninuote":1 or ..}
            1 .. 5 в зависимости от окончания
            

*/

DaiktavardisGeneric::DaiktavardisGeneric(AttributeType *cfg) : WordGeneric(cfg) {
}

/*void DaiktavardisGeneric::info() {
    int sz = static_cast<int>(value_.size());
    const wchar_t *p = value_.c_str();
    wchar_t ending[8];
    int tcnt = 0;
    if ((p[sz-2] == 'i' && p[sz-1] == 's')
        || (p[sz-2] == 'y' && p[sz-1] == 's')
        || (p[sz-2] == 'a' && p[sz-1] == 's')) {
        if (p[sz-3] == 'i' && p[sz-2] == 'a') {
            // -(i)as
            ending[tcnt++] = p[sz-3];
        }

        printf("Daiktaridziu 1 ?linksniuote?");
    }
}*/