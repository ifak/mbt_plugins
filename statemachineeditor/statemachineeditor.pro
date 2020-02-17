include($$PWD/../mbt_config.pri)

DEFINES += STATEMACHINEEDITOR_LIBRARY

PROVIDER = ifak

include(statemachineeditor_dependencies.pri)
include($$APPCREATOR_PATH/src/appcreatorplugin.pri)

win32-msvc*:QMAKE_CXXFLAGS += -wd4251 -wd4290 -wd4250

#CONFIG(release):DEFINES += QT_NO_DEBUG_OUTPUT

# statemachineeditor files

DISTFILES += \
    statemachineeditor.json \
    statemachineeditor.mimetypes.xml \
    statemachineeditor.pluginspec.in \
    statemachineeditor_dependencies.pri \
    examples/statemachine/statemachine_example_01.statemachine \
    examples/statemachine/statemachine_simple_01.statemachine \
    grammars/statemachine.g4 \
    grammars/statemachine.tokens \
    grammars/statemachineLexer.tokens \
    grammars/CommonLexer.g4

FORMS += \
    statemachineeditorgraphvizsettings.ui \
    statemachineeditortransitionsettings.ui \
    statemachineeditorsimplestatesettings.ui \
    statemachineeditorbackgroundsettings.ui \
    statemachineeditorotherstatesettings.ui \
    statemachineeditorhighlightsettings.ui

RESOURCES += \
    statemachineeditor.qrc

SOURCES += \
    statemachinedocument.cpp \
    statemachineeditor.cpp \
    statemachineeditorfactory.cpp \
    statemachineeditorwidget.cpp \
    statemachineeditorplugin.cpp \
    statemachineeditoroutline.cpp \
    statemachinecompletionassist.cpp \
    statemachinehoverhandler.cpp \
    statemachinehighlighter.cpp \
    statemachineoutlinemodel.cpp \
    combuildstatemachineoutlinemodel.cpp \
    statemachinegraphiceditor.cpp \
    statemachinegraphicwidget.cpp \
    statemachinegraphicdocument.cpp \
    statemachinegraphicfactory.cpp \
    statemachineeditorgraphvizsettingspage.cpp \
    statemachineeditortransitionsettingspage.cpp \
    statemachineeditorsimplestatesettingspage.cpp \
    statemachineeditorbackgroundsettingspage.cpp \
    statemachineeditorotherstatesettingspage.cpp \
    statemachineeditorhighlightsettingspage.cpp

HEADERS += \
    statemachineeditor.hpp \
    statemachineeditorfactory.hpp \
    statemachineeditorwidget.hpp \
    statemachineeditor_global.hpp \
    statemachineeditorconstants.hpp \
    statemachineeditorplugin.hpp \
    statemachineeditoroutline.hpp \
    statemachinedocument.hpp \
    statemachinecompletionassist.hpp \
    statemachinehoverhandler.hpp \
    statemachinehighlighter.hpp \
    statemachineoutlinemodel.hpp \
    combuildstatemachineoutlinemodel.hpp \
    statemachinegraphiceditor.hpp \
    statemachinegraphicwidget.hpp \
    statemachinegraphicdocument.hpp \
    statemachinegraphicfactory.hpp \
    statemachineeditorgraphvizsettingspage.hpp \
    statemachineeditortransitionsettingspage.hpp \
    statemachineeditorsimplestatesettingspage.hpp \
    statemachineeditorbackgroundsettingspage.hpp \
    statemachineeditorotherstatesettingspage.hpp \
    statemachineeditorhighlightsettingspage.hpp \
    statemachineeditorpreviewgraph.hpp

win32{
  copyexamples.commands = $(COPY_DIR) \"$$shell_path($$PWD/examples/statemachine)\" \"$$shell_path($$IDE_APP_PATH/examples/statemachine)\"
  QMAKE_EXTRA_TARGETS += copyexamples
  POST_TARGETDEPS += copyexamples
} else {
  copyexamples.commands = $(COPY_DIR) $$PWD/examples/statemachine $$IDE_APP_PATH/examples
  QMAKE_EXTRA_TARGETS += copyexamples
  POST_TARGETDEPS += copyexamples
}

SUBDIRS += \
    ../irdleditor/irdleditor.pro
