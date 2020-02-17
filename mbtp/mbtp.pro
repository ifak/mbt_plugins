include($$PWD/../mbt_config.pri)

DEFINES += MBTP_LIBRARY

PROVIDER = ifak

QT += quickwidgets

include(mbtp_dependencies.pri)
include($$APPCREATOR_PATH/src/appcreatorplugin.pri)

win32-msvc*:QMAKE_CXXFLAGS += -wd4251 -wd4290 -wd4250

SOURCES += \
    mbtpattributesdocument.cpp \
    mbtpattributeseditor.cpp \
    mbtpattributesfactory.cpp \
    mbtpplugin.cpp \
    mbtpproject.cpp \
    mbtpmode.cpp \
    mbtptoolchain.cpp \
    mbtprunconfiguration.cpp \
    mbtpbuildstep.cpp \
    mbtpbuildconfiguration.cpp \
    runtime/combuildmbtptestruntime.cpp \
    runtime/mbtpruntime.cpp \
    mbtpprojectimportwizard.cpp \
    mbtpprojectwizard.cpp \
    mbtpgraphicdocument.cpp \
    mbtpgraphiceditor.cpp \
    mbtpgraphicfactory.cpp \
    mbtpreportbuilder.cpp \
    mbtptablemodel.cpp \
    mbtpsettingspage.cpp

HEADERS += \
    mbtpattributesdocument.hpp \
    mbtpattributeseditor.hpp \
    mbtpattributesfactory.hpp \
    mbtpplugin.hpp \
    mbtpconstants.hpp \
    mbtpproject.hpp \
    mbtp_global.hpp \
    mbtpmode.hpp \
    mbtptoolchain.hpp \
    mbtprunconfiguration.hpp \
    mbtpbuildstep.hpp \
    mbtpbuildconfiguration.hpp \
    runtime/combuildmbtptestruntime.hpp \
    runtime/mbtpruntime.hpp \
    mbtpprojectimportwizard.hpp \
    mbtpprojectwizard.hpp \
    mbtpgraphicdocument.hpp \
    mbtpgraphiceditor.hpp \
    mbtpgraphicfactory.hpp \
    mbtpreportbuilder.hpp \
    mbtptablemodel.hpp \
    mbtpsettingspage.hpp

DISTFILES += \
    mbtp.json \
    resultView.qml \
    attributeseditor.qml

RESOURCES += \
    mbtp.qrc

OTHER_FILES += \
    mbtp.mimetypes.xml \
    mbtp.pluginspec.in \
    mbtp_dependencies.pri

FORMS += \
    mbtpsettings.ui
