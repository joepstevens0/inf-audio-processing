#include "FloatPropertyWidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>

FloatPropertyWidget::FloatPropertyWidget(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* hbox = new QHBoxLayout(this);
    setLayout(hbox);

    m_spinbox = new QDoubleSpinBox(this);
    m_spinbox->setMinimum(std::numeric_limits<float>::lowest());
    m_spinbox->setMaximum(std::numeric_limits<double>::max());
    m_spinbox->setAccelerated(true);
    m_spinbox->setMinimumWidth(150);
    m_spinbox->setDecimals(4);

    hbox->insertWidget(0, m_spinbox);

    connect(m_spinbox, SIGNAL(valueChanged(double)), this, SLOT(valueEditted(double)));
    connect(this, SIGNAL(updateWidgetSignal()), this, SLOT(updateWidgetSlot()));
}

FloatPropertyWidget::~FloatPropertyWidget()
{

}

void FloatPropertyWidget::setProperty(PropertyPtr property)
{
    PropertyObserver::setProperty(property);

    m_property = std::dynamic_pointer_cast<FloatProperty>(property);
    update();
}

void FloatPropertyWidget::update()
{
    emit updateWidgetSignal();
}

void FloatPropertyWidget::valueEditted(double)
{
    m_spinbox->blockSignals(true);
    m_property->setValue(m_spinbox->value());
    m_spinbox->blockSignals(false);
}

void FloatPropertyWidget::updateWidgetSlot()
{
    m_spinbox->blockSignals(true);
    m_spinbox->setValue(m_property->getValue());
    m_spinbox->blockSignals(false);
}
