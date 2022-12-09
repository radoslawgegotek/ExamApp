#ifndef APP_H
#define APP_H

#include <QObject>

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

signals:
    void printStudents(QVector<Student> vecStud);

private:
    MainWindow *gui = nullptr;
    Exam *mainExam = nullptr;
};

#endif // APP_H
