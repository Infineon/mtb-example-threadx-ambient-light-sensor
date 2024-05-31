#ifndef TSL2540_H
#define TSL2540_H

#include <stdint.h>
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

/*GAIN*/
enum TSL2540_GAIN
{
    TSL2540_GAIN_1X,
    TSL2540_GAIN_4X,
    TSL2540_GAIN_16X,
    TSL2540_GAIN_64X
};

/*Integration Time*/
enum TSL2540_INT_TIME
{
    TSL2540_INT_2_8_MS = 0,
    TSL2540_INT_5_6_MS = 1,
    TSL2540_INT_11_2_MS = 3,
    TSL2540_INT_22_5_MS = 7,
    TSL2540_INT_45_MS = 15,
    TSL2540_INT_90_MS = 31,
    TSL2540_INT_180_MS = 63,
    TSL2540_INT_360_MS = 127,
    TSL2540_INT_721_MS = 255,
};

/* register set */
uint8_t sensor_addr = 0x39;
uint8_t sensor_en_reg = 0x80;
uint8_t sensor_int_time_reg = 0x81;
uint8_t sensor_gain_reg = 0x90;
uint8_t vis_data_low_reg = 0x94;
uint8_t vis_data_high_reg = 0x95;

uint16_t visible_light;

// Initialize I2C Master
cyhal_i2c_cfg_t i2c_cfg = {
    .is_slave = false,
    .address = 0,
    .frequencyhal_hz = 400000};

cy_rslt_t tsl2540_init(cyhal_i2c_t *i2c_master_obj, uint8_t TSL2540_GAIN, uint8_t TSL2540_INT_TIME);
cy_rslt_t tsl2540_read_data(cyhal_i2c_t *i2c_master_obj, uint16_t *visible_light);

#endif
