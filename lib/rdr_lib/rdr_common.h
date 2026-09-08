/*用户自己的一些宏定义数据*/
#pragma once

#include "debounce.h"
#include "quantum.h"
#include "usb_descriptor.h"
#include "usb_main.h"
#include "raw_hid.h"
#include "action_util.h"

#define USER_EMI_COMMAND	    0XBB
#define USER_KEYBOARD_COMMAND	0X0A
#define USER_KEYBOARD_LENGTH    (64)

#define USER_SWITCH_2P4G_MODE	0X00
#define USER_SWITCH_BLE_1_MODE	0X01
#define USER_SWITCH_BLE_2_MODE	0X02
#define USER_SWITCH_BLE_3_MODE	0X03
#define USER_SWITCH_2P4G_PAIR	0X04
#define USER_SWITCH_BLE_1_PAIR	0X05
#define USER_SWITCH_BLE_2_PAIR	0X06
#define USER_SWITCH_BLE_3_PAIR	0X07
#define USER_SWITCH_USB_MODE	0X08

#define USER_KEYBOARD_SLEEP		0X09
#define USER_KEYBOARD_WAKEUP	0X0A

#define USER_KEY_BYTE_DATA		0X0B
#define USER_KEY_BIT_DATA		0X0C
#define USER_MOUSE_DATA			0X0D
#define USER_CONSUMER_DATA		0X0E
#define USER_SYSTEM_DATA		0X0F

#define USER_BATTERY_DATA		0X10

#define USER_GET_RF_STATUS	    0X11

#define USER_BLE1_WRITE_NAME	0X12
#define USER_BLE2_WRITE_NAME    0X13
#define USER_BLE3_WRITE_NAME    0X14

#define USER_SLEEP_TIME_WRITE   0X15
#define USER_DSLEEP_TIME_WRITE  0X16

#define USER_KEY_BYTE_LENGTH	0X08
#define USER_KEY_BIT_LENGTH		0X0F
#define USER_MOUSE_LENGTH		0X08
#define USER_CONSUMER_LENGTH	0X03
#define USER_SYSTEM_LENGTH		0X03
#define USER_BATTERY_LENGTH		0X02

#define KB_REPORT_ID            0x06
#define SYS_REPORT_ID     	    0x03
#define CON_REPORT_ID     	    0x04
#define MOUSE_REPORT_ID  	    0x02

#define LOGO_LED_ENABLE         (1)
#define SIDE_LED_ENABLE         (0)

/***************************全局变量***************************/
bool Usb_If_Ok_Led;
bool Usb_If_Ok;
uint16_t Usb_If_Ok_Delay;

uint16_t Usb_Change_Mode_Delay;
bool Usb_Change_Mode_Wakeup;
bool Mode_Synchronization_Signal;

unsigned int Debounce_Delay;

void User_Led_Show(void);
void User_Keyboard_Reset(void);
void User_Keyboard_Init(void);
void User_Keyboard_Post_Init(void);
void es_chibios_user_idle_loop_hook(void);
bool Key_Value_Dispose(uint16_t keycode, keyrecord_t *record);

void add_key_to_report(uint8_t key);
void General_Key_Reorder(uint8_t Spot_Index);
void del_key_from_report(uint8_t key);
void User_send_6kro_report(void);
void User_send_nkro_report(void);
void User_Clear_Board(void);
void User_Send_Key(uint8_t Code, bool Status);
/**************************************************************/

