QT       += core gui multimedia network

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
    clients.cpp \
    clients_list.cpp \
    extrainfo.cpp \
    ham_options.cpp \
    information_box.cpp \
    inout.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    modify.cpp \
    new.cpp \
    new_client.cpp \
    nights.cpp \
    no_task.cpp \
    notebook.cpp \
    page_1.cpp \
    page_2.cpp \
    records.cpp \
    reservas.cpp \
    tasks.cpp \
    update_benefits.cpp \
    update_client.cpp

HEADERS += \
    clients.h \
    clients_list.h \
    extrainfo.h \
    ham_options.h \
    information_box.h \
    inout.h \
    login.h \
    mainwindow.h \
    modify.h \
    new.h \
    new_client.h \
    nights.h \
    no_task.h \
    notebook.h \
    page_1.h \
    page_2.h \
    records.h \
    reservas.h \
    tasks.h \
    update_benefits.h \
    update_client.h

FORMS += \
    clients.ui \
    clients_list.ui \
    extrainfo.ui \
    ham_options.ui \
    information_box.ui \
    inout.ui \
    login.ui \
    mainwindow.ui \
    modify.ui \
    new.ui \
    new_client.ui \
    nights.ui \
    no_task.ui \
    notebook.ui \
    page_1.ui \
    page_2.ui \
    records.ui \
    reservas.ui \
    tasks.ui \
    update_benefits.ui \
    update_client.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_FILE = clientes_corporativos.rc

RESOURCES += \
    sources.qrc
