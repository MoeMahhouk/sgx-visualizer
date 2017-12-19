//
// Created by moe on 13.12.17.
//

#ifndef SCENE_MYTHREAD_H
#define SCENE_MYTHREAD_H


#include <string>
#include "SequenceDiagram.h"
#include "ECall.h"

namespace moe {
    struct MyThread {
        int id_;
        uint64_t pthread_id_,start_address_, start_address_normalized_, start_symbol_, start_time_, total_time_;
        std::string name_, start_symbol_file_name_;
        QVector<ECall*> threadEcalls_;

        MyThread(int id = 0, uint64_t pthread_id = 0, uint64_t start_address = 0,
                 uint64_t start_address_normalized = 0, uint64_t start_symbol = 0, uint64_t start_time = 0,
                 uint64_t total_time = 0, std::string name = "", std::string start_symbol_file_name = "",
                 int EcallNumbers=0) :
                id_(id), pthread_id_(pthread_id), start_address_(start_address),
                start_address_normalized_(start_address_normalized), start_symbol_(start_symbol), start_time_(start_time),
                total_time_(total_time), name_(name), start_symbol_file_name_(start_symbol_file_name)
                {
                    threadEcalls_ = QVector<ECall*>(EcallNumbers);
                }

        ~MyThread() {
            auto it = threadEcalls_.begin();
            while (it != threadEcalls_.end())
            {
                delete *it;
                it++;
            }
            this->threadEcalls_.clear();
        }

    SequenceDiagram* toRenderable() {
        SequenceDiagram *threadSequenceDiagram = new SequenceDiagram(Transform2D(), "Pthread --" + pthread_id_ % 1000, total_time_);
        return threadSequenceDiagram;
    }

    };
}


#endif //SCENE_MYTHREAD_H
