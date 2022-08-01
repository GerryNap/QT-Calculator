#include "calculator.h"
#include "ui_calculator.h"

bool divTrigger = false;
bool multTrigger = false;
bool addTrigger = false;
bool subTrigger = false;

double calcVal = 0.0;
double mem = 0.0;

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Calculator){
        ui->setupUi(this);

        ui->Display->setText(QString::number(calcVal));
        QPushButton *numButtons[10];
        for(int i = 0; i < 10; ++i){
            QString butName = "Button" + QString::number(i);
            numButtons[i] = Calculator::findChild<QPushButton *>(butName);
            connect(numButtons[i], SIGNAL(released()), this, SLOT(NumPressedUi()));
        }

        connect(ui->Add, SIGNAL(released()), this, SLOT(MathButtonPressedUi()));
        connect(ui->Multiply, SIGNAL(released()), this, SLOT(MathButtonPressedUi()));
        connect(ui->Subtract, SIGNAL(released()), this, SLOT(MathButtonPressedUi()));
        connect(ui->Divide, SIGNAL(released()), this, SLOT(MathButtonPressedUi()));

        connect(ui->Equals, SIGNAL(released()), this, SLOT(EqualButtonPressed()));

        connect(ui->ChangeSign, SIGNAL(released()), this, SLOT(ChangeNumberSignPressed()));

        connect(ui->Clear, SIGNAL(released()), this, SLOT(ClearPressed()));

        connect(ui->MemClear, SIGNAL(released()), this, SLOT(MemClearPressed()));
        connect(ui->MemAdd, SIGNAL(released()), this, SLOT(MemAddPressed()));
        connect(ui->MemGet, SIGNAL(released()), this, SLOT(MemGetPressed()));
}

Calculator::~Calculator(){
    delete ui;
}

void NumPressed(QString val, Ui::Calculator *ui){
    QString displayVal = ui->Display->text();
    if((displayVal.toDouble() == 0) || (displayVal.toDouble() == 0.0)){
        ui->Display->setText(val);
    } else {
        QString newVal = displayVal + val;
        double dblNewVal = newVal.toDouble();
        ui->Display->setText(QString::number(dblNewVal, 'g', 16));
    }
}

void MathButtonPressed(QString operation, Ui::Calculator *ui){
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;

    calcVal = ui->Display->displayText().toDouble();

    if(QString::compare(operation, "/", Qt::CaseInsensitive) == 0){
        divTrigger = true;;
    } else if(QString::compare(operation, "*", Qt::CaseInsensitive) == 0){
        multTrigger = true;
    } else if(QString::compare(operation, "+", Qt::CaseInsensitive) == 0){
        addTrigger = true;
    } else {
        subTrigger = true;
    }

    ui->Display->setText(operation);
}

// UI Controls

void Calculator::NumPressedUi(){
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();
    NumPressed(butVal, ui);
}

void Calculator::MathButtonPressedUi(){
    QPushButton *button = (QPushButton *)sender();
    MathButtonPressed(button->text(), ui);
}

void Calculator::EqualButtonPressed(){
    double solution = 0.0;
    double dblDisplayVal = ui->Display->text().toDouble();
    if(addTrigger || multTrigger || divTrigger || subTrigger){
        if(addTrigger){
            addTrigger = false;
            solution = calcVal + dblDisplayVal;
        } else if(multTrigger){
            multTrigger = false;
            solution = calcVal * dblDisplayVal;
        } else if(divTrigger){
            divTrigger = false;
            solution = calcVal / dblDisplayVal;
        } else {
            subTrigger = false;
            solution = calcVal - dblDisplayVal;
        }
    }
    ui->Display->setText(QString::number(solution));
}

void Calculator::ChangeNumberSignPressed(){
    QString displayVal = ui->Display->text();
    if(QRegularExpression("[-]?[0-9.]*").match(displayVal).hasMatch()){
        double dblDisplayVal = displayVal.toDouble();
        double dblDisplayValSign = -1 * dblDisplayVal;
        ui->Display->setText(QString::number(dblDisplayValSign));
    }
}

void Calculator::ClearPressed(){
    calcVal = 0.0;
    ui->Display->setText(QString::number(calcVal));
}

void Calculator::MemAddPressed(){ mem = ui->Display->text().toDouble(); }

void Calculator::MemClearPressed(){ mem = 0.0; }

void Calculator::MemGetPressed(){ ui->Display->setText(QString::number(mem)); }

// KeyBoard controls

void Calculator::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
        case Qt::Key_0:case Qt::Key_1:case Qt::Key_2:case Qt::Key_3:case Qt::Key_4:
        case Qt::Key_5:case Qt::Key_6:case Qt::Key_7:case Qt::Key_8:case Qt::Key_9:
            NumPressed(QString(char(event->key())), ui);
        break;
        case Qt::Key_Equal:case Qt::Key_Enter:case Qt::Key_Return:
            EqualButtonPressed();
        break;
        case Qt::Key_Plus:case Qt::Key_Minus:case Qt::Key_Slash:case Qt::Key_Asterisk:
            MathButtonPressed(QString(char(event->key())), ui);
        break;
    }
}
