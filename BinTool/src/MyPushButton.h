#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QObject>
#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT

public:
    explicit MyPushButton(const QString &text, QWidget *parent = nullptr);

    void SetIndex(int index){
        m_index = index;
    }

    int GetIndex(){
        return m_index;
    }

signals:
    void MyClicked(int index);
public slots:
    void GetClicked();

private:
    int m_index;
};

#endif // MYPUSHBUTTON_H
