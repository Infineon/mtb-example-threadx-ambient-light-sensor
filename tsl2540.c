#include <stdint.h>
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

extern uint8_t sensor_addr;
#define i2c_receive_data_size 1
#define i2c_timeout 0

cy_rslt_t tsl2540_init(cyhal_i2c_t *i2c_master_obj, uint8_t TSL2540_GAIN, uint8_t TSL2540_INT_TIME)
{
    cy_rslt_t result;
    extern uint8_t sensor_en_reg;
    extern uint8_t sensor_int_time_reg;
    extern uint8_t sensor_gain_reg;

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */

    printf("\x1b[2J\x1b[;H");

    printf("***************************************************************************\n");
    printf("           Interfacing Ambient Light Sensor Through I2C    \n");
    printf("***************************************************************************\n\n");

    // ENABLE SENSOR
    uint8_t write_data_80[2] = {sensor_en_reg, 0x03};
    result = cyhal_i2c_master_write(i2c_master_obj, sensor_addr, write_data_80, sizeof(write_data_80), i2c_timeout, true);

    // SET INTEGRATION TIME
    uint8_t write_data_81[2] = {sensor_int_time_reg, TSL2540_INT_TIME};
    if (result == CY_RSLT_SUCCESS)
    {
        result = cyhal_i2c_master_write(i2c_master_obj, sensor_addr, write_data_81, sizeof(write_data_81), i2c_timeout, true);

        if (result == CY_RSLT_SUCCESS)
        {
            // Read value of register 0x81
            uint8_t read_data_81 = 0;
            result = cyhal_i2c_master_write(i2c_master_obj, sensor_addr, &write_data_81[0], i2c_receive_data_size, i2c_timeout, true);
            if (result == CY_RSLT_SUCCESS)
            {
                result = cyhal_i2c_master_read(i2c_master_obj, sensor_addr, &read_data_81, i2c_receive_data_size, i2c_timeout, true);

                printf("-> The integration time is set to ≈ %f ms  \n\n", (read_data_81 + 1) * 2.81);
            }
        }
    }

    // SET GAIN
    uint8_t write_data_90[2] = {sensor_gain_reg, TSL2540_GAIN};
    if (result == CY_RSLT_SUCCESS)
    {
        result = cyhal_i2c_master_write(i2c_master_obj, sensor_addr, write_data_90, sizeof(write_data_90), i2c_timeout, true);

        if (result == CY_RSLT_SUCCESS)
        {
            // Read value of register 0x90
            uint8_t read_data_90 = 0;
            result = cyhal_i2c_master_write(i2c_master_obj, sensor_addr, &write_data_90[0], i2c_receive_data_size, i2c_timeout, true);
            if (result == CY_RSLT_SUCCESS)
            {
                result = cyhal_i2c_master_read(i2c_master_obj, sensor_addr, &read_data_90, i2c_receive_data_size, i2c_timeout, true);
                printf("-> The gain is set to %x\n\n", read_data_90);
            }
        }
    }

    return result;
}

cy_rslt_t tsl2540_read_data(cyhal_i2c_t *i2c_master_obj, uint16_t *visible_light)
{
    cy_rslt_t result;
    extern uint8_t vis_data_low_reg;
    extern uint8_t vis_data_high_reg;
    uint8_t vis_data_low = 0;
    uint8_t vis_data_high = 0;

    // Read Visible Data Low Byte
    result = cyhal_i2c_master_write(i2c_master_obj, sensor_addr, &vis_data_low_reg, sizeof(vis_data_low_reg), i2c_timeout, true);
    if (result == CY_RSLT_SUCCESS)
    {
        result = cyhal_i2c_master_read(i2c_master_obj, sensor_addr, &vis_data_low, sizeof(vis_data_low_reg), i2c_timeout, true);
    }

    // Read Visible Data High Byte
    if (result == CY_RSLT_SUCCESS)
    {
        result = cyhal_i2c_master_write(i2c_master_obj, sensor_addr, &vis_data_high_reg, sizeof(vis_data_high_reg), i2c_timeout, true);
        if (result == CY_RSLT_SUCCESS)
        {
            result = cyhal_i2c_master_read(i2c_master_obj, sensor_addr, &vis_data_high, i2c_receive_data_size, i2c_timeout, true);
        }
    }

    // Combine low bits and high bits to get visible light reading
    *visible_light = ((uint16_t)vis_data_high << 8) | vis_data_low;

    return result;
}
