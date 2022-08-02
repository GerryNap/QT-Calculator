#include "calculator.h"
#include "ui_calculator.h"
#include <iostream>

double calcVal = 0.0;
double mem = 0.0;

enum class Operation: char{ none, add, sub, mult, div, squareRoot, percent, mod };
Operation currentOperation = Operation::none;

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

        connect(ui->Equals, SIGNAL(released()), this, SLOT(EqualButtonPressed()));
        connect(ui->ChangeSign, SIGNAL(released()), this, SLOT(ChangeNumberSignPressed()));
        connect(ui->Dot, SIGNAL(released()), this, SLOT(DotPressed()));

        connect(ui->Clear, SIGNAL(released()), this, SLOT(ClearPressed()));
        connect(ui->Delete, SIGNAL(released()), this, SLOT(DeletePressed()));

        // Connect Memory
        connect(ui->MemClear, SIGNAL(released()), this, SLOT(MemClearPressed()));
        connect(ui->MemAdd, SIGNAL(released()), this, SLOT(MemAddPressed()));
        connect(ui->MemGet, SIGNAL(released()), this, SLOT(MemGetPressed()));

        //Connect Operations
        connect(ui->Add, SIGNAL(released()), this, SLOT(AddPressed()));
        connect(ui->Multiply, SIGNAL(released()), this, SLOT(MultPressed()));
        connect(ui->Subtract, SIGNAL(released()), this, SLOT(SubPressed()));
        connect(ui->Divide, SIGNAL(released()), this, SLOT(DivPressed()));
        connect(ui->SquareRoot, SIGNAL(released()), this, SLOT(SquareRootPressed()));
        connect(ui->Percent, SIGNAL(released()), this, SLOT(PercentPressed()));
        connect(ui->Mod, SIGNAL(released()), this, SLOT(ModPressed()));
}

Calculator::~Calculator(){
    delete ui;
}

void NumPressed(QString val, Ui::Calculator *ui){
    QString displayVal = ui->Display->text();
    if(displayVal.compare("0") == 0){
        ui->Display->setText(val);
    } else {
        QString newVal = displayVal + val;
        double dblNewVal = newVal.toDouble();
        ui->Display->setText(QString::number(dblNewVal, 'g', 16));
    }
}

// UI Controls

void Calculator::NumPressedUi(){
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();
    NumPressed(butVal, ui);
}

void Calculator::EqualButtonPressed(){
    double secondMember = ui->Display->text().toDouble();
    switch(currentOperation){
    case Operation::add:
        calcVal += secondMember;
        break;
    case Operation::div:
        calcVal /= secondMember;
        break;
    case Operation::mod:
        calcVal = std::fmod(calcVal, secondMember);
        break;
    case Operation::mult:
        calcVal *= secondMember;
        break;
    case Operation::sub:
        calcVal -= secondMember;
        break;
    case Operation::percent:
        calcVal = (secondMember / calcVal) * 100;
        break;
    default:
        return;
    }

    ui->Display->setText(QString::number(calcVal));
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
    currentOperation = Operation::none;
    calcVal = 0.0;
    ui->Display->setText(QString::number(calcVal));
}

void Calculator::DeletePressed(){ ui->Display->setText(QString("")); }

// Set point on number
bool existDot(QString str){
    for(int i = 0; i < str.length(); ++i)
        if(str.at(i) == '.')
            return true;
    return false;
}

void Calculator::DotPressed(){
    QString txt = ui->Display->text();
    if(existDot(txt))
        return;
    QString newVal = txt + '.';
    ui->Display->setText(newVal);
}

// Memory
void Calculator::MemAddPressed(){ mem = ui->Display->text().toDouble(); }
void Calculator::MemClearPressed(){ mem = 0.0; }
void Calculator::MemGetPressed(){ ui->Display->setText(QString::number(mem)); }

// Operations
void setCurrentOperation(Operation operation, Ui::Calculator *ui){
    calcVal = ui->Display->displayText().toDouble();
    currentOperation = operation;
    ui->Display->setText("");
}

void Calculator::AddPressed(){ setCurrentOperation(Operation::add, ui); }
void Calculator::SubPressed(){ setCurrentOperation(Operation::sub, ui); }
void Calculator::MultPressed(){ setCurrentOperation(Operation::mult, ui); }
void Calculator::DivPressed(){ setCurrentOperation(Operation::div, ui); }
void Calculator::PercentPressed(){ setCurrentOperation(Operation::percent, ui); }
void Calculator::ModPressed(){ setCurrentOperation(Operation::mod, ui); }
void Calculator::SquareRootPressed(){
    calcVal = std::sqrt(ui->Display->text().toDouble());
    ui->Display->setText(QString::number(calcVal));
    currentOperation = Operation::none;
}

// KeyBoard controls
void Calculator::keyPressEvent(QKeyEvent *event){
    // Check if is pressed a number
    if(Qt::Key_0 <= event->key() && event->key() <= Qt::Key_9){
        NumPressed(QString(char(event->key())), ui);
        return;
    }

    //Check if an operation is pressed
    switch(event->key()){
    case Qt::Key_Equal:case Qt::Key_Enter:case Qt::Key_Return:
        EqualButtonPressed();
        break;
    case Qt::Key_Plus:
        AddPressed();
        break;
    case Qt::Key_Minus:
        SubPressed();
        break;
    case Qt::Key_Slash:
        DivPressed();
        break;
    case Qt::Key_Asterisk:
        MultPressed();
        break;
    case Qt::Key_Percent:
        PercentPressed();
        break;
    case Qt::Key_Backspace:
        DeletePressed();
        break;
    case Qt::Key_Delete:
        ClearPressed();
        break;
    case 46:
        DotPressed();
        break;
    }
}
