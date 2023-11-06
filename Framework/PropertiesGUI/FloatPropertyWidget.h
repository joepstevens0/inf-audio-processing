#ifndef DOUBLEPROPERTYWIDGET_H
#define DOUBLEPROPERTYWIDGET_H

#include <QWidget>
#include "Framework/Properties/PropertyObserver.h"
#include "Framework/Properties/Properties.h"

class QLabel;
class QDoubleSpinBox;

class FloatPropertyWidget : public QWidget, public PropertyObserver
{
    Q_OBJECT
public:
    explicit FloatPropertyWidget(QWidget *parent = 0);
    ~FloatPropertyWidget();

    void setProperty(PropertyPtr property);
    void update();

private:
    std::shared_ptr<FloatProperty> m_property;

    QDoubleSpinBox* m_spinbox;

signals:
    void updateWidgetSignal();

public slots:
    void valueEditted(double);
    void updateWidgetSlot();

};

#endif // DOUBLEPROPERTYWIDGET_H
