#ifndef SIMULATION_HH
#define SIMULATION_HH

#include "mainwindow.hh"
#include "chebyshevpropagator.hh"
#include "simenvironment.hh"

////////////////////////////////////////////////////////////////////////////////
/**
 * @brief The Simulation class
 * This class handles the logic of the simulation. Mainly this class is used to
 * setup and control the simulation. This class communicates with the UI in
 * MainWindow to react to user actions and to plot the simulation.
 */
class Simulation : public QObject
{
    Q_OBJECT

/******************************************************************************/
public:
    Simulation(MainWindow &window, QWidget *parent = 0);
    void setEvenGrid(int nSites, double length);
    void setTimeStep(double dt);
    void setPotentialWall(double center, double height, double width);
    void setGaussianWavePacket(double mean, double std, double momentum);
    void setGrid(QVector<double> grid);
    void setInitialWavefunction(cx_vec wavefunction);
    void setPotential(QVector<double> potential);

/******************************************************************************/
public slots:
    void start();
    void pause();
    void resume();

/******************************************************************************/
private slots:
    void advance();

/******************************************************************************/
private:
    MainWindow &m_window;
    cx_vec m_wavefunction;
    ChebyshevPropagator m_propagator;   ///< Handles the propagation
    SimEnvironment m_env;               ///< Environment for the simulation
    QTimer m_timer;                     ///< Drives the simulation
    bool m_running;
};

#endif // SIMULATION_HH
