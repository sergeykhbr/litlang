﻿// 
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

#include "api_wrd.h"
#include <attribute.h>
#include "noun.h"
#include <utils.h>

static int idcnt_ = 0;
static std::list<WordGeneric *> zodynas_;

int WRD_unique_id() {
    return ++idcnt_;
}

void WRD_pridelioti_zodis(AttributeType *cfg) {
    WordGeneric *p = 0;
    if (!cfg->is_dict()) {
        return;
    }
    if ((*cfg)[L"Type"].is_equal(L"daiktavardis")) {
            p = new DaiktavardisGeneric(cfg);
    } else {
        printf_error(L"Undefined Type %s", (*cfg)[L"Type"].to_string());
    }

    if (p) {
        zodynas_.push_back(p);
    }
}

