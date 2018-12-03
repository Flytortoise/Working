#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QObject>
#include <QLineEdit>

class MyLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    enum{
        INIT,
        BIN,
        OCT,
        DEC,
        HEX
    };


    explicit MyLineEdit(const QString &text, QWidget *parent = nullptr);

    void SetIndex(int index){
        m_index = index;
    }

    int GetIndex(){
        return m_index;
    }

signals:
    void MyTextEdited(unsigned int index);

public slots:
    void GetTextEdited(const QString &);

private:
    int m_index;
};

#endif // MYLINEEDIT_H
