#pragma once




class HardwareParameters {
    public:
        const static float pendulumWeight = 0.25;       // kg
        const static float pendulumLength = 0.13;       // m
        const static float cartWeight = 0.045;          // kg
        const static float wheelRadius = 0.05;          // m
        const static float motorResistance = 12.3;      // Ohms
        const static float motorTorqueConstant = 0.35;  // Nm/A
        const static float motorInductance = 0.004;     // Henrys
        const static float gravity = 9.81
};