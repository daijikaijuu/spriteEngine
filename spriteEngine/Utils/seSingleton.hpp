//
//  seSingleton.h
//  spriteEngine
//
//  Created by Domrachev Alexandr on 27.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seSingleton_h
#define seSingleton_h

namespace spriteEngine {
    template<class T>
    class seSingleton {
        seSingleton(seSingleton<T> const &) {}
        seSingleton& operator =(seSingleton const &) {}

    protected:
        static T *m_instance;

        seSingleton() {
            m_instance = static_cast<T*>(this);
        }
        ~seSingleton() {}

    public:
        static T* GetInstance();
        static void Destroy();
    };

    template<class T>
    T* seSingleton<T>::GetInstance() {
        if (!m_instance)
            m_instance = new T;
        return static_cast<T *>(m_instance);
    }

    template<class T>
    void seSingleton<T>::Destroy() {
        if (seSingleton::m_instance) {
            delete seSingleton::m_instance;
            seSingleton::m_instance = nullptr;
        }
    }

    template<class T>
    T* seSingleton<T>::m_instance = 0;
}


#endif /* seSingleton_h */
