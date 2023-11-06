#include "OpenFilePropertyWidget.h"

#include <QFileDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLabel>


OpenFilePropertyWidget::OpenFilePropertyWidget(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* hbox = new QHBoxLayout(this);
    setLayout(hbox);

    m_selectFileButton = new QPushButton(this);
    m_selectFileButton->setMaximumWidth(250);
    hbox->insertWidget(0, m_selectFileButton);

    connect(m_selectFileButton, SIGNAL(clicked()), this, SLOT(valueEditted()));
    connect(this, SIGNAL(updateWidgetSignal()), this, SLOT(updateWidgetSlot()));
}

OpenFilePropertyWidget::~OpenFilePropertyWidget()
{

}

void OpenFilePropertyWidget::setProperty(PropertyPtr property)
{
    PropertyObserver::setProperty(property);

    m_property = std::dynamic_pointer_cast<OpenFileProperty>(property);
    update();
}

void OpenFilePropertyWidget::update()
{
    emit updateWidgetSignal();
}

void OpenFilePropertyWidget::valueEditted()
{
    QFileDialog fd;
    fd.setFileMode(QFileDialog::ExistingFile);
    fd.setWindowTitle(QString::fromStdString("Open: " + m_property->getName()));
    fd.setAcceptMode(QFileDialog::AcceptOpen);

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

void OpenFilePropertyWidget::updateWidgetSlot()
{
    m_selectFileButton->blockSignals(true);
    m_selectFileButton->setText(QString::fromStdString(m_property->getValue()));
    m_selectFileButton->blockSignals(false);
}
