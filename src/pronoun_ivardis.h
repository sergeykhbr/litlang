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

// местоимение, įvardis
class IvardisGeneric : public WordGeneric {
 public:
    IvardisGeneric(AttributeType *cfg);

    virtual void info() override;
    virtual std::wstring gautiForma(AttributeType &arg) override;

 protected:
    // определить парадигму
    void nustatyti_paradigma(const wchar_t *linksniuote);

 private:
    typedef enum ELinksniuote {
        Paradigma_nezinoma,
        Paradigma_V_tas,        // tas, šitas, anas, kas..
        Paradigma_V_kitas,      // kitas, visas, tam tikras...
        Paradigma_V_toks,       // toks, joks, visoks, kitoks
        Paradigma_V_jis,        // jis, šis
        Paradigma_V_pats,       // pats
        Paradigma_M_ta,         // ta, šita, kita, visa
        Paradigma_M_tokia,      // tokia, visokia
        Paradigma_M_ji,         // ji, ši, pati
        Paradigma_total
    } ELinksniuote;

    EGimine gimine_;
    ELinksniuote linksniuote_;
    std::wstring lt_[Skaicus_Total][Atvejis_Total];
    std::wstring ru_[Skaicus_Total][Atvejis_Total];
};

