CONFIG += qt plugin
CONFIG += c++14
CONFIG -= android_install
TARGET_NAME = $$lower($$replace(uri, \\., ))
TARGET = $$qtLibraryTarget($${TARGET_NAME}plugin)
#CONFIG(release, debug|release): TARGET = $$qtLibraryTarget($${TARGET_NAME}plugin)
#CONFIG(debug, debug|release): TARGET = $$qtLibraryTarget($${TARGET_NAME}plugind)
QMAKE_MOC_OPTIONS += -Muri=$$uri
