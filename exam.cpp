#include "exam.h"
#include "student.h"

Exam::Exam(QObject *parent)
    : QObject{parent}
{

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
            for(int i = 3; i < words.size(); i++)
            {
                s.addNotes(words[i].toDouble());
            }
            m_students.append(s);
        }
    }
    m_fileStudents.close();
}

const QVector<Student> &Exam::students() const
{
    return m_students;
}
