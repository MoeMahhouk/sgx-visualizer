//
// Created by moe on 13.12.17.
//

#ifndef SCENE_ECALL_H
#define SCENE_ECALL_H

#include <string>
#include "Call.h"

namespace moe  {
    /**
     * ecall event class
     */
    struct ECall : public Call {

        bool is_private_;
        ECall(int id = 0, int eid = 0, uint64_t symbol_address = 0, uint64_t start_time = 0, uint64_t relative_start_time = 0,
              uint64_t total_time = 0, bool is_private = false, QString symbol_name = "", int isFail = 0) :
                Call(id, eid, symbol_address,start_time,relative_start_time, total_time, symbol_name, isFail),
                is_private_(is_private)
        {
        }

        /**
         * translates the ecall event into a rendered object in an iterative process through its all children ocall events
         * and initialises potential clusters
         * @param factor
         * @return
         */
        virtual SeqDiagBlock* toRenderable(qreal factor) const override //ToDo added factor for test purposes
        {

            SeqDiagBlock *callBlock = new SeqDiagBlock(Transform2D(1, 0, 0, 1, 0, relative_start_time_*factor)
                                                        , 50, total_time_*factor, isFail_ ? new QPen(Qt::red) : new QPen(Qt::blue),
                                                       isFail_ ? new QBrush(Qt::red) : new QBrush(Qt::blue));
            for (int i = 0; i < callInfo.aexTimes.size(); ++i)
            {
                qreal aexYOffset = callInfo.aexTimes[i];
                callBlock->addBlock(new Line(Transform2D(1,0,0,1,-5,aexYOffset * factor),60,0,3,new QPen(QColor( 0xFF, 0xA0, 0x00 ))));
            }
            callBlock->initializeStats(callInfo);

            if (children_.size() > 1)
            {
                SeqDiagBlockCluster *clusterTest = new SeqDiagBlockCluster(Transform2D(),0,0,new QPen(Qt::gray), new QBrush(Qt::darkYellow, Qt::Dense3Pattern));
                callBlock->addBlock(clusterTest);
                QVector<SeqDiagBlockCluster *> subClusterList;
                subClusterList.push_back(new SeqDiagBlockCluster(Transform2D(),0,0,new QPen(Qt::gray), new QBrush(Qt::darkYellow, Qt::Dense3Pattern)));

                for (Call *call : children_)
                {
                    SeqDiagBlock *childRenderable = call->toRenderable(factor);

                    if (subClusterList.back()->checkClusterCriteria(childRenderable))
                    {
                        subClusterList.back()->addBlock(childRenderable);
                    } else {
                        subClusterList.push_back(new SeqDiagBlockCluster(Transform2D(),0,0,new QPen(Qt::gray), new QBrush(Qt::darkYellow, Qt::Dense3Pattern)));
                        subClusterList.back()->addBlock(childRenderable);
                    }
                }
                for (SeqDiagBlockCluster *subCluster: subClusterList)
                {
                    clusterTest->addBlock(subCluster);
                }
            } else {
                for (Call *call : children_)
                {
                    SeqDiagBlock *childRenderable = call->toRenderable(factor);
                    callBlock->addBlock(childRenderable);
                }
            }
            return callBlock;
        }
    };
}
#endif //SCENE_ECALL_H
