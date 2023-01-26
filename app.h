#ifndef APP_H
#define APP_H

#include <QObject>
#include "student.h"

class Exam;
class Student;

class MainWindow;

class App : public QObject
{
    Q_OBJECT
public:
    explicit App(QObject *parent = nullptr, Exam *exam = nullptr);
    ~App();

    void setGUI(MainWindow *ui = nullptr);

    //services methods
    void updateStudents(const QString &fileName);
    void updateQuestions(const QString &fileName);
    void switchCurrentStudent();
    void updateExam(int number);
    void updateDraw();
    void updateNoteFromBlok(int blok, double note);
    void saveRaport();
    QString updateInfoAboutFinalGrade();

signals:
    void showStudents(QVector<Student>);
    void pickStudent(QVector<Student>);
    void showQuestions(QVector<QVector<QString>>);
    void setupExam(QVector<Student> students, int blokNum, int id);
    void printExamNotes(QVector<Student>, int id);
    void drawedQuestions(QStringList queList);

private:
    MainWindow *gui = nullptr;
    Exam *mainExam = nullptr;
};

#endif // APP_H
