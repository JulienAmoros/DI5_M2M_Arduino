
// Environnement Arduino 1.0.6
// Mise en oeuvre d'un capteur de pression BMP085
// Communication sur bus I2C

#include <Wire.h>
#include "Arduino.h"

//bme280 capteur température pression et humidite


// Capteur de pression : adressage I2C
// adresse sur le bus I2C du capteur BMP085
#define BME280_ADDRESS 0x76

//Définition des registres
#define BME280_DIG_T1 0x88
#define BME280_DIG_T2 0x8A
#define BME280_DIG_T3 0x8C
#define BME280_DIG_P1 0x8E
#define BME280_DIG_P2 0x90
#define BME280_DIG_P3 0x92
#define BME280_DIG_P4 0x94
#define BME280_DIG_P5 0x96
#define BME280_DIG_P6 0x98
#define BME280_DIG_P7 0x9A
#define BME280_DIG_P8 0x9C
#define BME280_DIG_P9 0x9E
#define BME280_DIG_H1 0xA1
#define BME280_DIG_H2 0xE1
#define BME280_DIG_H3 0xE3
#define BME280_DIG_H4 0xE4
#define BME280_DIG_H5 0xE5
#define BME280_DIG_H6 0xE7

//Fréquence d'échantillonage
#define SAMPLING_x0 0b000,
#define SAMPLING_x1 0b001
#define SAMPLING_x2 0b010
#define SAMPLING_x4 0b011
#define SAMPLING_x8 0b100
#define SAMPLING_x16 0b101

//Information sur le capteur
#define BME280_CHIPID      0xD0
#define BME280_VERSION     0xD1
#define BME280_SOFTRESET   0xE0
#define BME280_CAL26       0xE1
#define BME280_CONTROLHUMID   0xF2
#define BME280_STATUS         0XF3
#define BME280_CONTROL        0xF4
#define BME280_CONFIG         0xF5
//Valeurs brutes de pression, température et humidité
#define BME280_PRESSUREDATA   0xF7
#define BME280_TEMPDATA       0xFA
#define BME280_HUMIDDATA      0xFD

// Coefficients de calibration du capteur de pression BMP085 : déclaration
//Valeur qui contiendront les registres
unsigned int dig_t1=0;
int dig_t2=0;
int dig_t3=0;
unsigned int dig_p1=0;
int dig_p2=0;
int dig_p3=0;
int dig_p4=0;
int dig_p5=0;
int dig_p6=0;
int dig_p7=0;
int dig_p8=0;
int dig_p9=0;
unsigned char dig_h1=0;
int dig_h2=0;
unsigned char dig_h3=0;
int dig_h4=0;
int dig_h5=0;
char dig_h6=0;
int32_t  sumTemp=0;
TwoWire wire;

//lecture entier de 8 bits
uint8_t read8(byte reg) {
    uint8_t value;
    wire.beginTransmission((uint8_t)BME280_ADDRESS);
    wire.write((uint8_t)reg);
    wire.endTransmission();
    wire.requestFrom((uint8_t)BME280_ADDRESS, (byte)1);
    value = wire.read();
    return value;
}

//lecture entier de 16 bits
uint16_t read16(byte reg)
{
    uint16_t value;

    wire.beginTransmission((uint8_t)BME280_ADDRESS);
    wire.write((uint8_t)reg);
    wire.endTransmission();
    wire.requestFrom((uint8_t)BME280_ADDRESS, (byte)2);
    value = (wire.read() << 8) | wire.read();
    return value;
}

//lecture entier de 16 bits en inversant les 2 octets
uint16_t read16_LE(byte reg) {
    uint16_t temp = read16(reg);
    return (temp >> 8) | (temp << 8);
}

//16 bits signe
int16_t readS16(byte reg)
{
    return (int16_t)read16(reg);
}

//lecture entier de 16 bits signé en inversant les 2 octets
int16_t readS16_LE(byte reg)
{
    return (int16_t)read16_LE(reg);
}

