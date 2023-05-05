QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_ICONS = 128_logo.ico
VERSION = 23.04.26.01
QMAKE_TARGET_COMPANY=AGICall
QMAKE_TARGET_DESCRIPTION=usb QQ:9421766 Tel:13810450502
QMAKE_TARGET_COPYRIGHT=AGICall QQ:9421766
QMAKE_TARGET_PRODUCT=AGI usb

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    3rdparty/include/libagi_ub.h \
    3rdparty/include/libwtevt_json.h \
    3rdparty/include/wtextern.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    agi_ub_pro_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -L$$PWD/3rdparty/lib/ -lagi_ub -lagi_evt

INCLUDEPATH += $$PWD/3rdparty/include
DEPENDPATH += $$PWD/3rdparty/include

