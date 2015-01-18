#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <algorithm>

/******************************************************************************/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_nScreenshot(0)
{
    m_ui->setupUi(this);
    m_probabilityPlot = m_ui->probabilityPlot;
    m_realImagPlot = m_ui->realImagPlot;

    // Center the window
    QScreen *screen = QGuiApplication::primaryScreen();
    int screenWidth = screen->availableSize().width();
    int screenHeight = screen->availableSize().height();
    int appWidth = 900.0;
    int appHeight = 700.0;
    float x = (screenWidth - appWidth) / 2.0;
    float y = (screenHeight - appHeight) / 2.0;
    setGeometry(x,y,appWidth,appHeight);

    // Setup the graphs
    setupGraphs();
}
/******************************************************************************/
MainWindow::~MainWindow()
{
    delete m_ui;
}
/******************************************************************************/
void MainWindow::setupGraphs()
{
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
    QMessageBox::critical(this, "", "You're using Qt < 4.7, the realtime data demo needs functions that are available with Qt 4.7 to work properly");
#endif

//    //include this section to fully disable antialiasing for higher performance:
//    m_probabilityPlot->setNotAntialiasedElements(QCP::aeAll);
//    QFont font;
//    font.setStyleStrategy(QFont::NoAntialias);
//    m_probabilityPlot->xAxis->setTickLabelFont(font);
//    m_probabilityPlot->yAxis->setTickLabelFont(font);
//    m_probabilityPlot->legend->setFont(font);

//    m_realImagPlot->setNotAntialiasedElements(QCP::aeAll);
//    m_realImagPlot->xAxis->setTickLabelFont(font);
//    m_realImagPlot->yAxis->setTickLabelFont(font);
//    m_realImagPlot->legend->setFont(font);

    // Potential
    m_probabilityPlot->addGraph();
    m_probabilityPlot->graph(0)->setPen(QPen(Qt::black));
    m_probabilityPlot->graph(0)->setBrush(QBrush(Qt::gray));
    m_probabilityPlot->graph(0)->setAntialiasedFill(false);

    // Probability
    m_probabilityPlot->addGraph();
    m_probabilityPlot->graph(1)->setPen(QPen(Qt::black));
    m_probabilityPlot->graph(1)->setBrush(QBrush(QColor(240, 255, 200)));
    m_probabilityPlot->graph(1)->setAntialiasedFill(false);

    // Set font to Computer Modern
    QFont cmuLabel("CMU Serif", 23, QFont::Light);
    QFont cmuTick("CMU Serif", 18, QFont::Light);
    m_probabilityPlot->xAxis->setLabelFont(cmuLabel);
    m_probabilityPlot->xAxis->setTickLabelFont(cmuTick);
    m_probabilityPlot->yAxis->setLabelFont(cmuLabel);
    m_probabilityPlot->yAxis->setTickLabelFont(cmuTick);
    m_realImagPlot->xAxis->setLabelFont(cmuLabel);
    m_realImagPlot->xAxis->setTickLabelFont(cmuTick);
    m_realImagPlot->yAxis->setLabelFont(cmuLabel);
    m_realImagPlot->yAxis->setTickLabelFont(cmuTick);

    // set a fixed tick-step to one tick per year value:
    m_probabilityPlot->xAxis->setAutoTickStep(false);
    m_probabilityPlot->xAxis->setTickStep(2);
    m_probabilityPlot->yAxis->setAutoTickStep(false);
    m_probabilityPlot->yAxis->setTickStep(1);
    m_realImagPlot->xAxis->setAutoTickStep(false);
    m_realImagPlot->xAxis->setTickStep(5);
    m_realImagPlot->yAxis->setAutoTickStep(false);
    m_realImagPlot->yAxis->setTickStep(1);

    // Labels
    m_probabilityPlot->xAxis->setLabel("Position");
    m_probabilityPlot->yAxis->setLabel("Probability");

    // Set initial axis ranges
    m_probabilityPlot->xAxis->setRange(0, 10);
    m_probabilityPlot->yAxis->setRange(0, 1.2);

    // Potential
    m_realImagPlot->addGraph();
    m_realImagPlot->graph(0)->setPen(QPen(Qt::blue));
    m_realImagPlot->graph(0)->setBrush(QBrush(Qt::gray));
    m_realImagPlot->graph(0)->setAntialiasedFill(false);
    m_realImagPlot->graph(0)->setName("Potential");

    // Real part
    m_realImagPlot->addGraph(); // blue line
    m_realImagPlot->graph(1)->setPen(QPen(Qt::red));
    m_realImagPlot->graph(1)->setBrush(Qt::NoBrush);
    m_realImagPlot->graph(1)->setAntialiasedFill(false);
    m_realImagPlot->graph(1)->setName("Real part");

    // Imaginary part
    m_realImagPlot->addGraph(); // blue line
    m_realImagPlot->graph(2)->setPen(QPen(Qt::blue));
    m_realImagPlot->graph(2)->setBrush(Qt::NoBrush);
    m_realImagPlot->graph(2)->setAntialiasedFill(false);
    m_realImagPlot->graph(2)->setName("Imaginary part");

    // Give the axes some labels:
    m_realImagPlot->xAxis->setLabel("Position");
    m_realImagPlot->yAxis->setLabel("Real/imaginary");

    // Set axes ranges, so we see all data:
    m_realImagPlot->xAxis->setRange(0, 2);
    m_realImagPlot->yAxis->setRange(-1.5, 1.5);

    // Allow zoom and drag
    m_probabilityPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    m_realImagPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}
