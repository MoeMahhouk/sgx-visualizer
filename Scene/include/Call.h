//
// Created by moe on 14.12.17.
//

#ifndef SCENE_CALL_H
#define SCENE_CALL_H

#include <string>
#include <QtCore/QVector>

namespace moe {

    struct Call{
        int id_, eid_, symbol_address_;
        std::string symbol_name_;
        QVector<Call*> children;
        Call(int id, int eid, int symbol_address, std::string symbol_name) :
                id_(id), eid_(eid), symbol_address_(symbol_address), symbol_name_(symbol_name) {}
    };
}

#endif //SCENE_CALL_H
