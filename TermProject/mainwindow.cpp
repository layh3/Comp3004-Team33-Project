#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QTimer>
#include <QDebug>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    powerOn(false),
    sessionTimer(new QTimer(this)),
    elapsedTime(0),
    contactLostTimer(new QTimer(this)),
    sessionDuration(5*60),  // this is changed to 60 later in start session
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
    ui->batteryChargeIndicator->setValue(0);
    updateBatteryIndicatorStyle(0);


    // Connect menu button click to showing the menu
    connect(ui->menuButton, &QPushButton::clicked, [this]() {

        //>>>>>>>>>>>   check for power on here otherwise seesion is started even if powerr is off
        ui->mainDisplay->setCurrentIndex(0); // Show menu page
    });

    //Default to blank display until menu is clicked
    ui->mainDisplay->setCurrentIndex(4);

    // Connect menu actions to slots
    connect(ui->newSessionButton, &QPushButton::clicked, this, &MainWindow::startNewSession);
    connect(ui->sessionLogButton, &QPushButton::clicked, this, &MainWindow::showSessionLog);
    connect(ui->dateAndTimeButton, &QPushButton::clicked, this, &MainWindow::showDateTimeSetting);

    //Timer setups
    sessionTimer->setInterval(1000); // 1000 ms = 1 second //>>>>>> sends a timeout signal every second
    redLightTimer = new QTimer(this);
    redLightTimer->setInterval(1000);
    contactLostTimer->setInterval(5000); // if u change this also change the disconnnecteddtimer in electrodes
    connect(contactLostTimer, &QTimer::timeout, this, &MainWindow::sessionTimeout);
    connect(redLightTimer, &QTimer::timeout, this, &MainWindow::toggleRedLight);
    connect(sessionTimer, &QTimer::timeout, this, &MainWindow::updateSessionProgress);

    //Making the Neureset Device
    neuresetDevice = new NeuresetDevice();

    //Adding reception for neuresetDevice signal
    connect(neuresetDevice, &NeuresetDevice::contactLost, this, &MainWindow::handleContactLost); //>>>>  this deals with headset disconnection


    // adding response to neuroset battery power lost
    connect(neuresetDevice, &NeuresetDevice::deadBattery, this, &MainWindow::sessionTimeout);


    // setup default electrode info display
    ui->electrodeSelection->setValue(0);
    ui->electrodeSelection->setRange(0,9); // this needs to be changed if u change the number of electrodes // we should jave a defs.h to standardize this

    // keep track of actively selected eletctrode
    ElectrodeInDisplay = neuresetDevice->displayElectrode( ui->electrodeSelection->value(), ui->electrodeSelection->value());

    // change active eleectrode if selection changed
    connect(ui->electrodeSelection, QOverload<int>::of(&QSpinBox::valueChanged), this, [this]() {

        qDebug()<< "updating displayed electrode";
        ElectrodeInDisplay = neuresetDevice->displayElectrode( ui->electrodeSelection->value(), ElectrodeInDisplay->getId());
        updateWavePlot();

    });


    //Adding date and time buttons
    connect(ui->CancelButton, &QPushButton::clicked, this, &MainWindow::onCancelMenuSetting);
    connect(ui->SubmitButton, &QPushButton::clicked, this, &MainWindow::onSubmitDateTimeSetting);

    onSubmitDateTimeSetting(); // running this so that selected date time has something initially

    //Addition SessionLogButtons
     connect(ui->CancelButton_2, &QPushButton::clicked, this, &MainWindow::onCancelMenuSetting);

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
    contactEstablished = neuresetDevice->getSessionState(); // not having this line causes the toggle red light slot to run infinitely
    sessionTimer->start();
    turnOnBlueLight();
    turnOffRedLight();
}

void MainWindow::onPauseClicked() {
    // Implement pause functionality
    if(!contactEstablished) return;
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
        ui->batteryChargeIndicator->setValue(neuresetDevice->getBatteryLevel());
        updateBatteryIndicatorStyle(neuresetDevice->getBatteryLevel());
    } else {
        qDebug() << "Powering off";
        ui->mainDisplay->setCurrentIndex(0);
        ui->mainDisplay->hide();
        ui->batteryChargeIndicator->setValue(0);
        updateBatteryIndicatorStyle(0);
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
    // qDebug() << "red light was turned on";
}

