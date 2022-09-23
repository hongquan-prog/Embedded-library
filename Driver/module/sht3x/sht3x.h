#pragma once

#include "iic_interface.h"

typedef enum
{
    /*  Measurement Commands for Single Shot 
        Data Acquisition Mode
    */
    SHT3X_HIGH_REPEAT_STRETCH = 0x2C06,
    SHT3X_MEDIUM_REPEAT_STRETCH = 0x2C0D,
    SHT3X_LOW_REPEAT_STRETCH = 0x2C10,
    SHT3X_HIGH_REPEAT_NO_STRETCH = 0x2400,
    SHT3X_MEDIUM_REPEAT_NO_STRETCH = 0x240B,
    SHT3X_LOW_REPEAT_NO_STRETCH = 0x2416,

    /*  The ART (accelerated response time) feature can be 
        activated by issuing the command in Table 11. After 
        issuing the ART command the sensor will start 
        acquiring data with a frequency of 4Hz 
    */
    SHT3X_ART_ENABLE = 0x2B32,

    /*  The periodic data acquisition mode can be stopped 
        using the break command 
    */
    SHT3X_BREAK = 0x3093,

    /*  The status register contains information on the 
        operational status of the heater, the alert mode and on 
        the execution status of the last command and the last 
        write sequence
    */
    SHT3X_READ_STATUS = 0xF32D,
    SHT3X_CLEAR_STATUS = 0x3041,

    /*  The SHT3x-DIS provides a soft reset mechanism that 
        forces the system into a well-defined state without 
        removing the power supply 
    */
    SHT3X_SOFT_RESET = 0x30A2,

    /*  The heater can be switched on and off by command.
        After a reset the heater is disabled
    */
    SHT3X_HEATER_ENABLE = 0x306D,
    SHT3X_HEATER_DISABLE = 0x3066
} sht3x_cmd_def;

typedef struct 
{
    /*  Write data checksum status
        '0': checksum of last write transfer was 
        correct
        '1': checksum of last write transfer failed
    */
    unsigned short crc_status : 1;
    
    /*  Command status
        '0': last command executed successfully
        '1': last command not processed. It was 
        either invalid, failed the integrated 
        command checksum
    */
    unsigned short cmd_status : 1;

    unsigned short : 2;

    /*  System reset detected 
        '0': no reset detected since last ‘clear 
        status register’ command
        '1': reset detected (hard reset, soft reset 
        command or supply fail)
    */
    unsigned short reset_status : 1;

    unsigned short : 5;
    
    /*  tempture tracking alert 
        ‘0’ : no alert
        ‘1’ . alert
    */
    unsigned short tempture_alert_status : 1;

    /*  Relative humidity tracking alert 
        ‘0’ : no alert
        ‘1’ . alert
    */
    unsigned short rh_alert_status : 1;

    unsigned short : 1;

    /*  Heater status
        ‘0’ : Heater OFF
        ‘1’ : Heater ON
    */
    unsigned short heater_status : 1;

    unsigned short : 1;

    /*  Alert pending status 
        '0': no pending alerts 
        '1': at least one pending alert
    */
    unsigned short alert_pending_status : 1;
} sht3x_status_t;


typedef struct
{
    float humidity;
    float tempture;
} sht3x_ret_t;


typedef struct sht3x
{
    iic_interface_t *iic;
} sht3x_t;

void sht3x_init(sht3x_t *obj, iic_interface_t *iic);
void sht3x_reset(sht3x_t *obj);
sht3x_ret_t sht3x_read_humudity_tempture(sht3x_t *obj);
sht3x_status_t sht3x_read_status(sht3x_t *obj);
void sht3x_clear_status(sht3x_t *obj);