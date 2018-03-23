//
// Created by moe on 09.01.18.
//

#ifndef SCENE_OCALLTYPES_H
#define SCENE_OCALLTYPES_H


#include "CallTypes.h"

namespace moe {
    /**
     * data structure for the distinct ocall types of the opened SGX trace
     */
    struct OCallTypes : public CallTypes {
        uint64_t symbol_address_normalized_;
        QString  symbol_file_name_;
        OCallTypes(int id = 0, int eid = 0, uint64_t symbol_address = 0, QString symbol_name = "",
                   uint64_t symbol_address_normaliyed = 0, QString symbol_file_name = "") :
                CallTypes(id, eid, symbol_address, symbol_name), symbol_address_normalized_(symbol_address_normaliyed),
                symbol_file_name_(symbol_file_name) {}
    };
}

#endif //SCENE_OCALLTYPES_H
