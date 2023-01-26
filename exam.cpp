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
                prepareStudentData();
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
    calculateFinal(m_StudExamID);
}

void Exam::prepareStudentData()
{
    for(int i = 0; i < m_students.size(); i++)
    {
        for(int j = 0; j < m_students[i].getNotes().size(); j++)
        {
            if(m_students[i].getNotes()[j] >= 5.5)
            {
               m_students[i].addBlokNote(j + 1, 5);
            }
        }
        calculateFinal(i);
    }
}

QStringList Exam::drawQuestions()
{
    QVector<QString> list;
    std::random_device seed; std::mt19937 rngGen;
    rngGen.seed(seed());
    int x, y, z;

    for(int i = 0; i < m_questions.size(); i++)
    {
        QString blok = QString("<span style=\" color:#ff0000; font-weight:400; font-size:18px;\">BLOK %1:</span>").arg(i + 1);
        //QString a ="--------------------------------- BLOK: " + QString::number(i + 1) + " ---------------------------------";
        list.append(blok);
        std::uniform_int_distribution<int> rngQueNum(0, m_questions[i].size() - 1);
        if(m_students[m_StudExamID].getNotes()[i] < 3.5){
            list.append(m_questions[i][rngQueNum(rngGen)]);
        }
        else if(m_students[m_StudExamID].getNotes()[i] < 4.5){
            x = rngQueNum(rngGen);
            list.append(m_questions[i][x]);
            do{
                y = rngQueNum(rngGen);
            }while(x == y);
            list.append(m_questions[i][y]);
        }
        else if(m_students[m_StudExamID].getNotes()[i] < 5.5){
            x = rngQueNum(rngGen);
            list.append(m_questions[i][x]);
            do{
                y = rngQueNum(rngGen);
            }while(x == y);
            list.append(m_questions[i][y]);
            do{
                z = rngQueNum(rngGen);
            }while(x == z || z == y);
            list.append(m_questions[i][z]);
        }
        else{
            list.append("BLOK ZALICZONY");
            addBlokNote(i, 5);
        }
    }

    return list;
}

void Exam::saveRaport()
{
    QDomDocument document;
    QDomElement root = document.createElement("Wyniki");
    document.appendChild(root);

    for(Student& s : m_students)
    {
        QDomElement student = document.createElement("Student");
        student.setAttribute("Imie", s.name());
        student.setAttribute("Nazwisko", s.surname());
        student.setAttribute("Album", s.ID());

        //zapisanie ocen czastkowych
        QString partNotes;
        for(int i = 0; i < s.getExamNotes().size(); i++)
        {
            partNotes = partNotes + QString::number(s.getExamNotes()[i + 1]) + " ";
        }
        student.setAttribute("OcenyCząstkowe", partNotes);

        //zapisanie oceny koncowej
        if(s.getExamNotes().size() == m_blokNumber)
        {
            for(auto it = s.getExamNotes().begin(); it != s.getExamNotes().end(); it++)
            {
                if(it.value() == 2)
                {
                    student.setAttribute("OcenaKońcowa", "Nie zaliczono");
                    break;
                }
                else
                    student.setAttribute("OcenaKońcowa", s.getFinalGrade());
            }
        }
        else
        {
            student.setAttribute("OcenaKońcowa", "Nie zaliczono");
        }

        root.appendChild(student);
    }


    QFile writeResults("wyniki.xml");
    if(writeResults.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream content(&writeResults);
        content << document.toString();
    }
    writeResults.close();
}

QString Exam::getInfoAboutFinalGrade()
{
    for(int key : m_students[m_StudExamID].getExamNotes().keys())
        {
            if(m_students[m_StudExamID].getExamNotes().value(key) == 2)
            {
                return "Brak zaliczenia";
            }
        }

        if(m_students[m_StudExamID].getExamNotes().size() == m_blokNumber)
            return QString::number(m_students[m_StudExamID].getFinalGrade());
        else
            return "Brak zaliczenia";
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

void Exam::calculateFinal(int i)
{
    double sum = 0;
    if(m_students[i].getExamNotes().size() == m_blokNumber)
    {
        for(int &key : m_students[i].getExamNotes().keys())
        {
            sum += m_students[i].getExamNotes().value(key);
        }
        m_students[i].setFinalGrade(sum/m_blokNumber);
    }
}
