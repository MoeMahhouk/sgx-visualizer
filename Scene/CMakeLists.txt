cmake_minimum_required(VERSION 3.5)

#name of the project
project(Scene)

#add the include folder, so that the compiler will find all files there
include_directories(include)

set(CMAKE_AUTOMOC ON)

#set(CMAKE_PREFIX_PATH $ENV{HOME}/moe/Qt/5.9.2/gcc_64)
set(CMAKE_CXX_STANDARD 11)
#find the QtWidgets library
#find_package(Qt5Widgets)

find_package(Qt5 REQUIRED COMPONENTS
        Core
        Sql
        Widgets
        Gui
        )

#take all .c files from src folder and add them to the SOURCE_FILE variable
file(GLOB SOURCE_FILES src/*.cpp src/DataBase/*.cpp src/DataBase/SGX/*.cpp src/Rendering/*.cpp)

#take all .h files from include folder and add them to the HEADER_FILES variable
file(GLOB HEADER_FILES include/*.h include/DataBase/*.h include/DataBase/SGX/*.h include/Rendering/*.h include/Utility/*.h)

#set(LIB_DIR_LOCATION /usr/local/lib)
#find_library(SQLite3 REQUIRED NAMES sqlite3 PATHS ${LIB_DIR_LOCATION})
set(INCLUDE_DIRS ${INCLUDE_DIRS} /usr/local/include)
#message(Include: ${INCLUDE_DIRS})

add_compile_options(
    -O0
    -g
    -pedantic
    -Wall
    -Wextra
    #-Werror
    -fsanitize=address
    -fno-omit-frame-pointer
    -fno-optimize-sibling-calls
    )

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")


add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} include/DataBase/SGX/ECallTypes.h include/DataBase/SGX/OCallTypes.h include/DataBase/SGX/CallTypes.h)

#use the widgets/Sql module from Qt 5
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Sql)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
