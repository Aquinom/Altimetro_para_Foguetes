#include "bmp280.h"
#include "i2c_helper.h"

#include <math.h>

#define BMP280_REG_ID      0xD0
#define BMP280_REG_CTRL    0xF4
#define BMP280_REG_CONFIG  0xF5
#define BMP280_REG_DATA    0xF7

#define SEA_LEVEL_PRESSURE 101325.0f


/*
=========================
READ CALIBRATION
=========================
*/
static void read_calibration(
    bmp280_t *dev
)
{
    uint8_t d[24];

    i2c_read(
        dev->port,
        dev->address,
        0x88,
        d,
        24
    );

    dev->calib.T1=(d[1]<<8)|d[0];
    dev->calib.T2=(d[3]<<8)|d[2];
    dev->calib.T3=(d[5]<<8)|d[4];

    dev->calib.P1=(d[7]<<8)|d[6];
    dev->calib.P2=(d[9]<<8)|d[8];
    dev->calib.P3=(d[11]<<8)|d[10];
    dev->calib.P4=(d[13]<<8)|d[12];
    dev->calib.P5=(d[15]<<8)|d[14];
    dev->calib.P6=(d[17]<<8)|d[16];
    dev->calib.P7=(d[19]<<8)|d[18];
    dev->calib.P8=(d[21]<<8)|d[20];
    dev->calib.P9=(d[23]<<8)|d[22];
}


/*
=========================
TEMP COMP
=========================
*/
static void compensate_temp(
    bmp280_t *dev,
    int32_t adc_T
)
{
    int32_t v1,v2;

    v1 =
        ((((adc_T >> 3) -
        ((int32_t)dev->calib.T1 << 1))) *
        ((int32_t)dev->calib.T2)) >> 11;

    v2 =
        (((((adc_T >> 4) -
        ((int32_t)dev->calib.T1)) *
        ((adc_T >> 4) -
        ((int32_t)dev->calib.T1))) >> 12) *
        ((int32_t)dev->calib.T3)) >> 14;

    dev->t_fine = v1 + v2;
}


/*
=========================
PRESSURE COMP
=========================
*/
static float compensate_pressure(
    bmp280_t *dev,
    int32_t adc_P
)
{
    int64_t v1,v2,p;

    v1=((int64_t)dev->t_fine)-128000;

    v2=v1*v1*(int64_t)dev->calib.P6;
    v2+=((v1*(int64_t)dev->calib.P5)<<17);
    v2+=(((int64_t)dev->calib.P4)<<35);

    v1=
        ((v1*v1*(int64_t)dev->calib.P3)>>8)
        +((v1*(int64_t)dev->calib.P2)<<12);

    v1=
        ((((int64_t)1<<47)+v1)*
        ((int64_t)dev->calib.P1))>>33;

    if(v1==0)
        return 0;

    p=1048576-adc_P;

    p=
        (((p<<31)-v2)*3125)
        /v1;

    v1=
        (((int64_t)dev->calib.P9)*
        (p>>13)*(p>>13))>>25;

    v2=
        (((int64_t)dev->calib.P8)*p)>>19;

    p=
        ((p+v1+v2)>>8)+
        (((int64_t)dev->calib.P7)<<4);

    return p/256.0f;
}


/*
=========================
READ RAW
=========================
*/
static void read_raw(
    bmp280_t *dev,
    int32_t *adc_P,
    int32_t *adc_T
)
{
    uint8_t raw[6];

    i2c_read(
        dev->port,
        dev->address,
        BMP280_REG_DATA,
        raw,
        6
    );

    *adc_P=
        ((raw[0]<<12)|
        (raw[1]<<4)|
        (raw[2]>>4));

    *adc_T=
        ((raw[3]<<12)|
        (raw[4]<<4)|
        (raw[5]>>4));
}


/*
=========================
INIT
=========================
*/
bool bmp280_init(
    bmp280_t *dev,
    uint8_t addr
)
{
    dev->address=addr;

    uint8_t id;

    i2c_read( dev->port, addr, BMP280_REG_ID, &id, 1);

    if(id!=0x58)
        return false;

    read_calibration(dev);

    i2c_write( dev->port, addr, BMP280_REG_CTRL, 0x27);

    i2c_write( dev->port, addr, BMP280_REG_CONFIG, 0x00);

    return true;
}


/*
=========================
GET PRESSURE
=========================
*/
float bmp280_get_pressure(
    bmp280_t *dev
)
{
    int32_t p,t;

    read_raw(dev, &p, &t);

    compensate_temp(dev, t);

    dev->pressure= compensate_pressure(dev, p);

    return dev->pressure;
}


/*
=========================
PRESSURE TO ALTITUDE
=========================
*/
float bmp280_pressure_to_altitude(
    float pressure
)
{
    return 44330.0f * (1.0f - powf(pressure/SEA_LEVEL_PRESSURE, 0.1903f));
}