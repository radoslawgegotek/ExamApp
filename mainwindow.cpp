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

    ui->noteCB->addItems({ "2", "3", "3.5", "4", "4.5", "5" });

    layoutLabNotes = qobject_cast<QVBoxLayout*>(ui->labsGrades->layout());
    layoutExamNotes = qobject_cast<QHBoxLayout*>(ui->examGrades->layout());


    ///USTAWIENIA GUI
    ui->startExam->setEnabled(false);
    ui->examBtn->setEnabled(false);
    ui->stackedWidget->setCurrentWidget(ui->page_3);

    connect(app,SIGNAL(showStudents(QVector<Student>)),this,SLOT(on_showStudents(QVector<Student>)));
    connect(app,SIGNAL(showQuestions(QVector<QVector<QString>>)),this,SLOT(on_showQuestions(QVector<QVector<QString>>)));
    connect(app,SIGNAL(pickStudent(QVector<Student>)),this,SLOT(on_pickStudent(QVector<Student>)));
    connect(app,SIGNAL(setupExam(QVector<Student>, int, int)),this,SLOT(on_examStart(QVector<Student>, int, int)));
    connect(app,SIGNAL(drawedQuestions(QVector<QString>)),this,SLOT(on_drawedQuestions(QVector<QString>)));
    connect(app,SIGNAL(printExamNotes(QVector<Student>, int)),this,SLOT(on_printExamNotes(QVector<Student>, int)));
}

MainWindow::~MainWindow()
{
    for(QLabel* item : qLabelsExamNotes)
    {
        delete item;
    }

    for(QLabel* item : qLabelsLabsNotes)
    {
        delete item;
    }

    delete layoutExamNotes;
    delete layoutLabNotes;
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
                                                    tr("Text (*.xml)"));
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

void MainWindow::on_pickStudent(QVector<Student> students)
{
    int position = ui->studCheckList->currentRow();

    //usuniecie poprzednio wypisanych ocen
    for(QLabel* item : qLabelsLabsNotes)
    {
        delete item;
    }
    qLabelsLabsNotes.clear();

    ui->IdText->clear();
    ui->surNamText->clear();
    QString data = students[position].name() + " " + students[position].surname();
    ui->surNamText->setText(data);
    ui->IdText->setText(students[position].ID());


    //wypisanie ocen z labolatorium w zakladce student
    QLabel* labInfo;

    for(int i = 0; i < students[position].getNotes().size(); i++)
    {
        QString blokText = tr("Lab: %1 \t").arg(layoutLabNotes->count() + 1);
        blokText.append(QString::number(students[position].getNotes()[i]));
        labInfo = new QLabel(ui->labsGrades);
        labInfo->setText(blokText);
        layoutLabNotes->insertWidget(0, labInfo);
        qLabelsLabsNotes.append(labInfo);
    }
}

void MainWindow::on_examStart(QVector<Student> students, int blokNum, int id)
{
    //w tym miejscu slotem dopisuja sie oceny z egzaminu jesli jest zaliczenie po lab

    ui->blokCB->clear();
    ui->IdText_2->clear();
    ui->surNamText_2->clear();
    QString data = students[id].name() + " " + students[id].surname();
    ui->surNamText_2->setText(data);
    ui->IdText_2->setText(students[id].ID());

    for(int i = 0; i < blokNum; i++)
    {
        if(students[id].getNotes()[i] >= 5.5)
            continue;
        ui->blokCB->addItem(QString::number(i + 1));
    }
//    for(int i = 0; i < blokNum; i++)
//    {
//        ui->blokCB->addItem(QString::number(i + 1));
//    }
}

void MainWindow::on_drawedQuestions(QVector<QString> list)
{
    ui->showQue->clear();
    for(QString &que : list)
    {
        ui->showQue->append(que);
        ui->showQue->append("\n");
    }
}

void MainWindow::on_wczPytBtn_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Wybierz plik z pytaniami"),
                                                    "./",
                                                    tr("Text (*.xml)"));
    mainApp->updateQuestions(fileName);
}

void MainWindow::on_studCheckList_itemDoubleClicked()
{
    mainApp->switchCurrentStudent();
    ui->startExam->setEnabled(true);
}


void MainWindow::on_startExam_clicked()
{
    mainApp->updateExam(ui->studCheckList->currentRow());
    ui->stackedWidget->setCurrentWidget(ui->page);
    ui->startExam->setEnabled(false);
    ui->examBtn->setEnabled(true);
    ui->showQue->clear();
}


void MainWindow::on_drawQuestionsBTN_clicked()
{
    mainApp->updateDraw();
}


void MainWindow::on_saveNoteFromBlokBTN_clicked()
{
    if(!(ui->blokCB->currentText().isEmpty()))
        mainApp->updateNoteFromBlok(ui->blokCB->currentText().toInt(),ui->noteCB->currentText().toDouble());
}

void MainWindow::on_endExamBTN_clicked()
{
    bool saveNotesAndFinishExam;
    saveNotesAndFinishExam = QMessageBox::question(this, "Zakończyć?", "Jeśli nie wszystkie oceny zostały wystawione student otrzyma ocenę niedostateczną",
                                                   QMessageBox::Yes, QMessageBox::No);

    if(saveNotesAndFinishExam)
    {
        setExamPageDefault();
        ui->stackedWidget->setCurrentWidget(ui->page_2);
        ui->studCheckList->reset();
        ui->startExam->setEnabled(false);
        ui->examBtn->setEnabled(false);
    }

}


void MainWindow::on_pobierzRapotyBTN_clicked()
{
    mainApp->saveRaport();
    QMessageBox::information(this, "Raport","Wyniki egzaminu zostały zapisane do pliku!");
}

void MainWindow::on_printExamNotes(QVector<Student> students, int id)
{
    for(QLabel* item : qLabelsExamNotes)
    {
        delete item;
    }
    qLabelsExamNotes.clear();

    QLabel* examGradesInfo;

    for(int &item : students[id].getExamNotes().keys())
    {
        QString labelText = tr("Ocena z bloku %1: ").arg(item);
        QString ocena = QString("<span style=\" color:#ff0000;\">%1</span>").arg(QString::number(students[id].getExamNotes().value(item)));
        labelText.append(ocena);
        examGradesInfo = new QLabel(ui->examGrades);
        examGradesInfo->setText(labelText);
        layoutExamNotes->insertWidget(0,examGradesInfo);
        qLabelsExamNotes.append(examGradesInfo);
    }


    //wypisuje ocene koncowa
        ui->finalNoteInfo->setText(mainApp->updateInfoAboutFinalGrade());
}

void MainWindow::setExamPageDefault()
{
    ui->surNamText_2->clear();
    ui->IdText_2->clear();
    ui->showQue->clear();
    for(QLabel* item : qLabelsExamNotes)
    {
        delete item;
    }
    qLabelsExamNotes.clear();
}

