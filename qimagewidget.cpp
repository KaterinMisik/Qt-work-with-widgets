#include "qimagewidget.h"

#include <QMouseEvent>

#include <QPainter>

#include <QFileDialog>

#include <QGraphicsScene>

#include <QDebug>

#include "QLineF"

#include <QResizeEvent>

#include <QDial>

#include <QMessageBox>

#include <QObject>

#include <QFile>

#include <QTextStream>

#include <QDrag>

#include<QDataStream>

#include <QString>

#include <QPushButton>

#include <QWidget>

#include "toolbar.h"


QImageWidget::QImageWidget(ToolBar *toolB, QWidget *parent) : QWidget(parent), toolBar(toolB), currentTriangle()

{


    _noImageMessage = "No image";

    _Tomany = "You can't draw both";

    _noFigureMessage = "You didn't peak any figure";

    _backgroundColor = Qt::white;

    _Color = Qt::black;

    _margin = 4;

  //  connect(this, SIGNAL(marginChanged()), SLOT(repaint()));


}


QImageWidget::~QImageWidget()

{


}

QRect QImageWidget::actualImageRect()

{

    QRect imageRect = rect();

    imageRect.setSize(_originalImage.size().scaled(size(), Qt::KeepAspectRatio));

    imageRect.moveCenter(rect().center());

    return imageRect;


}


void QImageWidget::updateMargins()

{

    double ratio = (double)_originalImage.height()/_originalImage.width();

    int hMargin = _margin * ratio;

    int vMargin = _margin;

    _margins.setBottom(hMargin);

    _margins.setTop(hMargin);

    _margins.setRight(vMargin);

    _margins.setLeft(vMargin);


}


void QImageWidget::setPixmap(QPixmap pixmap)

{

    _originalImage = pixmap;

    updateMargins();


}


void QImageWidget::SaveFile(QString filepath){

    QFile file(filepath);

    if(file.open(QIODevice::WriteOnly))

    {


        QTextStream  out(&file);


        for(int i=0; i<rects.count(); i++) {


            out<< QString::number(rects[i].topLeft().x())+","+


                  QString::number(rects[i].topLeft().y())+":"+


                  QString::number(rects[i].bottomRight().x()) + ","+


                  QString::number(rects[i].bottomRight().y()) + "\n" ;


        }

        for(int i=0; i<circles.count(); i++) {


            out<< QString::number(circles[i].topLeft().x())+","+


                  QString::number(circles[i].topLeft().y())+"/"+


                  QString::number(circles[i].bottomRight().x()) + ","+


                  QString::number(circles[i].bottomRight().y()) + "\n" ;

        }



        file.close();


    }

}


void QImageWidget::SaveFile2(QString filepath2){



    QFile file(filepath2);

    if(file.open(QIODevice::WriteOnly))

    {


        QTextStream  out(&file);


        for(int i=0; i<rects.count(); i++) {

            qreal propYN = (static_cast<double>(size().height() /static_cast<double>(1000)));

            qreal propXN = (static_cast<double>(size().width() / static_cast<double>(1000)));


            out<< QString::number((rects[i].left()*propYN), 'f', 0)+","+

                  QString::number((rects[i].top()*propYN), 'f', 0) +":"+

                  QString::number((rects[i].right()*propXN), 'f', 0) + ","+

                  QString::number((rects[i].bottom()*propYN),  'f', 0)+"\n" ;

        }

        for(int i=0; i<circles.count(); i++) {

            qreal propYN = (static_cast<double>(size().height() /static_cast<double>(1000)));

            qreal propXN = (static_cast<double>(size().width() / static_cast<double>(1000)));


            out<< QString::number((circles[i].left()*propYN), 'f', 0)+","+

                  QString::number((circles[i].top()*propYN), 'f', 0) +"/"+

                  QString::number((circles[i].right()*propXN), 'f', 0) + ","+

                  QString::number((circles[i].bottom()*propYN),  'f', 0)+"\n" ;

        }






        file.close();

    }


}


void QImageWidget::SaveFile3(QString filepath3){



    QFile file(filepath3);

    if(file.open(QIODevice::WriteOnly))

    {


        QTextStream out(&file);


        for(int i=0; i<triangles.count(); i++) {


            out<< triangles[i].ToSave();


        }


        file.close();

    }


}




