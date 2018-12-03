#include "MyPushButton.h"


MyPushButton::MyPushButton(const QString &text, QWidget *parent)
            : QPushButton (text, parent)
{
    connect(this, SIGNAL(clicked()), this, SLOT(GetClicked()));
}

void MyPushButton::GetClicked(){
    emit MyClicked(m_index);
}
