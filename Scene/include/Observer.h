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
       // Renderable* r1, *r2;
        qreal yScale_;
        qreal yOffset_;
        //Renderable* sequenceListNode_; //ToDo change it later qreal yOffset
       // ZoomEvent(Renderable* renderable1, Renderable* renderable2) : r1(renderable1), r2(renderable2) {}
        ZoomEvent(qreal yScale, qreal yOffset) : yScale_(yScale), yOffset_(yOffset) {}
    };

    /*struct ResetEvent : public Event {
        Renderable* r1, *r2;
        ResetEvent(Renderable* renderable1, Renderable* renderable2) : r1(renderable1), r2(renderable2) {}
    };*/

    struct ResetEvent : public Event {
    };

    struct ScrollEvent : public Event {
        //Renderable* r1, *r2;
        qreal yScale_;
        qreal yOffset_;
       // Renderable* sequenceListNode_; //ToDo change it later qreal yOffset
        ScrollEvent(qreal yScale, qreal yOffset) : yScale_(yScale), yOffset_(yOffset) {}
    };


    class Observer
    {
    public:

        virtual ~Observer() {}
        virtual void onNotify(Event* event) = 0;
    };
}



#endif //SCENE_OBSERVER_H
