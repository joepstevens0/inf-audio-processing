#ifndef INTEGERPROPERTYWIDGET_H
#define INTEGERPROPERTYWIDGET_H

#include <QWidget>

#include "Framework/Properties/PropertyObserver.h"
#include "Framework/Properties/Properties.h"

class QLabel;
class QSpinBox;

class IntegerPropertyWidget : public QWidget, public PropertyObserver
{
    Q_OBJECT
public:
    explicit IntegerPropertyWidget(QWidget *parent = 0);
    ~IntegerPropertyWidget();

    void setProperty(PropertyPtr property);
    void update();

private:
    std::shared_ptr<IntegerProperty> m_property;

    QSpinBox* m_spinbox;

signals:
    void updateWidgetSignal();

public slots:
    void valueEditted(int);
    void updateWidgetSlot();

};

#endif // INTEGERPROPERTYWIDGET_H
