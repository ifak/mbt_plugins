include($$PWD/../mbt_config.pri)

DEFINES += COMMONDECLEDITOR_LIBRARY

PROVIDER = ifak

include(commondecleditor_dependencies.pri)
include($$APPCREATOR_PATH/src/appcreatorplugin.pri)

win32-msvc*:QMAKE_CXXFLAGS += -wd4251 -wd4290 -wd4250

#CONFIG(release):DEFINES += QT_NO_DEBUG_OUTPUT

# commondecleditor files


win32{
  copyexamples.commands = $(COPY_DIR) \"$$shell_path($$PWD/examples/commondecl)\" \"$$shell_path($$IDE_APP_PATH/examples/commondecl)\"
  QMAKE_EXTRA_TARGETS += copyexamples
  POST_TARGETDEPS += copyexamples
} else {
  copyexamples.commands = $(COPY_DIR) $$PWD/examples/commondecl $$IDE_APP_PATH/examples
  QMAKE_EXTRA_TARGETS += copyexamples
  POST_TARGETDEPS += copyexamples
}

DISTFILES += \
    commondecleditor.json \
    commondecleditor_dependencies.pri \
    commondecleditor.mimetypes.xml \
    examples/commondecl/commondecl_example_01.decl \
    examples/commondecl/commondecl_simple_01.decl \
    commondecleditor.pluginspec.in

RESOURCES += \
    commondecleditor.qrc

HEADERS += \
    combuildcommondecloutlinemodel.hpp \
    commondeclautocompleter.hpp \
    commondeclcodeformatter.hpp \
    commondeclcompletionassist.hpp \
    commondecldocument.hpp \
    commondecleditor.hpp \
    commondecleditor_global.hpp \
    commondecleditorconstants.hpp \
    commondecleditorfactory.hpp \
    commondecleditoroutline.hpp \
    commondecleditorplugin.hpp \
    commondecleditorwidget.hpp \
    commondeclhighlighter.hpp \
    commondeclhoverhandler.hpp \
    commondeclindenter.hpp \
    commondecloutlinemodel.hpp

SOURCES += \
    combuildcommondecloutlinemodel.cpp \
    commondeclautocompleter.cpp \
    commondeclcodeformatter.cpp \
    commondeclcompletionassist.cpp \
    commondecldocument.cpp \
    commondecleditor.cpp \
    commondecleditorfactory.cpp \
    commondecleditoroutline.cpp \
    commondecleditorplugin.cpp \
    commondecleditorwidget.cpp \
    commondeclhighlighter.cpp \
    commondeclhoverhandler.cpp \
    commondecloutlinemodel.cpp
