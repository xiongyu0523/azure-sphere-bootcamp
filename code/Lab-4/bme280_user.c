#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <applibs/i2c.h>
#include <applibs/log.h>
#include "bme280.h"
#include "bme280_user.h"

static int8_t user_i2c_read(uint8_t id, uint8_t reg_addr, uint8_t* data, uint16_t len);
static int8_t user_i2c_write(uint8_t id, uint8_t reg_addr, uint8_t* data, uint16_t len);
static void user_delay_ms(uint32_t period);

static int i2cFd = -1;
static struct bme280_dev bme280;

static int8_t user_i2c_read(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
	if (i2cFd == -1) {
		return BME280_E_DEV_NOT_FOUND;
	}

	if (I2CMaster_WriteThenRead(i2cFd, (I2C_DeviceAddress)bme280.dev_id, &reg_addr, 1, data, len) == -1) {
		return BME280_E_COMM_FAIL;
	}

	return BME280_OK;
}

static int8_t user_i2c_write(uint8_t id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
	if (i2cFd == -1) {
		return BME280_E_DEV_NOT_FOUND;
	}

	size_t total_len = (size_t)(len + 1);

	uint8_t *p_buf = malloc(total_len);
	if (p_buf == NULL) {
		return BME280_E_INVALID_LEN;
	}

	*p_buf = reg_addr;
	memcpy(p_buf + 1, data, len);

	if (I2CMaster_Write(i2cFd, (I2C_DeviceAddress)bme280.dev_id, p_buf, total_len) == -1) {
		free(p_buf);
		return BME280_E_COMM_FAIL;
	}

	free(p_buf);
	return BME280_OK;
}

static void user_delay_ms(uint32_t period)
{
	struct timespec ts = {
		(time_t)(period / 1000),
		(long)((period % 1000) * 1000000)
	};

	while ((-1 == nanosleep(&ts, &ts)) && (EINTR == errno));
}

int bme280_component_config(int isu_port)
{
	i2cFd = I2CMaster_Open(isu_port);
	if (i2cFd < 0) {
		Log_Debug("I2C initialization fail\n");
		return -1;
	}

	bme280.chip_id = 0;
	bme280.dev_id = BME280_I2C_ADDR_PRIM;
	bme280.intf = BME280_I2C_INTF;
	bme280.read = user_i2c_read;
	bme280.write = user_i2c_write;
	bme280.delay_ms = user_delay_ms;
	bme280.settings.osr_p = BME280_OVERSAMPLING_8X;
	bme280.settings.osr_t = BME280_OVERSAMPLING_8X;
	bme280.settings.osr_h = BME280_OVERSAMPLING_8X;
	bme280.settings.filter = BME280_FILTER_COEFF_16;
	bme280.settings.standby_time = BME280_STANDBY_TIME_500_MS;

	if (bme280_init(&bme280) != BME280_OK) {
		Log_Debug("BME280 initialization fail\n");
		return -1;
	}

	uint8_t settings = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL | BME280_STANDBY_SEL;
	if (bme280_set_sensor_settings(settings, &bme280) != BME280_OK) {
		Log_Debug("BME280 sensor settings fail\n");
		return -1;
	}

	if (bme280_set_sensor_mode(BME280_NORMAL_MODE, &bme280) != BME280_OK) {
		Log_Debug("BME280 sensor mode fail\n");
		return -1;
	}

	return 0;
}

int bme280_component_getdata(struct bme280_data* p_buf) 
{
	bme280_get_sensor_data(BME280_ALL, p_buf, &bme280);
	Log_Debug("Temperature: %0.2f C, Pressure: %0.2f hPa, Humidity: %0.2f %%\n",
		p_buf->temperature, p_buf->pressure * 0.01, p_buf->humidity);

	return 0;
}