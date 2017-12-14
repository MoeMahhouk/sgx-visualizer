//
// Created by moe on 13.12.17.
//

#ifndef SCENE_ECALL_H
#define SCENE_ECALL_H

#include <string>
#include "Call.h"
namespace moe  {
    struct Ecall : public Call{
        bool is_private_;
        Ecall(int id, int eid, int symbol_address, bool is_private, std::string symbol_name) :
                Call(id, eid, symbol_address, symbol_name) , is_private_(is_private){}
    };
}
#endif //SCENE_ECALL_H
