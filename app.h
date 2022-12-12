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
    void chooseStudent(int number);
signals:
    void showStudents(QVector<Student>);
    void showStudentss(QVector<Student>);
    void showQuestions(QVector<QVector<QString>>);

private:
    MainWindow *gui = nullptr;
    Exam *mainExam = nullptr;
};

#endif // APP_H
