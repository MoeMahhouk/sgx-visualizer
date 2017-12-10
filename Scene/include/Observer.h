//
// Created by moe on 10.12.17.
//

#ifndef SCENE_OBSERVER_H
#define SCENE_OBSERVER_H

#include "Renderable.h"

namespace moe
{
    class Observer
    {
    public:
        enum Event
        {
            RESET=0,
            ZOOM,
            SCROLL
        };
        virtual ~Observer() {}
        virtual void onNotify(const Renderable& renderable1, const Renderable& renderable2, Event event) = 0;
    };
}



#endif //SCENE_OBSERVER_H
