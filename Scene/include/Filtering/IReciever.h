//
// Created by moe on 13.01.18.
//

#ifndef SCENE_IRECIEVER_H
#define SCENE_IRECIEVER_H

namespace TYPES
{
    enum ACTION_LIST
    {
        THREADFILTER,
        ECALLFILTER,
        OCALLFILTER
    };
};

namespace moe {

    class IReciever {

    public:

        virtual void SetAction(TYPES::ACTION_LIST action) = 0;

        virtual void getResult(QString conditionQuery) = 0;

    };
}
#endif //SCENE_IRECIEVER_H
