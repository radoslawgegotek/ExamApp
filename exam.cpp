#include "exam.h"
#include "random"

Exam::Exam(QObject *parent)
    : QObject{parent}
{
}

void Exam::setFileStudents(const QString &newFileStudents)
{
    QFileInfo file(newFileStudents);

    if(file.isFile() && file.exists())
    {
        m_fileStudents.setFileName(newFileStudents);
        m_students.clear();

        QDomDocument document;

        if(m_fileStudents.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            if(document.setContent(&m_fileStudents))
            {
                QDomElement root = document.firstChildElement();
                QDomNodeList studentsXml = root.elementsByTagName("Student");

                for(int i = 0; i < studentsXml.size(); i++)
                {
                    QDomElement student = studentsXml.at(i).toElement();
                    QString name = student.attribute("Imie");
                    QString surname = student.attribute("Nazwisko");
                    QStringList labNotes = student.attribute("Oceny").split(" ");

                    Student stud(name, surname);
                    stud.setID(student.attribute("Album"));

                    for(int i = 0; i < labNotes.size(); i++)
                    {
                        stud.addNotes(labNotes[i].toDouble());
                    }
                    m_students.append(stud);
                }
            }
        }
    }
    m_fileStudents.close();
}

const QVector<Student> &Exam::students() const
{
    return m_students;
}

void Exam::setFileQuestions(const QString &newFileQuestions)
{
    QFileInfo file(newFileQuestions);

    if(file.isFile() && file.exists())
    {
        m_fileQuestions.setFileName(newFileQuestions);
        QDomDocument document;
        QVector<QString> temp;

        if(m_fileQuestions.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            if(document.setContent(&m_fileQuestions))
            {
                QDomElement root = document.firstChildElement();
                QDomNodeList bloki = root.elementsByTagName("Blok");

                for(int i = 0; i < bloki.size(); i++)
                {
                    QDomElement blok = bloki.at(i).toElement();
                    QDomNodeList pytania = blok.elementsByTagName("Pytanie");

                    for(int i = 0; i < pytania.size(); i++)
                    {
                        QDomElement que = pytania.at(i).toElement();
                        temp.append(que.attribute("Text"));
                    }
                    m_questions.append(temp);
                    temp.clear();
                }
            }
        }
    }
    m_blokNumber = m_questions.size();
    m_fileQuestions.close();
}

void Exam::setStudExamID(int n)
{
    m_StudExamID = n;
}

void Exam::addBlokNote(int blok, double note)
{
    m_students[m_StudExamID].addBlokNote(blok, note);
}

void Exam::prepareStudentData()
{
    for(int i = 0; i < m_students[m_StudExamID].getNotes().size(); i++)
    {
        if(m_students[m_StudExamID].getNotes()[i] >= 5.5)
        {
           m_students[m_StudExamID].addBlokNote(i + 1, 5);
        }
    }
}

QStringList Exam::drawQuestions()
{
    QStringList list;

    std::random_device seed; std::mt19937 rngGen;
    rngGen.seed(seed());
    //std::uniform_int_distribution<int> rngQueNum(a, b);

    for(int i = 0; i < m_questions.size(); i++)
    {
        std::uniform_int_distribution<int> rngQueNum(0, m_questions[i].size() - 1);
        list.append(m_questions[i][rngQueNum(rngGen)]);
    }

    return list;
}

const QVector<QVector<QString>> &Exam::questions() const
{
    return m_questions;
}

int Exam::StudExamID() const
{
    return m_StudExamID;
}

int Exam::blokNumber() const
{
    return m_blokNumber;
}
