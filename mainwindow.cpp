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

    connect(app,SIGNAL(showStudents(QVector<Student>)),this,SLOT(on_showStudents(QVector<Student>)));
    connect(app,SIGNAL(showQuestions(QVector<QVector<QString>>)),this,SLOT(on_showQuestions(QVector<QVector<QString>>)));
    connect(app,SIGNAL(pickStudent(QVector<Student>)),this,SLOT(on_pickStudent(QVector<Student>)));
    connect(app,SIGNAL(setupExam(QVector<Student>, int, int)),this,SLOT(on_examStart(QVector<Student>, int, int)));
    connect(app,SIGNAL(drawedQuestions(QStringList)),this,SLOT(on_drawedQuestions(QStringList)));
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

    for(QLabel* item : listaBlokow)
    {
        delete item;
    }
    listaBlokow.clear();

    ui->IdText->clear();
    ui->surNamText->clear();
    QString data = students[position].name() + " " + students[position].surname();
    ui->surNamText->setText(data);
    ui->IdText->setText(students[position].ID());


    //wypisanie ocen z labolatorium w zakladce student
    QLabel* blokInfo;
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->labsGrades->layout());

    for(int i = 0; i < students[position].getNotes().size(); i++)
    {
        QString blokText = tr("lab.%1 \t").arg(layout->count() + 1);
        blokText.append(QString::number(students[position].getNotes()[i]));
        blokInfo = new QLabel(ui->labsGrades);
        blokInfo->setText(blokText);
        layout->insertWidget(0, blokInfo);
        listaBlokow.append(blokInfo);
    }
}

void MainWindow::on_examStart(QVector<Student> students, int blokNum, int id)
{
    ui->blokCB->clear();
    ui->IdText_2->clear();
    ui->surNamText_2->clear();
    QString data = students[id].name() + " " + students[id].surname();
    ui->surNamText_2->setText(data);
    ui->IdText_2->setText(students[id].ID());

    for(int i = 0; i < blokNum; i++)
    {
        ui->blokCB->addItem(QString::number(i + 1));
    }
}

void MainWindow::on_drawedQuestions(QStringList list)
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
    mainApp->updateStudExamID(ui->studCheckList->currentRow());
}


void MainWindow::on_startExam_clicked()
{
    mainApp->updateExam();
    ui->stackedWidget->setCurrentWidget(ui->page);
}


void MainWindow::on_drawQuestionsBTN_clicked()
{
    mainApp->updateDraw();
}


void MainWindow::on_saveNoteFromBlokBTN_clicked()
{
    mainApp->updateNoteFromBlok(ui->blokCB->currentText().toInt(),ui->noteCB->currentText().toDouble());
}

void MainWindow::on_endExamBTN_clicked()
{
    bool saveNotesAndFinishExam;
    saveNotesAndFinishExam = QMessageBox::question(this, "Zapisz i zakończ", "Zapisać oceny i zakończyć?",
                                                   QMessageBox::Yes, QMessageBox::No);

    if(saveNotesAndFinishExam)
    {
        //zapis wynikow do tablicy/vec/list

    }

}

