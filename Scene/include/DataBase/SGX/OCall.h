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

        OCall(int id, int eid, uint64_t symbol_address, uint64_t start_time, uint64_t relative_start_time , uint64_t total_time,
              QString symbol_name, uint64_t symbol_address_normalized, std::string symbol_file_name, int isFail) :
                Call(id, eid, symbol_address,start_time,relative_start_time , total_time, symbol_name,isFail),
                                              symbol_address_normalized_(symbol_address_normalized),
                                              symbol_file_name_(symbol_file_name)
        {
        }

        virtual SeqDiagBlock* toRenderable(qreal factor) const override //ToDo added factor for test purposes
        {
            SeqDiagBlock *callBlock = new SeqDiagBlock(Transform2D(1, 0, 0, 1, 0, relative_start_time_*factor)
                                                        , 50, total_time_*factor, new QPen(Qt::gray),
                                                       isFail_? new QBrush(Qt::red) : new QBrush(Qt::yellow));
            //std::cerr << "OCALL position mulitplicated with factor : " << relative_start_time_ * factor << std::endl;
            callBlock->initializeStats(callInfo);
            for (Call *call : children_)
            {
                Renderable *childRenderable = call->toRenderable(factor);
                callBlock->addBlock(childRenderable);
            }
            return callBlock;
        }
    };
}

#endif //SCENE_OCALL_H
