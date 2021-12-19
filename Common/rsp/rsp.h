#define MAX_MSG_SIZE        256

#define VERSION_REQ_ID      0x01
#define VERSION RESP_ID     0x02

typedef struct {
    uint8_t     version;
} RSP_VERSION_RESP;

typedef struct {
    uint8_t id;
    uint8_t bytes_to_follow;
} RSP_HEADER;

typedef struct { 
    RSP_HEADER  header;
    char        msg[MAX_MSG_SIZE];
} RSP_MSG;

