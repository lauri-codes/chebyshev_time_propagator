#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "../../../qcustomplot/qcustomplot.h"
#include <QMainWindow>
#include <armadillo>

namespace Ui {
class MainWindow;
}

using arma::cx_vec;

////////////////////////////////////////////////////////////////////////////////
/**
 * @brief The MainWindow class
 * This class provides an UI for the simulation. It uses the QCustomPlot widgets
 * for plotting the simulation. The Simulation -class plots into this UI and
 * interactions in this UI are signaled back to it.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

/******************************************************************************/
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setupGraphs();
    void plotWaveFunction(const QVector<double> &x,
                                      const QVector<double> &probability,
                                      const QVector<double> &real,
                                      const QVector<double> &imaginary);
    void plotPotentialWall(double x, double width);
    void plotPotentialFunction(QVector<double> x,
                       QVector<double> potential);
    void setXAxisLimits(double min, double max);
    void setYAxisLimits(double min, double max);
    void screenshot();

/******************************************************************************/
signals:
    void pause();
    void resume();

/******************************************************************************/
private slots:
    void on_pauseButton_clicked()       {emit pause();}
    void on_resumeButton_clicked()      {emit resume();}
    void on_screenshotButton_clicked()  {screenshot();}

/******************************************************************************/
private:
    Ui::MainWindow *m_ui;           ///< The UI defined in mainwindow.ui
    QCustomPlot *m_probabilityPlot;
    QCustomPlot *m_realImagPlot;
    int m_nScreenshot;              ///< Screenshot counter
};
#endif // MAINWINDOW_HH
