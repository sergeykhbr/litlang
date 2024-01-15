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

#pragma once

#include "wordgen.h"

// verb, глагол
class VeiksmazodisGeneric : public WordGeneric {
 public:
    VeiksmazodisGeneric(AttributeType *cfg);

    virtual void info() override;
    virtual std::wstring gautiForma(AttributeType &arg) override;

 protected:
    // обновить (заполнить таблицу склонений)
    void atnaujinti();

 protected:
    typedef enum EFormas {
        Tiesiogine_Esamasis_laikas,
        Tiesiogine_Butasis_laikas,
        Tiesiogine_ButasisDazninis_laikas,
        Tiesiogine_Busimasis_laikas,
        Liepiamoji_nuosaka,
        Tariamoji_nuosaka,
        Formas_total
    } EFormas;

    // asmuo - лицо
    typedef enum EAsmuo {
        as,
        tu,
        jis_ji,
        mes,
        jus,
        jie_jos,
        Asmuo_total
    } EAsmuo;

    int asmenuote_; // 1, 2 arba 3
    std::wstring lt_[Formas_total][Asmuo_total];
    std::wstring ru_;   // infinitive
};

