#ifndef TRANSFORM2D_H
#define TRANSFORM2D_H

#include <QtCore>
#include <QTransform>


namespace moe {
    class Transform2D : public QTransform {
        //static const Transform2D NORM_TRANSFORM = Transform2D(0, 0, 1, 1);
    public:


        /*Transform2D(QTransform transform) : qTransform(transform) {}

        Transform2D() : Transform2D(QTransform()) {}*/
        Transform2D() : QTransform() {}
        Transform2D(qreal h11, qreal h12, qreal h13, qreal h21, qreal h22, qreal h23, qreal h31, qreal h32, qreal h33) :
                QTransform(h11, h12, h13, h21, h22, h23, h31, h32, h33) {}
        Transform2D(qreal h11, qreal h12, qreal h21, qreal h22, qreal dx, qreal dy) : QTransform(h11, h12, h21, h22, dx, dy) {}
        Transform2D(const QTransform& copy) : QTransform(copy) {}

        // concatinates the transformations together so that if the parent objects get transformed,
        // his childrents gets transformed automatically according to their parents changes


        Transform2D operator*(const Transform2D &transform) {
           // return Transform2D(this->qTransform * transform.qTransform);
            return Transform2D(*static_cast<QTransform*>(this) * transform);
        }

        qreal getX() const {
            //return qTransform.dx();
            return dx();
        }

        qreal getY() const {
            //return qTransform.dy();
            return dy();
        }

        qreal xScale() const {
            //return qTransform.m11();
            return m11();
        }
        void setX(qreal x) {
           /* qTransform.setMatrix(qTransform.m11(),qTransform.m12(),qTransform.m13(),qTransform.m21(),qTransform.m22(), qTransform.m23(),
                                 x, qTransform.m32(), qTransform.m33());*/
            setMatrix(m11(), m12(), m13(), m21(), m22(), m23(), x, m32(),m33());
        }
        void setY(qreal y) {
            /*qTransform.setMatrix(qTransform.m11(),qTransform.m12(),qTransform.m13(),qTransform.m21(),qTransform.m22(), qTransform.m23(),
                                 qTransform.m31(), y, qTransform.m33());*/
            setMatrix(m11(), m12(), m13(), m21(), m22(), m23(), m31(), y, m33());
        }

        void setXScale(const qreal &xScale) {
            //qTransform = qTransform.scale(xScale / qTransform.m11(), 1);
            //qTransform.scale(1 / qTransform.m11(), 1);
            //qTransform.scale(xScale, 1);
            scale(xScale/m11(), 1);
        }

        qreal yScale() const {
            //return qTransform.m22();
            return  m22();
        }

        void setYScale(const qreal &yScale) {
            //qTransform = qTransform.scale(1, yScale / qTransform.m22());
            //qTransform.scale(1, 1 / qTransform.m22());
            //qTransform.scale(1, yScale);
            scale(1, yScale/ m22());
        }
        /*qreal transformX(qreal x) const {
            return x * xScale() + x_;
        }

        qreal transformY(qreal y) const {
            return y * yScale() + y_;
        }

         qreal getX() const {
             return x_;
         }

         qreal getY() const {
             return y_;
         }


        Transform2D transform(qreal x, qreal y) {
            return Transform2D(x + xOffset(), y + yOffset(), xScale(), yScale());
        }

        qreal xOffset() const { return xOffset_; }
        void setXOffset(const qreal& xOffset) {
            xOffset_ = xOffset;
        }

        qreal yOffset() const { return yOffset_;}
        void setYOffset(const qreal& yOffset) {
            yOffset_ = yOffset;
        }

        qreal xScale() const {
            return xScale_;
        }
        void setXScale(const qreal& xScale) {
            xScale_ = xScale;
        }

        qreal yScale() const {
            return yScale_;
        }
        void setYScale(const qreal& yScale) {
            yScale_ = yScale;
        }
        */
        /*QTransform getTransfor() const {
            return qTransform;
        }
        void setTransfor(const QTransform& transfor) {
            qTransform = transfor;
        }*/

       /* QTransform *getTransfor() const {
            return qTransform;
        }

        void setTransfor(QTransform *transfor) {
            qTransform = transfor;
        }*/

    };


}

#endif //TRANSFORM2D_H
