#ifndef SAVEFILEPROPERTYWIDGET_H
#define SAVEFILEPROPERTYWIDGET_H

#include <QWidget>

#include "Framework/Properties/Property.h"
#include "Framework/Properties/PropertyObserver.h"
#include "Framework/Properties/Properties.h"

class QLabel;
class QTextEdit;
class QPushButton;

class SaveFilePropertyWidget : public QWidget, public PropertyObserver
{
    Q_OBJECT
public:
    explicit SaveFilePropertyWidget(QWidget *parent = 0);
    ~SaveFilePropertyWidget();

    void setProperty(PropertyPtr property);
    void update();

private:
    std::shared_ptr<SaveFileProperty> m_property;

    QPushButton* m_selectFileButton;

signals:
    void updateWidgetSignal();

public slots:
    void valueEditted();
    void updateWidgetSlot();
};
#endif // SAVEFILEPROPERTYWIDGET_H
