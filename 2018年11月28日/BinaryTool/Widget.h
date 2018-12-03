#ifndef Widget_H
#define Widget_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

#include "def.h"
#include "MyPushButton.h"
#include "MyLineEdit.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();


public slots:
   void ChangeDate(int);
   void ClearZero();
   void Negation();     //取反
   void ChangeLineData(unsigned int);

private:
    void CreateInit();
    void CreateLayout();
    void OutShow();
private:
    QLabel *m_NumLabel[MAXSIZE];
    MyPushButton *m_NumPushButton[MAXSIZE];
    char m_NumState[MAXSIZE+1];

    QPushButton *m_ClearButton;        //清0
    QPushButton *m_NOTButton;          //取反

    //输出二进制
    QLabel *m_OutBinLabel;
    MyLineEdit *m_OutBinLineEdit;

    //输出十进制
    QLabel  *m_OutDecLabel;
    MyLineEdit *m_OutDecLineEdit;

    //输出八进制
    QLabel *m_OutOctLabel;
    MyLineEdit *m_OutOctLineEdit;

    //输出十六进制
    QLabel *m_OutHexLabel;
    MyLineEdit *m_OutHexLineEdit;

    QString m_string;
    unsigned int m_data;

};

#endif // Widget_H
