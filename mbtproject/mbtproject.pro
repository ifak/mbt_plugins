include($$PWD/../mbt_config.pri)

DEFINES += MBTPROJECT_LIBRARY

PROVIDER = ifak

include(mbtproject_dependencies.pri)
include($$APPCREATOR_PATH/src/appcreatorplugin.pri)

win32-msvc*:QMAKE_CXXFLAGS += -wd4251 -wd4290 -wd4250

#CONFIG(release):DEFINES += QT_NO_DEBUG_OUTPUT

# mbtproject files

DISTFILES += \
    mbtproject.json \
    mbtproject.json \
    mbtproject.mimetypes.xml \
    mbtproject.pluginspec.in \
    template_decl.decl \
    template_spenat.spenat \
    template_statemachine.statemachine \
    mbtspenatsettingspage.ui

RESOURCES += \
    mbtproject.qrc

OTHER_FILES += \
    mbtproject.mimetypes.xml \
    mbtproject.pluginspec.in \
    mbtproject_dependencies.pri

win32{
  copyexamples.commands = $(COPY_DIR) \"$$shell_path($$PWD/examples)\" \"$$shell_path($$IDE_APP_PATH/examples)\"
  QMAKE_EXTRA_TARGETS += copyexamples
  POST_TARGETDEPS += copyexamples
} else {
  copyexamples.commands = $(COPY_DIR) $$PWD/examples $$IDE_APP_PATH
  QMAKE_EXTRA_TARGETS += copyexamples
  POST_TARGETDEPS += copyexamples
}

HEADERS += \
    mbtproject_global.hpp \
    mbtproject.hpp \
    mbtprojectconstants.hpp \
    mbtprojectplugin.hpp \
    mbtspecmodelwizard.hpp \
    mbtspecmodel.hpp \
    mbtspenatmodel.hpp \
    mbtstatemachinemodel.hpp \
    mbtprojectwizard.hpp \
    mbtprojecttoolchain.hpp \
    mbtbuildstep.hpp \
    mbtbuildconfiguration.hpp \
    mbtrunconfiguration.hpp \
    mbtruncontrol.hpp \
    mbtspecmodelview.hpp \
    combuildmbtprojectmodel.hpp \
    mbtprojectmodel.hpp \
    mbtprojectconfiguration.hpp \
    mbtspecmodelsettings.hpp \
    mbtruntime.hpp \
    tcgcoverageproperty.hpp \
    comexporttestsuite.hpp \
    mbtmode.hpp

SOURCES += \
    mbtproject.cpp \
    mbtprojectplugin.cpp \
    mbtspecmodelwizard.cpp \
    mbtspecmodel.cpp \
    mbtspenatmodel.cpp \
    mbtstatemachinemodel.cpp \
    mbtprojectwizard.cpp \
    mbtprojecttoolchain.cpp \
    mbtbuildstep.cpp \
    mbtbuildconfiguration.cpp \
    mbtrunconfiguration.cpp \
    mbtruncontrol.cpp \
    mbtspecmodelview.cpp \
    combuildmbtprojectmodel.cpp \
    mbtprojectmodel.cpp \
    mbtprojectconfiguration.cpp \
    mbtspecmodelsettings.cpp \
    mbtruntime.cpp \
    tcgcoverageproperty.cpp \
    comexporttestsuite.cpp \
    mbtmode.cpp

#win32{
#  copyexamples.commands = $(COPY_DIR) \"$$PWD/examples/testcase\" \"$$IDE_APP_PATH/examples/testcase\"
#  QMAKE_EXTRA_TARGETS += copyexamples
#  POST_TARGETDEPS += copyexamples
#}
#linux-g++-64 {
#  copyexamples.commands = $(COPY_DIR) $$PWD/bin/linux64 $$IDE_APP_PATH/bin
#  QMAKE_EXTRA_TARGETS += copyexamples
#  POST_TARGETDEPS += copyexamples
#}

#linux*{
#  copybindir.commands = $(COPY_DIR) $$PWD/bin/linux/* $$IDE_APP_PATH/bin
#  QMAKE_EXTRA_TARGETS += copybindir
#  POST_TARGETDEPS += copybindir
#}
