#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <QRegularExpression>
#include <QKeyEvent>

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
    void MathButtonPressedUi();
    void EqualButtonPressed();
    void ChangeNumberSignPressed();
    void ClearPressed();
    void MemAddPressed();
    void MemClearPressed();
    void MemGetPressed();
    void DeletePressed();
    void PointPressed();
    void SquareRootPressed();
    void ModPressed();
    void PercentPressed();

protected:
    void keyPressEvent(QKeyEvent *event);
};
#endif // CALCULATOR_H
