#include "PropertyObserver.h"
#include "Property.h"

PropertyObserver::PropertyObserver()
{

}

PropertyObserver::~PropertyObserver()
{

}

std::string PropertyObserver::getName() const
{
    return m_globProperty->getName();
}
