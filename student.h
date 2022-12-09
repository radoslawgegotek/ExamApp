#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QVector>

class Student
{
public:
    Student(QString& name, QString& surname);

    void addNotes(double note);

    const QString &name() const;
    void setName(const QString &newName);

    const QString &surname() const;
    void setSurname(const QString &newSurname);

    double getFinalGrade() const;
    void setFinalGrade(double newFinalGrade);

private:
    QString m_name;
    QString m_surname;
    QVector<double> notes;
    double finalGrade;
};

#endif // STUDENT_H
