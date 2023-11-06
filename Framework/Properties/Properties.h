#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <map>
#include <vector>

#include "Property.h"
#include "TemplateProperty.h"

#include <list>
#include <memory>

#define DEFINE_PROPERTY(className, base) \
class className : public base { \
public: \
className(const std::string &name, const std::string &category, void* var) : \
base(name, category, var) {} \
std::string getTypeName() const { return #className; } \
}; \
typedef std::shared_ptr<className> className ## Ptr; \

DEFINE_PROPERTY(IntegerProperty, TemplateProperty<int>)
DEFINE_PROPERTY(FloatProperty, TemplateProperty<float>)
DEFINE_PROPERTY(BooleanProperty, TemplateProperty<bool>)
DEFINE_PROPERTY(OpenFileProperty, TemplateProperty<std::string>)
DEFINE_PROPERTY(SaveFileProperty, TemplateProperty<std::string>)

//DEFINE_PROPERTY(OpenDirectoryProperty, TemplateProperty<std::string>)

class Properties
{
public:
    typedef std::list<PropertyPtr> PropertyList;

    struct PropertyCategory
    {
        std::string name;
        PropertyList properties;

        std::string getName() const { return name; }
        PropertyList getProperties() const { return properties; }

        PropertyList::iterator getProperty(const std::string &name);

        void stopAllActions();
    };
    typedef std::list<PropertyCategory> PropertyCategoryList;

    Properties();

    ~Properties();

    void addProperty(PropertyPtr property);
    PropertyPtr getProperty(const std::string& name);
    PropertyPtr getProperty(const std::string &category, const std::string &name);


    PropertyCategoryList& getProperties() {return m_categories;}

    void stopAllActions();

private:
    PropertyCategoryList::iterator getCategory(const std::string &category);
    PropertyCategoryList m_categories;
public:
    static IntegerPropertyPtr createIntegerProperty(const std::string &name, const std::string &category, int *variable);
    static FloatPropertyPtr createFloatProperty(const std::string &name, const std::string &category, float *variable);
    static BooleanPropertyPtr createBooleanProperty(const std::string &name, const std::string &category, bool *variable);
    static OpenFilePropertyPtr createOpenFileProperty(const std::string &name, const std::string &category, std::string *variable);
    static SaveFilePropertyPtr createSaveFileProperty(const std::string &name, const std::string &category, std::string *variable);

public:
    /** Not part of interface, prone to change **/
    void releasePointers();
};

#endif // PROPERTIES_H
