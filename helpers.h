#pragma once
#include "homework02.h"

#define GLSL(src) "#version 400\n" #src

template<class T>
class Singleton
{
private:
    Singleton(Singleton<T> const &) {}
    Singleton& operator =(Singleton const &) {}

protected:
    static T *m_instance;

    Singleton() {
        m_instance = static_cast <T*> (this);
    }
    ~Singleton() {}

public:
    static T* GetInstance();
    static void Destroy();
};

template<class T>
T* Singleton<T>::GetInstance()
{
    if (!m_instance)
    {
       m_instance = new T;
    }
    return static_cast<T*>(m_instance);
}

template<class T>
void Singleton<T>::Destroy()
{
    if (Singleton::m_instance)
    {
        delete Singleton::m_instance;
        Singleton::m_instance = 0;
    }
}

template<class T>
T* Singleton<T>::m_instance = 0;
