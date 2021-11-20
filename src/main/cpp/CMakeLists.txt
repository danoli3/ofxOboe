# Sets the minimum version of CMake required to build the native
# library.
cmake_minimum_required(VERSION 3.10.2)

#project(superhexagon LANGUAGES CXX)

set(LOCAL_PATH ${CMAKE_SOURCE_DIR})
set(PRJ_OF_ROOT ${LOCAL_PATH}/../../../../../../../)

set(PURE_OF_ROOT ${LOCAL_PATH}/../../../../../../../)
set(CORE_OF_ROOT ${PURE_OF_ROOT}/libs/openFrameworks)
set(LIBS_ROOT ${PURE_OF_ROOT}/libs)

set(PRJ_ADDONS_PATH ${PURE_OF_ROOT}/addons)
set(PRJ_SOURCE_PATH ${LIBS_ROOT}/openFrameworks)
set(PRJ_LIBS_ROOT ${PURE_OF_ROOT}/libs)

set(OF_ANDROID ${PURE_OF_ROOT}/libs/openFrameworksCompiled/project/android)
set(OF_ANDROID_OUTPUT ${PURE_OF_ROOT}/libs/openFrameworksCompiled/lib/android)


set(PRJ_OFX_ANDROID_PATH ${PRJ_ADDONS_PATH}/ofxAndroid)
set(PRJ_OFX_ANDROID_CPP_PATH ${PRJ_OFX_ANDROID_PATH}/src)

set(SUPER_HEXAGON_SRC ${CMAKE_SOURCE_DIR}/../../../../../superhexagon-windows/src/)

macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c17 -Oz -DNDEBUG -DSUPHEX_ANDROID -frtti -Wall -fno-short-enums -fPIE -fPIC -fuse-ld=gold -fexceptions -ffunction-sections -fdata-sections")
set(CMAKE_CPP_FLAGS "${CMAKE_C_FLAGS} -std=c++17 -Oz -DNDEBUG -DSUPHEX_ANDROID -stdlib=libc++ -frtti -Wall -fno-short-enums -fPIE -fPIC -fuse-ld=gold -fexceptions -ffunction-sections -fdata-sections")


print_all_variables()

# Creates the project's shared lib: libnative-lib.so.
# The lib is loaded by this project's Java code in MainActivity.java:
#     System.loadLibrary("native-lib");
# The lib name in both places must match.
add_library( superhexagon #name
        SHARED # type
        # src
        ${SUPER_HEXAGON_SRC}/superhex.cpp
        ${SUPER_HEXAGON_SRC}/binaryblob.cpp
        ${SUPER_HEXAGON_SRC}/deviceclass.cpp
        ${SUPER_HEXAGON_SRC}/enemyclass.cpp
        ${SUPER_HEXAGON_SRC}/gameclass.cpp
        ${SUPER_HEXAGON_SRC}/global.cpp
        ${SUPER_HEXAGON_SRC}/graphicsclass.cpp
        ${SUPER_HEXAGON_SRC}/help.cpp
        ${SUPER_HEXAGON_SRC}/input.cpp
        ${SUPER_HEXAGON_SRC}/keypoll.cpp
        ${SUPER_HEXAGON_SRC}/Leaderboards.cpp
        ${SUPER_HEXAGON_SRC}/logic.cpp
        ${SUPER_HEXAGON_SRC}/main.cpp
        ${SUPER_HEXAGON_SRC}/musicclass.cpp
        ${SUPER_HEXAGON_SRC}/ofxShapeBatchRenderer.cpp
        ${SUPER_HEXAGON_SRC}/render.cpp
        ${SUPER_HEXAGON_SRC}/scoreclass.cpp
        ${SUPER_HEXAGON_SRC}/scoreloop.cpp
        ${SUPER_HEXAGON_SRC}/ScreenshotTaker.cpp
        ${SUPER_HEXAGON_SRC}/utils.cpp
        ${SUPER_HEXAGON_SRC}/events.cpp
        ${CMAKE_SOURCE_DIR}/ofxOboe.cpp
        ${CMAKE_SOURCE_DIR}/ofxOboeAndroidSoundPlayer.cpp
        ${CMAKE_SOURCE_DIR}/ofxOboeNDKDecoder.cpp
        ${CMAKE_SOURCE_DIR}/ofxOboeAssetDataSource.cpp
        ${CMAKE_SOURCE_DIR}/ofxOboePlayer.cpp
        ${CMAKE_SOURCE_DIR}/ofxOboeMixer.cpp
        )


