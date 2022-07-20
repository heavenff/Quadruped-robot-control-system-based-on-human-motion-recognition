QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ckinectreader.cpp \
    copenni.cpp \
    kinect.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    kinect.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += /usr/include
INCLUDEPATH += /home/heavenqian/kinect_driver/OpenNI-Bin-Dev-Linux-x64-v1.5.7.10/Include

LIBS += /home/heavenqian/kinect_driver/OpenNI-Bin-Dev-Linux-x64-v1.5.7.10/Lib/libOpenNI.so
LIBS += /home/heavenqian/kinect_driver/NITE-Bin-Dev-Linux-x64-v1.5.2.23/Bin/libXnVNite_1_5_2.so

LIBS += /home/heavenqian/kinect_driver/OpenNI-Bin-Dev-Linux-x64-v1.5.7.10/Lib/libnimCodecs.so
LIBS += /home/heavenqian/kinect_driver/OpenNI-Bin-Dev-Linux-x64-v1.5.7.10/Lib/libnimMockNodes.so
LIBS += /home/heavenqian/kinect_driver/OpenNI-Bin-Dev-Linux-x64-v1.5.7.10/Lib/libnimRecorder.so
LIBS += /home/heavenqian/kinect_driver/OpenNI-Bin-Dev-Linux-x64-v1.5.7.10/Lib/libOpenNI.jni.so

LIBS += /usr/lib/libOpenNI.so
LIBS += /usr/lib/libXnVNite_1_5_2.so

QMAKE_CFLAGS_ISYSTEM = -I
#解决问题/usr/include/c++/7/cstdlib:75: error: stdlib.h: No such file or directory
#include_next <stdlib.h>
