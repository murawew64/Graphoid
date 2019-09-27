#include "canvas.h"
#include <QDebug>
#include <QMouseEvent>
#include <math.h>
#include <QPainter>

//Вершина - настоящая вершина, нарисованная на полотне
//Фиктивная вершина - вершина, не нарисованная на полотне и
//задействованная для рисования анимации проведения ребра
//Ребро для анамации - его рисует пользователь до нужной вершины
//Основное ребро - ребро, уже отображенное на полотне

const Vertex checkVertex(const QSet<Vertex> &s, const QPoint &p, int r);
int checkArrow(const QList<Node> &n, const QPoint &p, int r);

Canvas::Canvas(QWidget *parent) : QWidget(parent), chv(false, Vertex()), chv_arrow(false, Vertex()), chv_node(false, -1), ver_radious(30) { }

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

    //отрисовка вершин
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
    }
    //отрисовка ребер

    //для теста

    painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));

    for(auto n = nodes.cbegin(); n != nodes.cend(); ++n){
        painter.drawLine(n->getVertex().first.getPoint(), n->getPoint());
        painter.drawEllipse(n->x() - 6, n->y() - 6, 12, 12);
        painter.drawLine( n->getPoint(), n->getVertex().second.getPoint());
    }
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    //по левой кнопке мыши передвигаем вершину
    if(event->button() == Qt::LeftButton){

        //проверяем, есть ли тут основное ребро
        int node_index = checkArrow(nodes, QPoint(event->x(), event->y()), 6);

        if(node_index != -1){
            chv_node.first = true;
            chv_node.second = node_index;
        }

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

//Добавлять ребра в graph!!!

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    //Проверяем, активно ли сейчас основное ребро
    if(chv_node.first){
        nodes[chv_node.second].setPoint(QPoint(event->x(), event->y()));
    }

    //проверяем, активна ли сейчас вершина
    if(chv.first){
        //если не сохранить вершину до изменений координат, то проверка
        //в цикле после будет выполнена не корректно
        Vertex not_change_ver = chv.second;

        auto move_v_data = graph[chv.second];
        //так как вершина это ключ, приходится удалять старую
        graph.remove(chv.second);
        chv.second.setPoint(QPoint(event->x(), event->y()));
        //и добавлять с новыми координатами
        graph[chv.second] = move_v_data;

        //-----------------------------!!!!!!!!----------------------
        //Вершина передвинулась => надо изменить координаты для ребер
        for(auto ar = nodes.begin(); ar != nodes.end(); ++ar){
            //в цикл входит, но не выполняются условия
            if(ar->getVertex().first == not_change_ver){
                ar->setVertex(chv.second, ar->getVertex().second);
            }
            if(ar->getVertex().second == not_change_ver){
                ar->setVertex(ar->getVertex().first, chv.second);
            }
        }
    }

    //проверяем, активно ли сейчас ребро для анимации
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

            Vertex check_ver = checkVertex(graph.getVertexAsKeys(), QPoint(event->x(), event->y()), ver_radious);
            //Если ребро довели до вершины
            //если вершина не та же самая(петли создаются через контекстное меню)
            if(!(chv_arrow.second == check_ver)){
                //----------------------!!!!!---------------------------
                //Здесь происходит создание узлов
                //у нас есть две вершины - chv_arraw.second и check_ver
                if(!check_ver.getName().isEmpty()){
                    //добаляем ребро в общую структуру graph
                    vertex_data.append(pair(check_ver,QList<int>()));
                    //создаем узел
                    int node_x = (chv_arrow.second.getPoint().x() + check_ver.getPoint().x()) / 2;
                    int node_y = (chv_arrow.second.getPoint().y() + check_ver.getPoint().y()) / 2;
                    nodes.append(Node(node_x, node_y, chv.second, check_ver));
                }
            }
            //либо добавили ребро, либо нет и вернули данные обратно
            graph[chv_arrow.second] = vertex_data;
            //перерисовавыем, чтобы изменения вступили в силу
            repaint();
        }

        //Делаем вершины и ребра не активными
        chv.first = false;
        chv_arrow.first = false;
        chv_node.first = false;
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

int checkArrow(const QList<Node> &n, const QPoint &p, int r)
{
    for(auto ar = n.cbegin(); ar != n.cend(); ++ar){
        int res = pow(ar->getPoint().x() - p.x(), 2) + pow(ar->getPoint().y() - p.y(), 2) * 0.9;
        if(pow(res, 0.5) < r)
            return n.indexOf(*ar);
    }
    return -1;
}

