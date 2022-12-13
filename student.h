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

    const QVector<double> &getNotes() const;

    const QString &ID() const;
    void setID(const QString &newID);

private:
    QString m_name;
    QString m_surname;
    QString m_ID;
    QVector<double> notes;
    QVector<double> examNotes;
    double finalGrade;
};

#endif // STUDENT_H
