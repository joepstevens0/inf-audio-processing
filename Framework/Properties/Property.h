#ifndef PROPERTY_H
#define PROPERTY_H

#include <string>
#include <vector>

#include <memory>

class PropertyObserver;

class Property
{
public:
    Property(const std::string& name, const std::string& category, void* var);
    virtual ~Property();

    std::string getName() const {return m_name;}

    std::shared_ptr<Property> getPointer() const { return m_pointer; }

    void setCategory(const std::string& category) {m_category = category;}
    std::string getCategory() const {return m_category;}

    void addObserver(PropertyObserver* observer);

    void release() { m_pointer.reset(); }

    virtual std::string getTypeName() const = 0;

protected:
    void notify();

    std::shared_ptr<Property> m_pointer;

    std::string m_name;
    std::string m_category;
    std::vector<PropertyObserver*> m_observers;

    void* m_var;
};
typedef std::shared_ptr<Property> PropertyPtr;

#endif // PROPERTY_H
