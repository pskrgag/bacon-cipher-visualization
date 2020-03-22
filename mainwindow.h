#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QScopedPointer>
#include <QPainter>
#include <QVector>
#include <QPlainTextEdit>
#include <QtMath>
#include <QtGlobal>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QThread>
#include <QPropertyAnimation>

#include <iostream>
namespace Ui {
class MainWindow;
}


class MyPlainTextEdit : public QPlainTextEdit{
    Q_OBJECT
    Q_PROPERTY(QString Text READ Text WRITE setText)
public:
    MyPlainTextEdit(QWidget *parent = 0):QPlainTextEdit(parent){}

    QString Text() const{
        return toPlainText();
    }
    void setText(QString& str){
        moveCursor(QTextCursor::End);
        std::cout << "Aaaa" << std::endl;
        insertPlainText(str);
        moveCursor (QTextCursor::End);
    }


};

class MyLabel : public QLabel{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
 public:
    MyLabel(QWidget *parent = 0):QLabel(parent){}
    MyLabel(QString str,QWidget *parent = 0):QLabel(parent){
        setText(str);
    }
      void setColor (QColor color){
        setStyleSheet(QString("color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));
      }
      QColor color() const{
        return Qt::black;
      }

      QColor getBackgroundColor() const{
          return palette().color(QWidget::backgroundRole());

      }


};

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent* );

private:
    QVector<MyLabel*> labels;
    QVector<MyLabel*> seq_labels;
    QScopedPointer<Ui::MainWindow> ui;
    QString sequence;
    MyPlainTextEdit* answer;
public slots:
    void seq_but_clicked();
    void animation_but_clicked();
};

#endif // MAINWINDOW_H
