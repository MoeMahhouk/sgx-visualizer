//
// Created by moe on 13.12.17.
//

#ifndef SCENE_ECALL_H
#define SCENE_ECALL_H

#include <string>

namespace moe {
    struct Ecall {
        int id_, eid_, symbol_address_;
        bool is_private_;
        const std::string symbol_name_;
        Ecall(int id, int eid, int symbol_address, bool is_private, std::string symbol_name) :
                id_(id), eid_(eid), symbol_address_(symbol_address), symbol_name_(symbol_name){}
    };
}
#endif //SCENE_ECALL_H
