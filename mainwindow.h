#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"



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

    void turing();

    void do_step();

    bool check();

    void createTape();

    void on_startProgramButton_clicked();

    void on_addSymbolButton_clicked();

    void on_addStateButton_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_pushButton_2_clicked();

    void move_to_current();

    void on_program_cellChanged(int row, int column);

    void on_clearProgram_clicked();

    void on_clearTape_clicked();

private:
    QColor* color;
    QTimer* timer;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
