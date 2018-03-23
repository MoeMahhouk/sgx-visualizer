//
// Created by moe on 22.01.18.
//

#ifndef SCENE_CALLHOVERINFO_H
#define SCENE_CALLHOVERINFO_H

#include <cstdint>
#include <QtCore/QString>

namespace moe {
    /**
     * data structure for the hover information of ecalls and ocalls
     */
    struct CallHoverInfo {
        int childrenCounter = 0;
        uint64_t childrenTotalRuntime = 0;
        QString callName;
        uint64_t callTotalTime = 0;
        int enclaveId = 0;
        int status = 0;
        int aex = -1;
        QVector<uint64_t > aexTimes;
        QString enclaveBinaryName = "";
    };
}
#endif //SCENE_CALLHOVERINFO_H
