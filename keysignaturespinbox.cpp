#include<QLineEdit>
#include<QRegExpValidator>
#include "keysignaturespinbox.hpp"

QVector<QString> KeySignatureSpinBox::m_keySigs({"C", "C#", "D", "D#", "E", "F",
                                                 "F#", "G", "G#", "A", "A#",
                                                 "B"});

KeySignatureSpinBox::KeySignatureSpinBox():
    m_pitch(0)
{
    m_validator = new QRegExpValidator(this);
    m_regExp = QRegExp("^([A-G])([#b]?)(-1|[0-9])$");
    m_validator->setRegExp(m_regExp);
    lineEdit()->setValidator(m_validator);
    lineEdit()->setText(textFromPitch(m_pitch));
    connect(lineEdit(), &QLineEdit::textEdited, this, &KeySignatureSpinBox::updatePitch);
}

void KeySignatureSpinBox::stepBy(int steps)
{
    m_pitch += steps;
    if(m_pitch < 0)
        m_pitch = wrapping()? m_pitch%127 : 0;
    else if(m_pitch > 127)
        m_pitch = wrapping()? m_pitch%127 : 127;
    lineEdit()->setText(textFromPitch(m_pitch));
}

QAbstractSpinBox::StepEnabled KeySignatureSpinBox::stepEnabled() const
{
    QAbstractSpinBox::StepEnabled enabled = QAbstractSpinBox::StepDownEnabled |
            QAbstractSpinBox::StepUpEnabled;
    if(!wrapping())
    {
        if(m_pitch == 0)
            enabled.setFlag(QAbstractSpinBox::StepDownEnabled, false);
        else if(m_pitch == 127)
            enabled.setFlag(QAbstractSpinBox::StepUpEnabled, false);
    }
    return enabled;
}

void KeySignatureSpinBox::updatePitch(const QString &text)
{
    m_pitch = pitchFromText(text);
}

int KeySignatureSpinBox::pitchFromText(const QString &text) const
{
    if(m_regExp.exactMatch(text))
    {
        QString key = m_regExp.cap(1);
        QString accidental = m_regExp.cap(2);
        if(accidental == "#")
        {
            if(key == "E")
            {
                key = "F";
                accidental = "";
            }
            else if (key == "B")
            {
                key = "C";
                accidental = "";
            }
        }
        else if(accidental == "b")
        {
            if(key == "F")
                key = "E";
            else if(key == "C")
                key = "B";
            else
                key = m_keySigs[m_keySigs.indexOf(key)-1];
            accidental = "";
        }
        int noteIndex = m_keySigs.indexOf(key+accidental);
        int octave = m_regExp.cap(3).toInt();
        return (octave+1)*12 + noteIndex;
    }
    else
        return m_pitch;
}

QString KeySignatureSpinBox::textFromPitch(int val) const
{
    int noteIndex = val % 12;
    int octave = val / 12 - 1;
    QString text = QString(m_keySigs[noteIndex]);
    text += QString::number(octave);
    return text;
}

/*QValidator::State KeySignatureSpinBox::validate(QString &input, int &pos) const
{
    m_validator
}*/
