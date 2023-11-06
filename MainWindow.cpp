#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Framework/PropertiesGUI/PropertiesWidget.h"

#include <sndfile.h>
#include <math.h>

#include "Patches/etapatch.h"
#include "Patches/delaypatch.h"
#include "Patches/mixerpatch.h"
#include "Patches/fileinputpatch.h"
#include "Patches/reverbcombpatch.h"
#include "Patches/reverballpasspatch.h"
#include "Patches/SineOscilatorPatch.h"
#include "Patches/sinewavetablepatch.h"
#include "Patches/allpasspatch.h"
#include "Patches/feedforwardcombpatch.h"
#include "Patches/feedbackcombpatch.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->comboBox->addItem("File input patch");
    ui->comboBox->addItem("Sine wavetable");
    ui->comboBox->addItem("Sine Oscilator");
    ui->comboBox->addItem("Mixer");
    ui->comboBox->addItem("Delay");
    ui->comboBox->addItem("Exponential time averaging");
    ui->comboBox->addItem("Feedforward comb");
    ui->comboBox->addItem("Feedback comb");
    ui->comboBox->addItem("AllPass");
    ui->comboBox->addItem("Reverberation (comb)");
    ui->comboBox->addItem("Reverberation (allpass)");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnOpen_clicked()
{
    QString selectedText = ui->comboBox->currentText();

    if (selectedText == "Sine Oscilator")
    {
        SineOscilatorPatch patch(this);
        patch.exec();
    } else if (selectedText == "AllPass"){
        AllPassPatch patch(this);
        patch.exec();
    } else if (selectedText == "Sine wavetable"){
        SineWaveTablePatch patch(this);
        patch.exec();
    } else if (selectedText == "Feedforward comb"){
        FeedforwardCombPatch patch(this);
        patch.exec();
    } else if (selectedText == "Feedback comb"){
        FeedbackCombPatch patch(this);
        patch.exec();
    } else if(selectedText == "Reverberation (comb)"){
        ReverbCombPatch patch(this);
        patch.exec();
    } else if(selectedText == "Reverberation (allpass)"){
        ReverbAllpassPatch patch(this);
        patch.exec();
    } else if (selectedText == "File input patch"){
        FileInputPatch patch(this);
        patch.exec();
    } else if (selectedText == "Mixer"){
        MixerPatch patch(this);
        patch.exec();
    } else if (selectedText == "Delay"){
        DelayPatch patch(this);
        patch.exec();
    } else if (selectedText == "Exponential time averaging"){
        ETAPatch patch(this);
        patch.exec();
    }

}
