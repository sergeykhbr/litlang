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
        Лицо {"Kas":"jis"}
            1 ед.ч aš
            2 ед.ч tu
            3 ед.ч jis, ji
            1 мн.ч mes
            2 мн.ч jūs
            3 мн.ч jie, jos

*/

VeiksmazodisGeneric::VeiksmazodisGeneric(AttributeType *cfg)
    : WordGeneric(cfg),
    formas_(Tiesiogine_Esamasis_laikas)
{
}
