#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Framework/Audio.h"
#include "Framework/AudioInput.h"
#include "Framework/AudioOutput.h"

class PropertiesWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_btnOpen_clicked();

private:
    Ui::MainWindow *ui;

    PropertiesWidget* propertiesWidget;
};

#endif // MAINWINDOW_H
