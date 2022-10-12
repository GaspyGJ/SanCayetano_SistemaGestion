#ifndef CONTAINEREXPANDIBLE_H
#define CONTAINEREXPANDIBLE_H

#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>

class ContainerExpandible : public QWidget{
    Q_OBJECT

private:
    QGridLayout mainLayout;
    QToolButton toggleButton;
    QFrame headerLine;
    QParallelAnimationGroup toggleAnimation;
    QScrollArea contentArea;
    int animationDuration{300};
public:
    explicit ContainerExpandible(const QString & title = "",
                                 const int animationDuration = 300, QWidget *parent = 0);
    void setContentLayout(QLayout & contentLayout);
};

#endif // CONTAINEREXPANDIBLE_H
