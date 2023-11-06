#include "Property.h"

#include "PropertyObserver.h"
#include <iostream>
#include <exception>

Property::Property(const std::string &name, const std::string& category, void* var)
{
    m_name = name;
    m_category = category;
    m_var = var;

    m_pointer = std::shared_ptr<Property>(this);
}


Property::~Property()
{
}

void Property::addObserver(PropertyObserver *observer)
{
    m_observers.push_back(observer);
    observer->setProperty(m_pointer);
}

void Property::notify()
{
    for (unsigned int i = 0; i < m_observers.size(); ++i)
        m_observers[i]->update();
}