/******************************************************************************/
void MainWindow::setXAxisLimits(double min, double max)
{
    m_probabilityPlot->xAxis->setRange(min, max);
    m_realImagPlot->xAxis->setRange(min, max);
}
/******************************************************************************/
void MainWindow::setYAxisLimits(double min, double max)
{
    m_probabilityPlot->yAxis->setRange(min, max);
    m_realImagPlot->yAxis->setRange(min, max);
}
/******************************************************************************/
void MainWindow::plotPotentialWall(double center,
                                   double width)
{
    // A suitable height for the potential wall is automatically determined from
    // y-axis maximum value
    double height = m_probabilityPlot->yAxis->range().upper*0.8;

    // Plot the wall in probability plot
    QCPBars *newBars1 = new QCPBars(m_probabilityPlot->xAxis,
                                   m_probabilityPlot->yAxis);
    newBars1->setWidth(width);
    newBars1->addData(center,height);
    newBars1->setPen(QPen(Qt::black));
    newBars1->setBrush(QBrush(QColor(100,100,100,50)));
    m_probabilityPlot->addPlottable(newBars1);

    // Plot the wall in real/imag plot
    QCPBars *newBars2 = new QCPBars(m_realImagPlot->xAxis,
                                   m_realImagPlot->yAxis);
    newBars2->setWidth(width);
    newBars2->addData(center, height);
    newBars2->setPen(QPen(Qt::black));
    newBars2->setBrush(QBrush(QColor(100,100,100,50)));
    m_realImagPlot->addPlottable(newBars2);
}
/******************************************************************************/
void MainWindow::plotPotentialFunction(QVector<double> x,
                                       QVector<double> potential)
{
    // A suitable height for the visualization is automatically determined
    auto minMaxV = std::minmax_element(potential.begin(),
                                       potential.end());
    double max = *minMaxV.second;
    double min = *minMaxV.first;
    double factor = std::max(std::abs(max),std::abs(min));
    double height = m_probabilityPlot->yAxis->range().upper*0.8;

    for (int site = 0; site < potential.length(); ++site)
    {
        potential[site] = potential[site]/factor*height;
    }

    m_probabilityPlot->graph(0)->setData(x,potential);
    m_realImagPlot->graph(0)->setData(x,potential);
}
/******************************************************************************/
void MainWindow::plotWaveFunction(const QVector<double> &x,
                                  const QVector<double> &probability,
                                  const QVector<double> &real,
                                  const QVector<double> &imaginary                                )
{
    // Calculate two new data points:
    #if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
        double key = 0;
    #else
        double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    #endif
    static double lastPointKey = 0;

    if (key-lastPointKey > 0.01) // at most add point every 10 ms
    {
        m_probabilityPlot->graph(1)->setData(x,probability);
        m_realImagPlot->graph(1)->setData(x,real);
        m_realImagPlot->graph(2)->setData(x,imaginary);
        lastPointKey = key;
    }

    // Replot the graphs
    m_probabilityPlot->replot();
    m_realImagPlot->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
        m_ui->statusBar->showMessage(
                    QString("%1 FPS")
                    .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
                    , 0);
        lastFpsKey = key;
        frameCount = 0;
    }
}
/******************************************************************************/
void MainWindow::screenshot()
{
    ++m_nScreenshot;
    QString number = QString::number(m_nScreenshot);
    m_ui->probabilityPlot->savePdf(number+"_probability.pdf");
    m_ui->realImagPlot->savePdf(number+"_parts.pdf");
}

