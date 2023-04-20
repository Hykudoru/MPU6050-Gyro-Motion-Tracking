#include <WirelessData.h>
#include <Adafruit_MotorShield.h>
#include <utility/Adafruit_MS_PWMServoDriver.h>
#include <RotationMatrixVectorLib/Vector.h>
#include <RotationMatrixVectorLib/Matrix.h>
#ifndef DRONE_H
#define DRONE_H

class Rigidbody
{
    public:
        float mass;
        float momentOfInertia;
        Vector3<float> position;
        Vector3<float> rotation;
        Vector3<float> velocity;
        Vector3<float> angularVelocity;
        ulong timestamp;
        Rigidbody(){};
        ~Rigidbody(){};
};

class IMU
{
    Adafruit_MPU6050 mpu;
    sensors_event_t a, g, temp;
public:
    Matrix3x3 rotationMatrix;

    Vector3<float> position;
    Vector3<float> rotation;
    Vector3<float> velocity;
    Vector3<float> angularVelocity;

    Vector3<float> prevPosition;
    Vector3<float> prevRotation;
    Vector3<float> prevVelocity;
    Vector3<float> prevAngularVelocity;

    Vector3<float> accelZeroOffset;
    Vector3<float> gyroZeroOffset;
    
    IMU();
    ~IMU();
    void Init();
    void Update();
    void Calibrate();
    void Reset();
};

class Drone
{
    private:
        //inertial measurement unit
        IMU imu;
        Adafruit_MotorShield motorShield;
        
    public:
    
        //Motors
        Adafruit_DCMotor *m1;// = motorShield.getMotor(1);
        Adafruit_DCMotor *m2;// = motorShield.getMotor(2);
        Adafruit_DCMotor *m3;// = motorShield.getMotor(3);
        Adafruit_DCMotor *m4;// = motorShield.getMotor(4);
        const byte motorMinSpeed = 0;
        const byte motorMaxSpeed = 255;
        byte m1Speed = 0;
        byte m2Speed = 0;
        byte m3Speed = 0;
        byte m4Speed = 0;
        
        Drone();
        ~Drone();
        void Init();
        void Update(JoystickControllerData* input);
        void Calibrate(); 
        // Vector3<float> GetAcceleration();
        Vector3<float> GetAngularVelocity() {return imu.angularVelocity;}
        Vector3<float> GetVelocity() {return imu.velocity;}
        Vector3<float> GetPosition() {return imu.position;}
        Vector3<float> GetRotation() {return imu.rotation;}
};
#endif
