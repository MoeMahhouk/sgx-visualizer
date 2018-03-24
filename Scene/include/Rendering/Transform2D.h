#ifndef TRANSFORM2D_H
#define TRANSFORM2D_H

#include <QtCore>
#include <QTransform>


namespace moe {
    /**
     * The transformation matrix class that is responsible for translating, scaling, rotation and shear operations
     */
    class Transform2D : public QTransform {
    public:

        Transform2D() : QTransform() {}
        Transform2D(qreal h11, qreal h12, qreal h13, qreal h21, qreal h22, qreal h23, qreal h31, qreal h32, qreal h33) :
                QTransform(h11, h12, h13, h21, h22, h23, h31, h32, h33) {}
        Transform2D(qreal h11, qreal h12, qreal h21, qreal h22, qreal dx, qreal dy) : QTransform(h11, h12, h21, h22, dx, dy) {}
        Transform2D(const QTransform& copy) : QTransform(copy) {}

        // concatinates the transformations together so that if the parent objects get transformed,
        // his childrens get transformed automatically according to their parents' changes
        Transform2D operator*(const Transform2D &transform)
        {
            return Transform2D(*static_cast<QTransform*>(this) * transform);
        }

        qreal getX() const
        {
            return dx();
        }

        qreal getY() const
        {
            return dy();
        }

        qreal xScale() const
        {
            return m11();
        }
        void setX(qreal x)
        {
            setMatrix(m11(), m12(), m13(), m21(), m22(), m23(), x, m32(),m33());
        }
        void setY(qreal y)
        {
            setMatrix(m11(), m12(), m13(), m21(), m22(), m23(), m31(), y, m33());
        }

        void setXScale(const qreal &xScale)
        {
            scale(xScale/m11(), 1);
        }

        qreal yScale() const
        {
            return  m22();
        }

        void setYScale(const qreal &yScale)
        {
            scale(1, yScale/ m22());
        }
    };
}

#endif //TRANSFORM2D_H