void MainWindow::turnOffRedLight() {
    redLightTimer->stop();
    ui->lightIndicatorRed->setStyleSheet("QLabel { background-color: darkred; border-radius: 10px; }");
    // qDebug() << "red light was turned off";
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

void MainWindow::setBatteryLevel(int level) {     //always call this function with 'neuresetDevice->getBatteryLevel()' as the level
    ui->batteryChargeIndicator->setValue(level);
    updateBatteryIndicatorStyle(level);
}

//Menu Functionality
void MainWindow::startNewSession() {
    // Initialize session state, start the timer, session object, etc.
    qDebug() << "Starting Session";

    // create new session object
    neuresetDevice->initializeSessionObject(selectedDateTime.toString("yyyy-MM-dd HH:mm:ss"));

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

    startWavePlot();
}

void MainWindow::updateSessionProgress() {
    if (!contactEstablished) {     // >>>>>>> big problem here, contact established is never updated so red light neverr turns off
        turnOnRedLight();
        // Device starts beeping, handle accordingly
        contactLostTimer->start(); // Start or continue the 5-minute countdown
    } else {
        turnOnBlueLight();
        contactLostTimer->stop();
        // Update session progress bar and time remaining display
        elapsedTime++; // Increment each second

        if (elapsedTime % 10 == 0){           //every 10 seconds, reduce battery by 1%
            int curr = neuresetDevice->getBatteryLevel() - 1;
            neuresetDevice->setBatteryLevel(curr);  //setting battery level in NeuresetDevice
            setBatteryLevel(neuresetDevice->getBatteryLevel());  //setting battery level in mainwindow
            if(curr <= 0) {
                neuresetDevice->lowBattery(); //a bit convoluted, but will be  nice if we want additional functionality from device low battery func
            }
        }

        int minutes = elapsedTime / 60;
        int seconds = elapsedTime % 60;
        ui->sessionTimerLabel->setText(QString("%1:%2").arg(minutes, 2, 10, QLatin1Char('0')).arg(seconds, 2, 10, QLatin1Char('0')));

        //PROGRESS BAR UPDATING TO BE FINISHED
        int progress = static_cast<int>((static_cast<double>(elapsedTime) / sessionDuration) * 100);
        ui->sessionProgressBar->setValue(progress); // Update progress bar
        updateWavePlot();

        if (elapsedTime >= sessionDuration) {
            sessionTimer->stop(); // Session complete
            qDebug() << "Session Complete";

            // store the sucessfully completed session object
            neuresetDevice->storeSessionObject();
            neuresetDevice->printAllSessions();

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

    contactLostTimer->stop();
    sessionTimer->stop();
    if(!powerOn) return;
    // Contact not reestablished after 5 minutes, turn off the device and erase the sessio
    elapsedTime = 0;
    ui->mainDisplay->setCurrentIndex(0); // Go back to the main menu
    ui->sessionProgressBar->setValue(0);
    neuresetDevice->powerOff();               //neuresetDevice->endSession();
    qDebug() << "Session timeout reached. Powering off device.";
    onPowerButtonClicked();
    // Turn off the device, erase session data, update UI accordingly
}

void MainWindow::showSessionLog() {
    // Show session log view
    ui->mainDisplay->setCurrentIndex(3);

}

void MainWindow::showDateTimeSetting() {
    // Show date and time setting UI
    ui->mainDisplay->setCurrentIndex(2);
}




void MainWindow::onCancelMenuSetting() {
    ui->mainDisplay->setCurrentIndex(0); // Switch back to the main menu
}

void MainWindow::onSubmitDateTimeSetting() {
    selectedDateTime = ui->dateTimeEdit->dateTime();  // the ui datetime is not updated so session start time and end time are always eqqual
    qDebug() << "Selected Date and Time:" << selectedDateTime.toString("yyyy-MM-dd HH:mm:ss");
    startTimedOperations();

    if(powerOn)
        ui->mainDisplay->setCurrentIndex(0);
}

//To keep track of internal clock
void MainWindow::startTimedOperations() {
    QTimer *operationTimer = new QTimer(this);
    connect(operationTimer, &QTimer::timeout, this, &MainWindow::performTimedOperation);
    operationTimer->start(1000); // Check or perform operations every second
    if (powerOn == false) {
        operationTimer->stop();
    }

}

void MainWindow::performTimedOperation() {
    selectedDateTime = selectedDateTime.addSecs(1);  // Simulate time passing
}

void MainWindow::handleDeadBattery() {
    qDebug() << "Battery dead. Shutting down device";
    onPowerButtonClicked();
}






void MainWindow::startWavePlot() {  // used in displaying waveform

    ui->wavePlot->addGraph();
    ui->wavePlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph


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
    ui->wavePlot->graph(0)->setData(ElectrodeInDisplay->xGraphForm  , ElectrodeInDisplay->yGraphForm);

    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    ui->wavePlot->graph(0)->rescaleAxes();


    // Note: we could have also just called ui->wavePlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    ui->wavePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}




void MainWindow::updateWavePlot() { // used in displaying waveform


    ui->wavePlot->clearGraphs();

    startWavePlot();

    ui->wavePlot->replot();
}




