//
// Created by moe on 10.12.17.
//

#ifndef SCENE_OBSERVER_H
#define SCENE_OBSERVER_H

#include "Rendering/Renderable.h"

namespace moe
{
    /**
     * abstract event class
     */
    struct Event {
        virtual ~Event(){}
    };

    /**
     * zoom event data structure
     */
    struct ZoomEvent : public Event {
       // Renderable* r1, *r2;
        qreal yScale_;
        qreal yOffset_;
        ZoomEvent(qreal yScale, qreal yOffset) : yScale_(yScale), yOffset_(yOffset) {}
    };

    /**
     * reset event
     */
    struct ResetEvent : public Event {
    };

    /**
     * scroll Event data structure
     */
    struct ScrollEvent : public Event {
        qreal yScale_;
        qreal yOffset_;
        ScrollEvent(qreal yScale, qreal yOffset) : yScale_(yScale), yOffset_(yOffset) {}
    };

    /**
     * Observer pattern abstract class
     */
    class Observer {
    public:

        virtual ~Observer() {}

        /**
         * executes an action based on the triggered event
         * @param event
         */
        virtual void onNotify(Event* event) = 0;
    };
}



#endif //SCENE_OBSERVER_H
