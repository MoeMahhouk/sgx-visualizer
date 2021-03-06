//
// Created by moe on 17.12.17.
//

#ifndef SCENE_DATABASEMANAGER_H
#define SCENE_DATABASEMANAGER_H


#include "DataBase/SGX/MyThread.h"


namespace moe {
    /**
     * abstract class for other databases than sgx
     */
    class DataBaseManager {

    public:

        DataBaseManager(const QString& path = "newDataBaseCreated")
        {
            (void)path;
        }

        virtual ~DataBaseManager() = default;

        virtual void close() = 0;
    };
}


#endif //SCENE_DATABASEMANAGER_H
