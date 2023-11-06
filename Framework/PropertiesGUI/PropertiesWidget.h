#ifndef PROPERTIESWIDGET_H
#define PROPERTIESWIDGET_H

#include <QWidget>
#include "Framework/Properties/Properties.h"

namespace Ui {
class PropertiesWidget;
}

class QGridLayout;
class PropertiesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PropertiesWidget(QWidget *parent = 0);
    ~PropertiesWidget();

    void setProperties(Properties properties);
private:

    void fillPropertyLayout(QGridLayout* grid, const Properties::PropertyList &properties);
    void addPropertyItem(PropertyPtr property, QGridLayout* grid);

    Ui::PropertiesWidget *ui;
    Properties m_properties;
};

#endif // PROPERTIESWIDGET_H
