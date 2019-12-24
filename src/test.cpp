#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsEffect>
#include <QImage>
#include <QLabel>
#include <QPainter>

#include "pdf-thumbnail.h"

class Shadow : public QGraphicsEffect {
  Q_OBJECT
public:
  explicit Shadow(QObject *parent = 0);

  void draw(QPainter *painter);

  QRectF boundingRectFor(const QRectF &rect) const;

  inline void setDistance(qreal distance) {
    _distance = distance;
    updateBoundingRect();
  }

  inline qreal distance() const { return _distance; }

  inline void setBlurRadius(qreal blurRadius) {
    _blurRadius = blurRadius;
    updateBoundingRect();
  }

  inline qreal blurRadius() const { return _blurRadius; }

  inline void setColor(const QColor &color) { _color = color; }
  inline QColor color() const { return _color; }

private:
  qreal _distance;
  qreal _blurRadius;
  QColor _color;
};

QT_BEGIN_NAMESPACE
extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter *p, qreal radius,
                                          bool quality, bool alphaOnly,
                                          int transposed = 0);
QT_END_NAMESPACE

Shadow::Shadow(QObject *parent)
    : QGraphicsEffect(parent), _distance(4.0f), _blurRadius(10.0f),
      _color(0, 0, 0, 80) {}

void Shadow::draw(QPainter *painter) {
  if ((blurRadius() + distance()) <= 0) {
    drawSource(painter);
    return;
  }

  PixmapPadMode mode = QGraphicsEffect::PadToEffectiveBoundingRect;
  QPoint offset;
  const QPixmap px = sourcePixmap(Qt::DeviceCoordinates, &offset, mode);

  if (px.isNull())
    return;

  QTransform restoreTransform = painter->worldTransform();
  painter->setWorldTransform(QTransform());

  QSize szi(px.size().width() + 2 * distance(),
            px.size().height() + 2 * distance());

  QImage tmp(szi, QImage::Format_ARGB32_Premultiplied);
  QPixmap scaled = px.scaled(szi);
  tmp.fill(0);
  QPainter tmpPainter(&tmp);
  tmpPainter.setCompositionMode(QPainter::CompositionMode_Source);
  tmpPainter.drawPixmap(QPointF(-distance(), -distance()), scaled);
  tmpPainter.end();

  QImage blurred(tmp.size(), QImage::Format_ARGB32_Premultiplied);
  blurred.fill(0);
  QPainter blurPainter(&blurred);
  qt_blurImage(&blurPainter, blurRadius(), false, true);
  blurPainter.end();

  tmp = blurred;

  tmpPainter.begin(&tmp);
  tmpPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
  tmpPainter.fillRect(tmp.rect(), color());
  tmpPainter.end();

  painter->drawImage(offset, tmp);

  painter->drawPixmap(offset, px, QRectF());

  painter->setWorldTransform(restoreTransform);
}

QRectF Shadow::boundingRectFor(const QRectF &rect) const {
  qreal delta = blurRadius() + distance();
  return rect.united(rect.adjusted(-delta, -delta, delta, delta));
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QLabel l;
  QString s("file:///home/brs/data/books/");
  PdfThumbnail p(s);
  QPainter pa;

  auto pxmap = p.generateThumbnail();
  l.setPixmap(pxmap);
  Shadow shadow(&l);
  shadow.setBlurRadius(10.0);
  shadow.setDistance(3.0);
  shadow.setColor(QColor(0, 0, 0, 80));
  l.show();
  return QApplication::exec();
}
