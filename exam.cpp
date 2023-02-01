#include "exam.h"
#include "random"

Exam::Exam(QObject *parent)
    : QObject{parent}
{
}

void Exam::setFileStudents(const QString &newFileStudents)
{
    m_students.clear();

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
    m_blokNumber = 0;
    m_questions.clear();

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
                    QDomElement component = bloki.at(i).firstChild().toElement();

                    while(!component.isNull())
                    {
                        QString pytanie;
                        if(component.tagName() == "Pytanie")
                        {
                            pytanie = component.attribute("temat");
                            QDomElement child = component.firstChild().toElement();

                            while(!child.isNull())
                            {
                                if (child.tagName()=="Podstawowe") {
                                    QString podst = QString("<span style=\" color: #ff0000;\">%1</span>").arg(child.firstChild().toText().data());
                                    pytanie = pytanie + " " + podst;
                                }
                                if (child.tagName()=="Dodatkowe") {
                                    QString dodat = QString("<span style=\" color: #00ff00;\">%1</span>").arg(child.firstChild().toText().data());
                                    pytanie = pytanie + " " + dodat;
                                }
                                child = child.nextSibling().toElement();
                            }
                        }
                        temp.append(pytanie);
                        component = component.nextSibling().toElement();
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
        }
    }
    return list;
}

void Exam::saveRaport()
{
    QDomDocument document;
    document.appendChild(document.createProcessingInstruction("xml-stylesheet", "href=\"styl.css\" type=\"text/css\""));

    QDomElement root = document.createElement("Wyniki");
    document.appendChild(root);

    for(Student& s : m_students)
    {
        QDomElement student = document.createElement("Student");

        QDomElement imie = document.createElement("Imie");
        QDomText imieText = document.createTextNode(s.name());
        imie.appendChild(imieText);
        student.appendChild(imie);

        QDomElement nazwisko = document.createElement("Nazwisko");
        QDomText nazwiskoText = document.createTextNode(s.surname());
        nazwisko.appendChild(nazwiskoText);
        student.appendChild(nazwisko);

        QDomElement album = document.createElement("Album");
        QDomText albumText = document.createTextNode(s.ID());
        album.appendChild(albumText);
        student.appendChild(album);


        //zapisanie ocen czastkowych
        QString partNotes;
        for(auto it = s.getExamNotes().begin(); it != s.getExamNotes().end(); it++)
        {
            partNotes.append(" ");
            partNotes.append(QString::number(it.value()));
        }

        QDomElement ocenyCzastkowe = document.createElement("OcenyCzastkowe");
        QDomText ocenyCzastkoweText = document.createTextNode(partNotes);
        ocenyCzastkowe.appendChild(ocenyCzastkoweText);
        student.appendChild(ocenyCzastkowe);

        //zapisanie oceny koncowej
        QDomElement ocenaKoncowa = document.createElement("OcenaKońcowa");
        QDomText ocenaKoncowaNEG = document.createTextNode("Nie zaliczono");
        QDomText niePrzeprowadzono = document.createTextNode("Nie wystawiono wszystkich ocen");
        QDomText ocenaKoncowaWartosc = document.createTextNode(QString::number(s.getFinalGrade()));
        if(s.getExamNotes().size() == m_blokNumber)
        {
            for(auto it = s.getExamNotes().begin(); it != s.getExamNotes().end(); it++)
            {
                if(it.value() == 2)
                {
                    ocenaKoncowa.appendChild(ocenaKoncowaNEG);
                    break;
                }
                else
                    ocenaKoncowa.appendChild(ocenaKoncowaWartosc);
            }
        }
        else
        {
            ocenaKoncowa.appendChild(niePrzeprowadzono);
        }
        student.appendChild(ocenaKoncowa);
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
