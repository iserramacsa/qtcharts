#pragma once
#include "plotter.h"

const unsigned int kMaxCounters = 5;
const unsigned int kMaxSamples = 15;

class CustomPlotter : public Plotter
{
		Q_OBJECT
	public:
		explicit CustomPlotter(QWidget *parent = nullptr);

		void PushData(const std::array<unsigned int, kMaxCounters>& data);
		void Clear();

	private:
		virtual void paintEvent(QPaintEvent *event) override;
		std::array<std::array<unsigned int, kMaxSamples>, kMaxCounters> _data;

		void updateLimits();
		void printMatrix() const;



};
