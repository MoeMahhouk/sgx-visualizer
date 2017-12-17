//
// Created by moe on 13.12.17.
//

#ifndef SCENE_ECALL_H
#define SCENE_ECALL_H

#include <string>
#include "Call.h"
namespace moe  {
    struct ECall : public Call{
        bool is_private_;
        ECall(uint64_t id = 0, uint64_t eid = 0, uint64_t symbol_address = 0, uint64_t start_time = 0,
              uint64_t total_time = 0, bool is_private = false, std::string symbol_name = "") :
                Call(id, eid, symbol_address,start_time, total_time, symbol_name), is_private_(is_private){}
    };
}
#endif //SCENE_ECALL_H
