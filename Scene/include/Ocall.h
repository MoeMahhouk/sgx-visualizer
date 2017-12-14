//
// Created by moe on 13.12.17.
//

#ifndef SCENE_OCALL_H
#define SCENE_OCALL_H

#include <string>
#include "Call.h"
namespace moe {
    struct Ocall : public Call{
        int symbol_address_normalized_;
        std::string  symbol_file_name_;
        Ocall(int id, int eid, int symbol_address_normalized, int symbol_address, std::string symbol_name,
              std::string symbol_file_name) : Call(id, eid, symbol_address, symbol_name),symbol_address_normalized_(symbol_address_normalized),
                                              symbol_file_name_(symbol_file_name) {}
    };
}

#endif //SCENE_OCALL_H