#define USER_DEFINE_KEY         (QK_KB)
enum Custom_Keycodes {
    QMK_KB_MODE_2P4G = USER_DEFINE_KEY,
    QMK_KB_MODE_BLE1,
    QMK_KB_MODE_BLE2,
    QMK_KB_MODE_BLE3,
    QMK_KB_MODE_USB,
    QMK_BATT_NUM,
    QMK_WIN_LOCK,
    QMK_KB_SIX_N_CH,
    QMK_TEST_COLOUR,
    QMK_DEBOUNCE,
    QMK_TIME_SET,
    QMK_DTIME_SET,
#if LOGO_LED_ENABLE
    LOGO_TOG,
    LOGO_MOD,
    LOGO_RMOD,
    LOGO_HUI,
    LOGO_HUD,
    LOGO_SAI,
    LOGO_SAD,
    LOGO_VAI,
    LOGO_VAD,
    LOGO_SPI,
    LOGO_SPD,
#endif
#if SIDE_LED_ENABLE
    SIDE_TOG,
    SIDE_MOD,
    SIDE_RMOD,
    SIDE_HUI,
    SIDE_HUD,
    SIDE_SAI,
    SIDE_SAD,
    SIDE_VAI,
    SIDE_VAD,
    SIDE_SPI,
    SIDE_SPD,
#endif
    QMK_KB_2P4G_PAIR,
    QMK_KB_BLE1_PAIR,
    QMK_KB_BLE2_PAIR,
    QMK_KB_BLE3_PAIR
};

enum Custom_KeyModes {
    QMK_BLE_MODE = 0,
    QMK_2P4G_MODE,
    QMK_USB_MODE
};

enum Custom_BleChannels {
    QMK_BLE_CHANNEL_1 = 1,
    QMK_BLE_CHANNEL_2,
    QMK_BLE_CHANNEL_3
};

enum Custom_Spi_Ack_S {
    SPI_NACK,
    SPI_ACK
};

enum Custom_Spi_Busy_S {
    SPI_BUSY,
    SPI_IDLE
};

enum Custom_Ble_24G_Status_S {
    BLE_24G_NONE,
    BLE_24G_PIAR,
    BLE_24G_RETURN
};

typedef struct {
    uint8_t Key_Mode;
    uint8_t Ble_Channel;
    uint8_t Batt_Number;
    uint8_t Nkro;
    uint8_t Mac_Win_Mode;
    uint8_t Win_Lock;
    uint8_t Debounce_Delay;
    uint32_t User_Sleep_Time;
    uint32_t User_DSleep_Time;
#if LOGO_LED_ENABLE
    uint8_t Logo_On_Off;
    uint8_t Logo_Mode;
    uint8_t Logo_Colour;
    uint8_t Logo_Saturation;
    uint8_t Logo_Brightness;
    uint8_t Logo_Speed;
#endif
#if SIDE_LED_ENABLE
    uint8_t Side_On_Off;
    uint8_t Side_Mode;
    uint8_t Side_Colour;
    uint8_t Side_Saturation;
    uint8_t Side_Brightness;
    uint8_t Side_Speed;
#endif
} Keyboard_Info_t;

extern Keyboard_Info_t Keyboard_Info;

typedef struct {
    uint8_t System_Work_Status;
    uint8_t System_Work_Mode;
    uint8_t System_Work_Channel;
    uint8_t System_Connect_Status;
    uint8_t System_Led_Status;
    uint8_t System_Sleep_Mode;
} Keyboard_Status_t;

typedef enum {
    KB_MODE_CONNECT_OK,
    KB_MODE_CONNECT_PAIR,
    KB_MODE_CONNECT_RETURN,
} keyboard_System_state_e;

typedef enum {
    USER_SLEEP_PASS,
    USER_SLEEP_FIAL,
} keyboard_System_Sleep_Status_s;

#define INIT_WORK_MODE              (QMK_USB_MODE)
#define INIT_BLE_CHANNEL            (QMK_BLE_CHANNEL_1)
#define INIT_BATT_NUMBER            (50)

#define INIT_SIX_KEY                (0)
#define INIT_ALL_KEY                (1)
#define INIT_ALL_SIX_KEY            (INIT_ALL_KEY)

#define INIT_WIN_MODE               (0)
#define INIT_MAC_MODE               (1)
#define INIT_WIN_MAC_MODE           (INIT_WIN_MODE)

