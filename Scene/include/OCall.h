//
// Created by moe on 13.12.17.
//

#ifndef SCENE_OCALL_H
#define SCENE_OCALL_H

#include <string>
#include "Call.h"
namespace moe {
    struct OCall : public Call{
        uint64_t symbol_address_normalized_;
        std::string  symbol_file_name_;
        OCall(int id, int eid, uint64_t symbol_address, uint64_t start_time, uint64_t total_time,
              std::string symbol_name, uint64_t symbol_address_normalized, std::string symbol_file_name) :
                Call(id, eid, symbol_address,start_time, total_time, symbol_name),
                                              symbol_address_normalized_(symbol_address_normalized),
                                              symbol_file_name_(symbol_file_name) {}

        virtual Rect* toRenderable() const override {
            Rect *callBlock = new Rect(Transform2D(1,0,0,1,-2,start_time_),15,total_time_,new QPen(Qt::yellow),new QBrush(Qt::yellow));
            for (Call *call : children_) {
                Renderable *childRenderable = call->toRenderable();
                callBlock->addBlock(childRenderable);
            }
            return callBlock;
        }
    };
}

#endif //SCENE_OCALL_H
