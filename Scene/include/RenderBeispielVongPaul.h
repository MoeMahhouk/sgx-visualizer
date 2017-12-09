#ifndef RENDERBEISPIELVONGPAUL_H
#define RENDERBEISPIELVONGPAUL_H

#include <QGraphicsScene>
#include <vector>

// <>
namespace Mett {
    class Transformation2D {
    public:
        qreal xoffset, yoffset;
        qreal xscale, yscale;

        Transformation2D(qreal x, qreal y, qreal scaleX, qreal scaleY) :
            xoffset(x), yoffset(y), xscale(scaleX), yscale(scaleY)
        {

        }

        Transformation2D() : Transformation2D(0, 0, 1, 1) {}

        qreal applyX(qreal x) { return x * xscale + xoffset; }
        qreal applyY(qreal y) { return y * yscale + yoffset; }

        Transformation2D transform(qreal x, qreal y) {
            return Transformation2D(xoffset + x, yoffset + y, xscale, yscale);
        }
    };

    class Renderable {
    public:
        std::vector<Renderable*> children;
        virtual void render(QGraphicsScene& scene, Transformation2D trafo = Transformation2D()) {
            for (Renderable* child : children)
                child->render(scene, trafo);
        }
    };

    class Block : public Renderable {
        qreal x, y, w, h;
        QPen pen;

    public:
        Block(qreal x, qreal y, qreal w, qreal h)
            : x(x), y(y), w(w), h(h)
        {
            pen = QPen(Qt::red);
        }

        virtual void render(QGraphicsScene& scene, Transformation2D trafo = Transformation2D()) override {
            scene.addRect(trafo.applyX(x), trafo.applyY(y), trafo.xscale * w, trafo.yscale * h, pen);
            Renderable::render(scene, trafo.transform(x, y));

        }
    };
}

#endif //RENDERBEISPIELVONGPAUL_H
