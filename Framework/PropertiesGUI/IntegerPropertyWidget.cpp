#include "IntegerPropertyWidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>

#include <limits>
IntegerPropertyWidget::IntegerPropertyWidget(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* hbox = new QHBoxLayout(this);
    setLayout(hbox);

    m_spinbox = new QSpinBox(this);
    m_spinbox->setMinimum(std::numeric_limits<int>::min());
    m_spinbox->setMaximum(std::numeric_limits<int>::max());
    m_spinbox->setAccelerated(true);
    m_spinbox->setMinimumWidth(150);

    hbox->insertWidget(0, m_spinbox);

    connect(m_spinbox, SIGNAL(valueChanged(int)), this, SLOT(valueEditted(int)));
    connect(this, SIGNAL(updateWidgetSignal()), this, SLOT(updateWidgetSlot()));
}

IntegerPropertyWidget::~IntegerPropertyWidget()
{

}

void IntegerPropertyWidget::setProperty(PropertyPtr property)
{
    PropertyObserver::setProperty(property);

    m_property = std::dynamic_pointer_cast<IntegerProperty>(property);
    update();
}

void IntegerPropertyWidget::update()
{
    emit updateWidgetSignal();
}

void IntegerPropertyWidget::valueEditted(int)
{
    m_spinbox->blockSignals(true);
    m_property->setValue(m_spinbox->value());
    m_spinbox->blockSignals(false);
}

void IntegerPropertyWidget::updateWidgetSlot()
{
    m_spinbox->blockSignals(true);
    m_spinbox->setValue(m_property->getValue());
    m_spinbox->blockSignals(false);
}

