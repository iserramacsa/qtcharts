#include "plotter.h"

#include <QPainter>
#include <QPaintEvent>
#include <QColor>

#include <math.h>

const QColor kGridPenColor = QColor(220,220,220);
const QColor kMarkPenColor = QColor(180,180,180);

Plotter::Plotter(QWidget *parent) : QWidget(parent)
{
    _grid = true;
    _xAxis = true;
    _yAxis = true;
    _showMarks = true;

    SetXRange(-50, 50);
    SetYRange(-10, 10);
}

void Plotter::ShowGrid(bool show)
{
    _grid = show;
    update();
}

void Plotter::ShowAxis(bool xAxis, bool yAxis, bool showMarks)
{
    _xAxis = xAxis;
    _yAxis = yAxis;
    _showMarks = showMarks;
    update();
}

void Plotter::SetXRange(qreal min, qreal max)
{
    _minX = std::min(min, max);
    _maxX = std::max(min, max);
    setGridXMarks();
    update();
}

void Plotter::SetYRange(qreal min, qreal max)
{
    _minY = std::min(min, max);
    _maxY = std::max(min, max);
    setGridYMarks();
    update();
}

void Plotter::setGridXMarks()
{
    int diffX = _maxX - _minX;
    _gridXMark = std::abs(diffX)/5;
    _minGridXMark = _gridXMark / 5;
}

void Plotter::setGridYMarks()
{
    int diffY = _maxY - _minY;
    _gridYMark = std::abs(diffY)/5;
    _minGridYMark = _gridYMark / 5;
}

void Plotter::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p (this);
    p.fillRect(rect(), Qt::white);

    qreal totalX = std::abs(_maxX) + std::abs(_minX) + _gridXMark;
    _dx = rect().width() * _gridXMark/ totalX;
    _dxmin = _dx * _minGridXMark / _gridXMark;
    qreal totalY = std::abs(_maxY) + std::abs(_minY) + _gridYMark;
    _dy = rect().height() * _gridYMark/ totalY;
    _dymin = _dy * _minGridYMark / _gridYMark;


    if (_grid) {
        paintGrid(p);
    }
    if (_xAxis || _yAxis) {
        paintAxis(p, _xAxis, _yAxis, _showMarks);
    }

}

void Plotter::paintGrid(QPainter &p)
{
    QPen oldPen = p.pen();

    p.save();

    paintVerticalGrid(p);
    paintHorizontalGrid(p);

    p.setPen(oldPen);
    p.restore();
}

void Plotter::paintVerticalGrid(QPainter &p)
{
    QPen pen = kGridPenColor;
    pen.setWidth(1);
    int nMark = _gridXMark / _minGridXMark;
    int mark = 0;

    for (qreal i = _minX - _minGridXMark; i < _maxX + _minGridXMark * 2; i += _minGridXMark , mark++) {
        if ( mark % nMark == 1) {
            pen = kMarkPenColor;
        }
        else {
            pen = kGridPenColor;
        }
        p.setPen(pen);
        qreal xpos = xToChartPos(i);
        p.drawLine(xpos, 0, xpos, this->rect().bottom());
    }
}

void Plotter::paintHorizontalGrid(QPainter &p)
{
    QPen pen = kGridPenColor;
    pen.setWidth(1);
    int nMark = _gridYMark / _minGridYMark;
    int mark = 0;

    for (qreal i = _minY - _minGridYMark; i < _maxY + _minGridYMark * 2; i += _minGridYMark , mark++) {
        if ( mark % nMark == 1) {
            pen = kMarkPenColor;
        }
        else {
            pen = kGridPenColor;
        }
        p.setPen(pen);
        qreal ypos = yToChartPos(i);
        p.drawLine(0, ypos, this->rect().right(), ypos);
    }
}

void Plotter::paintAxis(QPainter &p, bool xAxis, bool yAxis, bool marks)
{
    p.setPen(Qt::black);
    qreal minx = (_minX < 0 && _maxX > 0) ? 0 : std::min(_minX, _maxX);
    qreal miny = (_minY < 0 && _maxY > 0) ? 0 : std::min(_minY, _maxY);
    qreal x0 = xToChartPos(minx);
    qreal y0 = yToChartPos(miny);
    qreal markSize = _dymin;

    int mark = 0;
    if (marks){
        if (xAxis){
            mark = 0;
            for (qreal x = _minX; x <= _maxX; x += _gridXMark, mark++) {
                qreal pos = xToChartPos(x);
                QPointF point((pos + _minGridXMark * 2), (y0 + _minGridYMark * 2));
                p.drawText(point.x(), point.y(), _dx, _dy,  Qt::AlignTop | Qt::AlignLeft , QString::number(x));
                p.drawLine(pos, y0 - markSize /2, pos, y0 + markSize /2);

            }
        }
        if (yAxis){
            mark = 0;
            for (qreal y = _minY; y <= _maxY; y += _gridYMark, mark++) {
                QRectF pos ((x0 - _dxmin*2), (yToChartPos(y) + _dymin*2/3), _dx, _dy);
                p.drawText(pos, Qt::AlignTop| Qt::AlignLeft , QString::number(y));
                p.drawLine(x0 - markSize /2, yToChartPos(y) , x0 + markSize /2, yToChartPos(y));
            }
        }
    }

    if (yAxis) {
        p.drawLine(x0, 0, x0, rect().height());
    }
    if (xAxis) {
        p.drawLine(0, y0, rect().width(), y0);
    }
}

qreal Plotter::xToChartPos(qreal x) const
{
    qreal minX = _minX - _minGridXMark * 2;
    qreal maxX = _maxX + _minGridXMark * 2;
    return rect().width() * (x - minX) / (maxX - minX);
}

qreal Plotter::yToChartPos(qreal y) const
{
    qreal minY = _minY - _minGridYMark * 2;
    qreal maxY = _maxY + _minGridYMark * 2;
    return rect().height() - (rect().height() * (y - minY) / (maxY - minY));
}

