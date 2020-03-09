#-------------------------------------------------
#
# Project created by QtCreator 2020-02-09T19:24:35
#
#-------------------------------------------------

QT       += core gui network androidextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts

TARGET = GreenEar
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        ui/zmainui.cpp \
    ui/zvideoui.cpp \
    thread/zcommunicatethread.cpp \
    zgblparam.cpp \
    thread/ztimeoutscanthread.cpp \
    protocol/zmodbusprotocol.cpp \
    greenear.cpp \
    ui/zcurvedialog.cpp

HEADERS += \
        ui/zmainui.h \
    ui/zvideoui.h \
    thread/zcommunicatethread.h \
    zgblparam.h \
    thread/ztimeoutscanthread.h \
    protocol/zmodbusprotocol.h \
    greenear.h \
    ui/zcurvedialog.h

RESOURCES += \
    appres.qrc


CONFIG += mobility
MOBILITY = 


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/src/app/of/domain/OutVolume.java

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
