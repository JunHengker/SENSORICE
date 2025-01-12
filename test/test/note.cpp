// #include <Wire.h>
// #include <BH1750FVI.h>

// uint32_t sleepTimer;

// BH1750FVI myBH1750(BH1750_DEFAULT_I2CADDR, BH1750_ONE_TIME_HIGH_RES_MODE, BH1750_SENSITIVITY_DEFAULT, BH1750_ACCURACY_DEFAULT);
// int soilSensor = 36;

// void setup()
// {

//     Serial.begin(115200);
//     Serial.println();

//     while (myBH1750.begin() != true)
//     {
//         Serial.println(F("ROHM BH1750FVI is not present")); //(F()) saves string to flash & keeps dynamic memory free
//         delay(5000);
//     }

//     Serial.println(F("ROHM BH1750FVI is present"));

//     /* change BH1750 settings on the fly */
//     myBH1750.setCalibration(1.06);                           // call before "readLightLevel()", 1.06=white LED & artifical sun
//     myBH1750.setSensitivity(1.00);                           // call before "readLightLevel()", 1.00=no optical filter in front of the sensor
//     myBH1750.setResolution(BH1750_CONTINUOUS_HIGH_RES_MODE); // call before "readLightLevel()", continuous measurement with 1.00 lux resolution

//     // setup soil sensor
//     pinMode(soilSensor, INPUT);

//     sleepTimer = millis(); // update timer
// }

// void loop()
// {
//     float lightLevel = myBH1750.readLightLevel(); // start measurment -> wait for result -> read result -> retrun result or 4294967295 if communication error is occurred

//     Serial.println();
//     Serial.print(F("Light level.........: "));
//     if (lightLevel != BH1750_ERROR) // BH1750_ERROR=4294967295
//     {
//         Serial.print(lightLevel, 2);
//         Serial.println(F(" lux"));
//     }
//     else
//     {
//         Serial.println(F("error"));
//     }

//     Serial.print(F("Power for 555nm wave: "));
//     if (lightLevel != BH1750_ERROR) // BH1750_ERROR=4294967295
//     {
//         Serial.print((lightLevel / 683), 2);
//         Serial.println(F(" Watt/m^2"));
//     }
//     else
//     {
//         Serial.println(F("error"));
//     }

//     Serial.print(F("Resolution value....: 0x"));
//     Serial.println(myBH1750.getResolution(), HEX); // 0x10=CONT_HIGH_RES, 0x11=CONT_HIGH_RES2, 0x13=CONT_LOW_RES, 0x20=ONE_HIGH_RES, 0x21=ONE_HIGH_RES2, 0x23=ONE_LOW_RES

//     Serial.print(F("Calibration value...: "));
//     Serial.println(myBH1750.getCalibration());

//     Serial.print(F("Sensitivity value...: "));
//     Serial.println(myBH1750.getSensitivity());

//     if ((millis() - sleepTimer) > 5000) // sleep every 5 seconds
//     {
//         Serial.println();
//         Serial.print(F("sleep (^o^) ~zzzzzzzzz"));

//         // read soil sensor
//         Serial.print("Soil Moisture Sensor Voltage: ");
//         Serial.print((float(analogRead(soilSensor)) / 1023.0) * 3.3); // read sensor
//         Serial.println(" V");

//         myBH1750.powerDown(); // sleep, 1uA

//         delay(5000); // sleep for 5 seconds

//         myBH1750.powerOn(); // wake-up & wait for measurment command, 190uA
//         myBH1750.reset();   // clear measurement that is left unread in sensor register

//         sleepTimer = millis(); // update timer
//     }
// }