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

#include <string>
#include <list>
#include <attribute.h>

typedef enum EGimine {
    Gimine_nezinoma,
    Vyriskoji,
    Moteriskoji,
    Gimine_Total
} EGimine;

typedef enum ESkaicus {
    Vienaskaita,
    Daugiskaita,
    Skaicus_Total
} ESkaicus;


class WordGeneric {
 public:
    WordGeneric(AttributeType *cfg);

    virtual const wchar_t *getValue() { return value_.c_str(); }
    virtual bool isEqual(const wchar_t *w) { return value_ == w; }

    virtual void info() {}

 protected:
    virtual int add2wline(wchar_t *buf, int pos, const wchar_t *s, int align);

 protected:
    int id_;                                // to avoid pointer saving will use this one id
    std::wstring value_;
};

