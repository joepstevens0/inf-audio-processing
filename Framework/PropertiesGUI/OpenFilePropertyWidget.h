#ifndef OPENFILEPROPERTYWIDGET_H
#define OPENFILEPROPERTYWIDGET_H

#include <QWidget>

#include "Framework/Properties/Property.h"
#include "Framework/Properties/PropertyObserver.h"
#include "Framework/Properties/Properties.h"

class QLabel;
class QTextEdit;
class QPushButton;

class OpenFilePropertyWidget : public QWidget, public PropertyObserver
{
    Q_OBJECT
public:
    explicit OpenFilePropertyWidget(QWidget *parent = 0);
    ~OpenFilePropertyWidget();

    void setProperty(PropertyPtr property);
    void update();

private:
    std::shared_ptr<OpenFileProperty> m_property;

    QPushButton* m_selectFileButton;

signals:
    void updateWidgetSignal();

public slots:
    void valueEditted();
    void updateWidgetSlot();
};

#endif // OPENFILEPROPERTYWIDGET_H
