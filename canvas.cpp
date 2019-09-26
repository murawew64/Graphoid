#include "canvas.h"
#include <QDebug>
#include <QMouseEvent>
#include <math.h>
#include <QPainter>

const Vertex checkVertex(const QSet<Vertex> &s, const QPoint &p, int r);

Canvas::Canvas(QWidget *parent) : QWidget(parent), chv(false, Vertex()), chv_arrow(false, Vertex()), ver_radious(30) { }

//------------------------------!!!!---------------------------------
//Тут происходит вся отрисовка
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
        //qDebug() << v->getName();
        painter.drawEllipse(v->getPoint().x() - ver_radious/2, v->getPoint().y() - ver_radious/2, ver_radious, ver_radious);

        //отрисовка ребер каждой вершины
        auto data_list = graph[*v];//получили список всех вершин с которыми есть общие ребра

        //данное условие только тестирования(основное в комменте ниже)
        if(data_list.isEmpty())
            continue;
        if(data_list.back().first.getName().isEmpty()){//проверяем связанную вершину на имя ""
            painter.drawLine(v->getPoint(), data_list.back().first.getPoint());
        }
        /*
        for(auto pr = data_list.cbegin(); pr != data_list.cend(); pr++){
            //!!!!!!!!!!!!!!!пока что рисую одно ребро!!!!!!!
            painter.drawLine(v->getPoint(), pr->first.getPoint());
        }
        */
    }
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    //по левой кнопке мыши передвигаем вершину
    if(event->button() == Qt::LeftButton){
        //проверяем, есть ли тут вершина
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
    }

    //проверяем, активно ли сейчас ребро
    if(chv_arrow.first){
        //если активно, изменяем координаты фиктивной вершины
        auto vertex_data = graph[chv_arrow.second];
        vertex_data.back().first.setPoint(QPoint(event->x(), event->y()));
        graph[chv_arrow.second] = vertex_data;
    }
    //перерисоваваем
    repaint();
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(event->button() == Qt::LeftButton){

        //Добавляем ребро в общую структуру
        if(chv_arrow.first){
            //мы добавили фиктивную вершину, чтобы сделать анимацию отрисовки ребра
            //------------------теперь удалим ее
            auto vertex_data = graph[chv_arrow.second];
            vertex_data.pop_back();//там list и она была добавлена последней
            graph[chv_arrow.second] = vertex_data;

            Vertex check_ver = checkVertex(graph.getVertexAsKeys(), QPoint(event->x(), event->y()), ver_radious);
            //Если ребро довели до вершины
            if(!check_ver.getName().isEmpty()){

            }
            //перерисовавыем, чтобы изменения вступили в силу
            repaint();
        }

        //Делаем вершины и ребра не активными
        chv.first = false;
        chv_arrow.first = false;
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
    qDebug() << "d click";
    if(event->button() == Qt::LeftButton){
        //проверяем есть ли тут вершина
        Vertex check_ver = checkVertex(graph.getVertexAsKeys(), QPoint(event->x(), event->y()), ver_radious);

        //------------------------!!!!------------------------
        //Отрисовка вершины(рисуем, если в данном месте нет уже нарисованной вершины)
        if(check_ver.getName().isEmpty()){
            Vertex v("Ver" + QString::number(count), event->x(), event->y());
            graph.insert(v, QList<pair>());
            count++;
            //чтобы можно было сразу перетаскивать помечаем как активную
            chv.first = true;
            chv.second = v;
        }

        //------------------------!!!!------------------------
        //Если на данном месте есть вершина, начинается отрисовка ребра
        //тут создается фиктивная вершина!!!
        else {
            chv_arrow.first = true;
            chv_arrow.second = check_ver;
            auto vertex_data = graph[check_ver];
            vertex_data.append(pair(Vertex("", event->x(), event->y()), QList<int>()));
            graph[check_ver] = vertex_data;
        }
        //в том или другом случае идет перерисовка
        repaint();
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

