#-------------------------------------------------
#
# Project created by QtCreator 2014-07-06T12:08:44
#
#-------------------------------------------------

VERSION   = 2.0.2
QT       += core gui network svg
CODECFORSRC = UTF-8
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = kuro-player
TEMPLATE = app

CONFIG += c++11 link_pkgconfig
PKGCONFIG += mpv

DESTDIR = build
OBJECTS_DIR = $${DESTDIR}/obj
MOC_DIR = $${DESTDIR}/moc
RCC_DIR = $${DESTDIR}/rcc
UI_DIR = $${DESTDIR}/ui

unix {
    QT += x11extras
    PKGCONFIG += x11

    SOURCES += platform/linux.cpp
}

win32 {
    QT += winextras
    PKGCONFIG += libzip

    # mxe fix:
    CONFIG -= windows
    QMAKE_LFLAGS += $$QMAKE_LFLAGS_WINDOWS -pthread

    # xp (minimum version)
    DEFINES += "WINVER=0x0501" \
               "_WIN32_WINNT=0x0501"

    # application information
    RC_ICONS += img/logo.ico
    QMAKE_TARGET_PRODUCT += Kuro Player
    QMAKE_TARGET_DESCRIPTION += Kuro Player
    #RC_LANG +=

    SOURCES += platform/win.cpp
    RESOURCES += win_rsclist.qrc

    # 32 bit
    contains(QMAKE_HOST.arch, x86): SOURCES += platform/win32.cpp
    # 64 bit
    contains(QMAKE_HOST.arch, x86_64): SOURCES += platform/win64.cpp
}

# INSTROOT is the installation root directory, leave empty if not using a package management system
isEmpty(BINDIR):BINDIR=/usr/bin
isEmpty(MEDIADIR):MEDIADIR=/usr/share/pixmaps
isEmpty(APPDIR):APPDIR=/usr/share/applications
isEmpty(DOCDIR):DOCDIR=/usr/share/doc
isEmpty(MANDIR):MANDIR=/usr/share/man
isEmpty(LICENSEDIR):LICENSEDIR=/usr/share/licenses
isEmpty(KURODIR):KURODIR=/usr/share/kuro-player

target.path = $$INSTROOT$$BINDIR
logo.path = $$INSTROOT$$MEDIADIR
desktop.path = $$INSTROOT$$APPDIR
manual.path = $$INSTROOT$$DOCDIR/kuro-player
man.path = $$INSTROOT$$MANDIR/man1
license.path = $$INSTROOT$$LICENSEDIR/kuro-player
translations.path = $$INSTROOT$$KURODIR/translations

logo.files = ../etc/logo/kuro-player.svg
desktop.files = ../etc/kuro-player.desktop
manual.files = ../etc/doc/kuro-player.md
man.files = ../etc/doc/kuro-player.1.gz
license.files = ../LICENSE

INSTALLS += target logo desktop manual man license

RESOURCES += rsclist.qrc

isEmpty(TRANSLATIONS) {
    include(translations.pri)
}

TRANSLATIONS_COMPILED = $$TRANSLATIONS
TRANSLATIONS_COMPILED ~= s/\.ts/.qm/g

CONFIG(embed_translations) {
    # create translations resource file
    system("echo \'<RCC><qresource prefix=\"/\">\' > translations.qrc")
    for(translation, TRANSLATIONS_COMPILED):system("echo \'<file>$$translation</file>\' >> translations.qrc")
    system("echo \'</qresource></RCC>\'" >> translations.qrc)

    # add file to build
    RESOURCES += translations.qrc

    KURO_LANG_PATH += :/translations

    # make sure translations are updated and released
    CONFIG *= update_translations release_translations
}

CONFIG(install_translations) {
    # install translation files
    translations.files = $$TRANSLATIONS_COMPILED
    INSTALLS += translations

    KURO_LANG_PATH += $$KURODIR/translations

    # make sure translations are updated and released
    CONFIG *= update_translations release_translations
}

CONFIG(begin_translations) {
    isEmpty(lupdate):lupdate=lupdate
    system($$lupdate -locations absolute $$_PRO_FILE_)
}

CONFIG(update_translations) {
    isEmpty(lupdate):lupdate=lupdate
    system($$lupdate -locations none $$_PRO_FILE_)
}

CONFIG(release_translations) {
    isEmpty(lrelease):lrelease=lrelease
    system($$lrelease $$_PRO_FILE_)
}


isEmpty(SETTINGS_FILE):SETTINGS_FILE=kuro-player
DEFINES += "KURO_PLAYER_VERSION=\\\"$$VERSION\\\"" \
           "SETTINGS_FILE=\\\"$$SETTINGS_FILE\\\"" \
           "KURO_PLAYER_LANG_PATH=\\\"$$KURO_LANG_PATH\\\""
!isEmpty(KURO_LANG):DEFINES += "KURO_PLAYER_LANG=\\\"$$KURO_LANG\\\""

SOURCES += main.cpp\
    kuroengine.cpp \
    kurocommands.cpp \
    mpvhandler.cpp \
    updatemanager.cpp \
    gesturehandler.cpp \
    overlayhandler.cpp \
    util.cpp \
    settings.cpp \
    versions/1_9_9.cpp \
    versions/2_0_0.cpp \
    versions/2_0_1.cpp \
    versions/2_0_2.cpp \
    widgets/customlabel.cpp \
    widgets/customlineedit.cpp \
    widgets/customslider.cpp \
    widgets/customsplitter.cpp \
    widgets/dimdialog.cpp \
    widgets/indexbutton.cpp \
    widgets/openbutton.cpp \
    widgets/playlistwidget.cpp \
    widgets/seekbar.cpp \
    ui/aboutdialog.cpp \
    ui/inputdialog.cpp \
    ui/jumpdialog.cpp \
    ui/locationdialog.cpp \
    ui/mainwindow.cpp \
    ui/preferencesdialog.cpp \
    ui/screenshotdialog.cpp \
    ui/updatedialog.cpp \
    ui/keydialog.cpp

HEADERS  += \
    kuroengine.h \
    mpvhandler.h \
    mpvtypes.h \
    updatemanager.h \
    gesturehandler.h \
    overlayhandler.h \
    util.h \
    settings.h \
    widgets/customlabel.h \
    widgets/customlineedit.h \
    widgets/customslider.h \
    widgets/customsplitter.h \
    widgets/dimdialog.h \
    widgets/indexbutton.h \
    widgets/openbutton.h \
    widgets/playlistwidget.h \
    widgets/seekbar.h \
    ui/aboutdialog.h \
    ui/inputdialog.h \
    ui/jumpdialog.h \
    ui/locationdialog.h \
    ui/mainwindow.h \
    ui/preferencesdialog.h \
    ui/screenshotdialog.h \
    ui/updatedialog.h \
    ui/keydialog.h

FORMS    += \
    ui/aboutdialog.ui \
    ui/inputdialog.ui \
    ui/jumpdialog.ui \
    ui/locationdialog.ui \
    ui/mainwindow.ui \
    ui/preferencesdialog.ui \
    ui/screenshotdialog.ui \
    ui/updatedialog.ui \
    ui/keydialog.ui
