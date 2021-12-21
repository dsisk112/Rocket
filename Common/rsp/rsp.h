#include <stdint.h>

#define MAX_MSG_SIZE        4

#define VERSION_ID      0x01

typedef struct {
    uint8_t     version;
} RSP_VERSION_RESP;

typedef struct {
    uint8_t id;
} RSP_HEADER;

typedef struct { 
    RSP_HEADER  header;
    char        msg[MAX_MSG_SIZE];
} RSP_MSG;

typedef struct {
    int16_t X;
    int16_t Y;
    int16_t Z;
} vector3_t;

typedef struct {
    float q0;
    float q1;
    float q2;
    float q3;
} quaternion_t;

typedef struct {
    int16_t q0;
    int16_t q1;
    int16_t q2;
    int16_t q3;
} quaternion_t_tx;


typedef struct {

    quaternion_t_tx orientation; //*1000 (re-norm quaternion)
    uint32_t altitude; //*10 (centimeters)
    vector3_t velocity; //*1000 (mm/sec)
    union state {
        uint8_t pyro1:1;
        uint8_t pyro2:1;
        uint8_t launch:1;
        uint8_t apogee:1;
        uint8_t dlim:1;
        uint8_t mlim:1;
        uint8_t na1:1;
        uint8_t na2:1;
    };
    uint8_t checksum;
} ROCKET_DATA_MSG; 

