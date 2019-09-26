#include "graph.h"

Graph::Graph() {}

//получение всех текущих вершин
const QSet<Vertex>& Graph::getVertexAsKeys() const
{
    return ver_list;
}

void Graph::insert(const Vertex &v, const QList<pair> &vp)
{
    if(!ver_list.contains(v)){
        ver_list.insert(v);
        graph.insert(v, vp);
    }
}

int Graph::remove(const Vertex &v)
{
    ver_list.remove(v);
    return graph.remove(v);
}

//просто для удобства работы(полностью аналогичен последующему)
QList<pair>& Graph::vertexAt(const Vertex &v)
{
    if(ver_list.contains(v)){
        return graph[v];
    } else {
        insert(v, QList<pair>());//если вершины сейчас нет
        return graph[v];
    }
}

QList<pair>& Graph::operator[](const Vertex &v)
{
    if(ver_list.contains(v)){
        return graph[v];
    } else {
        insert(v, QList<pair>());//если вершины сейчас нет
        return graph[v];
    }
}

//проверка на вхождение
bool Graph::contains(const Vertex &v) const
{
    return ver_list.contains(v);
}

void Graph::clear()
{
    ver_list.clear();
    graph.clear();
}
