#include "exam.h"
#include "student.h"

Exam::Exam(QObject *parent)
    : QObject{parent}
{
    m_students = new QVector<Student>;
}

Exam::~Exam()
{
    delete m_students;
}

void Exam::setFileStudents(const QString &newFileStudents)
{
    m_fileStudents.setFileName(newFileStudents);

//    if(!m_fileStudents.open(QIODevice::ReadOnly)) {
//        QMessageBox::information(0,"Read error",studentsData.errorString());
//    }

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
            m_students->append(s);
        }
    }
    m_fileStudents.close();

    for(Student &el : *m_students)
    {
        qDebug() << el.name() << ' ' << el.ID();
        for(int i = 0; i < el.getNotes().size(); i++)
            qDebug() << el.getNotes().at(i);
    }
}

const QVector<Student> &Exam::students() const
{
    return *m_students;
}
