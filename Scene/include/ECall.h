//
// Created by moe on 13.12.17.
//

#ifndef SCENE_ECALL_H
#define SCENE_ECALL_H

#include <string>
#include "Call.h"
namespace moe  {
    struct ECall : public Call {
        bool is_private_;
        ECall(int id = 0, int eid = 0, uint64_t symbol_address = 0, uint64_t start_time = 0, uint64_t relative_start_time = 0,
              uint64_t total_time = 0, bool is_private = false, std::string symbol_name = "") :
                Call(id, eid, symbol_address,start_time,relative_start_time, total_time, symbol_name), is_private_(is_private){}

        virtual Rect* toRenderable(qreal factor) const override { //ToDo added factor for test purposes
            Rect *callBlock = new Rect(Transform2D(1,0,0,1,-2,relative_start_time_*factor),50,total_time_*factor,new QPen(Qt::green));
            std::cerr << " ECALL position mulitplicated with factor : " << relative_start_time_ * factor << std::endl;
            for (Call *call : children_) {
                Renderable *childRenderable = call->toRenderable(factor);
                callBlock->addBlock(childRenderable);
            }
            return callBlock;
        }
    };
}
#endif //SCENE_ECALL_H