# Specifies a path to native header files.
include_directories(
        ${PRJ_SOURCE_PATH}/3d
        ${PRJ_SOURCE_PATH}/app
        ${PRJ_SOURCE_PATH}/communication
        ${PRJ_SOURCE_PATH}/events
        ${PRJ_SOURCE_PATH}/gl
        ${PRJ_SOURCE_PATH}/graphics
        ${PRJ_SOURCE_PATH}/math
        ${PRJ_SOURCE_PATH}/sound
        ${PRJ_SOURCE_PATH}/types
        ${PRJ_SOURCE_PATH}/utils
        ${PRJ_SOURCE_PATH}/video
        ${PRJ_SOURCE_PATH}
        ${PURE_OF_ROOT}/addons/ofxAndroid/src
        ${PURE_OF_ROOT}/addons/ofxAccelerometer/src
        ${PURE_OF_ROOT}/addons/ofxXmlSettings/src
        ${PURE_OF_ROOT}/addons/ofxXmlSettings/libs
        ${PRJ_LIBS_ROOT}/FreeImage/include
        ${PRJ_LIBS_ROOT}/freetype/include
        ${PRJ_LIBS_ROOT}/freetype/include/freetype2
        ${PRJ_LIBS_ROOT}/freetype/include/freetype2/freetype/config
        ${PRJ_LIBS_ROOT}/freetype/include/freetype2/freetype/internal
        ${PRJ_LIBS_ROOT}/freetype/include/freetype2/freetype/internal/services
        ${PRJ_LIBS_ROOT}/glm/include
        ${PRJ_LIBS_ROOT}/pugixml/include
        ${PRJ_LIBS_ROOT}/json/include
        ${PRJ_LIBS_ROOT}/tess2/include
        ${PRJ_LIBS_ROOT}/utf8/include
        ${PRJ_LIBS_ROOT}/uriparser/include
        ${PRJ_LIBS_ROOT}/uriparser/include
        ${CMAKE_SOURCE_DIR}/
        ${OF_ANDROID}
)

find_library(android-lib android)
find_library(log-lib log)
find_library(GLES1-lib GLESv1_CM)
find_library(GLES2-lib GLESv2)
find_library(GLES3-lib GLESv3)
find_library(GLES3-lib GLESv3)
find_library(MediaandK mediandk)
find_library(OpenSLES-lib OpenSLES)
find_package(oboe REQUIRED CONFIG)
#find_package(games-controller REQUIRED CONFIG)

target_link_libraries(superhexagon
        EGL
        GLESv2
        GLESv3
        log
        c
        m
        z
        dl
        )

target_link_libraries( superhexagon
        ${android-lib} )
target_link_libraries( superhexagon
        ${GLES2-lib} )
target_link_libraries( superhexagon
        ${GLES3-lib} )
target_link_libraries( superhexagon
        ${GLES1-lib} )
target_link_libraries( superhexagon
        ${log-lib} )
target_link_libraries( superhexagon
        ${OpenSLES-lib} )
target_link_libraries( superhexagon
        ${MediaandK} )

target_link_libraries( superhexagon
        ${OF_ANDROID_OUTPUT}/${ANDROID_ABI}/libopenFrameworksAndroid.so)

target_link_libraries(superhexagon
        oboe::oboe)


