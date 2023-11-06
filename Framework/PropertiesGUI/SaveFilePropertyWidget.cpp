#include "SaveFilePropertyWidget.h"


#include <QFileDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLabel>


SaveFilePropertyWidget::SaveFilePropertyWidget(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* hbox = new QHBoxLayout(this);
    setLayout(hbox);

    m_selectFileButton = new QPushButton(this);
    m_selectFileButton->setMaximumWidth(250);
    hbox->insertWidget(0, m_selectFileButton);

    connect(m_selectFileButton, SIGNAL(clicked()), this, SLOT(valueEditted()));
    connect(this, SIGNAL(updateWidgetSignal()), this, SLOT(updateWidgetSlot()));
}

SaveFilePropertyWidget::~SaveFilePropertyWidget()
{

}

void SaveFilePropertyWidget::setProperty(PropertyPtr property)
{
    PropertyObserver::setProperty(property);

    m_property = std::dynamic_pointer_cast<SaveFileProperty>(property);
    update();
}

void SaveFilePropertyWidget::update()
{
    emit updateWidgetSignal();
}

void SaveFilePropertyWidget::valueEditted()
{
    QFileDialog fd;
    fd.setFileMode(QFileDialog::AnyFile);
    fd.setWindowTitle(QString::fromStdString("Open: " + m_property->getName()));
    fd.setAcceptMode(QFileDialog::AcceptSave);

    QStringList fileNames;
    if (fd.exec())
    {
        fileNames = fd.selectedFiles();

        if (!fileNames.isEmpty())
        {
            m_property->setValue(fileNames[0].toStdString());
        }
    }
}

void SaveFilePropertyWidget::updateWidgetSlot()
{
    m_selectFileButton->blockSignals(true);
    m_selectFileButton->setText(QString::fromStdString(m_property->getValue()));
    m_selectFileButton->blockSignals(false);
}
