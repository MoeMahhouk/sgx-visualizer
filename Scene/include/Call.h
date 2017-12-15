//
// Created by moe on 14.12.17.
//

#ifndef SCENE_CALL_H
#define SCENE_CALL_H

#include <string>
#include <QtCore/QVector>

namespace moe {

    struct Call{
        int id_, eid_, symbol_address_, start_time_, total_time_;
        std::string symbol_name_;
        QVector<Call*> children;
        Call(int id, int eid, int symbol_address, int start_time, int total_time , std::string symbol_name) :
                id_(id), eid_(eid), symbol_address_(symbol_address), start_time_(start_time), total_time_(total_time),
                symbol_name_(symbol_name) {}
    };
}

#endif //SCENE_CALL_H
