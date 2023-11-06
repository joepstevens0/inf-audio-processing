#include "Properties.h"

Properties::Properties()
{
    //Create default category
    Properties::PropertyCategory category;
    category.name = "";
    m_categories.push_back(category);
}

Properties::~Properties()
{
}

Properties::PropertyCategoryList::iterator Properties::getCategory(const std::string &category)
{
    for (PropertyCategoryList::iterator it = m_categories.begin(); it != m_categories.end(); ++it)
    {
        if (category == it->name)
        {
            return it;
        }
    }
    return m_categories.end();
}

Properties::PropertyList::iterator Properties::PropertyCategory::getProperty(const std::string &name)
{
    for (Properties::PropertyList::iterator it = properties.begin(); it != properties.end(); ++it)
    {
        if (name == (*it)->getName())
        {
            return it;
        }
    }
    return properties.end();
}

void Properties::addProperty(PropertyPtr property)
{
    PropertyCategoryList::iterator cit = getCategory(property->getCategory());

    if (cit != m_categories.end())
    {
        Properties::PropertyList::iterator pit = cit->getProperty(property->getName());
        if (pit == cit->properties.end())
            cit->properties.push_back(property);
        else
            std::cerr << "Property " << property->getCategory() << "::" << property->getName() << " already exists!" << std::endl;
    }
    else
    {
        Properties::PropertyCategory category;
        category.name = property->getCategory();
        category.properties.push_back(property);
        m_categories.push_back(category);
    }
}

PropertyPtr Properties::getProperty(const std::string &name)
{
    return getProperty("", name);
}

PropertyPtr Properties::getProperty(const std::string &category, const std::string &name)
{
    PropertyCategoryList::iterator cit;
    if ((cit = getCategory(category)) != m_categories.end())
    {
        PropertyList::iterator pit;
        if ((pit = cit->getProperty(name)) != cit->properties.end())
        {
            return *pit;
        }
    }
    return nullptr;
}

void Properties::releasePointers()
{
    PropertyCategoryList::iterator it;
    for (it = m_categories.begin(); it != m_categories.end(); ++it)
    {
        PropertyList list = it->properties;
        for (PropertyList::iterator pit = list.begin(); pit != list.end(); ++pit)
        {
            PropertyPtr prop = *pit;
            prop->release();
        }
    }
}


IntegerPropertyPtr Properties::createIntegerProperty(const std::string &name, const std::string &category, int* variable)
{
    IntegerProperty* property = new IntegerProperty(name, category, variable);
    return std::static_pointer_cast<IntegerProperty>(property->getPointer());
}

FloatPropertyPtr Properties::createFloatProperty(const std::string &name, const std::string &category, float* variable)
{
    FloatProperty* property = new FloatProperty(name, category, variable);
    return std::static_pointer_cast<FloatProperty>(property->getPointer());
}

BooleanPropertyPtr Properties::createBooleanProperty(const std::string &name, const std::string &category, bool* variable)
{
    BooleanProperty* property = new BooleanProperty(name, category, variable);
    return std::static_pointer_cast<BooleanProperty>(property->getPointer());
}

OpenFilePropertyPtr Properties::createOpenFileProperty(const std::string &name, const std::string &category, std::string *variable)
{
    OpenFileProperty* property = new OpenFileProperty(name, category, variable);
    return std::static_pointer_cast<OpenFileProperty>(property->getPointer());
}

SaveFilePropertyPtr Properties::createSaveFileProperty(const std::string &name, const std::string &category, std::string *variable)
{
    SaveFileProperty* property = new SaveFileProperty(name, category, variable);
    return std::static_pointer_cast<SaveFileProperty>(property->getPointer());
}
