#ifndef __MEMORY_MAP_H
#define __MEMORY_MAP_H

#include <stdint.h>

#ifndef FLASH_PAGE_SIZE
#define FLASH_PAGE_SIZE 256
#endif // !FLASH_PAGE_SIZE

#define VOLATILE_OFFSET FLASH_PAGE_SIZE      // 256 bytes
#define READ_ONLY_OFFSET FLASH_PAGE_SIZE * 2 // 512 bytes
#define MESSAGE_OFFSET FLASH_PAGE_SIZE * 3   // 768 bytes
#define REGISTER_SIZE FLASH_PAGE_SIZE * 16   // 4096 bytes

// ############################### //
// BEGIN OF MEMORY MAPPING OFFSETS //
// ############################### //

/* non volatile range */
// memory offset of the gain of the process values
#define GAIN_PV0_OFFSET 0
#define GAIN_PV1_OFFSET 4
#define GAIN_PV2_OFFSET 8
#define GAIN_PV3_OFFSET 12

// memory offset of the offset of the process values
#define OFFSET_PV0_OFFSET 16
#define OFFSET_PV1_OFFSET 20
#define OFFSET_PV2_OFFSET 24
#define OFFSET_PV3_OFFSET 28

// memory offset of cycle time in microseconds (4 bytes)
#define OFFSET_DESIRED_CYCLE_TIME 32

// memory offset of config bits (1 byte)
#define OFFSET_CONFIG_BITS 40

// memory offset of address of the device (1 byte)
#define OFFSET_ADDRESS 44

#define CONFIG_VAL0_OFFSET 48
#define CONFIG_VAL1_OFFSET 52
#define CONFIG_VAL2_OFFSET 56
#define CONFIG_VAL3_OFFSET 60

// ############################# //
// ###### Volatile range ####### //
// ############################# //

// memory offset of the process values
#define PV0_OFFSET VOLATILE_OFFSET + 0  // 256
#define PV1_OFFSET VOLATILE_OFFSET + 4  // 260
#define PV2_OFFSET VOLATILE_OFFSET + 8  // 264
#define PV3_OFFSET VOLATILE_OFFSET + 12 // 268

// memory offset of the mean values of the process values
#define MEAN_PV0_OFFSET VOLATILE_OFFSET + 16 // 272
#define MEAN_PV1_OFFSET VOLATILE_OFFSET + 20 // 276
#define MEAN_PV2_OFFSET VOLATILE_OFFSET + 24 // 280
#define MEAN_PV3_OFFSET VOLATILE_OFFSET + 28 // 284

// memory offset of the standard deviation of the process values
#define STDDEV_PV0_OFFSET VOLATILE_OFFSET + 32 // 288
#define STDDEV_PV1_OFFSET VOLATILE_OFFSET + 36 // 292
#define STDDEV_PV2_OFFSET VOLATILE_OFFSET + 40 // 296
#define STDDEV_PV3_OFFSET VOLATILE_OFFSET + 44 // 300

// memory offset of the minimum and maximum values of the process values
#define MIN_PV0_OFFSET VOLATILE_OFFSET + 48 // 304
#define MIN_PV1_OFFSET VOLATILE_OFFSET + 52 // 308
#define MIN_PV2_OFFSET VOLATILE_OFFSET + 56 // 312
#define MIN_PV3_OFFSET VOLATILE_OFFSET + 60 // 316

#define MAX_PV0_OFFSET VOLATILE_OFFSET + 64 // 320
#define MAX_PV1_OFFSET VOLATILE_OFFSET + 68 // 324
#define MAX_PV2_OFFSET VOLATILE_OFFSET + 72 // 328
#define MAX_PV3_OFFSET VOLATILE_OFFSET + 76 // 332

// memory offset of the discrete values, eg. digital inputs and outputs
#define DV0_OFFSET VOLATILE_OFFSET + 80 // 336
#define DV1_OFFSET VOLATILE_OFFSET + 84 // 340
#define DV2_OFFSET VOLATILE_OFFSET + 88 // 344
#define DV3_OFFSET VOLATILE_OFFSET + 92 // 348

// memory offset of the additional values
#define AV0_OFFSET VOLATILE_OFFSET + 96  // 352
#define AV1_OFFSET VOLATILE_OFFSET + 100 // 356
#define AV2_OFFSET VOLATILE_OFFSET + 104 // 360
#define AV3_OFFSET VOLATILE_OFFSET + 108 // 364

// signed values
#define SV0_OFFSET VOLATILE_OFFSET + 112 // 368
#define SV1_OFFSET VOLATILE_OFFSET + 116 // 372
#define SV2_OFFSET VOLATILE_OFFSET + 120 // 376
#define SV3_OFFSET VOLATILE_OFFSET + 124 // 380

// memory offset for the safety lock of the device memory (1 byte)
#define MEM_UNLOCKED_OFFSET VOLATILE_OFFSET + 128 // 384

// ############################# //
// ###### READ ONLY RANGE ###### //
// ############################# //

// memory offset of the status of the device (8 bytes)
#define STATUS_OFFSET READ_ONLY_OFFSET + 0 // 512
// memory offset of the errors of the device (8 bytes)
#define ERROR_OFFSET READ_ONLY_OFFSET + 8 // 520
// memory offset of the uid of the device (8 bytes)
#define UID_OFFSET READ_ONLY_OFFSET + 16 // 528

// memory offset of the net cycle time (4 bytes)
#define OFFSET_NET_CYCLE_TIME READ_ONLY_OFFSET + 32 // 544

// ############################# //
// END of memory mapping offsets //
// ############################# //

/* config masks */
/* If true use free run, if false: wait for sync packet */
#define MASK_CONFIG_FREE_RUN 1 << 0
/* if true, enable automatic calculation of the statistics */
#define MASK_CONFIG_CALC_STATS 1 << 1

/**
 * @brief Magic value to unlock the device memory
 *
 * The device memory can be unlocked by writing a value to the memory unlock. To unlock the memory, write this value to the memUnlock variable.
 *
 * @note This value is not a secret. It is just a value that is not likely to be written by accident.
 *
 */
constexpr uint32_t MEM_UNLOCKED_VAL = 0x55AA55AA;

/**
 * @brief The configuration bits of the device
 *
 */
typedef struct
{
    bool freeRun : 1;  // enable free run of device
    bool calcStat : 1; // enable calculation of statistics
    bool bit2 : 1;
    bool bit3 : 1;
    bool bit4 : 1;
    bool bit5 : 1;
    bool bit6 : 1;
    bool bit7 : 1;
} ConfigBits;

/**
 * @brief Union to access the configuration bits as a byte
 *
 */
union ConfigBitsUnion
{
    ConfigBits bits;
    uint8_t all;
};

#endif //__MEMORY_MAP_H