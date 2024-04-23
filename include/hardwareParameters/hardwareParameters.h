#pragma once

class HardwareParameters {
    public:
        constexpr static float pendulumWeight = 0.25;       // kg
        constexpr static float pendulumLength = 0.13;       // m
        constexpr static float cartMass = 0.045;          // kg
        constexpr static float wheelRadius = 0.03;          // m
        constexpr static float motorResistance = 12.3;      // Ohms
        constexpr static float motorTorqueConstant = 0.35;  // Nm/A
        constexpr static float motorInductance = 0.004;     // Henrys
        constexpr static float gravity = 9.81;              // m/s/s
        constexpr static float batteryMax = 12.0;           // Volts
        constexpr static float gearRatio = 100.0;
};