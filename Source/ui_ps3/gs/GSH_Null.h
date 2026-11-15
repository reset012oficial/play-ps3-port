#ifndef GSH_NULL_H
#define GSH_NULL_H

#include <functional>

class CGSHandler
{
public:
    using FactoryFunction = std::function<CGSHandler*()>;
    virtual ~CGSHandler() = default;
    virtual void Init() {}
    virtual void Destroy() {}
    virtual void Reset() {}
};

class CGSH_Null : public CGSHandler
{
public:
    static CGSHandler* GetFactoryFunction()
    {
        return new CGSH_Null();
    }

    CGSH_Null() = default;
    virtual ~CGSH_Null() = default;

    virtual void Init() override {}
    virtual void Destroy() override {}
    virtual void Reset() override {}
};

#endif // GSH_NULL_H