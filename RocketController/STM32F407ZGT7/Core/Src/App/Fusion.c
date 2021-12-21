
// Header files

#include "fusion.h"
#include <stdint.h>
#include <math.h>
#include "3d.h"

void InitFusion(quaternion_t *orientation) {

	orientation->q0 = 1.0f;
	orientation->q1 = 0.0f;
	orientation->q2 = 0.0f;
	orientation->q3 = 0.0f;

}


// No Mag algorithm update

void UpdateIMU(vector3_t *accel, vector3_t *gyro, quaternion_t *orientation, float dt, float gain) {
  float recipNorm;
  float s0, s1, s2, s3;
  float qDot1, qDot2, qDot3, qDot4;
  float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2, _8q1, _8q2, q0q0, q1q1, q2q2,
      q3q3;

  // Convert gyroscope milli-degrees/sec to radians/sec
  float gx = gyro->X * 0.0000174533f;
  float gy = gyro->Y * 0.0000174533f;
  float gz = gyro->Z * 0.0000174533f;

  // Convert accel milli-g to m/s
  float ax = accel->X * 0.00981f;
  float ay = accel->Y * 0.00981f;
  float az = accel->Z * 0.00981f;

  // Rate of change of quaternion from gyroscope
  qDot1 = 0.5f * (-(orientation->q1) * gx - orientation->q2 * gy - orientation->q3 * gz);
  qDot2 = 0.5f * (orientation->q0 * gx + orientation->q2 * gz - orientation->q3 * gy);
  qDot3 = 0.5f * (orientation->q0 * gy - orientation->q1 * gz + orientation->q3 * gx);
  qDot4 = 0.5f * (orientation->q0 * gz + orientation->q1 * gy - orientation->q2 * gx);

  // Compute feedback only if accelerometer measurement valid (avoids NaN in
  // accelerometer normalisation)
  if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

    // Normalise accelerometer measurement
    recipNorm = invSqrt(ax * ax + ay * ay + az * az);
    ax *= recipNorm;
    ay *= recipNorm;
    az *= recipNorm;

    // Auxiliary variables to avoid repeated arithmetic
    _2q0 = 2.0f * orientation->q0;
    _2q1 = 2.0f * orientation->q1;
    _2q2 = 2.0f * orientation->q2;
    _2q3 = 2.0f * orientation->q3;
    _4q0 = 4.0f * orientation->q0;
    _4q1 = 4.0f * orientation->q1;
    _4q2 = 4.0f * orientation->q2;
    _8q1 = 8.0f * orientation->q1;
    _8q2 = 8.0f * orientation->q2;
    q0q0 = orientation->q0 * orientation->q0;
    q1q1 = orientation->q1 * orientation->q1;
    q2q2 = orientation->q2 * orientation->q2;
    q3q3 = orientation->q3 * orientation->q3;

    // Gradient decent algorithm corrective step
    s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
    s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * orientation->q1 - _2q0 * ay - _4q1 +
         _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
    s2 = 4.0f * q0q0 * orientation->q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 +
         _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
    s3 = 4.0f * q1q1 * orientation->q3 - _2q1 * ax + 4.0f * q2q2 * orientation->q3 - _2q2 * ay;
    recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude

    s0 *= recipNorm;
    s1 *= recipNorm;
    s2 *= recipNorm;
    s3 *= recipNorm;

    // Apply feedback step
    qDot1 -= gain * s0;
    qDot2 -= gain * s1;
    qDot3 -= gain * s2;
    qDot4 -= gain * s3;
  }

  // Integrate rate of change of quaternion to yield quaternion
  orientation->q0 += qDot1 * dt;
  orientation->q1 += qDot2 * dt;
  orientation->q2 += qDot3 * dt;
  orientation->q3 += qDot4 * dt;

  // Normalise quaternion
  recipNorm = invSqrt(orientation->q0 * orientation->q0 + orientation->q1 * orientation->q1 + orientation->q2 * orientation->q2 + orientation->q3 * orientation->q3);
  orientation->q0 *= recipNorm;
  orientation->q1 *= recipNorm;
  orientation->q2 *= recipNorm;
  orientation->q3 *= recipNorm;
}

// Fast inverse square-root
// http://en.wikipedia.org/wiki/Fast_inverse_square_root

float invSqrt(float x) {
  float halfx = 0.5f * x;
  float y = x;
  long i = *(long *)&y;
  i = 0x5f3759df - (i >> 1);
  y = *(float *)&i;
  y = y * (1.5f - (halfx * y * y));
  y = y * (1.5f - (halfx * y * y));
  return y;
}


/*
void computeAngles(quaternion_t *orientation) {
  roll = atan2f(q0 * q1 + q2 * q3, 0.5f - q1 * q1 - q2 * q2);
  pitch = asinf(-2.0f * (q1 * q3 - q0 * q2));
  yaw = atan2f(q1 * q2 + q0 * q3, 0.5f - q2 * q2 - q3 * q3);
}

*/
