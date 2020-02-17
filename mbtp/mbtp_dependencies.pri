MBTP_VERSION=1.0

isEmpty(3RD_PATH) {
3RD_PATH = $$PWD/../../../3rd
}

MOBATA_LIBRARY_TYPE = staticlib
include($${3RD_PATH}/mobata/mobata/mobata.pri)

QTC_PLUGIN_NAME = mbtp

QTC_PLUGIN_DEPENDS += \
  projectexplorer
