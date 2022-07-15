QT       += core gui sql multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#QMAKE_CXXFLAGS += /utf-8

RC_FILE = resource\image\icon\icon.rc

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Ui/LocationGame/endwidget.cpp \
    Ui/LocationGame/gamewidget.cpp \
    Ui/LocationGame/hallwidget.cpp \
    Ui/LocationGame/loglocationwidget.cpp \
    Ui/LocationGame/regwidget.cpp \
    Ui/LocationGame/stopwidget.cpp \
    Ui/mainwidget.cpp \
    Ui/option/locationwidget.cpp \
    Ui/option/onlinewidget.cpp \
    Ui/option/settingwidget.cpp \
    Ui/option/startwidget.cpp \
    controller/LocationGame/bulletcontroller.cpp \
    controller/LocationGame/charactercontroller.cpp \
    controller/LocationGame/monstercontroller.cpp \
    controller/LocationGame/propcontroller.cpp \
    controller/LocationGame/thread/hitbarrierthread.cpp \
    controller/LocationGame/thread/hitcharacterthread.cpp \
    controller/LocationGame/thread/hitmonsterthread.cpp \
    controller/LocationGame/thread/monsterthread.cpp \
    controller/LocationGame/thread/pickthingsthread.cpp \
    controller/LocationGame/viewwidget.cpp \
    controller/inputting.cpp \
    controller/mediaController/mediacontroller.cpp \
    main.cpp \
    model/bodylabel.cpp \
    model/box.cpp \
    model/bullet.cpp \
    model/character.cpp \
    model/coin.cpp \
    model/element.cpp \
    model/magicpoint.cpp \
    model/map.cpp \
    model/message.cpp \
    model/monsterbullet.cpp \
    model/monstercharacter.cpp \
    model/smallmap.cpp \
    model/weapon.cpp \
    tools/myanimation.cpp

HEADERS += \
    Ui/LocationGame/endwidget.h \
    Ui/LocationGame/gamewidget.h \
    Ui/LocationGame/hallwidget.h \
    Ui/LocationGame/loglocationwidget.h \
    Ui/LocationGame/regwidget.h \
    Ui/LocationGame/stopwidget.h \
    Ui/mainwidget.h \
    Ui/option/locationwidget.h \
    Ui/option/onlinewidget.h \
    Ui/option/settingwidget.h \
    Ui/option/startwidget.h \
    controller/LocationGame/bulletcontroller.h \
    controller/LocationGame/charactercontroller.h \
    controller/LocationGame/monstercontroller.h \
    controller/LocationGame/propcontroller.h \
    controller/LocationGame/thread/hitbarrierthread.h \
    controller/LocationGame/thread/hitcharacterthread.h \
    controller/LocationGame/thread/hitmonsterthread.h \
    controller/LocationGame/thread/monsterthread.h \
    controller/LocationGame/thread/pickthingsthread.h \
    controller/LocationGame/viewwidget.h \
    controller/inputting.h \
    controller/mediaController/mediacontroller.h \
    model/bodylabel.h \
    model/box.h \
    model/bullet.h \
    model/character.h \
    model/coin.h \
    model/element.h \
    model/magicpoint.h \
    model/map.h \
    model/message.h \
    model/monsterbullet.h \
    model/monstercharacter.h \
    model/smallmap.h \
    model/weapon.h \
    tools/myanimation.h

FORMS += \
    Ui/LocationGame/endwidget.ui \
    Ui/LocationGame/gamewidget.ui \
    Ui/LocationGame/hallwidget.ui \
    Ui/LocationGame/loglocationwidget.ui \
    Ui/LocationGame/regwidget.ui \
    Ui/LocationGame/stopwidget.ui \
    Ui/mainwidget.ui \
    Ui/option/locationwidget.ui \
    Ui/option/onlinewidget.ui \
    Ui/option/settingwidget.ui \
    Ui/option/startwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resouces.qrc
