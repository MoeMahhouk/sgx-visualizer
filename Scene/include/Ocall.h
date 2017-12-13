//
// Created by moe on 13.12.17.
//

#ifndef SCENE_OCALL_H
#define SCENE_OCALL_H

#include <string>

namespace moe {
    struct Ocall{
        int id_, eid_, symbol_address_normalized_,symbol_address_;
        std::string symbol_name_, symbol_file_name_;
        Ocall(int id, int eid, int symbol_address_normalized, int symbol_address, std::string symbol_name,
              std::string symbol_file_name) : id_(id), eid_(eid), symbol_address_normalized_(symbol_address_normalized),
                                              symbol_address_(symbol_address), symbol_name_(symbol_name),
                                              symbol_file_name_(symbol_file_name) {}
    };
}

#endif //SCENE_OCALL_H
