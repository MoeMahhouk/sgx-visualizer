//
// Created by moe on 09.01.18.
//

#ifndef SCENE_CALLTYPES_H
#define SCENE_CALLTYPES_H

#include <cstdint>
#include <QtCore/QString>

namespace moe {
    /**
     * data structure for the distince ecall/ocall types
     */
    struct CallTypes {
        int id_, eid_;
        uint64_t symbol_address_;
        QString symbol_name_;

        CallTypes(int id, int eid, uint64_t symbol_address, QString symbol_name) :
                id_(id), eid_(eid), symbol_address_(symbol_address), symbol_name_(symbol_name){}
    };
}
#endif //SCENE_CALLTYPES_H
