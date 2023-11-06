#include "PropertiesWidget.h"
#include "ui_PropertiesWidget.h"

#include "Framework/Properties/Properties.h"

#include <iostream>

#include <QTextEdit>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>

#include "Framework/PropertiesGUI/SaveFilePropertyWidget.h"
#include "Framework/PropertiesGUI/OpenFilePropertyWidget.h"
#include "Framework/PropertiesGUI/IntegerPropertyWidget.h"
#include "Framework/PropertiesGUI/FloatPropertyWidget.h"
#include "Framework/PropertiesGUI/BooleanPropertyWidget.h"

PropertiesWidget::PropertiesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PropertiesWidget)
{
    ui->setupUi(this);
}

PropertiesWidget::~PropertiesWidget()
{
    delete ui;
}

void PropertiesWidget::setProperties(Properties properties)
{
    m_properties = properties;

    Properties::PropertyCategoryList& props = m_properties.getProperties();
    Properties::PropertyCategoryList::iterator it;
    for (it = props.begin(); it != props.end(); ++it)
    {
        QGroupBox* gb = new QGroupBox(QString::fromStdString(it->getName()), this);
        QGridLayout* grid = new QGridLayout(gb);
        gb->setLayout(grid);

        ui->verticalLayout->addWidget(gb);
        fillPropertyLayout(grid, it->properties);
    }

}

void PropertiesWidget::fillPropertyLayout(QGridLayout* grid, const Properties::PropertyList &properties)
{
    Properties::PropertyList::const_iterator it;
    for (it = properties.begin(); it != properties.end(); ++it)
    {
        addPropertyItem(*it, grid);
    }
}

void PropertiesWidget::addPropertyItem(PropertyPtr property, QGridLayout* grid)
{
    bool addTextLabel = true;

    QString name = QString::fromStdString(property->getName());

    if ( std::dynamic_pointer_cast<IntegerProperty>(property) )
    {
        IntegerPropertyWidget* sb = new IntegerPropertyWidget(this);
        property->addObserver(sb);
        grid->addWidget(sb, grid->rowCount(), 1);
    }
    else if ( std::dynamic_pointer_cast<FloatProperty>(property) )
    {
        FloatPropertyWidget* sb = new FloatPropertyWidget(this);
        property->addObserver(sb);
        grid->addWidget(sb, grid->rowCount(), 1);
    }
    else if ( std::dynamic_pointer_cast<BooleanProperty>(property) )
    {
        BooleanPropertyWidget* sb = new BooleanPropertyWidget(this);
        property->addObserver(sb);
        grid->addWidget(sb, grid->rowCount(), 1);
    }
    else if ( std::dynamic_pointer_cast<OpenFileProperty>(property) )
    {
        OpenFilePropertyWidget* sb = new OpenFilePropertyWidget(this);
        property->addObserver(sb);
        grid->addWidget(sb, grid->rowCount(), 1);
    }
    else if ( std::dynamic_pointer_cast<SaveFileProperty>(property) )
    {
        SaveFilePropertyWidget* sb = new SaveFilePropertyWidget(this);
        property->addObserver(sb);
        grid->addWidget(sb, grid->rowCount(), 1);
    }

    if (addTextLabel)
    {
        QLabel* label = new QLabel(name);
        grid->addWidget(label, grid->rowCount()-1, 0);
    }


}
