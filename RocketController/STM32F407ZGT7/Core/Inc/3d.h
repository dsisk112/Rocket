#ifndef __3d__h
#define __3d__h

	typedef struct
	{
		int16_t X;
		int16_t Y;
		int16_t Z;
	} vector3_t;

	typedef struct
	{
		float q0;
		float q1;
		float q2;
		float q3;

	} quaternion_t;

	typedef struct
	{
		float P;
		float T;
		float Alt;
	} barometer_t;



#endif
