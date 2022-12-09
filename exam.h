#ifndef EXAM_H
#define EXAM_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class Student;

class Exam : public QObject
{
    Q_OBJECT
public:
    explicit Exam(QObject *parent = nullptr);
    ~Exam();

    void setFileStudents(const QString &newFileStudents);

    const QVector<Student> &students() const;

signals:
    //void sendStudents(QVector<Student> &st);

private:
    QFile m_fileQuestions;
    QFile m_fileStudents;
    QVector<Student> *m_students;
};

#endif // EXAM_H
