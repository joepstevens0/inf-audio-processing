#ifndef PROPERTYOBSERVER_H
#define PROPERTYOBSERVER_H

#include <string>
#include <memory>

class Property;
class PropertyObserver
{
public:
    PropertyObserver();
    ~PropertyObserver();

    virtual void setProperty(std::shared_ptr<Property> property) { m_globProperty = property; }
    virtual void update() = 0;

    std::string getName() const;

private:
    std::shared_ptr<Property> m_globProperty;
};

#endif // PROPERTYOBSERVER_H
