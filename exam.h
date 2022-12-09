#ifndef EXAM_H
#define EXAM_H

#include <QObject>
#include <QFile>
#include <QTextStream>

class Student;

class Exam : public QObject
{
    Q_OBJECT
public:
    explicit Exam(QObject *parent = nullptr);

    void setFileStudents(const QString &newFileStudents);

    const QVector<Student> &students() const;

signals:

private:
    QFile m_fileQuestions;
    QFile m_fileStudents;
    QVector<Student> m_students;
};

#endif // EXAM_H
