#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "tsl2540.h"

int main(void)
{
    cy_rslt_t result;
    cyhal_i2c_t i2c_master_obj;

    // Initialize the device and board peripherals
    result = cybsp_init();

    /* Initialize Retarget-IO to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);

    /* Initialize the I2C */
    result = cyhal_i2c_init(&i2c_master_obj, CYBSP_I2C_SDA, CYBSP_I2C_SCL, NULL);
    if (result == CY_RSLT_SUCCESS)
    {
        cyhal_i2c_configure(&i2c_master_obj, &i2c_cfg);
    }

    /* Initialize the ALS (TSL2540) */
    result = tsl2540_init(&i2c_master_obj, TSL2540_GAIN_1X, TSL2540_INT_721_MS);

    while (CY_RSLT_SUCCESS == result)
    {
        // Read lux value from the ALS
        result = tsl2540_read_data(&i2c_master_obj, &visible_light);
        printf("Visible Light Value = %d lux\n\n", visible_light);
        cyhal_system_delay_ms(1000); // Delay 1 second
    }

    // Free I2C Master
    cyhal_i2c_free(&i2c_master_obj);

    return 0;
}
