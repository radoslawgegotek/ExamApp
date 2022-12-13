#include "exam.h"

Exam::Exam(QObject *parent)
    : QObject{parent}
{
}

void Exam::setFileStudents(const QString &newFileStudents)
{
    m_fileStudents.setFileName(newFileStudents);

    m_students.clear();
    if(m_fileStudents.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream file(&m_fileStudents);
        while(!file.atEnd())
        {
            QString line = file.readLine();
            QStringList words = line.split(" ");
            Student s(words[0],words[1]);
            s.setID(words[2]);
            for(int i = 3; i < words.size(); i++)
            {
                s.addNotes(words[i].toDouble());
            }
            m_students.append(s);
        }
    }
    m_fileStudents.close();
//    for(Student &el : m_students)
//    {
//        qDebug() << el.name() << ' ' << el.ID();
//        for(int i = 0; i < el.getNotes().size(); i++)
//            qDebug() << el.getNotes().at(i);
//    }
}

const QVector<Student> &Exam::students() const
{
    return m_students;
}

void Exam::setFileQuestions(const QString &newFileQuestions)
{
    m_fileQuestions.setFileName(newFileQuestions);
    m_questions.clear();

    QVector<QString> temp;
    QString line;
    QString check;
    int countLine = 0;
    if(m_fileQuestions.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream file(&m_fileQuestions);

        while(!file.atEnd())
        {
            if(countLine == 3)
            {
                temp.append(line);
                line.clear();
                countLine = 0;
            }

            check = file.readLine();
            if(check == "BLOK")
            {
                m_questions.append(temp);
                temp.clear();
                line.clear();
                check.clear();
                countLine = 0;
                continue;
            }
            line = line + check;
            check.clear();
            countLine++;
        }
    }
    m_fileQuestions.close();
}

void Exam::setStudExamID(int n)
{
    m_StudExamID = n;
}

const QVector<QVector<QString>> &Exam::questions() const
{
    return m_questions;
}

int Exam::StudExamID() const
{
    return m_StudExamID;
}
