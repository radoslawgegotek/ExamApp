#include "mainwindow.h"
#include "app.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, App *app)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      mainApp(app)
{
    ui->setupUi(this);
    mainApp->setGUI(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_questionBtn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_3);
}


void MainWindow::on_examBtn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
}


void MainWindow::on_studentBtn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}


void MainWindow::on_wczStudBtn_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Wybierz plik ze Studentami"),
                                                    "./",
                                                    tr("Text (*.txt)"));
    mainApp->updateStudents(fileName);
}

