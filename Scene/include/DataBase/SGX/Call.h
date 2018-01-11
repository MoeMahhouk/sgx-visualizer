//
// Created by moe on 14.12.17.
//

#ifndef SCENE_CALL_H
#define SCENE_CALL_H

#include <string>
#include <QtCore/QVector>

namespace moe {
    struct Call {
        int id_, eid_;
        uint64_t symbol_address_, start_time_,relative_start_time_, total_time_;
        std::string symbol_name_;
        QVector<Call*> children_;
        Call(int id, int eid, uint64_t symbol_address, uint64_t start_time, uint64_t relative_start_time, uint64_t total_time, std::string symbol_name) :
                id_(id), eid_(eid), symbol_address_(symbol_address), start_time_(start_time),
                relative_start_time_(relative_start_time), total_time_(total_time), symbol_name_(symbol_name) {}

        virtual ~Call() {
            auto it = children_.begin();
            while (it != children_.end())
            {
                delete *it;
                it++;
            }
            this->children_.clear();
        }
        virtual SeqDiagBlock* toRenderable(qreal factor) const = 0; //ToDo added factor for test purposes
    };
}

#endif //SCENE_CALL_H
