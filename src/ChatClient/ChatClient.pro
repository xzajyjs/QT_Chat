QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
CONFIG += c++11, sdk_no_version_check

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    clientdialog.cpp \
    logindialog.cpp \
    myprofiledialog.cpp \
    registerdialog.cpp \
    forgetpasswddialog.cpp \
    menuwidget.cpp

HEADERS += \
    clientdialog.hpp \
    logindialog.hpp \
    myprofiledialog.hpp \
    registerdialog.hpp \
    forgetpasswddialog.hpp \
    menuwidget.hpp

FORMS += \
    clientdialog.ui \
    logindialog.ui \
    myprofiledialog.ui \
    registerdialog.ui \
    forgetpasswddialog.ui \
    menuwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pic.qrc
