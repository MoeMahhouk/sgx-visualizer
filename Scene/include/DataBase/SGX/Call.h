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
        QString symbol_name_;
        QVector<Call*> children_;
        int isFail_;
        /*
         * ToDO under progress for hover and statistics later implementation
         */
        CallHoverInfo callInfo;

        Call(int id, int eid, uint64_t symbol_address, uint64_t start_time, uint64_t relative_start_time,
             uint64_t total_time, QString symbol_name, int isFail) :
                id_(id), eid_(eid), symbol_address_(symbol_address), start_time_(start_time),
                relative_start_time_(relative_start_time), total_time_(total_time), symbol_name_(symbol_name),
                isFail_(isFail){}

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

    /*struct CallCluster : public Call {
        CallCluster(int id = 0, int eid = 0, uint64_t symbol_address = 0, uint64_t start_time = 0, uint64_t relative_start_time = 0,
              uint64_t total_time = 0, bool is_private = false, QString symbol_name = "", int isFail = 0) :
                Call(id, eid, symbol_address,start_time,relative_start_time, total_time, symbol_name, isFail)
        {
        }


    };*/
}

#endif //SCENE_CALL_H
