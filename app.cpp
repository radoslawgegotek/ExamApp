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

void App::switchCurrentStudent()
{
    //wcześniej tutaj przekazywany był akt. index listy stud do klasy exam
    //-->jakby był błąd to tu sprawdzić
    //mainExam->setStudExamID(number);
    emit pickStudent(mainExam->students());
}

void App::updateExam(int number)
{
    mainExam->setStudExamID(number);
    mainExam->prepareStudentData();
    emit setupExam(mainExam->students(),mainExam->blokNumber(),mainExam->StudExamID());
    emit printExamNotes(mainExam->students(), mainExam->StudExamID());
}

void App::updateDraw()
{
    emit drawedQuestions(mainExam->drawQuestions());
}

void App::updateNoteFromBlok(int blok, double note)
{
    mainExam->addBlokNote(blok, note);
    emit printExamNotes(mainExam->students(), mainExam->StudExamID());
}

void App::saveRaport()
{
    mainExam->saveRaport();
}

QString App::updateInfoAboutFinalGrade()
{
    return mainExam->getInfoAboutFinalGrade();
}