#define INIT_WIN_NLOCK              (0)
#define INIT_WIN_LOCK               (1)
#define INIT_WIN_LOCK_NLOCK         (INIT_WIN_NLOCK)

#define DEBOUNCE_DELAY_ONE          (2)
#define DEBOUNCE_DELAY_TWO          (5)
#define DEBOUNCE_DELAY_CLASS        (DEBOUNCE_DELAY_TWO)

#define SLEEP_TIME_ONE              (60)
#define SLEEP_TIME_TWO              (180)
#define SLEEP_TIME_THREE            (600)
#define SLEEP_TIME_FOUR             (1800)
#define SLEEP_TIME_CLASS            (SLEEP_TIME_TWO)

#define U_PWM                       (RGB_MATRIX_MAXIMUM_BRIGHTNESS)

#if LOGO_LED_ENABLE
#define LOGO_LED_PLAY_SPEED	        (0)
#define LOGO_LED_SIZE	            (4)

#define LOGO_LED_ON                 (0)
#define LOGO_LED_OFF                (1)

#define LOGO_WAVE_RGB_MODE          (1)
#define LOGO_WAVE_DS_MODE           (2)
#define LOGO_SPECTRUM_MODE          (3)
#define LOGO_BREATH_MODE            (4)
#define LOGO_LIGHT_MODE             (5)
#define LOGO_OFF_MODE               (6)

#define LOGO_MAX_COLOUR             (255)
#define LOGO_MIN_COLOUR             (0)
#define COLOUR_LEVEL                (15)

#define LOGO_MAX_SATURATION         (0)
#define LOGO_MIN_SATURATION         (255)
#define SATURATION_LEVEL            (15)

#define LOGO_MAX_BRIGHTNESS         (140)
#define LOGO_MIN_BRIGHTNESS         (0)
#define BRIGHTNESS_LEVEL            (14)

#define LOGO_MAX_SPEED              (4)
#define LOGO_MIN_SPEED              (0)
#define SPEED_LEVEL                 (1)

#define INIT_LOGO_ON_OFF            (LOGO_LED_ON)
#define INIT_LOGO_MODE              (LOGO_WAVE_RGB_MODE)
#define INIT_LOGO_COLOUR            (LOGO_MIN_COLOUR)
#define INIT_LOGO_SATURATION        (LOGO_MAX_SATURATION)
#define INIT_LOGO_BRIGHTNESS        (LOGO_MAX_BRIGHTNESS)
#define INIT_LOGO_SPEED             (2)

void User_Via_Qmk_Logo_Get_Value(uint8_t *data);
void User_Via_Qmk_Logo_Set_Value(uint8_t *data);
void User_Via_Qmk_Logo_Command(uint8_t *data, uint8_t length);

#endif
//--------------------------------------------------------------------------------------------------------
#if SIDE_LED_ENABLE
#define SIDE_LED_PLAY_SPEED	        (0)
#define SIDE_LED_SIZE	            (32)

#define SIDE_LED_ON                 (0)
#define SIDE_LED_OFF                (1)

#define SIDE_WAVE_RGB_MODE          (1)
#define SIDE_WAVE_DS_MODE           (2)
#define SIDE_SPECTRUM_MODE          (3)
#define SIDE_BREATH_MODE            (4)
#define SIDE_LIGHT_MODE             (5)
#define SIDE_OFF_MODE               (6)

#define SIDE_MAX_COLOUR             (255)
#define SIDE_MIN_COLOUR             (0)
#define SIDE_COLOUR_LEVEL           (15)

#define SIDE_MAX_SATURATION         (0)
#define SIDE_MIN_SATURATION         (255)
#define SIDE_SATURATION_LEVEL       (15)

#define SIDE_MAX_BRIGHTNESS         (RGB_MATRIX_MAXIMUM_BRIGHTNESS)
#define SIDE_MIN_BRIGHTNESS         (0)
#define SIDE_BRIGHTNESS_LEVEL       (15)

