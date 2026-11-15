#ifndef CONTROLLERINFO_H
#define CONTROLLERINFO_H

class CControllerInfo
{
public:
    enum Button
    {
        DPAD_UP = 0,
        DPAD_DOWN,
        DPAD_LEFT,
        DPAD_RIGHT,
        SELECT,
        START,
        SQUARE,
        TRIANGLE,
        CIRCLE,
        CROSS,
        L1,
        L2,
        L3,
        R1,
        R2,
        R3,
        BUTTON_MAX
    };

    enum Axis
    {
        ANALOG_LEFT_X = 0,
        ANALOG_LEFT_Y,
        ANALOG_RIGHT_X,
        ANALOG_RIGHT_Y,
        AXIS_MAX
    };
};

#endif // CONTROLLERINFO_H