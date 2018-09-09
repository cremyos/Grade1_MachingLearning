QT -= gui



INCLUDEPATH += E:/software/QT/OpencvUSE/include/opencv \
               E:/software/QT/OpencvUSE/opencv2 \
               E:/software/QT/OpencvUSE/include

LIBS +=E:/software/QT/OpencvUSE/lib/libopencv_calib3d300.dll.a
LIBS +=E:/software/QT/OpencvUSE/lib/libopencv_photo300.dll.a
LIBS +=E:/software/QT/OpencvUSE/lib/libopencv_objdetect300.dll.a
LIBS +=E:/software/QT/OpencvUSE/lib/libopencv_ml300.dll.a
LIBS +=E:/software/QT/OpencvUSE/lib/libopencv_video300.dll.a
LIBS +=E:/software/QT/OpencvUSE/lib/libopencv_features2d300.dll.a
LIBS +=E:/software/QT/OpencvUSE/lib/libopencv_highgui300.dll.a
LIBS +=E:/software/QT/OpencvUSE/lib/libopencv_imgproc300.dll.a
LIBS +=E:/software/QT/OpencvUSE/lib/libopencv_flann300.dll.a
LIBS +=E:/software/QT/OpencvUSE/lib/libopencv_core300.dll.a
LIBS +=E:/software/QT/OpencvUSE/lib/libopencv_highgui300.dll.a
LIBS +=E:/software/QT/OpencvUSE/lib/libopencv_imgcodecs300.dll.a
LIBS +=E:/software/QT/OpencvUSE/lib/libopencv_video300.dll.a
LIBS +=E:/software/QT/OpencvUSE/lib/libopencv_videoio300.dll.a
LIBS +=E:/software/QT/OpencvUSE/lib/libopencv_videostab300.dll.a

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp
