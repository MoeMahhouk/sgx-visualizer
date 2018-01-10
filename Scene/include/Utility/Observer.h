//
// Created by moe on 10.12.17.
//

#ifndef SCENE_OBSERVER_H
#define SCENE_OBSERVER_H

#include "Rendering/Renderable.h"

namespace moe
{
    struct Event {
        virtual ~Event(){}
    };

    struct ZoomEvent : public Event {
       // Renderable* r1, *r2;
        qreal yScale_;
        qreal yOffset_;
        ZoomEvent(qreal yScale, qreal yOffset) : yScale_(yScale), yOffset_(yOffset) {}
    };

    struct ResetEvent : public Event {
    };

    struct ScrollEvent : public Event {
        qreal yScale_;
        qreal yOffset_;
        ScrollEvent(qreal yScale, qreal yOffset) : yScale_(yScale), yOffset_(yOffset) {}
    };


    class Observer {
    public:

        virtual ~Observer() {}
        virtual void onNotify(Event* event) = 0;
    };
}



#endif //SCENE_OBSERVER_H
