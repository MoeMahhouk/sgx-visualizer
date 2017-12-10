//
// Created by moe on 10.12.17.
//

#ifndef SCENE_NOTIFIER_H
#define SCENE_NOTIFIER_H

#include <QtCore/QSet>
#include "Observer.h"
#include "Renderable.h"

namespace moe
{
    class Notifier
    {
    public:
        virtual void registerObserver(Observer* observer)
        {
            observers_.insert(observer);
        }

        virtual void unRegisterObersver(Observer* observer)
        {
            QSet<Observer*>::const_iterator iter = observers_.find(observer);
            if(iter != observers_.end())
            {
                observers_.erase(iter);
            } else {
              std::cout << "Could not unregister the specified Observer object as it is not registered."<< std::endl;
            }
        }

    protected:
        virtual void notify(const Renderable& renderable1, const Renderable& renderable2, Observer::Event event)
        {
            std::for_each(observers_.begin(),observers_.end(), [&](Observer* observer) {observer->onNotify(renderable1, renderable2, event);});
        }
    private:
        QSet<Observer*> observers_;
    };
}


#endif //SCENE_NOTIFIER_H
