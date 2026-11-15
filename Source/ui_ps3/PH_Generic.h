#ifndef PH_GENERIC_H
#define PH_GENERIC_H

#include "ControllerInfo.h"
#include <cstdint>
#include <functional>

class CPadHandler
{
public:
    using FactoryFunction = std::function<CPadHandler*()>;
    virtual ~CPadHandler() = default;
    virtual void Update(uint8_t* data) = 0;
    virtual bool CheckExitCombination() const = 0;

    // Button state
    virtual void SetButtonState(int button, bool pressed) { (void)button; (void)pressed; }
    virtual void SetAxisState(int axis, float value) { (void)axis; (void)value; }
};

class CPH_Generic : public CPadHandler
{
public:
    virtual ~CPH_Generic() = default;
    void Update(uint8_t* data) override { (void)data; }
    bool CheckExitCombination() const override { return false; }
};

#endif // PH_GENERIC_H