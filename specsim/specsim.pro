include($$PWD/../mbt_config.pri)

DEFINES += SPECSIM_LIBRARY

PROVIDER = ifak

include(specsim_dependencies.pri)
include($$APPCREATOR_PATH/src/appcreatorplugin.pri)

#win32-msvc*:QMAKE_CXXFLAGS += -wd4251 -wd4290 -wd4250

DISTFILES += \
    specsim.json \
    specsim_dependencies.pri \
    specsim.pluginspec.in \
    specsim.mimetypes.xml \
    template_decl.decl \
    template_statemachine.statemachine \
    template_spenat.spenat \
    template_decl.decl \
    template_statemachine.statemachine \
    template_spenat.spenat

RESOURCES += \
    specsim.qrc

HEADERS += \
    specsim_global.hpp \
    specsimplugin.hpp \
    specsimmode.hpp \
    specsimconstants.hpp \
    specsimproject.hpp \
    specsimbuildconfiguration.hpp \
    specsimprojectconfiguration.hpp \
    specsimrunconfiguration.hpp \
    specsimbuildstep.hpp \
    specsimprojecttoolchain.hpp \
    specsimmodelwizard.hpp \
    specsimprojectwizard.hpp \
    specsimmodel.hpp \
    specsimmodelsettings.hpp \
    specsimruncontrol.hpp \
    specsimprojectmodel.hpp \
    specsimruntime.hpp \
    specsimruntimeview.hpp \
    combuildspecsimprojectmodel.hpp \
    simsystemoutlinemodel.hpp \
    simsystemeditoroutline.hpp \
    simsystemeditorfactory.hpp \
    simsystemeditor.hpp \
    simsystemdocument.hpp \
    combuildsimsystemoutlinemodel.hpp \
    simsystemeditorwidget.hpp \
    simsystemautocompleter.hpp \
    simsystemcompletionassist.hpp

SOURCES += \
    specsimplugin.cpp \
    specsimmode.cpp \
    specsimproject.cpp \
    specsimbuildconfiguration.cpp \
    specsimprojectconfiguration.cpp \
    specsimrunconfiguration.cpp \
    specsimbuildstep.cpp \
    specsimprojecttoolchain.cpp \
    specsimmodelwizard.cpp \
    specsimprojectwizard.cpp \
    specsimmodel.cpp \
    specsimmodelsettings.cpp \
    specsimruncontrol.cpp \
    specsimprojectmodel.cpp \
    specsimruntime.cpp \
    specsimruntimeview.cpp \
    combuildspecsimprojectmodel.cpp \
    simsystemoutlinemodel.cpp \
    simsystemeditoroutline.cpp \
    simsystemeditorfactory.cpp \
    simsystemeditor.cpp \
    simsystemdocument.cpp \
    combuildsimsystemoutlinemodel.cpp \
    simsystemeditorwidget.cpp \
    simsystemautocompleter.cpp \
    simsystemcompletionassist.cpp
