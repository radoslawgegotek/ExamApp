#ifndef EXAM_H
#define EXAM_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>
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

    //geter
    const QVector<Student> &students() const;
    const QVector<QVector<QString>> &questions() const;

    int StudExamID() const;

signals:

private:
    QFile m_fileQuestions;
    QFile m_fileStudents;
    QVector<Student> m_students;
    QVector<QVector<QString>> m_questions;
    int m_StudExamID;
};

#endif // EXAM_H
