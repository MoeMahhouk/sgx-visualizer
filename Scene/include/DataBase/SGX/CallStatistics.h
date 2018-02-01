//
// Created by moe on 28.01.18.
//

#ifndef SCENE_CALLSTATISTICS_H
#define SCENE_CALLSTATISTICS_H

#include <qplatformdefs.h>
#include <QtCore/QString>

namespace moe {

    struct CallStatistics {
        QString callSymbolName_;
        int callId_;
        qreal callAvg_, median_,standardDeviation_, _99thPercentile_, _95thPercentile_, _90thPercentile_, count_;
    };
}
#endif //SCENE_CALLSTATISTICS_H
