#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QListWidget>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "qlabel.h"
#include "student.h"

class App;
class Student;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, App *app = nullptr);
    ~MainWindow();

    //styl
    void setExamPageDefault();

private slots:
    //sloty do kontrolek gui
    void on_questionBtn_clicked();
    void on_examBtn_clicked();
    void on_studentBtn_clicked();
    void on_wczStudBtn_clicked();
    void on_wczPytBtn_clicked();
    void on_studCheckList_itemDoubleClicked();
    void on_startExam_clicked();
    void on_drawQuestionsBTN_clicked();
    void on_saveNoteFromBlokBTN_clicked();

    //sloty z klasa app
    void on_showStudents(QVector<Student> students);
    void on_showQuestions(QVector<QVector<QString>> questions);
    void on_pickStudent(QVector<Student> students);
    void on_examStart(QVector<Student> students, int blokNum, int id);
    void on_drawedQuestions(QStringList list);


    void on_endExamBTN_clicked();
    void on_pobierzRapotyBTN_clicked();

    void on_printExamNotes(QVector<Student> students, int id);



private:
    Ui::MainWindow *ui;
    App *mainApp = nullptr;

    //lista QLabeli w mainwindow
    QList<QLabel*> qLabelsLabsNotes;
    QVBoxLayout* layoutLabNotes;

    //QLabeli w zakładce przeprowadzającej egzamin
    QList<QLabel*> qLabelsExamNotes;
    QHBoxLayout* layoutExamNotes;
};
#endif // MAINWINDOW_H
