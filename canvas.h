#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include "graph.h"

/*Приложение V 1.0
 * Умеет:
 * 1)рисовать вершины (двойной щелчок по свободному месту)
 * 2)двигать вершины (щелкнуть по вершине и тащить)
 * Идеи:
 * 1)сделать ver_radious как static параметр класса отрисовки
 * 2)сделать общую структуру с условиями(как вектор условий)
 * Предложения:
 * 1)Создать отдельный класс, в котором будет храниться информация обо всех
 * вершинах и ребрах, но только с точки зрения рисования(цвета, стили, толщина)
 * 2)создать класс отрисовки, куда помещается graph и класс из предыдущего пункта
 */

using CheckVertex = QPair<bool, Vertex>;

class Canvas : public QWidget
{
    Q_OBJECT

public:
    Canvas(QWidget *parent = nullptr);
    ~Canvas();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    Graph graph;
    int count = 0;//счетчик вершин
    CheckVertex chv;//нужно для отрисовки вершины
    CheckVertex chv_arrow;//нужно для отрисовки ребра
    int ver_radious;
    bool mouse_press = false;
};
#endif // CANVAS_H
