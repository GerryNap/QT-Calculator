#include "calculator.h"
#include "ui_calculator.h"

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Calculator){
        ui->setupUi(this);

        ui->Display->setText(QString::number(calcVal));
        QPushButton *numButtons[10];
        for(int i = 0; i < 10; ++i){
            QString butName = "Button" + QString::number(i);
            numButtons[i] = Calculator::findChild<QPushButton *>(butName);
            connect(numButtons[i], SIGNAL(released()), this, SLOT(NumPressed()));
        }

        connect(ui->Add, SIGNAL(released()), this, SLOT(MathButtonPressed()));
        connect(ui->Multiply, SIGNAL(released()), this, SLOT(MathButtonPressed()));
        connect(ui->Subtract, SIGNAL(released()), this, SLOT(MathButtonPressed()));
        connect(ui->Divide, SIGNAL(released()), this, SLOT(MathButtonPressed()));

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

void Calculator::NumPressed(){
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();
    QString displayVal = ui->Display->text();
    if((displayVal.toDouble() == 0) || (displayVal.toDouble() == 0.0)){
        ui->Display->setText(butVal);
    } else {
        QString newVal = displayVal + butVal;
        double dblNewVal = newVal.toDouble();
        ui->Display->setText(QString::number(dblNewVal, 'g', 16));
    }
}

void Calculator::MathButtonPressed(){
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;

    calcVal = ui->Display->displayText().toDouble();

    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();

    QString operation;

    if(QString::compare(butVal, "/", Qt::CaseInsensitive) == 0){
        divTrigger = true;
        operation = "/";
    } else if(QString::compare(butVal, "*", Qt::CaseInsensitive) == 0){
        multTrigger = true;
        operation = "*";
    } else if(QString::compare(butVal, "+", Qt::CaseInsensitive) == 0){
        addTrigger = true;
        operation = "+";
    } else {
        subTrigger = true;
        operation = "-";
    }

    ui->Display->setText(operation);
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
