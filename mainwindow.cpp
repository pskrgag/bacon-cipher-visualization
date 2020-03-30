#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

static QPair<bool, QString> is_correct(const QString& text){
    for(int i = 0; i < text.size() - 5; ++i){
        if(QString(text.mid(i+1)).contains(text.midRef(i, 5))) return {false, text.midRef(i, 5).toString()};
    }

    return {true, "\0"};
}


//static_cast<int>((current + qPow(-1, current - 97)))
static QString random_seq_generator(std::size_t size){

    std::map<QString, int> prefix_count;
    QString sequence;
    static std::random_device device;
    static std::mt19937 generator(device());
    static std::uniform_int_distribution<> dis(97, 98);
    for(int i = 0; i < 6; ++i){
        sequence += dis(generator);
        if(i == 2) prefix_count[sequence]++;
        if(i == 4) prefix_count[sequence]++;
    }

    for(int i = 6; i < size; ++i){
        int current = dis(generator);
        if(prefix_count[QString(sequence + current).mid(i-4, 4)] == 2){
            std::cout << QString(sequence + current).mid(i-4, 4).toStdString() << std::endl;
            sequence+=static_cast<int>((current + qPow(-1, current - 97)));
            prefix_count[sequence.mid(i-3,4)]++;
        }
        else{
            if(!is_correct(sequence + current).first) current = static_cast<int>((current + qPow(-1, current - 97)));
            sequence+=current;
            prefix_count[sequence.mid(i-3,4)]++;
        }


    }

    std::cout << sequence.toStdString() << std::endl;
    return sequence;

}

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
    ui->pushButton_4->setEnabled(false);
    ui->plainTextEdit->setReadOnly(true);
    ui->pushButton_5->setEnabled(false);



    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::seq_but_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::animation_but_clicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::generate_sequence_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::open_file_clicked);
    connect(this, &MainWindow::file_opened, this, &MainWindow::animation_but_clicked);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::save_file_clicked);

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
    ui->pushButton->move(width()*3/4 + width()/40, height()/3 + height()/20);
    ui->pushButton_3->move(width()*3/4 + width()/9, height()/3 + height()/20);

    ui->plainTextEdit->move(width()*3/4, height()/2);
    ui->pushButton_2->move(width()*3/4 + width()/30, height()/2 + height()/7);
    ui->pushButton_4->move(width()*3/4 + width()/7, height()/2 + height()/7);

    ui->pushButton_5->move(width()*4/5, 29*height()/30);

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




void MainWindow::seq_but_clicked(){
    if(ui->lineEdit_seq->text().size() == 0) QMessageBox::warning(this, "Warning","Field is empty!");
    if(ui->lineEdit_seq->text().size() != labels.size()) QMessageBox::warning(this, "Warning","Sequence has wrong size!\nCorrect size :" + QString::number(labels.size()) +"\nInput size : " + QString::number(ui->lineEdit_seq->text().size()));
    else{
        ui->pushButton_2->setEnabled(true);
        ui->plainTextEdit->setReadOnly(false);
        ui->pushButton_4->setEnabled(true);
        auto tmp = is_correct(ui->lineEdit_seq->text());
        if(!tmp.first) QMessageBox::warning(this, "Warning","Sequence isn't correct: " + tmp.second + " is repeated!");
        else{
            for(int j = 0; auto& i: seq_labels){
                i = new MyLabel(QString(ui->lineEdit_seq->text()[j]), this);
                j++;
                i->show();
            }
        }
    }
}


void MainWindow::animation_but_clicked(){
    if(!ui->plainTextEdit->toPlainText().size()){
        QMessageBox::warning(this, "Warning","Field is empty!");
        return;
    }
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_2->setEnabled(false);

    QColor def = QColor(0,0,0);
    QVector<QPropertyAnimation*> animations(5);
    if(answer) delete answer;
    answer = new MyPlainTextEdit(this);
    answer->show();
    answer->setReadOnly(true);
    for(int i = 0; i < ui->plainTextEdit->toPlainText().size(); ++i){
        if(ui->plainTextEdit->toPlainText()[i] == ' ') answer->insertPlainText (" ");
        if(ui->plainTextEdit->toPlainText()[i] == '\n') answer->insertPlainText ("\n");
        if(ui->plainTextEdit->toPlainText()[i] > 96 && ui->plainTextEdit->toPlainText()[i] < 123){
             int j = ui->plainTextEdit->toPlainText().toStdString()[i] - 97;

             QPropertyAnimation* outside_animation = new QPropertyAnimation(labels[j], "color", this);
             outside_animation->setStartValue(QColor(255,0,0));
             outside_animation->setEndValue(def);
             outside_animation->setDuration(6000);
             outside_animation->start(QAbstractAnimation::DeleteWhenStopped);
             QString add;
             for(int k = 0; k < 5; ++k){
                animations[k] = new QPropertyAnimation(seq_labels[j - k >= 0 ? j - k : seq_labels.size() + (j - k) ], "color", this);
                add+=seq_labels[j - k >= 0 ? j - k : seq_labels.size() + (j - k) ]->text();
                animations[k]->setStartValue(QColor(0,255,0));
                animations[k]->setEndValue(def);
                animations[k]->setDuration(6000);
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
            outside_animation->setDuration(6000);
            outside_animation->start(QAbstractAnimation::DeleteWhenStopped);
            QString add;
            for(int k = 0; k < 5; ++k){
               animations[k] = new QPropertyAnimation(seq_labels[j - k >= 0 ? j - k : seq_labels.size() + (j - k) ], "color", this);
               add+=seq_labels[j - k >= 0 ? j - k : seq_labels.size() + (j - k) ]->text();
               animations[k]->setStartValue(QColor(0,255,0));
               animations[k]->setEndValue(def);
               animations[k]->setDuration(6000);
               animations[k]->start(QAbstractAnimation::DeleteWhenStopped);
            }

            while (animations[4]->state() != QAbstractAnimation::Stopped) QCoreApplication::processEvents();
            answer->insertPlainText(add);



        }
    }
    ui->pushButton_5->setEnabled(true);


}
void MainWindow::generate_sequence_clicked(){
    ui->lineEdit_seq->setText(random_seq_generator(labels.size()));

}
void MainWindow::open_file_clicked(){

    QFile file(QFileDialog::getOpenFileName(this, "Choose text file", ""));
    if(!file.open(QIODevice::ReadOnly)) QMessageBox::warning(this, "Error!","Can't open file");
    QTextStream stream(&file);
    QString tmp;
    while(!(tmp = stream.readLine()).isNull()) ui->plainTextEdit->insertPlainText(tmp);

    emit file_opened();


}


void MainWindow::save_file_clicked(){
    QFile file(QFileDialog::getSaveFileName(this, "Choose directory and file name", ""));
    if(!file.open(QIODevice::ReadWrite)) QMessageBox::warning(this, "Error!","Can't open file");
    QTextStream stream(&file);
    stream << answer->toPlainText();

}
