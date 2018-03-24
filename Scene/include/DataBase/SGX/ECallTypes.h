//
// Created by moe on 09.01.18.
//

#ifndef SCENE_ECALLTYPES_H
#define SCENE_ECALLTYPES_H

#include "CallTypes.h"

namespace moe {
    /**
     * ecall types data structure for the distinct ecalls of the opened trace
     */
    struct ECallTypes : public CallTypes {
        bool isPrivate_;
        ECallTypes(int id = 0, int eid = 0, uint64_t symbol_address = 0, QString symbol_name = "", bool isPrivate = 0) :
                CallTypes(id, eid, symbol_address, symbol_name), isPrivate_(isPrivate) {}
    };
}

#endif //SCENE_ECALLTYPES_H
