//
// Created by moe on 13.12.17.
//

#ifndef SCENE_MYTHREAD_H
#define SCENE_MYTHREAD_H


#include <string>
#include "Ecall.h"
namespace moe {
    struct MyThread {
        int id_,pthread_id_,start_address_, start_address_normalized_, start_symbol_;
        const std::string name_, start_symbol_file_name_;
        QVector<Ecall> threadEcalls;

        MyThread(int id, int pthread_id, std::string name , std::string start_symbol_file_name,
                 int start_address, int start_symbol , int start_address_normalized) :
                id_(id), pthread_id_(pthread_id), name_(name), start_symbol_file_name_(start_symbol_file_name),
                start_address_(start_address), start_symbol_(start_symbol),
                start_address_normalized_(start_address_normalized){}
    };
}


#endif //SCENE_MYTHREAD_H
