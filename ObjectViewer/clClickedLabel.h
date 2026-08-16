#ifndef CLCLICKEDLABEL_H
#define CLCLICKEDLABEL_H

#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

class clClickedLabel : public QLabel { 
    Q_OBJECT 

public:
    explicit clClickedLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~clClickedLabel();

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);

};

#endif // CLICKABLELABEL_H
