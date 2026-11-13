#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class CSingleton
{
protected:
    CSingleton() = default;
    virtual ~CSingleton() = default;

public:
    CSingleton(const CSingleton&) = delete;
    CSingleton& operator=(const CSingleton&) = delete;

    static T& GetInstance()
    {
        static T instance;
        return instance;
    }

    static T* GetInstancePtr()
    {
        return &GetInstance();
    }
};

#endif // SINGLETON_H