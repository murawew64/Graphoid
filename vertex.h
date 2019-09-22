#ifndef VERTEX_H
#define VERTEX_H

#include <qstring.h>
#include <qpoint.h>

class Vertex;
uint qHash(const Vertex &v);

class Vertex
{
public:
    Vertex();
    Vertex(const QString & str, int x, int y);
    Vertex(const QString & str, const QPoint &p);
    void setName(const QString & str);
    void setPoint(const QPoint & p);
    const QString & getName() const;
    const QPoint & getPoint() const;
    bool operator==(const Vertex &v) const;
    bool operator<(const Vertex &v) const;
    bool operator>(const Vertex &v) const;
private:
    QString name;
    QPoint point;
};

#endif // VERTEX_H
