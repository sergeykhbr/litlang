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
#include "verb_veiksmazodis.h"

// причастие, dalyvis
class DalyvisGeneric : public WordGeneric {
 public:
    DalyvisGeneric(AttributeType *cfg, VeiksmazodisGeneric *verb);

    virtual void info() override;
    virtual std::wstring gautiForma(AttributeType &arg) override;

 protected:
    // rūšis = залог
    typedef enum ERusis {
        Veikiamoji,         // акт. залог (veikiamoji rūšis) (идущий)
        Neveikiamoji,       // пас. залог (neveikiamoji rūšis), (просмотренный)
        Rusis_total
    } ERusis;

    // 
    typedef enum EFormas {
        Liepiamoji,         // причастие долженствования
        Vienalaikiskumas,   // одноврменно -dam-
        Formas_total
    } EFormas;

 protected:
    // обновить (заполнить таблицу склонений)
    void atnaujinti();
    ERusis str2rusis(const wchar_t *s);

 protected:
    VeiksmazodisGeneric *veiksmazodis_;
    std::wstring lt_[Rusis_total][Laikas_total][Skaicus_Total][Atvejis_Total][Gimine_Total];
    std::wstring lt_ivardziuotinis[Rusis_total][Laikas_total][Skaicus_Total][Atvejis_Total][Gimine_Total];  // ĮvardžiUotinis: местоименная форма
    std::wstring lt_padaiyvinis_[Laikas_total];                 // деепричастная (герундная) форма
};