#define SIDE_MAX_SPEED              (4)
#define SIDE_MIN_SPEED              (0)
#define SIDE_SPEED_LEVEL            (1)

#define INIT_SIDE_ON_OFF            (SIDE_LED_ON)
#define INIT_SIDE_MODE              (SIDE_WAVE_RGB_MODE)
#define INIT_SIDE_COLOUR            (SIDE_MIN_COLOUR)
#define INIT_SIDE_SATURATION        (SIDE_MAX_SATURATION)
#define INIT_SIDE_BRIGHTNESS        (SIDE_MAX_BRIGHTNESS)
#define INIT_SIDE_SPEED             (2)

void User_Via_Qmk_Side_Get_Value(uint8_t *data);
void User_Via_Qmk_Side_Set_Value(uint8_t *data);
void User_Via_Qmk_Side_Command(uint8_t *data, uint8_t length);

#endif

#define APP_2G4_BUF_SIZE            (USER_KEYBOARD_LENGTH)
#define APP_2G4_BUF_CNT             (20)

#define USER_BATT_POWER_SCAN_COUNT  (10)
#define USER_BATT_SCAN_COUNT        (10)

#define USER_BATT_HIGH_POWER        (2550)
#define USER_BATT_LOW_POWER         (1980)
#define USER_BATT_STDOWN_POWER      (1865)

#define USER_BATT_DELAY_TIME        (100 * 25)
#define USER_TIME_3S_TIME           (100 * 3)

#define SPI_DELAY_RF_TIME           (60)
#define SPI_DELAY_USB_TIME          (500 * 3)

void User_Sleep(void);
void Emi_Read_Data(uint8_t *User_Data, uint8_t User_Length);
void Emi_Write_Data(uint8_t *User_Data, uint8_t User_Length);

#define MAX_NAME_LEN                (18)
#define USER_BlE_ID                 (0X0007)
#define USER_BlE1_NAME              "WAVE 75 RGB-1"
#define USER_BlE2_NAME              "WAVE 75 RGB-2"
#define USER_BlE3_NAME              "WAVE 75 RGB-3"

#define USER_DSLEEP_TIME            0XFFFFFFFE

#define KEYBAORD_COL                (16)
#define KEYBAORD_ROL                (7)

#define MATRIX_USER_COL_PINS        { D15, D14, C15, C14, C13, D3, D2, C12, C11, C10, A14, C9, C8, C7, C6, B15 }
#define MATRIX_USER_ROW_PINS        { B0, B3, B4, B5, B6, B7, B1 }

#define WIN_COL                     (1)
#define WIN_ROL                     (3)

#define MAC_COL                     (2)
#define MAC_ROL                     (3)

#define KC_K29 	KC_BACKSLASH
#define KC_K42 	KC_NONUS_HASH
#define KC_K45 	KC_NONUS_BACKSLASH
#define KC_K56 	KC_INTERNATIONAL_1
#define KC_K14  KC_INTERNATIONAL_3
#define KC_K132	KC_INTERNATIONAL_4
#define KC_K131	KC_INTERNATIONAL_5
#define KC_K133	KC_INTERNATIONAL_2
#define KC_K151	KC_LANGUAGE_1
#define KC_K150	KC_LANGUAGE_2

#define MD_24G	QMK_KB_MODE_2P4G
#define MD_BLE1	QMK_KB_MODE_BLE1
#define MD_BLE2	QMK_KB_MODE_BLE2
#define MD_BLE3	QMK_KB_MODE_BLE3
#define MD_USB	QMK_KB_MODE_USB
#define QK_BAT  QMK_BATT_NUM
#define QK_WLO	QMK_WIN_LOCK
#define SIX_N	QMK_KB_SIX_N_CH
#define TEST_CL	QMK_TEST_COLOUR
#define KEY_DEB	QMK_DEBOUNCE
#define TIME_ST	 QMK_TIME_SET
#define TIME_DT	 QMK_DTIME_SET