void QImageWidget::SaveFile4(QString filepath4){



    QFile file(filepath4);

    if(file.open(QIODevice::WriteOnly))

    {


        QTextStream  out(&file);



for(int i=0; i<triangles.count(); i++) {

    qreal propYN = (static_cast<double>(size().height() /static_cast<double>(1000)));

    qreal propXN = (static_cast<double>(size().width() / static_cast<double>(1000)));



    out<< QString::number((triangles[i].getPoints()[0].y()*propYN), 'f', 0)+","+

    QString::number((triangles[i].getPoints()[0].x()*propXN), 'f', 0)+","+

    QString::number((triangles[i].getPoints()[1].y()*propYN), 'f', 0)+","+

    QString::number((triangles[i].getPoints()[1].x()*propXN), 'f', 0)+","+

    QString::number((triangles[i].getPoints()[2].y()*propYN), 'f', 0)+","+

   QString::number((triangles[i].getPoints()[2].x()*propXN), 'f', 0)+"\n" ;

}






file.close();

}


}




void QImageWidget::UploadPointsToConfig() {


    UploadPoints("C:/Users/kamil/Desktop/data.txt");

 //   UploadPoints("C:/Users/kamil/Desktop/circledata.txt");


}

//void QImageWidget::UploadPointsToConfig1() {


  //  UploadPoints("C:/Users/kamil/Desktop/triangledata.txt");



//}


void QImageWidget::UploadPoints(QString filepathtoupload){


    QFile file(filepathtoupload);

    if(file.open(QIODevice::ReadOnly | QFile::Text)){

        QTextStream  in(&file);

        while (!in.atEnd())

        {

            QString line = in.readLine();

           rects.append(RectToDraw(line,':',','));

           circles.append(RectToDraw(line,'/',','));


        }

        file.close();

    }


}








/*



void QImageWidget::UploadPoints1(QString filepathtoupload){


    QFile file(filepathtoupload);

    if(file.open(QIODevice::ReadOnly | QFile::Text)){

        QTextStream  in(&file);

        while (!in.atEnd())

        {

            QString line = in.readLine();


           triangles.append(TriangleToDraw(line, ','));

        }

        file.close();

    }


}
*/

void QImageWidget::mouseMoveEvent(QMouseEvent *event)

{


    if (Rectangling || Circling || Triangling>0) {

        endPos = event->pos();

    }



    QWidget::update();

}


void QImageWidget::drawRect(const QPoint &endPoint) {

    endPos = endPoint;


}

void QImageWidget::drawRectangle(const QPoint &endPoint) {

    endPos = endPoint;


}


void QImageWidget::drawEllipse(const QPoint &creatorRect) {

    drawRect(creatorRect);


}

void QImageWidget::drawTriangle(const QPoint &endPoint) {

    lastPoint = endPoint;


}


void QImageWidget::mousePressEvent(QMouseEvent *event)

{


    if (event->button() == Qt::LeftButton)

    {


        if(toolBar->isTriangleEnabled()) {

            lastPoint = event->pos();

            if(Triangling==0) {

                Triangling=1;

            }


        }

        if(toolBar->isCircleEnabled()) {

            lastPoint = event->pos();

            Circling=true;

            RectanglingforCircle=true;

            Triangling=0;

        }


        if(toolBar->isrectanglingEnabled()) {

            lastPoint = event->pos();

            Rectangling=true;

            Triangling=0;

        }


        if (toolBar->isSaveBoxEnabled()){

            saveboxcontrol=true;

            SaveFile("C:/Users/kamil/Desktop/data.txt");

        }


        if (toolBar->isSaveBoxEnabled()){

            saveboxcontrol=true;

            SaveFile2("C:/Users/kamil/Desktop/data2.txt");


        }

    }

}


void QImageWidget::mouseReleaseEvent(QMouseEvent *event)

{

    if (event->button() == Qt::LeftButton && Rectangling) {

        rects.append(creatorRectangle);

        creatorRectangle.setTopLeft(QPoint(0,0));

        creatorRectangle.setBottomRight(QPoint(0,0));

        Rectangling=false;

        QWidget::update();

    }


    if (event->button() == Qt::LeftButton && Circling) {

        circles.append(creatorRectangle);

        creatorRectangle.setTopLeft(QPoint(0,0));

        creatorRectangle.setBottomRight(QPoint(0,0));

        Circling=false;

        QWidget::update();

    }


    if (event->button() == Qt::LeftButton && Triangling!=0) {

        if(Triangling==1) {


            currentTriangle.setPoint(0, lastPoint);

            currentTriangle.setPoint(1, event->pos());

            Triangling++;


        }

        else if(Triangling==2) {


            currentTriangle.setPoint(2,lastPoint);

            Triangling=0;

            triangles.append(currentTriangle);


        }


        if(Triangling>2) Triangling=0;


        QWidget::update();


    }

}


