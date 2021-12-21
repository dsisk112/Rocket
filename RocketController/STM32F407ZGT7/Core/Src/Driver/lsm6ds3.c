
#include <lsm6ds3.h>

	GPIO_TypeDef *GPIO_CS;
	uint16_t GPIO_PIN_CS;
	SPI_HandleTypeDef hspi;
	uint8_t ctrlx = 0;
	uint8_t ctrlg = 0;

	uint8_t LSM6DS3_Init(GPIO_TypeDef *GPIO_CS_let, uint16_t GPIO_CS_Num, SPI_HandleTypeDef spi, uint8_t Accel_G, uint8_t Gyro_R, uint8_t Freq)
	{

		GPIO_CS = GPIO_CS_let;
		GPIO_PIN_CS = GPIO_CS_Num;
		hspi = spi;
		ctrlx = Accel_G;
		ctrlg = Gyro_R;

		uint8_t AccelMode = Accel_G | Freq;
		uint8_t GyroMode = Gyro_R | Freq | 0x02;

		uint8_t AccelSetup[2] = {LSM6DSL_ACC_GYRO_CTRL1_XL, AccelMode};
		uint8_t GyroSetup[2] = {LSM6DSL_ACC_GYRO_CTRL2_G, GyroMode};

		uint8_t lsmSetup[6] = {LSM6DSL_ACC_GYRO_WHO_AM_I_REG | 0x80, 0x00,
								LSM6DSL_ACC_GYRO_CTRL1_XL | 0x80, 0x00,
								LSM6DSL_ACC_GYRO_CTRL2_G | 0x80, 0x00};

		uint8_t lsmCheck[6] = {0,0,0,0,0,0};

		HAL_GPIO_WritePin(GPIO_CS, GPIO_PIN_CS, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&hspi, AccelSetup, 2, 1000);
		HAL_GPIO_WritePin(GPIO_CS, GPIO_PIN_CS, GPIO_PIN_SET);

		HAL_GPIO_WritePin(GPIO_CS, GPIO_PIN_CS, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&hspi, GyroSetup, 2, 1000);
		HAL_GPIO_WritePin(GPIO_CS, GPIO_PIN_CS, GPIO_PIN_SET);

		HAL_GPIO_WritePin(GPIO_CS, GPIO_PIN_CS, GPIO_PIN_RESET);
		HAL_SPI_TransmitReceive(&hspi, lsmSetup, lsmCheck, 6, 1000);
		HAL_GPIO_WritePin(GPIO_CS, GPIO_PIN_CS, GPIO_PIN_SET);


		if ((lsmCheck[1] == LSM6DSL_ACC_GYRO_WHO_AM_I) && (AccelMode == lsmCheck[2]) && (GyroMode == lsmCheck[3]))
			return(HAL_OK);
		else
			return((uint8_t)1);

	}

	void LSM6DS3_AccReadXYZ(vector3_t *data)
	{

	  float sensitivity = 0;

	  uint8_t DataReq[7] = {LSM6DSL_ACC_GYRO_OUTX_H_XL | 0x80, LSM6DSL_ACC_GYRO_OUTX_L_XL | 0x80,
			  	  	  	  	LSM6DSL_ACC_GYRO_OUTY_H_XL | 0x80, LSM6DSL_ACC_GYRO_OUTY_L_XL | 0x80,
							LSM6DSL_ACC_GYRO_OUTZ_H_XL | 0x80, LSM6DSL_ACC_GYRO_OUTZ_L_XL | 0x80,
							0x00};
	  uint8_t OutBuf[7] = {0,0,0,0,0,0,0};

	  int16_t DataX, DataY, DataZ;

	  HAL_GPIO_WritePin(GPIO_CS, GPIO_PIN_CS, GPIO_PIN_RESET);
	  HAL_SPI_TransmitReceive(&hspi, DataReq, OutBuf, 7, 1000);
	  HAL_GPIO_WritePin(GPIO_CS, GPIO_PIN_CS, GPIO_PIN_SET);

	  /* Normal mode */
	  /* Switch the sensitivity value set in the CRTL1_XL */
	  switch(ctrlx & 0x0C)
	  {
	  case LSM6DSL_ACC_FULLSCALE_2G:
		sensitivity = LSM6DSL_ACC_SENSITIVITY_2G;
		break;
	  case LSM6DSL_ACC_FULLSCALE_4G:
		sensitivity = LSM6DSL_ACC_SENSITIVITY_4G;
		break;
	  case LSM6DSL_ACC_FULLSCALE_8G:
		sensitivity = LSM6DSL_ACC_SENSITIVITY_8G;
		break;
	  case LSM6DSL_ACC_FULLSCALE_16G:
		sensitivity = LSM6DSL_ACC_SENSITIVITY_16G;
		break;
	  }

	  DataX = ((((uint16_t)OutBuf[1]) << 8) | (uint16_t)OutBuf[2]);
	  DataY = ((((uint16_t)OutBuf[3]) << 8) | (uint16_t)OutBuf[4]);
	  DataZ = ((((uint16_t)OutBuf[5]) << 8) | (uint16_t)OutBuf[6]);


	  data->X = (int16_t)(DataX * sensitivity);
	  data->Y = (int16_t)(DataY * sensitivity);
	  data->Z = (int16_t)(DataZ * sensitivity);

	}

	void LSM6DS3_GyroReadXYZ(vector3_t *data)
	{

	  float sensitivity = 0;

	  uint8_t DataReq[7] = {LSM6DSL_ACC_GYRO_OUTX_H_G | 0x80, LSM6DSL_ACC_GYRO_OUTX_L_G | 0x80,
			  	  	  	  	LSM6DSL_ACC_GYRO_OUTY_H_G | 0x80, LSM6DSL_ACC_GYRO_OUTY_L_G | 0x80,
							LSM6DSL_ACC_GYRO_OUTZ_H_G | 0x80, LSM6DSL_ACC_GYRO_OUTZ_L_G | 0x80,
							0x00};
	  uint8_t OutBuf[7] = {0,0,0,0,0,0,0};

	  int16_t DataX, DataY, DataZ;

	  HAL_GPIO_WritePin(GPIO_CS, GPIO_PIN_CS, GPIO_PIN_RESET);
	  HAL_SPI_TransmitReceive(&hspi, DataReq, OutBuf, 7, 1000);
	  HAL_GPIO_WritePin(GPIO_CS, GPIO_PIN_CS, GPIO_PIN_SET);

	  /* Normal mode */
	  /* Switch the sensitivity value set in the CRTL2_G */
	  switch(ctrlg & 0x0C)
	  {
	  case LSM6DSL_GYRO_FS_245:
		sensitivity = LSM6DSL_GYRO_SENSITIVITY_245DPS;
		break;
	  case LSM6DSL_GYRO_FS_500:
		sensitivity = LSM6DSL_GYRO_SENSITIVITY_500DPS;
		break;
	  case LSM6DSL_GYRO_FS_1000:
		sensitivity = LSM6DSL_GYRO_SENSITIVITY_1000DPS;
		break;
	  case LSM6DSL_GYRO_FS_2000:
		sensitivity = LSM6DSL_GYRO_SENSITIVITY_2000DPS;
		break;
	  }

	  DataX = ((((uint16_t)OutBuf[1]) << 8) | (uint16_t)OutBuf[2]);
	  DataY = ((((uint16_t)OutBuf[3]) << 8) | (uint16_t)OutBuf[4]);
	  DataZ = ((((uint16_t)OutBuf[5]) << 8) | (uint16_t)OutBuf[6]);


	  data->X = (int16_t)(DataX * sensitivity);
	  data->Y = (int16_t)(DataY * sensitivity);
	  data->Z = (int16_t)(DataZ * sensitivity);

	}

