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
    virtual std::wstring imkSaknis(int idx) override;
    virtual std::wstring imkZodis(int idx) override;
    virtual std::wstring gautiForma(AttributeType &arg) override;
    virtual int imkAsmenuote() { return asmenuote_; }

 protected:
    // обновить (заполнить таблицу склонений)
    void atnaujinti();
    bool yraSangrazinis();                          // -tis - только возвратная форма

 protected:

    int asmenuote_; // 1, 2 arba 3
    std::wstring lt_[Laikas_total][Asmuo_total];    // Tiesiogine nuosaka - изъявительное наклонение (tiesiog - просто)
    std::wstring lt_liepiamoji_[Asmuo_total];       // повелительное наклонение
    std::wstring lt_tariamoji_[Asmuo_total];        // условное наклонение
    std::wstring lt_si_[Laikas_total][Asmuo_total]; // Tiesiogine nuosaka - изъявительное наклонение, возвратная форма
    std::wstring ru_;   // infinitive
};

