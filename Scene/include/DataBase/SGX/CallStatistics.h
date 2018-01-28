//
// Created by moe on 28.01.18.
//

#ifndef SCENE_CALLSTATISTICS_H
#define SCENE_CALLSTATISTICS_H

#include <qplatformdefs.h>
#include <QtCore/QString>

namespace moe {
    typedef unsigned double udouble_t;
    struct CallStatistics {
        QString callSymbolName_;
        int callId_;
        udouble_t callAvg_, meanValue_,standardDeviation_, _99thPercentile_, _95thPercentile_, _90thPercentile_;
    };
}
#endif //SCENE_CALLSTATISTICS_H
