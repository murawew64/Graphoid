#include "canvas.h"
#include <QDebug>
#include <QMouseEvent>
#include <math.h>
#include <QPainter>

const Vertex checkVertex(const QSet<Vertex> &s, const QPoint &p, int r);

Canvas::Canvas(QWidget *parent) : QWidget(parent), chv(false, Vertex()), ver_radious(30) { }

void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    // Создаём объект отрисовщика
    QPainter painter(this);
    // Устанавливаем кисть абриса
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));

    //общая отрисовка
    QSet<Vertex> s = graph.getVertexAsKeys();

    for(auto v = s.cbegin(); v != s.cend(); ++v){
        //отрисовка вершин
        painter.drawEllipse(v->getPoint().x() - ver_radious/2, v->getPoint().y() - ver_radious/2, ver_radious, ver_radious);
    }
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    //по левой кнопке мыши передвигаем вершину
    if(event->button() == Qt::LeftButton){
        //проверяем есть ли тут вершина
         Vertex check_ver = checkVertex(graph.getVertexAsKeys(), QPoint(event->x(), event->y()), ver_radious);
         //если есть помечаем как активную
         if(!check_ver.getName().isEmpty()){
             chv.first = true;
             chv.second = check_ver;
         }
    }

    //по правой контекстное меню
    if(event->button() == Qt::RightButton){

    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    //проверяем, активна ли сейчас вершина
    if(chv.first){
        auto move_v_data = graph[chv.second];
        //так как вершина это ключ, приходится удалять старую
        graph.remove(chv.second);
        chv.second.setPoint(QPoint(event->x(), event->y()));
        //и добавлять с новыми координатами
        graph[chv.second] = move_v_data;
        repaint();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(event->button() == Qt::LeftButton){
        chv.first = false;
    }
    if(event->button() == Qt::RightButton){

    }
}

void Canvas::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    //тут задействована только левая кнопка мыши
    //Происходит обработка двух событий:
    //1)рисуется вершина(кликнуть два раза по пустому месту)
    //2)рисуется ребро от данной вершины(если кликнуть два раза на существующую)

    if(event->button() == Qt::LeftButton){
        //проверяем есть ли тут вершина
        Vertex check_ver = checkVertex(graph.getVertexAsKeys(), QPoint(event->x(), event->y()), ver_radious);
        //если нет рисуем
        if(check_ver.getName().isEmpty()){
            Vertex v("Ver" + QString::number(count), event->x(), event->y());
            graph.insert(v, QList<pair>());
            count++;
            //чтобы можно было сразу перетаскивать помечаем как активную
            chv.first = true;
            chv.second = v;
            repaint();
        }
        //если есть ведем ребро
        else {

        }

    }
}

Canvas::~Canvas()
{
}

//-------------------!!!!!!!!!----------------------------
//Проверка на то, указывает ли сейчас мышка на одну из вершин
const Vertex checkVertex(const QSet<Vertex> &s, const QPoint &p, int r)
{
    for(auto v = s.cbegin(); v != s.cend(); ++v){
        int res = pow(v->getPoint().x() - p.x(), 2) + pow(v->getPoint().y() - p.y(), 2) * 0.9;
        if(pow(res, 0.5) < r)
            return *v;
    }
    return Vertex();
}

