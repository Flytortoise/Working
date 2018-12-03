#include "MyLineEdit.h"

#include <QDebug>

int GetNum(QChar t){
    if(t >= '0' && t <='9')
        return t.toLatin1() - '0';

    if(t >= 'A' && t <= 'F')
        return t.toLatin1() - 'A' + 10;

    if(t >= 'a' && t <= 'f')
        return t.toLatin1() - 'a' + 10;

    return -1;
}

void ChangeHexNum(QString str, unsigned int &result){
    int index = 0;
    while(str[index] == '0')
        index++;

    if(str[index] == 'x' || str[index] == 'X')
        index++;

    for(; index < str.size(); index++){
        int num = GetNum(str[index]);
        if(num == -1)
            break;

        result *= 16;
        result += static_cast<unsigned int>(num);
    }
}

void ChangeNum(QString str, unsigned int &result, unsigned int flag){
    //qDebug() << str;
    int index = 0;
    while(str[index] == '0')
        index++;

    for(; index < str.size(); index++){
        if(str[index] <'0' || str[index] > '9')
            break;

        result *= flag;
        result += static_cast<unsigned int>(str[index].toLatin1() - '0');

    }
    //qDebug() << result;
}

MyLineEdit::MyLineEdit(const QString &text, QWidget *parent)
    : QLineEdit (text, parent)
{
    m_index = INIT;
    connect(this, SIGNAL(textEdited(const QString&)), this, SLOT(GetTextEdited(const QString&)));
}

void MyLineEdit::GetTextEdited(const QString &str){

    if(str.isEmpty())
        emit MyTextEdited(0);

    unsigned int result = 0;

    switch(m_index){
    case INIT: result = 0; break;
    case BIN: ChangeNum(str, result, 2); break;
    case OCT: ChangeNum(str, result, 8); break;
    case DEC: ChangeNum(str, result, 10);break;
    case HEX: ChangeHexNum(str, result);break;

    default:
        result = 0;
        break;
    }

    emit MyTextEdited(result);
}
