//
// Created by moe on 10.12.17.
//

#ifndef SCENE_OBSERVER_H
#define SCENE_OBSERVER_H

#include "Renderable.h"

namespace moe
{
    struct Event {
        virtual ~Event(){}
        //virtual void doSmthn() {};
    };

    struct ZoomEvent : public Event {
        Renderable* r1, *r2;
        ZoomEvent(Renderable* renderable1, Renderable* renderable2) : r1(renderable1), r2(renderable2) {}
    };

    struct ResetEvent : public Event {
        Renderable* r1, *r2;
        ResetEvent(Renderable* renderable1, Renderable* renderable2) : r1(renderable1), r2(renderable2) {}
    };

    struct ScrollEvent : public Event {
        Renderable* r1, *r2;
        ScrollEvent(Renderable* renderable1, Renderable* renderable2) : r1(renderable1), r2(renderable2) {}
    };


    class Observer
    {
    public:

        virtual ~Observer() {}
        virtual void onNotify(Event* event) = 0;
    };
}



#endif //SCENE_OBSERVER_H
