#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    sessionTimer(new QTimer(this)),
    elapsedTime(0),
    contactLostTimer(new QTimer(this)) {
    ui->setupUi(this);


    //pause, play and stop button clicks
    connect(ui->pushButtonPlay, &QPushButton::clicked, this, &MainWindow::onPlayClicked);
    connect(ui->pushButtonPause, &QPushButton::clicked, this, &MainWindow::onPauseClicked);
    connect(ui->pushButtonStop, &QPushButton::clicked, this, &MainWindow::onStopClicked);

    //power button clicks
    connect(ui->powerButton, &QPushButton::clicked, this, &MainWindow::onPowerButtonClicked);

    // Initialize light indicators with 'off' colors
    ui->lightIndicatorRed->setStyleSheet("QLabel { background-color: darkred; border-radius: 10px; }");
    ui->lightIndicatorBlue->setStyleSheet("QLabel { background-color: darkblue; border-radius: 10px; }");
    ui->lightIndicatorGreen->setStyleSheet("QLabel { background-color: darkgreen; border-radius: 10px; }");

    //Initialize battery to 100 (Could need to be changed)
    ui->batteryChargeIndicator->setValue(100);
    updateBatteryIndicatorStyle(100);


    // Connect menu button click to showing the menu
    connect(ui->menuButton, &QPushButton::clicked, [this]() {
        ui->mainDisplay->setCurrentIndex(0); // Show menu page
    });

    //Default to blank display until menu is clicked
    ui->mainDisplay->setCurrentIndex(2);

    // Connect menu actions to slots
    connect(ui->newSessionButton, &QPushButton::clicked, this, &MainWindow::startNewSession);
    //connect(sessionLogAction, &QAction::triggered, this, &MainWindow::showSessionLog);
    //connect(dateTimeSettingAction, &QAction::triggered, this, &MainWindow::showDateTimeSetting);

    //Timer setups
    sessionTimer->setInterval(1000); // 1000 ms = 1 second
    connect(sessionTimer, &QTimer::timeout, this, &MainWindow::updateSessionProgress);



}

MainWindow::~MainWindow()
{
    delete ui;
}

//Main buttons
void MainWindow::onPlayClicked() {
    // Implement play functionality
    qDebug() << "Play clicked";
}

void MainWindow::onPauseClicked() {
    // Implement pause functionality
    qDebug() << "Pause clicked";
}

void MainWindow::onStopClicked() {
    // Implement stop functionality, possibly resetting to an initial state
    qDebug() << "Stop clicked";
}

void MainWindow::onPowerButtonClicked() {
    powerOn = !powerOn;  // Toggle the power state
    if (powerOn) {
        qDebug() << "Powering on";
    } else {
        qDebug() << "Powering off";
    }

}

//Light settings
void MainWindow::turnOnRedLight() {
    ui->lightIndicatorRed->setStyleSheet("QLabel { background-color: red; border-radius: 10px; }");
}

void MainWindow::turnOffRedLight() {
    ui->lightIndicatorRed->setStyleSheet("QLabel { background-color: darkred; border-radius: 10px; }");
}

void MainWindow::turnOnBlueLight() {
    ui->lightIndicatorBlue->setStyleSheet("QLabel { background-color: blue; border-radius: 10px; }");
}

void MainWindow::turnOffBlueLight() {
    ui->lightIndicatorBlue->setStyleSheet("QLabel { background-color: darkblue; border-radius: 10px; }");
}

void MainWindow::turnOnGreenLight() {
    ui->lightIndicatorGreen->setStyleSheet("QLabel { background-color: green; border-radius: 10px; }");
}

void MainWindow::turnOffGreenLight() {
    ui->lightIndicatorGreen->setStyleSheet("QLabel { background-color: darkgreen; border-radius: 10px; }");
}

//Battery functions
void MainWindow::updateBatteryIndicatorStyle(int chargeLevel) {
    QString color;
    if (chargeLevel > 75) {
        color = "green";
    } else if (chargeLevel > 25) {
        color = "yellow";
    } else {
        color = "red";
    }

    ui->batteryChargeIndicator->setStyleSheet(QString("QProgressBar::chunk { background-color: %1; }"
                                                       "QProgressBar { border: 2px solid grey; border-radius: 5px; }").arg(color));
}

void MainWindow::setBatteryLevel(int level) {
    ui->batteryChargeIndicator->setValue(level);
    updateBatteryIndicatorStyle(level);
}

//Menu Functionality
void MainWindow::startNewSession() {
    // Initialize session state, start the timer, etc.
    qDebug() << "Starting Session";

    //NEED TO INITIATE CONTACT HERE
        //WHEN CONNECTION IS ESTABLISHED START TIMER HERE
        ui->mainDisplay->setCurrentIndex(1);
        elapsedTime = 0;
         sessionDuration = 60;
        sessionTimer->start(1000);
        ui->sessionProgressBar->setValue(0);

        //FOR TESTING TO GET INTO NEW SESSION
        contactEstablished = true;
}

void MainWindow::updateSessionProgress() {
    if (!contactEstablished) {
        turnOnRedLight();
        // Device starts beeping, handle accordingly
        contactLostTimer->start(); // Start or continue the 5-minute countdown
    } else {
        turnOnBlueLight();
        contactLostTimer->stop();
        // Update session progress bar and time remaining display
        elapsedTime++; // Increment each second
        int minutes = elapsedTime / 60;
        int seconds = elapsedTime % 60;
        ui->sessionTimerLabel->setText(QString("%1:%2").arg(minutes, 2, 10, QLatin1Char('0')).arg(seconds, 2, 10, QLatin1Char('0')));

        //PROGRESS BAR UPDATING TO BE FINISHED
        //int progress = static_cast<int>((static_cast<double>(elapsedTime) / sessionDuration) * 100);
        //ui->sessionProgressBar->setValue(progress); // Update progress bar

        if (elapsedTime >= sessionDuration) {
            sessionTimer->stop(); // Session complete
            qDebug() << "Session Complete";
            // Optionally, reset to the menu page or indicate session completion
            ui->mainDisplay->setCurrentIndex(0); // Switch back to the menu page
        }
    }
}

void MainWindow::handleContactLost() {
    // This could be connected to or called by a signal indicating contact was lost
    contactEstablished = false;
}

void MainWindow::sessionTimeout() {
    // Contact not reestablished after 5 minutes, turn off the device and erase the session
    sessionTimer->stop();
    contactLostTimer->stop();
    // Turn off the device, erase session data, update UI accordingly
}

void MainWindow::showSessionLog() {
    // Show session log view
}

void MainWindow::showDateTimeSetting() {
    // Show date and time setting UI
}
