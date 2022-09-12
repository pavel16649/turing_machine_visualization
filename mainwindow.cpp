#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include <QTimer>
#include <QTime>


#define styleshit ("QHeaderView::section{""border-top:0px solid #D8D8D8;""border-left:0px solid #D8D8D8;""border-right:1px solid #D8D8D8;""border-bottom: 1px solid #D8D8D8;""background-color:white;""padding:4px;""}""QTableCornerButton::section{""border-top:0px solid #D8D8D8;""border-left:0px solid #D8D8D8;""border-right:1px solid #D8D8D8;""border-bottom: 1px solid #D8D8D8;""background-color:white;""}")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    color = new QColor();
    color->setRgb(255, 0, 234);

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(do_step()));

    createTape();
    ui->messageBox->append("'-' is space\nIf there's no actions when reading a symbol, please input '_' in the row of this symbol in the table.\nFormat of the program: char to write, direction to move, state to change. (ALq1, _L_, __q2, etc.)\n");
    ui->program->horizontalHeader()->setStyleSheet(styleshit);
    ui->program->verticalHeader()->setStyleSheet(styleshit);
    ui->messageBox->ensureCursorVisible();
}


MainWindow::~MainWindow()
{
    delete ui;
}

int curr_pos = 5000;
int curr_state = 0;
const int max_steps = 10000;
int curr_step = 0;
bool running = false;


void MainWindow::do_step()
{
    if(curr_step >= max_steps) {
        ui->messageBox->append("I gomt tiremd :(\n");
        timer->stop();
        return;
    }

    QString ch = ui->tape->item(0, curr_pos)->text();
    int col = 0;
    if (ch != "-") {
        for ( ; col < ui->program->columnCount(); ++col) {
            if (ui->program->horizontalHeaderItem(col)->text() == ch) {
                break;
            }
        }
    }

    int add = 0;
    QString command = ui->program->item(curr_state, col)->text();
    for (int i = 0; i < ui->program->item(curr_state, col)->text().size(); ++i) {
        if (command[i] == "!") {
            timer->stop();
            return;
        }

        if (command[i] == "_" || command[i] == "q") {
            continue;
        } else if (i == 0) {
            QString* tmp = new QString(command[i]);
            ui->tape->item(0, curr_pos)->setText(*tmp);
            delete (tmp);
        } else if (command[i] == "R" && i == 1) {
            ++add;
        } else if (command[i] == "L" && i == 1) {
            --add;
        } else if (command[i].isDigit() && i == 3) {
            QString* tmp = new QString(command[i]);
            curr_state = tmp->toInt();
            delete (tmp);
        } else {
            ui->messageBox->append("Incorrmect promgram :(");
            timer->stop();
            return;
        }
    }

    ui->tape->item(0, curr_pos)->setBackground(Qt::white);
    curr_pos += add;
    if (curr_pos > 9999 || curr_pos < 0) {
        timer->stop();
        ui->messageBox->append("\nThme tape is omver :(\nPlease clear tape before running the program again\n");
        return;
    }
    ui->tape->item(0, curr_pos)->setBackground(*color);
    if (ui->checkBox->isChecked()) {
        move_to_current();
    }
    ui->tape->repaint();

    ++curr_step;
}

void MainWindow::createTape()
{
    ui->tape->clear();
    ui->tape->setRowCount(1);
    ui->tape->setColumnCount(10000);
    for (int i = 0; i < 10000; ++i) {
        ui->tape->setItem(0, i, new QTableWidgetItem("-"));
        ui->tape->item(0, i)->setTextAlignment(Qt::AlignCenter);
    }
    ui->tape->scrollToItem(ui->tape->item(0, curr_pos+4));
    ui->tape->item(0, curr_pos)->setBackground(*color);
}

void MainWindow::turing()
{
    ui->tape->item(0, curr_pos)->setBackground(*color);

    if (ui->horizontalSlider->value() < 4) {
        timer->setInterval(1500/ui->horizontalSlider->value());
    } else {
        timer->setInterval(0);
    }

    timer->start();
    curr_step = 0;
}

bool MainWindow::check()
{
    for (int i = 0; i < ui->program->rowCount(); ++i) {
        for (int j = 0; j < ui->program->columnCount(); ++j) {
            if (ui->program->item(i, j)->text().size() > 4) {
                ui->messageBox->append("\nInvamlid inmput promgram :(\n");
                return false;
            }

        }
    }

    return true;
}

void MainWindow::on_startProgramButton_clicked()
{
    if (running) {
        timer->stop();
        running = false;
        return;
    }
    if (check()) {
        running = true;
        curr_state = 0;
        curr_step = 0;
        turing();
    }
}



void MainWindow::on_addSymbolButton_clicked()
{
    if (ui->addSymbolLine->text().isEmpty()) {
        return;
    }

    if (ui->addSymbolLine->text().size() > 1) {
        return;
    }

    QString ch = ui->addSymbolLine->text();
    if (true) {
        ui->program->insertColumn(ui->program->columnCount());
        ui->program->setHorizontalHeaderItem(ui->program->columnCount() - 1, new QTableWidgetItem(ch));
    }

}

void MainWindow::on_addStateButton_clicked()
{
    ui->program->insertRow(ui->program->rowCount());
    int row_count = ui->program->rowCount() - 1;
    QString tmp = "q";
    tmp += (char)('0' + row_count);
    ui->program->setVerticalHeaderItem(row_count, new QTableWidgetItem(tmp));
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    ui->horizontalSlider->setValue(position);
    if (ui->horizontalSlider->value() < 4) {
        timer->setInterval(1500/ui->horizontalSlider->value());
    } else {
        timer->setInterval(0);
    }
}


void MainWindow::move_to_current()
{
    ui->tape->scrollToItem(ui->tape->item(0, 0));
    ui->tape->scrollToItem(ui->tape->item(0, curr_pos+4));
}

void MainWindow::on_pushButton_2_clicked()
{
    move_to_current();
}


void MainWindow::on_program_cellChanged(int row, int column)
{
    ui->program->item(row, column)->setTextAlignment(Qt::AlignCenter);
}

void MainWindow::on_clearProgram_clicked()
{
    timer->stop();

    ui->program->clear();
    ui->program->setColumnCount(0);
    ui->program->setRowCount(0);
    ui->program->insertColumn(0);
    ui->program->setHorizontalHeaderItem(0, new QTableWidgetItem("Space"));
    ui->program->insertRow(0);
    ui->program->setVerticalHeaderItem(0, new QTableWidgetItem("q0"));
}

void MainWindow::on_clearTape_clicked()
{
    curr_pos = 5000;
    curr_state = 0;

    createTape();
}
