#ifndef PATCHDIALOG_H
#define PATCHDIALOG_H

#include <QDialog>

#include "Framework/Audio.h"
namespace Ui {
class PatchDialog;
}

class PatchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PatchDialog(QWidget *parent = 0);
    virtual ~PatchDialog();

    void setOutputToFile(bool _outputToFile) { outputToFile = _outputToFile; }
protected:
    virtual void createPatch() = 0;

    AudioEngine* engine;
    bool outputToFile;

protected:
    void initializePatch();
    void addGUIProperties();

private slots:
    void on_btnStart_clicked();
    void on_btnStop_clicked();


private:
    Ui::PatchDialog *ui;

};

#endif // PATCHDIALOG_H