//lecture octet de 24 bits
uint32_t read24(byte reg)
{
    uint32_t value;

    wire.beginTransmission((uint8_t)BME280_ADDRESS);
    wire.write((uint8_t)reg);
    wire.endTransmission();
    wire.requestFrom((uint8_t)BME280_ADDRESS, (byte)3);

    value = wire.read();
    value <<= 8;
    value |= wire.read();
    value <<= 8;
    value |= wire.read();
    return value;
}

//Ecriture d'un entier de 8 bits
void write8(byte reg, byte value) {
  wire.beginTransmission((uint8_t)BME280_ADDRESS);
  wire.write((uint8_t)reg);
  wire.write((uint8_t)value);
  wire.endTransmission();
}
/*-----------------------------------------------*/
// Acquisition de la valeurs des parametres de calibration

//Indique si le capteur est encore en train de lire la calibration
boolean isReadingCalibration(void)
{
  uint8_t const rStatus = read8(BME280_STATUS);

  return (rStatus & (1 << 0)) != 0;
}

//recupere les registres de calibration
boolean getbmp085Calibration()
{
    wire.begin();
    if (read8(BME280_CHIPID) != 0x60)
        return false;
    //reset le capteur
    write8(BME280_SOFTRESET, 0xB6);
    
    delay(400);
    //Attend que la lecture de la lecture de la calibration soit terminée
    while (isReadingCalibration())
    {
          delay(400);
    }

   //Lecture de tous les registres
   dig_t1 = read16_LE(BME280_DIG_T1);
   dig_t2 = readS16_LE(BME280_DIG_T2);
   dig_t3 = readS16_LE(BME280_DIG_T3);

   dig_p1 = read16_LE(BME280_DIG_P1);
   dig_p2 = readS16_LE(BME280_DIG_P2);
   dig_p3 = readS16_LE(BME280_DIG_P3);
   dig_p4 = readS16_LE(BME280_DIG_P4);
   dig_p5 = readS16_LE(BME280_DIG_P5);
   dig_p6 = readS16_LE(BME280_DIG_P6);
   dig_p7 = readS16_LE(BME280_DIG_P7);
   dig_p8 = readS16_LE(BME280_DIG_P8);
   dig_p9 = readS16_LE(BME280_DIG_P9);

   dig_h1 = read8(BME280_DIG_H1);
   dig_h2 = readS16_LE(BME280_DIG_H2);
   dig_h3 = read8(BME280_DIG_H3);
   dig_h4 = (read8(BME280_DIG_H4) << 4) | (read8(BME280_DIG_H4+1) & 0xF);
   dig_h5 = (read8(BME280_DIG_H5+1) << 4) | (read8(BME280_DIG_H5) >> 4);
   dig_h6 = (int8_t)read8(BME280_DIG_H6);
  
   //Définie le mode dans lequel sera utilisé le capteur
   defineMode();
}
/*-----------------------------------------------*/
// Calcul de la température connaissant ut.
// La valeur retournée est exprimée en 1/10 de °C (valeur entiére) puis convertie en °C (valeur réelle)
float bme280GetTemperature(long int ut)
{
  //10.1 de la doc constructeur
  long int var1, var2;

    // Vérifie que la mesure de la température est activée
    if (ut == 0x800000)
   {
     Serial.println("Oups !");
     return NAN;
   }
    //Calcul de la température     
    ut >>= 4;
    var1 = ((((ut>>3) - ((long int)dig_t1 <<1))) *
            ((long int)dig_t2)) >> 11;
             
    var2 = (((((ut>>4) - ((long int)dig_t1)) *
              ((ut>>4) - ((long int)dig_t1))) >> 12) *
            ((long int)dig_t3)) >> 14;
    sumTemp=var1+var2;
    float T = ((sumTemp) * 5 + 128) >> 8;
    return T/100; 
}

