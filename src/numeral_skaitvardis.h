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

// Skaitvardis, числительное
class SkaitvardisGeneric : public WordGeneric {
 public:
    SkaitvardisGeneric(AttributeType *cfg);

    virtual void info() override;
    virtual std::wstring gautiForma(AttributeType &arg) override;

 private:
    std::wstring lt_[Gimine_Total][Skaicus_Total][Atvejis_Total];
    std::wstring ru_[Gimine_Total][Skaicus_Total][Atvejis_Total];
};

