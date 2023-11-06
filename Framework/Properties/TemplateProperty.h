#ifndef TEMPLATEPROPERTY_H
#define TEMPLATEPROPERTY_H

#include "Property.h"

#include <functional>
#include <mutex>

template<typename T>
class TemplateProperty : public Property
{
public:
    TemplateProperty(const std::string &name, const std::string& category, void* var);

    virtual ~TemplateProperty();

    virtual void setValue(const T& value);
    T getValue();

    virtual std::string getTypeName() const = 0;

private:
    T m_value;

    std::mutex m_lock;
};

#include "TemplateProperty.cpp"

#endif // TEMPLATEPROPERTY_H
