
#include "bmp38x.h"
#include <math.h>

	GPIO_TypeDef *GPIO_CS_Baro;
	uint16_t GPIO_PIN_CS_Baro;
	SPI_HandleTypeDef hspi_Baro;
	bmp_calibration_data *calib_data;


uint8_t BMP38X_Init(GPIO_TypeDef *GPIO_CS_let, uint16_t GPIO_CS_Num, SPI_HandleTypeDef spi, bmp_calibration_data *calib_data)
{
	GPIO_CS_Baro = GPIO_CS_let;
	GPIO_PIN_CS_Baro = GPIO_CS_Num;
	hspi_Baro = spi;

	unsigned char BMP_Test[3] = {REG_CHIP_ID | 0x80, 0x00, 0x00};
	unsigned char BMP_WhoOut[3] = {0x00, 0x00, 0x00};

	unsigned char Write_Power_On[6] = {REG_PWR_CNTRL, 0x07, REG_OSR, 0x0D ,REG_ODR, 0x00};

	unsigned char CalibrationTX[23] = {REG_T1 | 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char CalibrationData[23] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


	HAL_GPIO_WritePin(GPIO_CS_Baro, GPIO_PIN_CS_Baro, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi_Baro, BMP_Test, BMP_WhoOut, 3, 1000);
	HAL_GPIO_WritePin(GPIO_CS_Baro, GPIO_PIN_CS_Baro, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIO_CS_Baro, GPIO_PIN_CS_Baro, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi_Baro, Write_Power_On, 6, 1000);
	HAL_GPIO_WritePin(GPIO_CS_Baro, GPIO_PIN_CS_Baro, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIO_CS_Baro, GPIO_PIN_CS_Baro, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi_Baro, CalibrationTX, CalibrationData, 23, 1000);
	HAL_GPIO_WritePin(GPIO_CS_Baro, GPIO_PIN_CS_Baro, GPIO_PIN_SET);


	calib_data->t1 = (float)(((uint16_t)CalibrationData[3] << 8 | (uint16_t)CalibrationData[2])/(2^(-8)));
	calib_data->t2 = (float)(((uint16_t)CalibrationData[5] << 8 | (uint16_t)CalibrationData[4])/(2^(30)));
	calib_data->t3 = (float)((int8_t)CalibrationData[6])/(2^(48));
	calib_data->p1 = (float)((int16_t)((uint16_t)CalibrationData[8] << 8 | (uint16_t)CalibrationData[7])/(2^(20)));
	calib_data->p2 = (float)((int16_t)((uint16_t)CalibrationData[10] << 8 | (uint16_t)CalibrationData[9])/(2^(29)));
	calib_data->p3 = (float)((int8_t)CalibrationData[11])/(2^(32));
	calib_data->p4 = (float)((int8_t)CalibrationData[12])/(2^(37));
	calib_data->p5 = (float)(((uint16_t)CalibrationData[14] << 8 | (uint16_t)CalibrationData[13])/(2^(-3)));
	calib_data->p6 = (float)(((uint16_t)CalibrationData[16] << 8 | (uint16_t)CalibrationData[15])/(2^(6)));
	calib_data->p7 = (float)((int8_t)CalibrationData[17])/(2^(8));
	calib_data->p8 = (float)((int8_t)CalibrationData[18])/(2^(15));
	calib_data->p9 = (float)((int16_t)((uint16_t)CalibrationData[20] << 8 | (uint16_t)CalibrationData[29])/(2^(48)));
	calib_data->p10 = (float)((int8_t)CalibrationData[21])/(2^(48));
	calib_data->p11 = (float)((int8_t)CalibrationData[22])/(2^(65));



	if(BMP_WhoOut[2] == BMP_CHIP_ID)
		return(HAL_OK);
	else
		return(1);
}

void BMP_Read_Data(barometer_t *BMP, bmp_calibration_data *calib_data)
{

	unsigned char statusTX[3] = {0x03 | 0x80, 0x00, 0x00};
	unsigned char statusRX[3] = {0x00,0x00,0x00};

	HAL_GPIO_WritePin(GPIO_CS_Baro, GPIO_PIN_CS_Baro, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi_Baro, statusTX, statusRX, 8, 1000);
	HAL_GPIO_WritePin(GPIO_CS_Baro, GPIO_PIN_CS_Baro, GPIO_PIN_SET);

	unsigned char DataTX[8] = {0x04 | 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char DataRX[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


	HAL_GPIO_WritePin(GPIO_CS_Baro, GPIO_PIN_CS_Baro, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi_Baro, DataTX, DataRX, 8, 1000);
	HAL_GPIO_WritePin(GPIO_CS_Baro, GPIO_PIN_CS_Baro, GPIO_PIN_SET);

	uint32_t uncomp_temp = DataRX[7] << 16 | DataRX[6] << 8 | DataRX[5];
	uint32_t uncomp_press = DataRX[4] << 16 | DataRX[3] << 8 | DataRX[2];

	float comp_temp = BMP_Compensate_T(uncomp_temp, calib_data);
	float comp_press = BMP_Compensate_P(uncomp_press, calib_data);

	BMP->T = comp_temp;
	BMP->P = comp_press;
	float partial_alt = pow((1013.25/comp_press), (1/5.257));
	BMP->Alt = (((partial_alt - 1.0f) * (comp_temp + 273.15))/0.0065f);

}

float BMP_Compensate_T(uint32_t uncomp_temp, bmp_calibration_data *calib_data)
{

	float partial_data1;
	float partial_data2;

	partial_data1 = (float)(uncomp_temp - calib_data->t1);
	partial_data2 = (float)(partial_data1 * calib_data->t2);

	calib_data->t_comp = partial_data2 + (partial_data1 * partial_data1) * calib_data->t3;

	return(calib_data->t_comp);
}

float BMP_Compensate_P(uint32_t uncomp_press, bmp_calibration_data *calib_data)
{

	float comp_press;

	float partial_data1;
	float partial_data2;
	float partial_data3;
	float partial_data4;
	float partial_out1;
	float partial_out2;


	partial_data1 = calib_data->p6 * calib_data->t_comp;
	partial_data2 = calib_data->p7 * (calib_data->t_comp * calib_data->t_comp);
	partial_data3 = calib_data->p8 * (calib_data->t_comp * calib_data->t_comp * calib_data->t_comp);
	partial_out1 = calib_data->p5 + partial_data1 + partial_data2 + partial_data3;

	partial_data1 = calib_data->p2 * calib_data->t_comp;
	partial_data2 = calib_data->p3 * (calib_data->t_comp * calib_data->t_comp);
	partial_data3 = calib_data->p4 * (calib_data->t_comp * calib_data->t_comp * calib_data->t_comp);
	partial_out2 = (float)uncomp_press * (calib_data->p1 + partial_data1 + partial_data2 + partial_data3);

	partial_data1 = (float)uncomp_press * (float)uncomp_press;
	partial_data2 = calib_data->p9 + calib_data->p10 * calib_data->t_comp;
	partial_data3 = partial_data1 * partial_data2;
	partial_data4 = partial_data3 + ((float)uncomp_press * (float)uncomp_press * (float)uncomp_press * calib_data->p11);

	comp_press =  partial_out1 + partial_out2 + partial_data4;

	return(comp_press);


}
