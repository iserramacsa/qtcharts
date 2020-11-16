#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QFormLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(buildCentralWidget());
}

MainWindow::~MainWindow()
{
    delete ui;
}

QWidget *MainWindow::buildCentralWidget()
{
    QWidget* central = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(central);

    _plotter = new Plotter(central);

    layout->addWidget(buildChartOptionsBar(central));
    layout->addWidget(_plotter);

    _showGrid->setChecked(_plotter->IsGridVisible());
    _showMarks->setChecked(_plotter->AxisMarksVisible());
    _showXAxis->setChecked(_plotter->XAxisVisible());
    _showYAxis->setChecked(_plotter->YAxisVisible());

    _minX->setValue(_plotter->GetMinX());
    _maxX->setValue(_plotter->GetMaxX());
    _minY->setValue(_plotter->GetMinY());
    _maxY->setValue(_plotter->GetMaxY());


    connect(_showGrid, SIGNAL(clicked(bool)), SLOT(onShowGridChanged()));
    connect(_showMarks, SIGNAL(clicked(bool)), SLOT(onShowYAxisChanged()));
    connect(_showXAxis, SIGNAL(clicked(bool)), SLOT(onShowXAxisChanged()));
    connect(_showYAxis, SIGNAL(clicked(bool)), SLOT(onShowYAxisChanged()));


    connect(_minX, SIGNAL(valueChanged(double)), SLOT(onChangeXAxis()));
    connect(_maxX, SIGNAL(valueChanged(double)), SLOT(onChangeXAxis()));
    connect(_minY, SIGNAL(valueChanged(double)), SLOT(onChangeYAxis()));
    connect(_maxY, SIGNAL(valueChanged(double)), SLOT(onChangeYAxis()));

    return central;
}

QWidget *MainWindow::buildChartOptionsBar(QWidget* parent)
{
    QWidget* bar = new QWidget(parent);
    bar->setMaximumWidth(200);
    QFormLayout* layout =  new QFormLayout(bar);

    _chartType = new QComboBox(bar);
    _chartType->addItem("Graphical");
    layout->addRow("Chart type:", _chartType);

    _showXAxis = new QCheckBox(bar);
    _showYAxis = new QCheckBox(bar);
    _showGrid = new QCheckBox(bar);
    _showMarks = new QCheckBox(bar);

    layout->addRow("Show X axis:", _showXAxis);
    layout->addRow("Show Y axis:", _showYAxis);
    layout->addRow("Show grid:", _showGrid);
    layout->addRow("Show marks:", _showMarks);

    _minX = new QDoubleSpinBox(bar);
    _maxX = new QDoubleSpinBox(bar);
    _minY = new QDoubleSpinBox(bar);
    _maxY = new QDoubleSpinBox(bar);

    layout->addRow("Min X axis:", _minX);
    layout->addRow("Max x axis:", _maxX);
    layout->addRow("Min Y axis:", _minY);
    layout->addRow("Max Y axis:", _maxY);


    return bar;
}

void MainWindow::onShowXAxisChanged()
{
    _plotter->ShowXAxis(_showXAxis->isChecked(), _showMarks->isChecked());
}
void MainWindow::onShowYAxisChanged()
{
    _plotter->ShowYAxis(_showYAxis->isChecked(), _showMarks->isChecked());
}
void MainWindow::onShowGridChanged()
{
    _plotter->ShowGrid(_showGrid->isChecked());
}
void MainWindow::onShowMarksChanged()
{
    _plotter->ShowAxis(_showXAxis->isChecked(), _showYAxis->isChecked(), _showMarks->isChecked());
}

void MainWindow::onChangeXAxis()
{
    blockSignals(true);
    _minX->setRange(-999, _maxX->value() - 1);
    _maxX->setRange(_minX->value() + 1, 999);
    blockSignals(false);
    _plotter->SetXRange(_minX->value(), _maxX->value());
}

void MainWindow::onChangeYAxis()
{
    blockSignals(true);
    _minY->setRange(-999, _maxY->value() - 1);
    _maxY->setRange(_minY->value() + 1, 999);
    blockSignals(false);
    _plotter->SetYRange(_minY->value(), _maxY->value());
}
