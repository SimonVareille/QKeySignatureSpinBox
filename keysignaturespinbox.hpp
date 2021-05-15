#ifndef KEYSIGNATURESPINBOX_HPP
#define KEYSIGNATURESPINBOX_HPP

#include<QAbstractSpinBox>

#include "KeySignatureSpinBox_global.hpp"

class KEYSIGNATURESPINBOX_EXPORT KeySignatureSpinBox: public QAbstractSpinBox
{
public:
    KeySignatureSpinBox();

    virtual QAbstractSpinBox::StepEnabled stepEnabled() const override;
    void stepBy(int steps) override;
    //QValidator::State validate(QString &input, int &pos) const override;

protected:
    virtual int pitchFromText(const QString &text) const;
    virtual QString textFromPitch(int val) const;

protected slots:
    void updatePitch(const QString &text);

private:
    static QVector<QString> m_keySigs;
    int m_pitch;
    QRegExpValidator *m_validator;
    QRegExp m_regExp;

};

#endif // KEYSIGNATURESPINBOX_HPP
