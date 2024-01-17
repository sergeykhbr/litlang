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

// Būdvardis, прилагательное, adjective
class BudvardisGeneric : public WordGeneric {     // глагол
 public:
    BudvardisGeneric(AttributeType *cfg);

    virtual void info() override;
    // šaknis = корень (getRoot)
    virtual std::wstring imkSaknis(int idx) override;

 protected:
    // определить парадигму
    void nustatyti_paradigma(int linksniuote);
    // обновить (заполнить таблицу склонений)
    void atnaujinti();

    int imkLinksniuote();

 protected:
    typedef enum ELinksniuote {
        Linksniuote_nezinoma,
        Linksniuote_V1,
        Linksniuote_V2,
        Linksniuote_V3,
        Linksniuote_M1,
        Linksniuote_M2,
        Linksniuote_M3,
        Linksniuote_Total
    } ELinksniuote;

    typedef enum EParadigma {
        Paradigma_nezinoma,
        // Vyriškoji 1 linksniuotė
        Paradigma_V1_as,
        Paradigma_V1_ias,
        Paradigma_V1_is,
        // Vyriškoji 2 linksniuotė
        Paradigma_V2_us,
        // Vyrųkoji 3 linksniuotė
        Paradigma_V3_is,
        // Moteriųkoji 1 linksniuotė
        Paradigma_M1_a,
        Paradigma_M1_ia,
        // Moteriųkoji 2 linksniuotė
        Paradigma_M2_i,
        // Moteriųkoji 3 linksniuotė
        Paradigma_M3_e,  // ė
        Pardigma_Total
    } EParadigma;

    EGimine gimine_;
    EParadigma paradigma_;
    std::wstring lentele_[Skaicus_Total][Gimine_Total][Atvejis_Total];
    std::wstring ru_[Skaicus_Total][Gimine_Total][Atvejis_Total];
};

