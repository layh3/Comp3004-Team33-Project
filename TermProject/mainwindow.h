#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "NeuresetDevice.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onPlayClicked();
    void onPauseClicked();
    void onStopClicked();
    void onPowerButtonClicked();
    void turnOnRedLight();
    void turnOffRedLight();
    void toggleRedLight();
    void turnOnBlueLight();
    void turnOffBlueLight();
    void turnOnGreenLight();
    void turnOffGreenLight();
    void updateBatteryIndicatorStyle(int);
    void setBatteryLevel(int);

    //Menu Slots
    void startNewSession();
    void updateSessionProgress();
    void handleContactLost();
    void sessionTimeout();
    void showSessionLog();
    void showDateTimeSetting();

private:
    Ui::MainWindow *ui;
    bool powerOn = false;
    QTimer *sessionTimer;
    int elapsedTime;
    QTimer *contactLostTimer;
    int sessionDuration;
    bool contactEstablished;
    NeuresetDevice *neuresetDevice;
    QTimer *redLightTimer;
    bool redLightOn;
};
#endif // MAINWINDOW_H

// testing testing
