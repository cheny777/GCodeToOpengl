#include "widget.h"
#include "ui_widget.h"

#include <QHBoxLayout>
#include <QFileDialog>
#include <QVBoxLayout>
#include "gcodeinterpreter.h"
#include "gcodevisualizer.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QVBoxLayout *layour = new QVBoxLayout(this);
    m_pview = new BasicViewer(this);
    layour->addWidget(m_pview);
    layour->addWidget(ui->pushButton);
    layour->addWidget(ui->pushButton_2);


    QString ttty = "   1   ";
    qDebug()<<ttty.toInt();


}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    QString fil = QFileDialog::getOpenFileName(NULL,"opne","./","tr(*.*)");

    if(fil == "")
    {
        return;
    }
    ModelFileRead re;
    re.read(fil.toStdString());
    if(re.is_read())
    {
        m_pview->adddata(re.getModel());
    }
    m_pview->updateV();
}

void Widget::on_pushButton_2_clicked()
{
    QString fil = QFileDialog::getOpenFileName(NULL,"opne","./","tr(*.*)");

    if(fil == "")
    {
        return;
    }
    GCodeInterpreter gc(fil);
    QList<GCodeCommand> list = gc.interpret();

    QList<QPair<int, int> > m_segmentList;
    auto a = GCodeVisualizer().visualize(list,m_segmentList);

    m_pview->adddata(a);
    m_pview->updateV();
    //    NCfileread re;
    //    re.read(fil.toStdString());
    //    if(re.is_read())
    //    {
    //        m_pview->adddata(re.getLine());
    //    }
    //    m_pview->updateV();




}
