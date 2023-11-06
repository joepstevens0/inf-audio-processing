#ifndef TEMPLATEPROPERTY_CPP
#define TEMPLATEPROPERTY_CPP

#include "TemplateProperty.h"
#include <iostream>

template<typename T>
TemplateProperty<T>::TemplateProperty(const std::string &name, const std::string &category, void *var) : Property(name, category, var)
{
    T* p = static_cast<T*>(m_var);
    m_value = *p;
}

template<typename T>
TemplateProperty<T>::~TemplateProperty()
{

}

template<typename T>
void TemplateProperty<T>::setValue(const T &value)
{
    m_lock.lock();
    m_value = value;
    if (m_var)
    {
        T* p = static_cast<T*>(m_var);
        *p = value;
    }
    m_lock.unlock();
    notify();
}

template<typename T>
T TemplateProperty<T>::getValue()
{
    T copy;
    m_lock.lock();
    copy = m_value;
    m_lock.unlock();
    return copy;
}


#endif
