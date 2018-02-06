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
              uint64_t total_time = 0, bool is_private = false, QString symbol_name = "", int isFail = 0) :
                Call(id, eid, symbol_address,start_time,relative_start_time, total_time, symbol_name, isFail),
                is_private_(is_private)
        {
        }

        virtual SeqDiagBlock* toRenderable(qreal factor) const override //ToDo added factor for test purposes
        {

            SeqDiagBlock *callBlock = new SeqDiagBlock(Transform2D(1, 0, 0, 1, 0, relative_start_time_*factor)
                                                        , 50, total_time_*factor, new QPen(Qt::green),
                                                       isFail_ ? new QBrush(Qt::red) : new QBrush(Qt::blue));
            //std::cerr << " ECALL position mulitplicated with factor : " << relative_start_time_ * factor << std::endl;
            //std::cerr << " ECALL position mulitplicated with factor : " << relative_start_time_  << std::endl;
            //std::cerr << " ECALL position mulitplicated with factor : " << (relative_start_time_ + total_time_)  << std::endl;
            //std::cerr << " ECALL totaltime mulitplicated with factor (height) : " << (total_time_ * factor)  << std::endl;
            //SeqDiagBlockCluster *clusterTest = new SeqDiagBlockCluster(Transform2D(),0,0,new QPen(Qt::green), new QBrush(Qt::yellow));
            callBlock->initializeStats(callInfo);
            if (!children_.isEmpty())
            {
                //Call *tmpCall;
                SeqDiagBlockCluster *clusterTest = new SeqDiagBlockCluster(Transform2D(),0,0,new QPen(Qt::gray), new QBrush(Qt::yellow));
                callBlock->addBlock(clusterTest);
                QVector<SeqDiagBlockCluster *> subClusterList;
                subClusterList.push_back(new SeqDiagBlockCluster(Transform2D(),0,0,new QPen(Qt::green), new QBrush(Qt::yellow)));

                for (Call *call : children_)
                {
                    SeqDiagBlock *childRenderable = call->toRenderable(factor);

                    if (subClusterList.back()->checkClusterCriteria(childRenderable))
                    {
                        subClusterList.back()->addBlock(childRenderable);
                    } else {
                        subClusterList.push_back(new SeqDiagBlockCluster(Transform2D(),0,0,new QPen(Qt::green), new QBrush(Qt::yellow)));
                        subClusterList.back()->addBlock(childRenderable);
                    }
                    //callBlock->addBlock(childRenderable);
                }
                for (SeqDiagBlockCluster *subCluster: subClusterList) {
                    if (subCluster->size())
                    {

                    }
                    clusterTest->addBlock(subCluster);
                }
            }
            return callBlock;
        }
    };
}
#endif //SCENE_ECALL_H
