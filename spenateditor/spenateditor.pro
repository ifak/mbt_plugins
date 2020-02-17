include($$PWD/../mbt_config.pri)

DEFINES += SPENATEDITOR_LIBRARY

PROVIDER = ifak

include(spenateditor_dependencies.pri)
include($$APPCREATOR_PATH/src/appcreatorplugin.pri)

win32-msvc*:QMAKE_CXXFLAGS += -wd4251 -wd4290 -wd4250

#CONFIG(release):DEFINES += QT_NO_DEBUG_OUTPUT

# spenateditor files

DISTFILES += \
    spenateditor.json \
    spenateditor.mimetypes.xml \
    spenateditor.pluginspec.in \
    spenateditor_dependencies.pri \
    examples/spenat/spenat_example_01.spenat \
    examples/spenat/spenat_simple_01.spenat \
    grammars/Spenat.g4 \
    grammars/Spenat.tokens \
    grammars/SpenatLexer.tokens \
    grammars/CommonLexer.g4

FORMS += \
    spenateditorgraphvizsettings.ui \
    spenateditorplacesettings.ui \
    spenateditortransitionsettings.ui \
    spenateditorarcsettings.ui \
    spenateditorbackgroundsettings.ui \
    spenateditorhighlightsettings.ui

RESOURCES += \
    spenateditor.qrc

SOURCES += \
    spenatdocument.cpp \
    spenateditor.cpp \
    spenateditorfactory.cpp \
    spenateditorwidget.cpp \
    spenateditorplugin.cpp \
    spenateditoroutline.cpp \
    spenatcompletionassist.cpp \
    spenathoverhandler.cpp \
    spenathighlighter.cpp \
    spenatoutlinemodel.cpp \
    combuildspenatoutlinemodel.cpp \
    spenatgraphiceditor.cpp \
    spenatgraphicwidget.cpp \
    spenatgraphicdocument.cpp \
    spenatgraphicfactory.cpp \
    spenateditorgraphvizsettingspage.cpp \
    spenateditorplacesettingspage.cpp \
    spenateditortransitionsettingspage.cpp \
    spenateditorarcsettingspage.cpp \
    spenateditorbackgroundsettingspage.cpp \
    spenateditorhighlightsettingspage.cpp

HEADERS += \
    spenateditor.hpp \
    spenateditorfactory.hpp \
    spenateditorwidget.hpp \
    spenateditor_global.hpp \
    spenateditorconstants.hpp \
    spenateditorplugin.hpp \
    spenateditoroutline.hpp \
    spenatdocument.hpp \
    spenatcompletionassist.hpp \
    spenathoverhandler.hpp \
    spenathighlighter.hpp \
    spenatoutlinemodel.hpp \
    combuildspenatoutlinemodel.hpp \
    spenatgraphiceditor.hpp \
    spenatgraphicwidget.hpp \
    spenatgraphicdocument.hpp \
    spenatgraphicfactory.hpp \
    spenateditorgraphvizsettingspage.hpp \
    spenateditorplacesettingspage.hpp \
    spenateditortransitionsettingspage.hpp \
    spenateditorarcsettingspage.hpp \
    spenateditorbackgroundsettingspage.hpp \
    spenateditorhighlightsettingspage.hpp \
    spenateditorpreviewgraph.hpp

win32{
  copyexamples.commands = $(COPY_DIR) \"$$shell_path($$PWD/examples/spenat)\" \"$$shell_path($$IDE_APP_PATH/examples/spenat)\"
  QMAKE_EXTRA_TARGETS += copyexamples
  POST_TARGETDEPS += copyexamples
} else {
  copyexamples.commands = $(COPY_DIR) $$PWD/examples/spenat $$IDE_APP_PATH/examples
  QMAKE_EXTRA_TARGETS += copyexamples
  POST_TARGETDEPS += copyexamples
}
