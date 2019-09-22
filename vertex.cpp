#include "vertex.h"

Vertex::Vertex() : Vertex("", 0, 0) {};

Vertex::Vertex(const QString & str, int x, int y): name(str), point(QPoint(x, y)) { }

Vertex::Vertex(const QString & str, const QPoint &p): name(str), point(p) { }

void Vertex::setName(const QString & str)
{
    name = str;
}

void Vertex::setPoint(const QPoint & p)
{
    point = p;
}

const QString & Vertex::getName() const
{
    return name;
}

const QPoint & Vertex::getPoint() const
{
    return point;
}

bool Vertex::operator==(const Vertex &v) const
{
    if(this->name != v.name)
        return false;
    if(this->point.x() != v.point.x())
        return false;
    if(this->point.y() != v.point.y())
        return false;

    return true;
}

bool Vertex::operator<(const Vertex &v) const
{
    if(this->point.x() < v.point.x())
        return true;
    else{
        if(this->point.x() > v.point.x())
            return false;
        if(this->point.y() < v.point.y())
            return true;
    }

}

bool Vertex::operator>(const Vertex &v) const
{

}

uint qHash(const Vertex &v)
{
    uint hash = 2139062143;
   const QChar *str = v.getName().data();
   for(; str->unicode(); str++){
       hash = 37 * hash + str->unicode();
   }
}
