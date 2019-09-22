#ifndef GRAPH_H
#define GRAPH_H

/*версия 1.1
 *
 * Граф поддерживает методы:
 * добавление вершины
 * просмотр/изменение текущих данных о вершине
 * удаление вершины
 * получение данных о всех вершинах
 * очистка
 * проверка на вхождение вершины в мн. вершин графа
 *
 * Возможные доработки:
 * 1)Сделать итераторы
 * 2)Сделать поиск вершины(удобно через итераторы)
 *
 * По сравнению с предыдущей версией:
 * set вместо list (добавлена hash - функция)
*/

#include <qmap.h>
#include <qvector.h>
#include <qset.h>
#include <qlist.h>
#include "vertex.h"

using pair = QPair<Vertex, QList<int>>;

class Graph
{
public:
    Graph();
    void insert(const Vertex &v, const QList<pair> &vp);
    int remove(const Vertex &v);
    QList<pair>& vertexAt(const Vertex &v);
    QList<pair>& operator[](const Vertex &v);
    const QSet<Vertex>& getVertexAsKeys() const;
    bool contains(const Vertex &v) const;
    void clear();
private:
    //если вершину удалили, то множество ведущих к ней дуг удаляются при перерисовке!
    QSet<Vertex> ver_list;//set всех вершин
    QMap<Vertex, QList<pair>> graph;//структура, хранящая полные данные о текущем графе
};

#endif // GRAPH_H
