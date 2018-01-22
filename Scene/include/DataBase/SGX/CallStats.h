//
// Created by moe on 22.01.18.
//

#ifndef SCENE_CALLSTATS_H
#define SCENE_CALLSTATS_H

#include <cstdint>
#include <QtCore/QString>

namespace moe {

    struct CallStats {
        int childrenCounter = 0;
        uint64_t childrenTotalRuntime = 0;
        QString callName;
        uint64_t callTotalTime = 0;
        int enclaveId = 0;
        QString enclaveBinaryName = "";
    };
}
#endif //SCENE_CALLSTATS_H