void QImageWidget::mouseDoubleClickEvent(QMouseEvent *event)

{

    if (event->button()!=Qt::LeftButton)

        return;

    QString filename = QFileDialog::getOpenFileName(this);

    if(filename.isEmpty())

        return;

    setPixmap(QPixmap(filename));

}


void QImageWidget::resizeEvent(QResizeEvent *event)

{

    for(int i=0; i<rects.count(); i++) {


        qreal propX = (static_cast<double>(event->size().width()) / static_cast<double>(event->oldSize().width()));

        qreal propY = (static_cast<double>(event->size().height()) / static_cast<double>(event->oldSize().height()));


        rects[i].setTopLeft(QPointF(rects[i].left()*propX,rects[i].top()*propY));

        rects[i].setBottomRight(QPointF(rects[i].right()*propX,rects[i].bottom()*propY));


    }

    for(int i=0; i<circles.count(); i++) {


        qreal propX = (static_cast<double>(event->size().width()) / static_cast<double>(event->oldSize().width()));

        qreal propY = (static_cast<double>(event->size().height()) / static_cast<double>(event->oldSize().height()));


        circles[i].setTopLeft(QPointF(circles[i].left()*propX,circles[i].top()*propY));

        circles[i].setBottomRight(QPointF(circles[i].right()*propX,circles[i].bottom()*propY));

    }


    for(int i=0; i<triangles.count(); i++) {


        qreal propX = (static_cast<double>(event->size().width()) / static_cast<double>(event->oldSize().width()));

        qreal propY = (static_cast<double>(event->size().height()) / static_cast<double>(event->oldSize().height()));


        triangles[i].setPoint(0, QPointF(triangles[i].getPoints()[0].x()*propX, triangles[i].getPoints()[0].y()*propY));

        triangles[i].setPoint(1, QPointF(triangles[i].getPoints()[1].x()*propX, triangles[i].getPoints()[1].y()*propY));

        triangles[i].setPoint(2, QPointF(triangles[i].getPoints()[2].x()*propX, triangles[i].getPoints()[2].y()*propY));


    }


    QImageWidget::update();


}


void QImageWidget::paintEvent(QPaintEvent *event)

{

    QWidget::paintEvent(event);

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    if(_originalImage.isNull()){

        painter.drawText(rect(),Qt::AlignCenter, _noImageMessage);

        return;

    }

    painter.drawPixmap(0,0, this->size().width(), this->size().height(), _originalImage);

    if (Rectangling) {

        painter.setPen(Qt::blue);

        creatorRectangle.setTopLeft(lastPoint);

        creatorRectangle.setBottomRight(endPos);

        painter.drawRect(creatorRectangle);

    }

    if (Circling) {

        painter.setPen(Qt::NoPen);

        creatorRectangle.setTopLeft(lastPoint);

        creatorRectangle.setBottomRight(endPos);

        painter.drawRect(creatorRectangle);

        painter.setPen(Qt::black);

        painter.drawEllipse(creatorRectangle);

    }

    if (Triangling!=0) {

        if(Triangling==1) {


            painter.drawLine(QLineF(lastPoint,endPos));


        }  if(Triangling==2) {


            painter.drawLine(currentTriangle.getLines()[0]);

            painter.drawLine(QLineF(currentTriangle.getLines()[0].p2(),endPos));

            painter.drawLine(QLineF(currentTriangle.getLines()[0].p1(),endPos));

        }

    }


    foreach(QRectF rectangle, rects) {


        painter.setPen(Qt::blue);

        painter.drawRect(rectangle);

        saveboxcontrol=true;

        SaveFile("C:/Users/kamil/Desktop/data.txt");

        saveboxcontrol=true;

        SaveFile2("C:/Users/kamil/Desktop/data2.txt");

    }


    foreach(QRectF circle, circles) {


        painter.setPen(Qt::blue);

        painter.drawEllipse(circle);

        saveboxcontrol=true;

        SaveFile("C:/Users/kamil/Desktop/circledata.txt");

        saveboxcontrol=true;

        SaveFile2("C:/Users/kamil/Desktop/circledata2.txt");


    }

    foreach(TriangleToDraw triangle, triangles){


        painter.setPen(Qt::blue);

        painter.drawLines(triangle.getLines(),3);


        saveboxcontrol=true;

        SaveFile3("C:/Users/kamil/Desktop/triangledata.txt");


    }

}