//Définition du mode d'utilisation du capteur 
void defineMode()
{
    //Capteur d'humidité
    unsigned int osrs_h =0b101; //SAMPLING_X16;
    write8(BME280_CONTROLHUMID,osrs_h);
    
    //Une mesure toutes les secondes
    unsigned int t_sb=0b101; //STANDBY_MS_0_5;

    //Désactive le filtre
    unsigned int filter = 0b000;// FILTER_OFF;

    // valeur par défaut
    unsigned int spi3w_en = 1;
            
    write8(BME280_CONFIG,(t_sb << 5) | (filter << 3) | spi3w_en);
    
    //Capteur température
    unsigned int osrs_t = 0b101;//SAMPLING_X16;
    //Capteur pression
    unsigned int osrs_p =0b101; // SAMPLING_X16;
    
    //Fonctionnement en mode normal
    unsigned int mode = 0b11; //MODE_NORMAL;

    write8(BME280_CONTROL, (osrs_t << 5) | (osrs_p << 3) | mode);
}
/*-----------------------------------------------*/
// Calcul de la pression en hPa = 100 Pa
// Les paramètres de calibration doivent être connus
// Le calcul de la valeur du coefficient b5 doit être effectué au préalable.
// La valeur retournée est exprimé en Pa (valeur entiére), puis convertie en hPa (valeur réelle).

float bme280GetPressure(int32_t up)
{
   int64_t var1, var2, p;


    //Vérifie que la mesure de pression est activée   
    if (up == 0x800000)
        return NAN;
    up >>= 4;

    var1 = ((int64_t)sumTemp) - 128000;
    var2 = var1 * var1 * (int64_t)dig_p6;
    var2 = var2 + ((var1*(int64_t)dig_p5)<<17);
    var2 = var2 + (((int64_t)dig_p4)<<35);
    var1 = ((var1 * var1 * (int64_t)dig_p3)>>8) +
           ((var1 * (int64_t)dig_p2)<<12);
    var1 = (((((int64_t)1)<<47)+var1))*((int64_t)dig_p1)>>33;

    if (var1 == 0) {
        return 0; // avoid exception caused by division by zero
    }
    p = 1048576 - up;
    p = (((p<<31) - var2)*3125) / var1;
    var1 = (((int64_t)dig_p9) * (p>>13) * (p>>13)) >> 25;
    var2 = (((int64_t)dig_p8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((int64_t)dig_p7)<<4);
    return (float)p/256;

}

/*-----------------------------------------------*/
// Calcul de l'altitude par rapport au niveau de la mer
// A faire : compensation du calcul
//PARTIE OPTIONNELLE
float estime_altitude (float pression, float temperature)
{

  //Pression niveau de la mer
   float Po=101325;
   //hypsometric formula
   float altitude=((pow(Po/pression,1/5.257)-1)*(temperature+273.15))/0.0065;
   
   return altitude;
}

//Mesure de l'humidite
float bme208GetHumidity(){
    long int ut= bme280ReadUT(); // must be done first to get t_fine
    bme280GetTemperature(ut);

    int32_t hum = read16(BME280_HUMIDDATA);
    //Vérifie que la mesure d'humidité n'est pas désactivée
    if (hum == 0x8000) 
        return NAN;
        
    int32_t humidite;

    humidite = (sumTemp - ((int32_t)76800));

    humidite = (((((hum << 14) - (((int32_t)dig_h4) << 20) -
                    (((int32_t)dig_h5) * humidite)) + ((int32_t)16384)) >> 15) *
                 (((((((humidite * ((int32_t)dig_h6)) >> 10) *
                      (((humidite * ((int32_t)dig_h3)) >> 11) + ((int32_t)32768))) >> 10) +
                    ((int32_t)2097152)) * ((int32_t)dig_h2) + 8192) >> 14));

    humidite = (humidite - (((((humidite >> 15) * (humidite >> 15)) >> 7) *
                               ((int32_t)dig_h1)) >> 4));

    humidite = (humidite < 0) ? 0 : humidite;
    humidite = (humidite > 419430400) ? 419430400 :humidite;
    float h = (humidite>>12);
    return  h / 1024.0;
}
/*-----------------------------------------------*/
// Lecture de la valeur non compensée de la température ut
long int bme280ReadUT()
{
  long int  ut = read24(BME280_TEMPDATA);
  return ut;
}
/*-----------------------------------------------*/
// Lecture de la valeur non compensée de la pression up
int32_t bme280ReadUP()
{
    long int ut= bme280ReadUT(); // must be done first to get t_fine
    bme280GetTemperature(ut);
    int32_t up = read24(BME280_PRESSUREDATA);
    return up;
}
