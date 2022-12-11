#include "app.h"
#include "exam.h"
#include "mainwindow.h"

App::App(QObject *parent, Exam *exam)
    : QObject{parent},
      mainExam(exam)
{
    mainExam = new Exam;
}

App::~App()
{
    delete mainExam;
}

void App::setGUI(MainWindow *ui)
{
    gui = ui;
}

void App::updateStudents(const QString &fileName)
{
    mainExam->setFileStudents(fileName);
    emit showStudents(mainExam->students());
}

void App::updateQuestions(const QString &fileName)
{
    mainExam->setFileQuestions(fileName);
    emit showQuestions(mainExam->questions());
}
