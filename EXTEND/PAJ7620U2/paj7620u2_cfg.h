#ifndef __PAJ7620U2_CFG_H
#define __PAJ7620U2_CFG_H



#define INIT_SIZE sizeof(init_Array)/2
//上电初始化数组
const unsigned char init_Array[][2] = {

    {0xEF,0x00},
    {0x37,0x07},
    {0x38,0x17},
    {0x39,0x06},
    {0x41,0x00},
    {0x42,0x00},
    {0x46,0x2D},
    {0x47,0x0F},
    {0x48,0x3C},
    {0x49,0x00},
    {0x4A,0x1E},
    {0x4C,0x20},
    {0x51,0x10},
    {0x5E,0x10},
    {0x60,0x27},
    {0x80,0x42},
    {0x81,0x44},
    {0x82,0x04},
    {0x8B,0x01},
    {0x90,0x06},
    {0x95,0x0A},
    {0x96,0x0C},
    {0x97,0x05},
    {0x9A,0x14},
    {0x9C,0x3F},
    {0xA5,0x19},
    {0xCC,0x19},
    {0xCD,0x0B},
    {0xCE,0x13},
    {0xCF,0x64},
    {0xD0,0x21},
    {0xEF,0x01},
    {0x02,0x0F},
    {0x03,0x10},
    {0x04,0x02},
    {0x25,0x01},
    {0x27,0x39},
    {0x28,0x7F},
    {0x29,0x08},
    {0x3E,0xFF},
    {0x5E,0x3D},
    {0x65,0x96},
    {0x67,0x97},
    {0x69,0xCD},
    {0x6A,0x01},
    {0x6D,0x2C},
    {0x6E,0x01},
    {0x72,0x01},
    {0x73,0x35},
    {0x74,0x00},
    {0x77,0x01},
};

#define PROXIM_SIZE sizeof(proximity_arry)/2
//接近检测初始化数组
const unsigned char proximity_arry[][2]= {

    {0xEF,0x00},
    {0x41,0x00},
    {0x42,0x00},
    {0x48,0x3C},
    {0x49,0x00},
    {0x51,0x13},
    {0x83,0x20},
    {0x84,0x20},
    {0x85,0x00},
    {0x86,0x10},
    {0x87,0x00},
    {0x88,0x05},
    {0x89,0x18},
    {0x8A,0x10},
    {0x9f,0xf8},
    {0x69,0x96},
    {0x6A,0x02},
    {0xEF,0x01},
    {0x01,0x1E},
    {0x02,0x0F},
    {0x03,0x10},
    {0x04,0x02},
    {0x41,0x50},
    {0x43,0x34},
    {0x65,0xCE},
    {0x66,0x0B},
    {0x67,0xCE},
    {0x68,0x0B},
    {0x69,0xE9},
    {0x6A,0x05},
    {0x6B,0x50},
    {0x6C,0xC3},
    {0x6D,0x50},
    {0x6E,0xC3},
    {0x74,0x05},


};

#define GESTURE_SIZE sizeof(gesture_arry)/2
//手势识别初始化数组
const unsigned char gesture_arry[][2]= {

    {0xEF,0x00},
    {0x41,0x00},
    {0x42,0x00},
    {0xEF,0x00},
    {0x48,0x3C},
    {0x49,0x00},
    {0x51,0x10},
    {0x83,0x20},
    {0x9F,0xF9},
    {0xEF,0x01},
    {0x01,0x1E},
    {0x02,0x0F},
    {0x03,0x10},
    {0x04,0x02},
    {0x41,0x40},
    {0x43,0x30},
    {0x65,0x96},
    {0x66,0x00},
    {0x67,0x97},
    {0x68,0x01},
    {0x69,0xCD},
    {0x6A,0x01},
    {0x6B,0xB0},
    {0x6C,0x04},
    {0x6D,0x2C},
    {0x6E,0x01},
    {0x74,0x00},
    {0xEF,0x00},
    {0x41,0xFF},
    {0x42,0x01},

};
#endif
