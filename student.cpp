#include "student.h"

Student::Student(QString &name, QString &surname)
    :m_name(name), m_surname(surname)
{
}

void Student::addNotes(double note)
{
    notes.append(note);
}

const QString &Student::name() const
{
    return m_name;
}

void Student::setName(const QString &newName)
{
    m_name = newName;
}

const QString &Student::surname() const
{
    return m_surname;
}

void Student::setSurname(const QString &newSurname)
{
    m_surname = newSurname;
}

double Student::getFinalGrade() const
{
    return finalGrade;
}

void Student::setFinalGrade(double newFinalGrade)
{
    finalGrade = newFinalGrade;
}

const QVector<double> &Student::getNotes() const
{
    return notes;
}

const QString &Student::ID() const
{
    return m_ID;
}

void Student::setID(const QString &newID)
{
    m_ID = newID;
}
