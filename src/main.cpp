#include "Wire.h"
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"       // Librerias I2C para controlar el mpu6050
#include "MPU6050.h"
#include <SSD1306Wire.h>  // legacy: #include "SSD1306.h"

////////////////////////// Inicializar MPU6050 //////////////////////////

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 sensor;


int16_t accelX = 0, accelY =0, accelZ;
int16_t gyroX = 0, gyroY =0, gyroZ;

////////////////////////// Inicializar SSD1306 //////////////////////////

// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, 21, 22);  // ADDRESS, SDA, SCL  -  If not, they can be specified manually.

unsigned long lastTime = 0, sampleTime = 100;

void setup() {
    
    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(115200);

    display.init();
    //display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.setFont(ArialMT_Plain_10);

    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();           //Iniciando I2C  

    // initialize device
    Serial.println("Initializing I2C devices...");
    sensor.initialize();    //Iniciando el sensor

    // verify connection
    if (sensor.testConnection()) Serial.println("Sensor iniciado correctamente"); 
    else delay(500); //Serial.println("Error al iniciar el sensor");

}

void loop() {
    
    if(millis()-lastTime>=sampleTime) {

        lastTime = millis();
        // read raw accel/gyro measurements from device
        //sensor.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        
        // Leer las aceleraciones y velocidades angulares
        sensor.getAcceleration(&accelX, &accelY, &accelZ);
        sensor.getRotation(&gyroX, &gyroY, &gyroZ);

        /*
        En este ejemplo vamos a escalar las lecturas a valores con las unidades de aceleración y velocidad angular.
        Primero tenemos que saber los rangos con los que está configurado nuestro MPU6050, 
        dichos rangos pueden ser 2g/4g/8g/16g para el acelerómetro y 250/500/1000/2000(°/s) para el giroscopio.
        Para este ejemplo trabajaremos con los rangos por defecto (2g y 250°/s):
        
        -------------------------------------------------------------------------
        Variable          :  valor mínimo  :  valor central :  valor máximo     :
        -------------------------------------------------------------------------
        Lectura MPU6050   :     -32768     :      0         :     +32767        :
                          :                :                :                   :
        Aceleración       :     -2g        :      0g        :     +2g           :
                          :                :                :                   :
        Velocidad angular :     -250°/s    :      0°/s      :     +250°/s       :
        -------------------------------------------------------------------------
    
        Para escalar simplemente debemos usar una ecuación para convertir el valor leído en un valor de aceleración 
        o velocidad angular. */
        
        float ax_m_s2 = accelX * (9.81/16384.0);
        float ay_m_s2 = accelY * (9.81/16384.0);
        float az_m_s2 = accelZ * (9.81/16384.0);
        float gx_deg_s = gyroX * (250.0/32768.0);
        float gy_deg_s = gyroY * (250.0/32768.0);
        float gz_deg_s = gyroZ * (250.0/32768.0);
        
        // Datos puros, directamente del sensor
    //    Serial.println(accelX);
    //    Serial.println(accelY);
    //    Serial.println(accelZ);
    //    Serial.println(gyroX);
    //    Serial.println(gyroY);
    //    Serial.println(gyroZ);

        Serial.println(ax_m_s2);
        Serial.println(ay_m_s2);
        Serial.println(az_m_s2);
        Serial.println(gx_deg_s);
        Serial.println(gy_deg_s);
        Serial.println(gz_deg_s);

        display.clear();
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(64, 0, "Accelerometer - m/s^2");
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.drawString(0, 10, "X");
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(64, 10, "Y");
        display.setTextAlignment(TEXT_ALIGN_RIGHT);
        display.drawString(128, 10, "Z");
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.drawString(0, 20, String(ax_m_s2));
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(64, 20, String(ay_m_s2));
        display.setTextAlignment(TEXT_ALIGN_RIGHT);
        display.drawString(128, 20, String(az_m_s2));
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(64, 30, "Gyroscope - rad/s");
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.drawString(0, 40, "X");
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(64, 40, "Y");
        display.setTextAlignment(TEXT_ALIGN_RIGHT);
        display.drawString(128, 40, "Z");
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.drawString(0, 50, String(gx_deg_s));
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(64, 50, String(gy_deg_s));
        display.setTextAlignment(TEXT_ALIGN_RIGHT);
        display.drawString(128, 50, String(gz_deg_s));
        display.display();
        //delay(10);
        
    }
}