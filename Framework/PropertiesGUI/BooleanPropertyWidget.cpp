#include "BooleanPropertyWidget.h"

#include <QHBoxLayout>
#include <QCheckBox>
#include <QLabel>

BooleanPropertyWidget::BooleanPropertyWidget(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* hbox = new QHBoxLayout(this);
    setLayout(hbox);

    m_checkbox = new QCheckBox(this);
    hbox->insertWidget(0, m_checkbox);

    connect(m_checkbox, SIGNAL(stateChanged(int)), this, SLOT(valueEditted(int)));
    connect(this, SIGNAL(updateWidgetSignal()), this, SLOT(updateWidgetSlot()));
}

BooleanPropertyWidget::~BooleanPropertyWidget()
{

}

void BooleanPropertyWidget::setProperty(PropertyPtr property)
{
    PropertyObserver::setProperty(property);

  //  std::cerr << property->getName() << " : In setProperty()" << std::endl;

    m_property = std::dynamic_pointer_cast<BooleanProperty>(property);
    update();

  //  std::cerr << property->getName() << " : End of setProperty()" << std::endl;

}

void BooleanPropertyWidget::update()
{
    emit updateWidgetSignal();
}

void BooleanPropertyWidget::valueEditted(int)
{
//    std::cerr << m_property->getName() << " : In valueEditted()" << std::endl;

    m_property->setValue(m_checkbox->isChecked());

//    std::cerr << m_property->getName() << " : After valueEditted()" << std::endl;

}

void BooleanPropertyWidget::updateWidgetSlot()
{
//    std::cerr << m_property->getName() << " : In updateWidgetSlot()" << std::endl;

    m_checkbox->blockSignals(true);
    m_checkbox->setChecked(m_property->getValue());
    m_checkbox->blockSignals(false);

//    std::cerr << m_property->getName() << " : After updateWidgetSlot()" << std::endl;

}
