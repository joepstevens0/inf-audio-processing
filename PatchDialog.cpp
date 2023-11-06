#include "PatchDialog.h"
#include "ui_PatchDialog.h"

#include "Framework/PropertiesGUI/PropertiesWidget.h"

PatchDialog::PatchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatchDialog)
{
    outputToFile = false;
    ui->setupUi(this);

    setWindowTitle("Empty Patch");
}

PatchDialog::~PatchDialog()
{
    delete engine;
    delete ui;
}

void PatchDialog::initializePatch()
{
    engine = new AudioEngine();
    engine->init(windowTitle().toStdString().c_str());

    this->createPatch();

    addGUIProperties();
}

void PatchDialog::on_btnStart_clicked()
{
    engine->activate();
}

void PatchDialog::on_btnStop_clicked()
{
    engine->deactivate();
}


void PatchDialog::addGUIProperties()
{
    PropertiesWidget* propertiesWidget = new PropertiesWidget(this);
    this->ui->verticalLayout->addWidget(propertiesWidget);

    propertiesWidget->setProperties(*engine->getProperties());
}
