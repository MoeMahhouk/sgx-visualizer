//
// Created by moe on 13.01.18.
//

#ifndef SCENE_IRECIEVER_H
#define SCENE_IRECIEVER_H

namespace TYPES
{
    /**
     * Filter Command list
     */
    enum ACTION_LIST
    {
        THREADFILTER,
        ECALLOCALLFILTER,
    };
}

namespace moe {
    /**
     * Abstract class for command pattern
     */
    class IReciever {

    public:
        /**
         * Sets the action of the commands pattern
         * @param action
         */
        virtual void SetAction(TYPES::ACTION_LIST action) = 0;

        /**
        * executes the command based on the setted action
        * @param conditionQuery
        */
        virtual void getResult(QString conditionQuery) = 0;

    };
}
#endif //SCENE_IRECIEVER_H
