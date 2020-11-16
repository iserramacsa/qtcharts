#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QCheckBox>
#include <QDoubleSpinBox>

#include "plotter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QComboBox* _chartType;

    Plotter *_plotter;
    QCheckBox* _showXAxis;
    QCheckBox* _showYAxis;
    QCheckBox* _showMarks;
    QCheckBox* _showGrid;
    QDoubleSpinBox* _minX;
    QDoubleSpinBox* _maxX;
    QDoubleSpinBox* _minY;
    QDoubleSpinBox* _maxY;


    QWidget* buildCentralWidget();
    QWidget* buildChartOptionsBar(QWidget *parent);


private slots:
    void onShowXAxisChanged();
    void onShowYAxisChanged();
    void onShowGridChanged();
    void onShowMarksChanged();
    void onChangeXAxis();
    void onChangeYAxis();
};
#endif // MAINWINDOW_H
