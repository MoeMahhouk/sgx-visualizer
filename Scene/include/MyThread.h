//
// Created by moe on 13.12.17.
//

#ifndef SCENE_MYTHREAD_H
#define SCENE_MYTHREAD_H


#include <string>
#include "ECall.h"
namespace moe {
    struct MyThread {
        int id_,pthread_id_,start_address_, start_address_normalized_, start_symbol_;
        std::string name_, start_symbol_file_name_;
        QVector<ECall> threadEcalls;

        MyThread(int id = 0, int pthread_id = 0, int start_address = 0, int start_address_normalized = 0,
                 int start_symbol = 0, std::string name = "", std::string start_symbol_file_name = "" ) :
                id_(id), pthread_id_(pthread_id), start_address_(start_address),
                start_address_normalized_(start_address_normalized),start_symbol_(start_symbol),name_(name),
                start_symbol_file_name_(start_symbol_file_name)

                {}
    };
}


#endif //SCENE_MYTHREAD_H
