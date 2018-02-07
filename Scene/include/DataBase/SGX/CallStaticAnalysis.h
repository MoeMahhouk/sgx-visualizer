//
// Created by moe on 06.02.18.
//

#ifndef SCENE_CALLSTATICANALYSIS_H
#define SCENE_CALLSTATICANALYSIS_H


#include <QtCore/QString>

namespace moe {

    struct CallStaticAnalysis {
        int callId_, totalCount_, totalOfLowerThanMicroSeconds_, totalOflowerThan10MicroSeconds_;
        QString callName_;
    };
}
#endif //SCENE_CALLSTATICANALYSIS_H
