#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <QRegularExpression>
#include <QKeyEvent>
#include <cmath>

QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE

class Calculator : public QMainWindow{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private:
    Ui::Calculator *ui;

private slots:
    void NumPressedUi();
    void EqualButtonPressed();
    void ChangeNumberSignPressed();
    void ClearPressed();
    void DeletePressed();
    void DotPressed();

    // Memory
    void MemAddPressed();
    void MemClearPressed();
    void MemGetPressed();

    // Operations
    void AddPressed();
    void SubPressed();
    void MultPressed();
    void DivPressed();
    void SquareRootPressed();
    void ModPressed();
    void PercentPressed();

protected:
    void keyPressEvent(QKeyEvent *event);
};
#endif // CALCULATOR_H
