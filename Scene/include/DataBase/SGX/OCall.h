//
// Created by moe on 13.12.17.
//

#ifndef SCENE_OCALL_H
#define SCENE_OCALL_H

#include <string>
#include "Call.h"

namespace moe {

    /**
     * data structure for ocall events
     */
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

        /**
         * translates ocall objects into a rendered object in an iterative process through all its children ecall events
         * @param factor
         * @return
         */
        virtual SeqDiagBlock* toRenderable(qreal factor) const override //ToDo added factor for test purposes
        {
            SeqDiagBlock *callBlock = new SeqDiagBlock(Transform2D(1, 0, 0, 1, 0, relative_start_time_*factor)
                                                        , 50, total_time_*factor, isFail_? new QPen(Qt::red) : new QPen(Qt::yellow),
                                                       isFail_? new QBrush(Qt::red) : new QBrush(Qt::yellow));
            //std::cerr << "OCALL position mulitplicated with factor : " << relative_start_time_ * factor << std::endl;
            callBlock->initializeStats(callInfo);

            /*if (!children_.isEmpty())
            {
                SeqDiagBlockCluster *clusterTest = new SeqDiagBlockCluster(Transform2D(),0,0,new QPen(Qt::green), new QBrush(Qt::blue));
                callBlock->addBlock(clusterTest);
                QVector<SeqDiagBlockCluster *> subClusterList;
                subClusterList.push_back(new SeqDiagBlockCluster(Transform2D(),0,0,new QPen(Qt::green), new QBrush(Qt::blue)));
            */
                for (Call *call : children_)
                {
                    SeqDiagBlock *childRenderable = call->toRenderable(factor);

                    /*if (subClusterList.back()->checkClusterCriteria(childRenderable))
                    {
                        subClusterList.back()->addBlock(childRenderable);
                    } else {
                        subClusterList.push_back(new SeqDiagBlockCluster(Transform2D(),0,0,new QPen(Qt::green), new QBrush(Qt::blue)));
                        subClusterList.back()->addBlock(childRenderable);
                    }*/
                    callBlock->addBlock(childRenderable);
                }
                /*for (SeqDiagBlockCluster *subCluster: subClusterList)
                {
                    clusterTest->addBlock(subCluster);
                }*/
            //}
            return callBlock;
        }
    };
}

#endif //SCENE_OCALL_H
