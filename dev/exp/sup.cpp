#include "Wire.h"
#include "MPU6050_6Axis_MotionApps20.h"

MPU6050 mpu;
bool dmpReady = false;
uint8_t devStatus;
uint16_t packetSize;
uint8_t fifoBuffer[64];
#define ACCEL_SCALE (9.81 / 16384.0)  // Scale factor for acceleration (MPU6500: ±2g → 16384 LSB/g)
#define GYRO_SCALE (250.0 / 32768.0)  // Scale factor for gyro (MPU6500: ±250°/s → 32768 LSB)

Quaternion q;
VectorFloat gravity;
float ypr[3];

int16_t ax, ay, az;
int16_t gx, gy, gz;

unsigned long prevMillis = 0;  // Timer for non-blocking delay
const int sampleRate = 1000;     // Adjust delay internally (100ms = 10Hz)

void setup() {
    Serial.begin(115200);
    Wire.begin(21, 22);  // Set ESP32 SDA=21, SCL=22

    Serial.println("Initializing MPU6050...");
    mpu.initialize();

    devStatus = mpu.dmpInitialize();
    if (devStatus == 0) {
        Serial.println("DMP Initialization Successful!");
        mpu.setDMPEnabled(true);
        packetSize = mpu.dmpGetFIFOPacketSize();
        dmpReady = true;
    } else {
        Serial.print("DMP Initialization Failed (Code ");
        Serial.print(devStatus);
        Serial.println(")");
    }
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - prevMillis >= sampleRate) { // Non-blocking delay
        prevMillis = currentMillis;

        // Read Raw Accelerometer & Gyroscope Data
        mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        Serial.print("Accel (m/s^2): "); Serial.print(ax * ACCEL_SCALE ); Serial.print("\t");
        Serial.print(ay * ACCEL_SCALE); Serial.print("\t");
        Serial.println(az* ACCEL_SCALE );

        Serial.print("Gyro (°/s): "); Serial.print(gx * GYRO_SCALE); Serial.print("\t");
        Serial.print(gy* GYRO_SCALE ); Serial.print("\t");
        Serial.println(gz * GYRO_SCALE);

        // Read DMP (Yaw, Pitch, Roll)
        if (dmpReady && mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

            Serial.print("Yaw: "); Serial.print(ypr[0] * 180 / M_PI);
            Serial.print(" Pitch: "); Serial.print(ypr[1] * 180 / M_PI);
            Serial.print(" Roll: "); Serial.println(ypr[2] * 180 / M_PI);
        }

        Serial.println("---------------------------------");
    }
}
