#pragma once

#include "iic_interface.h"

#define HMC_IDENTIFICATION_LEN 3

typedef enum hmc_reg_addr
{
    HMC_CRA = 0x00,         // Configuration Register A
    HMC_CRB = 0x01,         // Configuration Register B
    HMC_MR  = 0x02,         // Mode Register
    HMC_DO_XH = 0x03,       // Data Output X MSB Register
    HMC_DO_XL = 0x04,       //Data Output X LSB Register 
    HMC_DO_ZH = 0x05,       // Data Output Z MSB Register 
    HMC_DO_ZL = 0x06,       // Data Output Z LSB Register 
    HMC_DO_YH = 0x07,       // Data Output Y MSB Register 
    HMC_DO_YL = 0x08,       // Data Output Y LSB Register 
    HMC_SR = 0x09,          // Status Register 
    HMC_IRA = 0x0A,         // Identification Register A 
    HMC_IRB = 0x0B,         // Identification Register B 
    HMC_IRC = 0x0C,         // Identification Register C 
    HMC_TMP_H = 0x31,       // Temperature Output MSB Register 
    HMC_TMP_L = 0x32        // Temperature Output LSB Register 
}hmc_reg_addr_t;

typedef enum hmc_measurement_average
{
    HMC_MA_1 = 0,
    HMC_MA_2 = 1,
    HMC_MA_4 = 2,
    HMC_MA_8 = 3
}hmc_measurement_average_t;

typedef enum hmc_data_output_rate
{
    HMC_0_75_HZ = 0,
    HMC_1_5_HZ  = 1,
    HMC_3_HZ  = 2,
    HMC_7_5_HZ  = 3,
    HMC_15_HZ  = 4,
    HMC_30_HZ  = 5,
    HMC_75_HZ  = 6,
    HMC_220_HZ  = 7
}hmc_data_output_rate_t;

typedef enum hmc_measurement_configuration
{
    HMC_CFG_DEFAULT,        /*  Normal measurement configuration (Default). In normal measurement 
                                configuration the device follows normal measurement flow. The positive and 
                                negative pins of the resistive load are left floating and high impedance.
                            */
    HMC_CFG_POSIVIVE_BIAS,  /*  Positive bias configuration for X, Y, and Z axes. In this configuration, a positive 
                                current is forced across the resistive load for all three axes. 
                            */
    HMC_CFG_NEGAVIVE_BIAS,  /*  Negative bias configuration for X, Y and Z axes. In this configuration, a negative 
                                current is forced across the resistive load for all three axes.
                            */
    HMC_CFG_TEMPERATURE     /*  Temperature sensor only. Magnetic sensor will not be enabled during 
                                measurement.
                            */
}hmc_measurement_configuration_t;

typedef enum hmc_gain
{
    HMC_GAIN_0_88,
    HMC_GAIN_1_3, 
    HMC_GAIN_1_9,
    HMC_GAIN_2_5,
    HMC_GAIN_4_0,
    HMC_GAIN_4_7,
    HMC_GAIN_5_6,
    HMC_GAIN_8_1
}hmc_gain_t;

typedef enum hmc_measurement_mode
{
    HMC_CONTINUOUS_MEASUREMENT,
    HMC_SINGLE_MEASUREMENT,
    HMC_IDLE
}hmc_measurement_mode_t;

typedef struct hmc5983
{
    char dentification[HMC_IDENTIFICATION_LEN];
    // iic interface
    iic_interface_t *iic;
    // work mode
    hmc_measurement_mode_t mode;
}hmc5983_t;

unsigned char hmc5983_init(hmc5983_t *obj, iic_interface_t* iic, hmc_measurement_mode_t mode);
static unsigned char hmc5983_set_parameter(hmc5983_t *obj, hmc_reg_addr_t reg, unsigned char begin, int len, unsigned char value);
static unsigned char hmc5983_get_parameter(hmc5983_t *obj, hmc_reg_addr_t reg, unsigned char begin, int len, unsigned char *val);
unsigned char hmc5983_set_temperature_compensation(hmc5983_t *obj, unsigned char state);
unsigned char hmc5983_set_measurement_average(hmc5983_t *obj, hmc_measurement_average_t times);
unsigned char hmc5983_set_data_out_rate(hmc5983_t *obj, hmc_data_output_rate_t rate);
unsigned char hmc5983_set_measurement_configuration(hmc5983_t *obj, hmc_measurement_configuration_t mode);
unsigned char hmc5983_set_gain(hmc5983_t *obj, hmc_gain_t gain);
unsigned char hmc5983_set_high_speed_mode(hmc5983_t *obj, unsigned char state);
unsigned char hmc5983_set_low_power_mode(hmc5983_t *obj, unsigned char state);
unsigned char hmc5983_set_measurement_mode(hmc5983_t *obj, hmc_measurement_mode_t mode);
unsigned char hmc5983_get_result(hmc5983_t *obj, short *x, short *y, short *z);
unsigned char hmc5983_is_over_written(hmc5983_t *obj);
unsigned char hmc5983_is_lock(hmc5983_t *obj);
unsigned char hmc5983_is_ready(hmc5983_t *obj);
char *hmc5983_read_identification(hmc5983_t *obj);
unsigned int hmc5983_read_temperature(hmc5983_t *obj);
float hmc5983_temperature_convert(hmc5983_t *obj, unsigned int value);
