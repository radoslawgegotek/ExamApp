#ifndef EXAM_H
#define EXAM_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtXml>
#include "student.h"

class Student;

class Exam : public QObject
{
    Q_OBJECT
public:
    explicit Exam(QObject *parent = nullptr);

    //ustawienie sciezek do plikow i pobranie zawartosci
    void setFileStudents(const QString &newFileStudents);
    void setFileQuestions(const QString &newFileQuestions);

    void setStudExamID(int n);

    //dodanie oceny z bloku
    void addBlokNote(int, double);

    //przygotowanie danych do startu egzaminu, wystawia ocene 5 jesli student ma ocene z lab >= 5.5
    void prepareStudentData();

    //losowanie pytan
    QStringList drawQuestions();

    //geter
    const QVector<Student> &students() const;
    const QVector<QVector<QString>> &questions() const;

    int StudExamID() const;
    int blokNumber() const;

signals:

private:
    QFile m_fileQuestions;
    QFile m_fileStudents;
    QVector<Student> m_students;
    QVector<QVector<QString>> m_questions;
    int m_StudExamID;
    int m_blokNumber = 0;
};

#endif // EXAM_H
