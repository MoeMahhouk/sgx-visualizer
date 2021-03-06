//
// Created by moe on 13.12.17.
//

#ifndef SCENE_EVENTMAP_H
#define SCENE_EVENTMAP_H

#include <cstdint>

namespace moe{
    /**
     * event map of the SGX trace
     */
    enum class EventMap : std::int8_t {
        Event = 0,
        SignalEvent,
        ThreadEvent,
        ThreadCreationEvent,
        ThreadCreatorEvent,
        ThreadDestructionEvent,
        ThreadSetNameEvent,
        EnclaveEvent,
        EnclaveCreationEvent,
        EnclaveDestructionEvent,
        EnclavePagingEvent,
        EnclavePageOutEvent,
        EnclavePageInEvent,
        EnclaveCallEvent,
        EnclaveECallEvent,
        EnclaveECallReturnEvent,
        EnclaveOCallEvent,
        EnclaveOCallReturnEvent
    };
}


#endif //SCENE_EVENTMAP_H
