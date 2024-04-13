#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QTimer>
#include <QDebug>


// problems 2: //>>>>>>>>>>>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    powerOn(false),
    sessionTimer(new QTimer(this)),
    elapsedTime(0),
    contactLostTimer(new QTimer(this)),
    sessionDuration(5*60),
    contactEstablished(false),
    redLightOn(false)
{
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

        //>>>>>>>>>>>   check for power on here otherwise seesion is started even if powerr is off

        ui->mainDisplay->setCurrentIndex(0); // Show menu page
    });

    //Default to blank display until menu is clicked
    ui->mainDisplay->setCurrentIndex(2);

    // Connect menu actions to slots
    connect(ui->newSessionButton, &QPushButton::clicked, this, &MainWindow::startNewSession);
    //connect(sessionLogAction, &QAction::triggered, this, &MainWindow::showSessionLog);
    //connect(dateTimeSettingAction, &QAction::triggered, this, &MainWindow::showDateTimeSetting);

    //Timer setups
    sessionTimer->setInterval(1000); // 1000 ms = 1 second //>>>>>> sends a timeout signal every second
    redLightTimer = new QTimer(this);
    contactLostTimer->setInterval(3000); // if u change this also change the disconnnecteddtimer in electrodes
    connect(contactLostTimer, &QTimer::timeout, this, &MainWindow::sessionTimeout);
    connect(redLightTimer, &QTimer::timeout, this, &MainWindow::toggleRedLight);
    connect(sessionTimer, &QTimer::timeout, this, &MainWindow::updateSessionProgress);

    //Making the Neureset Device
    neuresetDevice = new NeuresetDevice();

    //Adding reception for neuresetDevice signal
    connect(neuresetDevice, &NeuresetDevice::contactLost, this, &MainWindow::handleContactLost); //>>>>  this deals with headset disconnection



    // graphb test

    showSessionLog();




}

MainWindow::~MainWindow()
{
    delete ui;
    delete sessionTimer;
    delete contactLostTimer;
}

//Main buttons
void MainWindow::onPlayClicked() {
    // Implement play functionality
    qDebug() << "Play clicked";
    if(!powerOn) return;
    neuresetDevice->resumeSession();
    sessionTimer->start();
    turnOnBlueLight();
    turnOffRedLight();
}

void MainWindow::onPauseClicked() {
    // Implement pause functionality
    qDebug() << "Pause clicked";
    if(!powerOn) return;
    neuresetDevice->pauseSession();
    sessionTimer->stop();
    contactLostTimer->start();
    turnOffGreenLight();
}

void MainWindow::onStopClicked() {
    // Implement stop functionality, possibly resetting to an initial state
    qDebug() << "Stop clicked";
    if(!powerOn) return;
    neuresetDevice->endSession();
    sessionTimer->stop();
    elapsedTime = 0;
    ui->mainDisplay->setCurrentIndex(0);
    qDebug() << "Session stopped. Returning to menu screen.";
    turnOffRedLight();
    turnOffBlueLight();
    turnOffGreenLight();
}

void MainWindow::onPowerButtonClicked() {
    powerOn = !powerOn;  // Toggle the power state
    if (powerOn) {
        qDebug() << "Powering on";
        ui->mainDisplay->setCurrentIndex(0);
        ui->mainDisplay->show();
    } else {
        qDebug() << "Powering off";
        ui->mainDisplay->setCurrentIndex(0);
        ui->mainDisplay->hide();
        neuresetDevice->powerOff();
        turnOffRedLight();
        turnOffBlueLight();
        turnOffGreenLight();
    }

}

//Light settings
void MainWindow::turnOnRedLight() {
    //ui->lightIndicatorRed->setStyleSheet("QLabel { background-color: red; border-radius: 10px; }");
    redLightTimer->start(500);
}

void MainWindow::turnOffRedLight() {
    redLightTimer->stop();
    ui->lightIndicatorRed->setStyleSheet("QLabel { background-color: darkred; border-radius: 10px; }");
}

void MainWindow::toggleRedLight(){
    //Created toggle function so that the light flashes instead of just changing color.
    if (redLightOn) {
            ui->lightIndicatorRed->setStyleSheet("QLabel { background-color: darkred; border-radius: 10px; }");
        } else {
            ui->lightIndicatorRed->setStyleSheet("QLabel { background-color: red; border-radius: 10px; }");
            qDebug() << "Beep!";
        }
        redLightOn = !redLightOn;
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

    //Establish contact
    neuresetDevice->startSession();
    contactEstablished = neuresetDevice->getSessionState();
    if (!contactEstablished) {
        turnOnRedLight();
        contactLostTimer->start();
    } else {
        //Successful contact results in session starting
        ui->mainDisplay->setCurrentIndex(1);
        elapsedTime = 0;
        sessionDuration = 60;
        sessionTimer->start();
        ui->sessionProgressBar->setValue(0);

    }
}

void MainWindow::updateSessionProgress() {
    if (!contactEstablished) {     // >>>>>>> big problem here, contact established is never updated
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
        int progress = static_cast<int>((static_cast<double>(elapsedTime) / sessionDuration) * 100);
        ui->sessionProgressBar->setValue(progress); // Update progress bar

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
    qDebug() << "Contact with the device lost.";
    contactEstablished = false;
    turnOnRedLight();
    contactLostTimer->start();
}

void MainWindow::sessionTimeout() {
    // Contact not reestablished after 5 minutes, turn off the device and erase the session
    sessionTimer->stop();
    contactLostTimer->stop();
    elapsedTime = 0;
    ui->mainDisplay->setCurrentIndex(0); // Go back to the main menu
    ui->sessionProgressBar->setValue(0);
    neuresetDevice->powerOff();
    qDebug() << "Session timeout reached. Powering off device.";
    onPowerButtonClicked();
    // Turn off the device, erase session data, update UI accordingly
}

void MainWindow::showSessionLog() {
    // Show session log view


    // add two new graphs and set their look:
    ui->wavePlot->addGraph();
    ui->wavePlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    ui->wavePlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    ui->wavePlot->addGraph();
    ui->wavePlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
    // generate some points of data (y0 for first, y1 for second graph):
    QVector<double> x(200), y0(200);  //, y1(251);
    for (int i=0; i<20; ++i)
    {
      x[i] = i;
      y0[i] = qCos(i/0.03) + qCos(i/0.11) + qCos(i/10); // exponentially decaying cosin
    }
    // configure right and top axis to show ticks but no labels:
    // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
    ui->wavePlot->xAxis2->setVisible(true);
    ui->wavePlot->xAxis2->setTickLabels(false);
    ui->wavePlot->yAxis2->setVisible(true);
    ui->wavePlot->yAxis2->setTickLabels(false);

    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(ui->wavePlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->wavePlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->wavePlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->wavePlot->yAxis2, SLOT(setRange(QCPRange)));

    // pass data points to graphs:
    ui->wavePlot->graph(0)->setData(x, y0);

    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    ui->wavePlot->graph(0)->rescaleAxes();


    // Note: we could have also just called ui->wavePlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    ui->wavePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);


}

void MainWindow::showDateTimeSetting() {
    // Show date and time setting UI
}
