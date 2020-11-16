#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QPainter>

class Plotter : public QWidget
{
    Q_OBJECT
public:
    explicit Plotter(QWidget *parent = nullptr);

    void ShowGrid(bool show);
    void ShowAxis(bool xAxis, bool yAxis, bool showMarks = true);
    inline void ShowXAxis(bool xAxis, bool showMarks = true) {return ShowAxis(xAxis, _yAxis, showMarks);}
    inline void ShowYAxis(bool yAxis, bool showMarks = true) {return ShowAxis(_xAxis, yAxis, showMarks);}
    inline bool IsGridVisible() const {return _grid;}
    inline bool XAxisVisible() const {return _xAxis;}
    inline bool YAxisVisible() const {return _yAxis;}
    inline bool AxisMarksVisible() const {return _showMarks;}

    void SetXRange(qreal min, qreal max);
    inline void SetMinX(qreal min) {return SetXRange(min, _maxX);}
    inline void SetMaxX(qreal max) {return SetXRange(_minX, max);}
    inline qreal GetMinX() const {return _minX;}
    inline qreal GetMaxX() const {return _maxX;}

    void SetYRange(qreal min, qreal max);
    inline void SetMinY(qreal min) {return SetXRange(min, _maxY);}
    inline void SetMaxY(qreal max) {return SetXRange(_minY, max);}
    inline qreal GetMinY() const {return _minY;}
    inline qreal GetMaxY() const {return _maxY;}

    private:
    qreal _minX;
    qreal _maxX;
    qreal _minY;
    qreal _maxY;
    bool _grid;
    bool _xAxis;
    bool _yAxis;
    bool _showMarks;
    qreal _minGridXMark;
    qreal _gridXMark;
    qreal _minGridYMark;
    qreal _gridYMark;
    qreal _dxmin;
    qreal _dymin;
    qreal _dx;
    qreal _dy;

    void setGridXMarks();
    void setGridYMarks();
    virtual void paintEvent(QPaintEvent *event);
    void paintGrid(QPainter& p);
    void paintVerticalGrid(QPainter& p);
    void paintHorizontalGrid(QPainter& p);
    void paintAxis(QPainter& p, bool xAxis, bool yAxis, bool marks);
    qreal xToChartPos(qreal x)const;
    qreal yToChartPos(qreal y)const;
};

#endif // PLOTTER_H
