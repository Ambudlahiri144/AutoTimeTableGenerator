QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = QtLanguage
TEMPLATE = app

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    GeneticCore/uniquepriorityqueue.cpp \
    Tests/test.cpp \
    main.cpp \
    mainwindow.cpp \
    mycheckbox.cpp \
    parameterwidget.cpp \
    ruleform.cpp \
    selectordialog.cpp \
    GeneticCore/timetable/timetable.cpp \
    GeneticCore/ga/ga.cpp

HEADERS += \
    GeneticCore/uniquepriorityqueue.h \
    Tests/doctest.h \
    mainwindow.h \
    mycheckbox.h \
    parameterwidget.h \
    ruleform.h \
    selectordialog.h \
    GeneticCore/timetable/timetable.h \
    GeneticCore/ga/ga.h

FORMS += \
    mainwindow.ui \
    parameterwidget.ui \
    ruleform.ui \
    selectordialog.ui \


TRANSLATIONS += \
    QtLanguage_ru.ts \
    QtLanguage_en.ts


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Tests/doctest.h.gch

RESOURCES +=
