#include "Widget.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QDebug>

void SwapQString(QString &str){
    int size = str.size();
    for(int i = 0; i < size / 2; i++){
        QChar tmp = str[i];
        str[i] = str[size-i-1];
        str[size-i-1] = tmp;
    }
}


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    CreateInit();
    CreateLayout();


    this->setWindowTitle(tr("二进制转换工具"));

}

Widget::~Widget()
{

}

void Widget::CreateInit(){
    m_data = 0;

    for(int i = 0; i < MAXSIZE; i++){
        m_NumLabel[i] = new QLabel(m_string.sprintf("%d",i));
        m_NumLabel[i]->setAlignment(Qt::AlignHCenter);

        m_NumState[i] = '0';
        m_NumPushButton[i] = new MyPushButton(m_string.sprintf("%c",m_NumState[i]));
        m_NumPushButton[i]->SetIndex(i);

        connect(m_NumPushButton[i], SIGNAL(MyClicked(int)),this,SLOT(ChangeDate(int)));
    }

    m_NumState[MAXSIZE] = 0;

    m_ClearButton = new QPushButton("清零");
    connect(m_ClearButton, SIGNAL(clicked()), this, SLOT(ClearZero()));

    m_NOTButton = new QPushButton("取反");
    connect(m_NOTButton, SIGNAL(clicked()), this, SLOT(Negation()));

    m_OutBinLabel = new QLabel(tr("二进制："));
    m_OutBinLabel->setAlignment(Qt::AlignRight);
    m_OutBinLineEdit = new MyLineEdit(m_string.sprintf("%s",m_NumState));
    m_OutBinLineEdit->SetIndex(MyLineEdit::BIN);
    connect(m_OutBinLineEdit, SIGNAL(MyTextEdited(unsigned int)), this, SLOT(ChangeLineData(unsigned int)));

    m_OutDecLabel = new QLabel(tr("十进制："));
    m_OutDecLabel->setAlignment(Qt::AlignRight);
    m_OutDecLineEdit = new MyLineEdit(m_string.sprintf("%u",m_data));
    m_OutDecLineEdit->SetIndex(MyLineEdit::DEC);
    connect(m_OutDecLineEdit, SIGNAL(MyTextEdited(unsigned int)), this, SLOT(ChangeLineData(unsigned int)));

    m_OutOctLabel = new QLabel(tr("八进制："));
    m_OutOctLabel->setAlignment(Qt::AlignRight);
    m_OutOctLineEdit = new MyLineEdit(m_string.sprintf("0%o",m_data));
    m_OutOctLineEdit->SetIndex(MyLineEdit::OCT);
    connect(m_OutOctLineEdit, SIGNAL(MyTextEdited(unsigned int)), this, SLOT(ChangeLineData(unsigned int)));


    m_OutHexLabel = new QLabel(tr("十六进制："));
    m_OutHexLabel->setAlignment(Qt::AlignRight);
    m_OutHexLineEdit = new MyLineEdit(m_string.sprintf("0x%x",m_data));
    m_OutHexLineEdit->SetIndex(MyLineEdit::HEX);
    connect(m_OutHexLineEdit, SIGNAL(MyTextEdited(unsigned int)), this, SLOT(ChangeLineData(unsigned int)));

}

void Widget::CreateLayout(){
    QGridLayout *mainLayout = new QGridLayout(this);


    for(int i = 0; i < 8; i++){
        mainLayout->addWidget(m_NumLabel[MAXSIZE-1-i],0,i);
        mainLayout->addWidget(m_NumPushButton[MAXSIZE-1-i],1,i);
    }

    int tmp = 8;
    for(int i = tmp; i < 16; i++){
        mainLayout->addWidget(m_NumLabel[MAXSIZE-1-i],2,i-tmp);
        mainLayout->addWidget(m_NumPushButton[MAXSIZE-1-i],3,i-tmp);
    }

    tmp = 16;
    for(int i = tmp; i < 24; i++){
        mainLayout->addWidget(m_NumLabel[MAXSIZE-1-i],4,i-tmp);
        mainLayout->addWidget(m_NumPushButton[MAXSIZE-1-i],5,i-tmp);
    }

    tmp = 24;
    for(int i = tmp; i < 32; i++){
        mainLayout->addWidget(m_NumLabel[MAXSIZE-1-i],6,i-tmp);
        mainLayout->addWidget(m_NumPushButton[MAXSIZE-1-i],7,i-tmp);
    }

    mainLayout->addWidget(m_NOTButton,8,0,1,1);
    mainLayout->addWidget(m_OutOctLabel, 8,1,1,1);
    mainLayout->addWidget(m_OutOctLineEdit, 8, 2, 1, 2);
    mainLayout->addWidget(m_OutBinLabel, 8, 4, 1, 1);
    mainLayout->addWidget(m_OutBinLineEdit, 8, 5, 1, 2);

    mainLayout->addWidget(m_ClearButton,9,0,1,1);
    mainLayout->addWidget(m_OutDecLabel, 9,1,1,1);
    mainLayout->addWidget(m_OutDecLineEdit, 9, 2, 1, 2);
    mainLayout->addWidget(m_OutHexLabel, 9, 4, 1, 1);
    mainLayout->addWidget(m_OutHexLineEdit, 9, 5, 1, 2);


}

void Widget::ChangeDate(int index){

    if(m_NumState[index] == '0'){
        m_NumState[index] = '1';
        m_data|= (1 << index);
    }
    else{
        m_NumState[index] = '0';
        m_data &= ~(1 << index);
    }

    m_NumPushButton[index]->setText(m_string.sprintf("%c",m_NumState[index]));

    m_string.sprintf("%s",m_NumState);
    SwapQString(m_string);

    OutShow();

}

void Widget::OutShow(){
    m_OutBinLineEdit->setText(m_string);
    m_OutDecLineEdit->setText(m_string.sprintf("%u",m_data));
    m_OutOctLineEdit->setText(m_string.sprintf("0%o",m_data));
    m_OutHexLineEdit->setText(m_string.sprintf("0x%x",m_data));
}


void Widget::ClearZero(){
    for(int i = 0; i < MAXSIZE; i++){
        if(m_NumState[i] == '1'){
            m_NumState[i] = '0';
            m_NumPushButton[i]->setText(m_string.sprintf("%c",m_NumState[i]));
        }
    }

    m_string.sprintf("%s",m_NumState);
    m_data = 0;
    OutShow();
}

void Widget::Negation(){
    for(int i = 0; i < MAXSIZE; i++){
        if(m_NumState[i] == '1'){
            m_NumState[i] = '0';
            m_data &= ~(1 << i);
        }
        else{
            m_NumState[i] = '1';
            m_data |= (1 << i);
        }

        m_NumPushButton[i]->setText(m_string.sprintf("%c",m_NumState[i]));
    }

    m_string.sprintf("%s",m_NumState);
    SwapQString(m_string);
    OutShow();
}


void Widget::ChangeLineData(unsigned int data){
    m_data = data;
    for(int i = 0; i < MAXSIZE; i++){
        m_NumState[i] = (data & 0x01) + '0';
        m_NumPushButton[i]->setText(QString(m_NumState[i]));

        data >>= 1;
    }

    //qDebug() << m_data;

    m_string.sprintf("%s",m_NumState);
    SwapQString(m_string);
    OutShow();
}
















