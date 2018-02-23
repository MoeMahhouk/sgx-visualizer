//
// Created by moe on 13.12.17.
//

#ifndef SCENE_MYTHREAD_H
#define SCENE_MYTHREAD_H


#include <string>
#include <Rendering/SeqDiagBlockCluster.h>
#include "Rendering/SequenceDiagram.h"
#include "ECall.h"

namespace moe {
    struct MyThread {
        int id_;
        uint64_t pthread_id_, start_address_, start_address_normalized_, start_symbol_, start_time_, total_time_;
        QString name_, start_symbol_file_name_;
        QVector<ECall *> threadEcalls_;

        MyThread(int id = 0, uint64_t pthread_id = 0, uint64_t start_address = 0,
                 uint64_t start_address_normalized = 0, uint64_t start_symbol = 0, uint64_t start_time = 0,
                 uint64_t total_time = 0, QString name = "", QString start_symbol_file_name = "") :
                id_(id), pthread_id_(pthread_id), start_address_(start_address),
                start_address_normalized_(start_address_normalized), start_symbol_(start_symbol),
                start_time_(start_time),
                total_time_(total_time), name_(name), start_symbol_file_name_(start_symbol_file_name) {}

        ~MyThread() {
            auto it = threadEcalls_.begin();
            while (it != threadEcalls_.end()) {
                delete *it;
                it++;
            }
            this->threadEcalls_.clear();
        }

        SequenceDiagram *toRenderable(qreal factor) const
        {
            QString threadName = "";
            this->name_.isEmpty() ? threadName.append("Thread " + QString::number(id_)) : threadName.append(name_);
            SequenceDiagram *threadSeqDiag = new SequenceDiagram(Transform2D(), threadName,
                                                                 total_time_ * factor
            );

            if (threadEcalls_.size() > 1)
            {
                QVector<SeqDiagBlockCluster *> subClusterList;
                subClusterList.push_back(new SeqDiagBlockCluster(Transform2D(),0,0,new QPen(Qt::gray), new QBrush(Qt::darkMagenta, Qt::Dense3Pattern)));
                for (ECall *eCall : threadEcalls_)
                {

                    SeqDiagBlock *eCallRenderable = eCall->toRenderable(factor);
                    if (subClusterList.back()->checkClusterCriteria(eCallRenderable))
                    {
                        subClusterList.back()->addBlock(eCallRenderable);
                    } else {
                        subClusterList.push_back(new SeqDiagBlockCluster(Transform2D(),0,0,new QPen(Qt::gray), new QBrush(Qt::darkMagenta, Qt::Dense3Pattern)));
                        subClusterList.back()->addBlock(eCallRenderable);
                    }
                }
                if(subClusterList.size() > 1)
                {
                    SeqDiagBlockCluster *clusterTest = new SeqDiagBlockCluster(Transform2D(),0,0,new QPen(Qt::gray), new QBrush(Qt::darkMagenta, Qt::Dense3Pattern));

                    for (SeqDiagBlockCluster *subCluster: subClusterList)
                    {
                        clusterTest->addBlock(subCluster);
                    }
                    threadSeqDiag->addBlock(clusterTest);
                } else {
                    threadSeqDiag->addBlock(subClusterList.last());// because subClusterList in this case has only one child
                }
            } else {
                for (ECall *eCall : threadEcalls_)
                {
                    SeqDiagBlock *eCallRenderable = eCall->toRenderable(factor);
                    threadSeqDiag->addBlock(eCallRenderable);
                }
            }
            return threadSeqDiag;

        }
    };
}


#endif //SCENE_MYTHREAD_H
