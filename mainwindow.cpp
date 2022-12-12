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

    connect(app,SIGNAL(showStudents(QVector<Student>)),this,SLOT(on_showStudents(QVector<Student>)));
    connect(app,SIGNAL(showQuestions(QVector<QVector<QString>>)),this,SLOT(on_showQuestions(QVector<QVector<QString>>)));

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

void MainWindow::on_showStudents(QVector<Student> students)
{
    ui->studList->clear();
    for(Student &stud : students)
    {
        QString data = stud.name() + " " + stud.surname() + " " + stud.ID();
        ui->studList->append(data);
    }

    ui->studCheckList->clear();
    for(Student &stud : students)
    {
        QString data = stud.name() + " " + stud.surname() + " " + stud.ID();
        ui->studCheckList->addItem(data);
    }
}

void MainWindow::on_showQuestions(QVector<QVector<QString>> questions)
{
    ui->questionList->clear();

    for(int i = 0; i < questions.size(); i++)
    {
        for(int j = 0; j < questions[i].size(); j++)
        {
            ui->questionList->append(questions[i][j]);
        }
    }
}


void MainWindow::on_wczPytBtn_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Wybierz plik z pytaniami"),
                                                    "./",
                                                    tr("Text (*.txt)"));
    mainApp->updateQuestions(fileName);
}

void MainWindow::on_updateList_clicked()
{

}

