#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    labels(36),
    seq_labels(36)
{
    ui->setupUi(this);
    this->resize(700,700);
    this->setMinimumSize(1200,900);
    ui->plainTextEdit->move(width()*3/4, height()/2);
    ui->lineEdit_seq->setText("aaaaabbbbbabbbaabbababbaaababaabaaaa");
    ui->pushButton_2->move(width()*3/4 + width()/20, height()/2 + height()/7);
    ui->pushButton_2->setEnabled(false);
    ui->plainTextEdit->setReadOnly(true);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::seq_but_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::animation_but_clicked);
    for(int j = 0; auto& i: labels){
        if(j < 26) i = new MyLabel(QString(j + 65) , this);
        else i = new MyLabel(QString::number(j - 26) , this);
        j++;
    }

    answer = new MyPlainTextEdit(this);

}

MainWindow::~MainWindow() = default;


void MainWindow::paintEvent(QPaintEvent* e){
    ui->lineEdit_seq->move(width()*3/4, height()/3);
    ui->pushButton->move(width()*3/4 + width()/20, height()/3 + height()/20);

    ui->plainTextEdit->move(width()*3/4, height()/2);
    ui->pushButton_2->move(width()*3/4 + width()/20, height()/2 + height()/7);

    answer->move(width()*3/4, 3*height()/4);
    QPainter paint(this);
    QBrush brush(Qt::black);
    QPen pen(brush, 4);
    QFont font("Times", 15);
    paint.setPen(pen);
    const double angle_per_label = 360/(labels.size() + 0.9);
    const double r = qMin(height()/3, width()/3);
    paint.drawEllipse(QPointF(width()/3, height()/2), r, r);
    for(int j = 0 ;auto& i: labels){
        i->setGeometry(width()/3 - 8 + std::sin(angle_per_label * j * M_PI/175)*(r+30), height()/2 - 8 + std::cos(angle_per_label * j * M_PI/175)*(r+30) , 22, 25);
        i->show();
        i->setFont(font);
        if(seq_labels[j]){
            seq_labels[j]->setGeometry(width()/3 - 8 + std::sin(angle_per_label * j * M_PI/175)*(r-30), height()/2 - 8 + std::cos(angle_per_label * j * M_PI/175)*(r-30) , 22, 25);
            seq_labels[j]->show();
        }
        j++;
    }
}


static QPair<bool, QString> is_correct(const QString& text){
    for(int i = 0; i < text.size() - 5; ++i){
        if(QString(text.mid(i+1)).contains(text.midRef(i, 5))) return {false, text.midRef(i, 5).toString()};
    }

    return {true, "\0"};
}

void MainWindow::seq_but_clicked(){
    if(ui->lineEdit_seq->text().size() == 0) QMessageBox::warning(this, "Warning","Field is empty!");
    if(ui->lineEdit_seq->text().size() != labels.size()) QMessageBox::warning(this, "Warning","Sequence has wrong size!\nCorrect size :" + QString::number(labels.size()) +"\nInput size : " + QString::number(ui->lineEdit_seq->text().size()));
    else{
        auto tmp = is_correct(ui->lineEdit_seq->text());
        if(!tmp.first) QMessageBox::warning(this, "Warning","Sequence isn't correct: " + tmp.second + " is repeated!");
        else{
            for(int j = 0; auto& i: seq_labels){
                i = new MyLabel(QString(ui->lineEdit_seq->text()[j]), this);
                j++;
                i->show();
                ui->pushButton_2->setEnabled(true);
                ui->plainTextEdit->setReadOnly(false);
            }
        }
    }
}


void MainWindow::animation_but_clicked(){
    QColor def = QColor(0,0,0);
    QVector<QPropertyAnimation*> animations(5);
    if(answer) delete answer;
    answer = new MyPlainTextEdit(this);
    answer->show();
    answer->setReadOnly(true);
    for(int i = 0; i < ui->plainTextEdit->toPlainText().size(); ++i){
        if(ui->plainTextEdit->toPlainText()[i] == ' ') answer->insertPlainText (" ");
        if(ui->plainTextEdit->toPlainText()[i] > 96 && ui->plainTextEdit->toPlainText()[i] < 123){
             int j = ui->plainTextEdit->toPlainText().toStdString()[i] - 97;

             QPropertyAnimation* outside_animation = new QPropertyAnimation(labels[j], "color", this);
             outside_animation->setStartValue(QColor(255,0,0));
             outside_animation->setEndValue(def);
             outside_animation->setDuration(4000);
             outside_animation->start(QAbstractAnimation::DeleteWhenStopped);
             QString add;
             for(int k = 0; k < 5; ++k){
                animations[k] = new QPropertyAnimation(seq_labels[j - k >= 0 ? j - k : seq_labels.size() + (j - k) ], "color", this);
                add+=seq_labels[j - k >= 0 ? j - k : seq_labels.size() + (j - k) ]->text();
                animations[k]->setStartValue(QColor(0,255,0));
                animations[k]->setEndValue(def);
                animations[k]->setDuration(4000);
                animations[k]->start(QAbstractAnimation::DeleteWhenStopped);
             }

             while (animations[4]->state() != QAbstractAnimation::Stopped) QCoreApplication::processEvents();
             answer->insertPlainText(add);


         }
        if(ui->plainTextEdit->toPlainText()[i] > 47 && ui->plainTextEdit->toPlainText()[i] < 58){

            int j = 25 + ui->plainTextEdit->toPlainText().toStdString()[i] - 47;

            QPropertyAnimation* outside_animation = new QPropertyAnimation(labels[j], "color", this);
            outside_animation->setStartValue(QColor(255,0,0));
            outside_animation->setEndValue(def);
            outside_animation->setDuration(4000);
            outside_animation->start(QAbstractAnimation::DeleteWhenStopped);
            QString add;
            for(int k = 0; k < 5; ++k){
               animations[k] = new QPropertyAnimation(seq_labels[j - k >= 0 ? j - k : seq_labels.size() + (j - k) ], "color", this);
               add+=seq_labels[j - k >= 0 ? j - k : seq_labels.size() + (j - k) ]->text();
               animations[k]->setStartValue(QColor(0,255,0));
               animations[k]->setEndValue(def);
               animations[k]->setDuration(4000);
               animations[k]->start(QAbstractAnimation::DeleteWhenStopped);
            }

            while (animations[4]->state() != QAbstractAnimation::Stopped) QCoreApplication::processEvents();
            answer->insertPlainText(add);



        }
    }


}
