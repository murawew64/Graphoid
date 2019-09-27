#ifndef NODE_H
#define NODE_H

#include <vertex.h>
#include <QPair>

using arrow_pair = QPair<Vertex, Vertex>;

class Node
{
public:
    Node();
    Node(int x, int y, Vertex ver1, Vertex ver2);
    int x() const;
    int y() const;
    const QPoint getPoint() const;
    void setPoint(int x, int y);
    void setPoint(QPoint point);
    const arrow_pair getVertex() const;
    void setVertex(Vertex ver1, Vertex ver2);
    void setVertex(const arrow_pair &pair);
    bool operator==(const Node &n);

private:
    int node_x;
    int node_y;
    Vertex first;
    Vertex second;
};

#endif // NODE_H
