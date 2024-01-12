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

// существительное, daiktavardis
class DaiktavardisGeneric : public WordGeneric {
 public:
    DaiktavardisGeneric(AttributeType *cfg);

    virtual void info() override;
    virtual std::wstring gautiForma(AttributeType &arg) override;

 protected:
    // определить парадигму
    void nustatyti_paradigma(int linksniuote);
    // обновить (заполнить таблицу склонений)
    void atnaujinti();

    // šaknis = корень (getRoot)
    std::wstring imkSaknis();
    int imkLinksniuote();

 private:
    typedef enum ELinksniuote {
        Linksniuote_nezinoma,
        Linksniuote_1,
        Linksniuote_2,
        Linksniuote_3,
        Linksniuote_4,
        Linksniuote_5,
        Linksniuote_Total
    } ELinksniuote;

    typedef enum EParadigma {
        Paradigma_nezinoma,
        // 1 linksniuotė
        Paradigma_1_as,
        Paradigma_1_ias,
        Paradigma_1_is,
        Paradigma_1_ys,
        // 2 linksniuotė
        Paradigma_2_a,
        Paradigma_2_ia,
        Paradigma_2_e,  // ė
        // 3 linksniuotė
        Paradigma_3_is,
        // 4 linksniuotė
        Paradigma_4_us,
        Paradigma_4_ius,
        // 5 linksniuotė
        Paradigma_5_uo,
        Paradigma_5_e,  // ė
        Pardigma_Total
    } EParadigma;

    EGimine gimine_;
    EParadigma paradigma_;
    std::wstring lentele_[Skaicus_Total][Atvejis_Total];
    std::wstring ru_[Skaicus_Total][Atvejis_Total];
};

