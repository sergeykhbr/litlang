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

#include "api_wrd.h"
#include <iostream>
#include <string.h>
#include <utils.h>
#include <io.h>
#include <fcntl.h>

int main(int argc, const char *argv[]) {
    AttributeType databuf;
    AttributeType config;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-z") == 0) {
            utils_read_json_file(argv[++i], &databuf);
        }
    }
    config.from_config(databuf.to_string());

    AttributeType &zodis = config[L"Items"];
    for (unsigned i = 0; i < zodis.size(); i++) {
        WRD_pridelioti_zodis(&zodis[i]);
    }

    _setmode(_fileno(stdout), _O_U8TEXT);

    WRD_info();

    // Create sentense: "существительного падежи" = "daiktavardžių linksniai"
    // 
    // "Вопросительные слова" = Klausiamieji žodziai 'kas' ir 'ar'
    return 0;
}