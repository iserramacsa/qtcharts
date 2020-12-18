#include "customplotter.h"


const char* kLegendTitle[] = {"A_VALID", "B_VALID", "A_NO_VALID", "B_NO_VALID", "DIR_CHANGE"};
								//      Orange              violet            green             blue
const QColor kSerialColors[] = {QColor(255,131,0), QColor(144,0,255), QColor(0,189,0), QColor(0,0,255)};

const unsigned int kMinVerticalAxis = 5;


CustomPlotter::CustomPlotter(QWidget *parent) :
	Plotter(parent)
{}


void CustomPlotter::PushData(const std::array<unsigned int, kMaxCounters>& data)
{
	if (data.size() == kMaxCounters) {
		for (unsigned i = 0; i < kMaxCounters; i++) {
			for (unsigned t = 0; t < kMaxSamples - 1; t++) {
				_data[i][t] = _data[i][t + 1];
			}
			_data[i][kMaxSamples - 1] = data[i];
		}
	}
	updateLimits();

	for (unsigned int i = 0; i < kMaxCounters - 1; i++) {
		_series[kLegendTitle[i]] = kSerialColors[i];
	}
	printMatrix();
}

void CustomPlotter::Clear()
{

	for (unsigned i = 0; i < kMaxCounters; i++) {
		_data[i].fill(0);
	}
	SetXDivisions(4);
	SetXRange(0, kMaxSamples - 4);
	SetYRange(0, kMinVerticalAxis);
	setMinimumWidth(400);
	ShowXAxis(false);
	ShowLegend(true);
}

void CustomPlotter::paintEvent(QPaintEvent *event)
{
	if (isEnabled()) {
		Plotter::paintEvent(event);

		QPainter p (this);

		//Draw counters
		std::array<QPointF[kMaxSamples], kMaxCounters> polylineData;
		for (unsigned int t = 0; t < kMaxSamples; t++) {
			for (unsigned int i = 0; i < kMaxCounters; i++) {
				polylineData[i][t] = QPoint(xToChartPos(t), yToChartPos(_data[i][t]));
			}
		}
		for (unsigned int counter = 0; counter < kMaxCounters - 1; counter++) {
			p.setPen(QPen(kSerialColors[counter], 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
			p.drawPolyline(polylineData[counter], kMaxSamples);
		}
	}
	else {
		QPainter p (this);
		p.fillRect(rect(), QColor(51,51,51));
		p.setPen(Qt::white);
		p.drawText(QRect(rect().width()/4, rect().height()/4, rect().width()/2, rect().height()/2),
				   Qt::AlignCenter | Qt::TextWordWrap, tr("Enable external encoder to use the plotter"));
	}
}

void CustomPlotter::updateLimits()
{
	unsigned int yMax = kMinVerticalAxis;
	unsigned int yMin = _data[0][0];
	for (unsigned i = 0; i < kMaxCounters -1; i++) {
		for (unsigned int t = 0; t < kMaxSamples; t++) {
			if (yMax < _data[i][t]) {
				yMax = _data[i][t];
			}
			if (yMin > _data[i][t]) {
				yMin = _data[i][t];
			}
		}
	}

	if (yMax > 1000) {
		SetYDivisions(10);
	}
	else {
		SetYDivisions(5);
	}
	if (yMin > 100) {
		yMin -= 100;
	}

	SetYRange(yMin, yMax);
}

#include <sstream>
#include <iostream>
#include <iomanip>
void CustomPlotter::printMatrix() const
{
	std::cout << std::endl << "   ";
	for (unsigned i = 0; i < kMaxCounters; i++) {
		std::cout << std::setw(12) << std::internal << std::setfill(' ') << kLegendTitle[i];
	}
	for (unsigned t = 0; t < kMaxSamples; t++) {
		std::cout << std::endl << "---------------------------------------------------------------" << std::endl;
		std::cout << std::setw(2) << std::setfill(' ') << t << " ";
		for (unsigned i = 0; i < kMaxCounters; i++) {
			std::cout << std::setw(12) << std::internal << std::setfill(' ') << _data[i][t];
		}
	}
	std::cout << std::endl << "     X max: " << std::setw(6) << std::setfill(' ') << GetMaxX() << "     Y max: " << std::setw(6) << std::setfill(' ') << GetMaxY();
	std::cout << std::endl << "===============================================================" << std::endl << std::endl;

}
