#ifndef BOOLEANPROPERTYWIDGET_H
#define BOOLEANPROPERTYWIDGET_H

#include <QWidget>
#include "Framework/Properties/PropertyObserver.h"
#include "Framework/Properties/Properties.h"

class QLabel;
class QCheckBox;

class BooleanPropertyWidget : public QWidget, public PropertyObserver
{
    Q_OBJECT
public:
    explicit BooleanPropertyWidget(QWidget *parent = 0);
    ~BooleanPropertyWidget();

    void setProperty(PropertyPtr property);
    void update();

private:
    std::shared_ptr<BooleanProperty> m_property;

    QCheckBox* m_checkbox;

signals:
    void updateWidgetSignal();

public slots:
    void valueEditted(int);
    void updateWidgetSlot();

};

#endif // BOOLEANPROPERTYWIDGET_H
