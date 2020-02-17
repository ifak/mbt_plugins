SPECSIM_VERSION=0.1

include($${3RD_PATH}/mobata/libs/modsim/modsim/modsim.pri)

QTC_PLUGIN_NAME = specsim

QTC_PLUGIN_DEPENDS += \
  mbtproject


QTC_LIB_DEPENDS += \
    aggregation \
    extensionsystem \
    languageutils \
    utils

QTC_PLUGIN_DEPENDS += \
    dsleditor
