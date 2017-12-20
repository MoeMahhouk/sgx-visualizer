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

        virtual Rect* toRenderable(qreal factor) const override { //ToDo added factor for test purposes
            Rect *callBlock = new Rect(Transform2D(1,0,0,1,-2,start_time_*factor),35,total_time_*factor,new QPen(Qt::yellow),new QBrush(Qt::yellow));
            std::cerr << "OCALL position mulitplicated with factor : " << start_time_ * factor << std::endl;
            for (Call *call : children_) {
                Renderable *childRenderable = call->toRenderable(factor);
                callBlock->addBlock(childRenderable);
            }
            return callBlock;
        }
    };
}

#endif //SCENE_OCALL_H
