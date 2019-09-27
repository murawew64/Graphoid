#include "node.h"

Node::Node() : Node(0, 0, Vertex(), Vertex()) { }

Node::Node(int x, int y, Vertex ver1, Vertex ver2) : node_x(x), node_y(y), first(ver1), second(ver2) { }

int Node::x() const
{
    return node_x;
}

int Node::y() const
{
    return node_y;
}

const QPoint Node::getPoint() const
{
    return QPoint(node_x, node_y);
}

void Node::setPoint(int x, int y)
{
    node_x = x;
    node_y = y;
}

void Node::setPoint(QPoint point)
{
    node_x = point.x();
    node_y = point.y();
}

const arrow_pair Node::getVertex() const
{
    return arrow_pair(first, second);
}

void Node::setVertex(Vertex ver1, Vertex ver2)
{
    first = ver1;
    second = ver2;
}

void Node::setVertex(const QPair<Vertex, Vertex> &pair)
{
    first = pair.first;
    second = pair.second;
}

bool Node::operator==(const Node &n)
{
    if(node_x != n.x())
        return false;
    if(node_y != n.y())
        return false;
    return true;
}
