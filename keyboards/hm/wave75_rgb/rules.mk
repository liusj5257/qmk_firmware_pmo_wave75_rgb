# Build Options
#   change yes to no to disable
#
# Board: it should exist either in <chibios>/os/hal/boards/
#  or <this_dir>/boards
BOARD = FS026
MCU = cortex-m0
MCU_ARCH = cortex-m0
ARMV = 6
MCU_FAMILY = ES32
MCU_SERIES = FS026
MCU_PORT_NAME = ES32
MCU_STARTUP = FS026
MCU_LDSCRIPT = FS026
EEPROM_DRIVER = custom
NO_USB_STARTUP_CHECK = yes
BLUETOOTH_CUSTOM = yes

ENCODER_MAP_ENABLE = yes    # Enable ENCODER MAP
ENCODER_DRIVER = custom
DEBOUNCE_TYPE = asym_eager_defer_pk
