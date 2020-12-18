#include "plotter.h"

#include <QPainter>
#include <QPaintEvent>
#include <QColor>

#include <math.h>

const QColor kGridPenColor = QColor(220,220,220);
const QColor kMarkPenColor = QColor(180,180,180);
const unsigned int kDefaultFontSize = 11;
const unsigned int kLegendHeight = 25;
const unsigned int kLegendMargin = 3;

Plotter::Plotter(QWidget *parent) : QWidget(parent)
{
    _grid = true;
    _xAxis = true;
    _yAxis = true;
    _showMarks = true;

	_XDivs = 5;
	_YDivs = 5;
	SetXRange(-10, 10);
    SetYRange(-10, 10);

	SetFontSize(kDefaultFontSize);
	_showLegend = false;
	SetSeries();
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

void Plotter::SetXDivisions(uint32_t divisions)
{
	_XDivs = divisions;
	setGridXMarks();
	update();
}

void Plotter::SetYDivisions(uint32_t divisions)
{
	_YDivs = divisions;
	setGridYMarks();
	update();
}

void Plotter::SetFontSize(uint32_t size)
{
	_font.setPointSize(size);
}

void Plotter::ShowLegend(bool show)
{
	_showLegend = show;
	update();
}

void Plotter::drawLegend(QPainter &p, QMap<QString, QColor> series)
{
	QRectF legend (0 + 1, rect().height() - kLegendHeight - kLegendMargin, rect().width() - 2, kLegendHeight);
	p.setPen(QPen(kGridPenColor, 1));
	p.setBrush(Qt::white);
	p.drawRect(legend);

	if (series.size()){
		QRectF rSerie = legend;
		rSerie.setWidth(rSerie.width() / series.size());

		QMapIterator<QString, QColor> serie(series);
		while (serie.hasNext()) {
			serie.next();
			p.setPen(serie.value());
			p.drawText(rSerie, Qt::AlignCenter, serie.key());
			rSerie.moveLeft(rSerie.left() + (rSerie.width()));
		}
	}
}

void Plotter::setGridXMarks()
{
    int diffX = _maxX - _minX;
	_gridXMark = std::abs(diffX) / _XDivs;
	_minGridXMark = _gridXMark / 4;
}

void Plotter::setGridYMarks()
{
    int diffY = _maxY - _minY;
	_gridYMark = std::abs(diffY) / _YDivs;
	_minGridYMark = _gridYMark / 4;
}

void Plotter::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p (this);
    p.fillRect(rect(), Qt::white);

	p.setFont(_font);

    qreal totalX = std::abs(_maxX) + std::abs(_minX) + _gridXMark;
    _dx = rect().width() * _gridXMark/ totalX;
    _dxmin = _dx * _minGridXMark / _gridXMark;
    qreal totalY = std::abs(_maxY) + std::abs(_minY) + _gridYMark;

	if(_showLegend) {
		_dy = (rect().height() - kLegendHeight - kLegendMargin * 2) * _gridYMark/ totalY;
	}
	else {
		_dy = rect().height() * _gridYMark/ totalY;
	}
    _dymin = _dy * _minGridYMark / _gridYMark;


    if (_grid) {
        paintGrid(p);
    }
    if (_xAxis || _yAxis) {
        paintAxis(p, _xAxis, _yAxis, _showMarks);
    }

	if (_showLegend) {
		drawLegend(p, _series);
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


				p.drawText(point.x(), point.y(), _dx, _dy,  Qt::AlignTop | Qt::AlignLeft | Qt::TextDontClip, QString::number(x));
                p.drawLine(pos, y0 - markSize /2, pos, y0 + markSize /2);

				//ToDo: review when legend is visible
            }
        }
        if (yAxis){
            mark = 0;
            for (qreal y = _minY; y <= _maxY; y += _gridYMark, mark++) {
				QRectF pos ((x0 - _dxmin*2), yToChartPos(y), _dx/2, _dy);
				p.drawText(pos, Qt::AlignTop| Qt::AlignRight | Qt::TextDontClip , QString::number(y));
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

	qreal result = rect().height() - (rect().height() * (y - minY) / (maxY - minY));
	if (_showLegend) {
		result -= kLegendHeight - kLegendMargin * 2;
	}
	return result;
}

