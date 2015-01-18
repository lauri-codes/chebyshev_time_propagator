#include "simulation.hh"
#include <algorithm>

/******************************************************************************/
Simulation::Simulation(MainWindow &window, QWidget *parent)
    : QObject(parent)
    , m_window(window)
    , m_propagator(ChebyshevPropagator(m_env,m_wavefunction))
    , m_running(false)
{
    // Advance simulation with timer signal
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(advance()));

    // Connect UI signals here
    connect(&m_window, SIGNAL(resume()), this, SLOT(resume()));
    connect(&m_window, SIGNAL(pause()), this, SLOT(pause()));
}
/******************************************************************************/
void Simulation::setTimeStep(double dt)
{
    m_env.m_dt = dt;
}
/******************************************************************************/
void Simulation::setEvenGrid(int nSites, double length)
{
    m_env.m_nSites = nSites;
    QVector<double> grid(m_env.m_nSites,0);
    m_env.m_dx = length/(m_env.m_nSites-1);
    for (int site = 0; site < m_env.m_nSites; ++site)
    {
        grid[site] = site*m_env.m_dx;
    }
    setGrid(grid);
}
/******************************************************************************/
void Simulation::setGaussianWavePacket(double mean,
                                         double std,
                                         double momentum)
{
    cx_vec wavePacket(m_env.m_nSites);
    for (int site = 0; site < m_env.m_nSites; ++site)
    {
        double x = m_env.m_grid[site];
        wavePacket(site) = 1.0/(std*sqrt(2*pi))*
                                exp(cx_double(-pow(x-mean,2)/(2.0*pow(std,2)),
                                            momentum*x));
    }
    setInitialWavefunction(wavePacket);
}
/******************************************************************************/
void Simulation::setPotentialWall(double center, double height, double width)
{
    double start = center-width/2;
    double end = center+width/2;

    QVector<double> potential(m_env.m_nSites,0);

    for (int site = 0; site < m_env.m_nSites; ++site)
    {
        if (m_env.m_grid[site] >= start && m_env.m_grid[site] <= end)
        {
            potential[site] = height;
        }
    }
    setPotential(potential);
    m_window.plotPotentialWall(center,width);
}
/******************************************************************************/
void Simulation::setPotential(QVector<double> potential)
{
    m_env.m_potential = potential;
}
/******************************************************************************/
void Simulation::setInitialWavefunction(cx_vec wavefunction)
{
    m_wavefunction = wavefunction;
}
/******************************************************************************/
void Simulation::setGrid(QVector<double> grid)
{
    m_env.m_grid = grid;
    m_window.setXAxisLimits(grid.first(),grid.last());
}
/******************************************************************************/
void Simulation::advance()
{
    // Propagate the wavefunction
    m_propagator.propagate();

    // Extract real part, imaginary part and amplitude for plotting
    int nSites = m_env.m_nSites;
    static QVector<double> real(nSites,0);
    static QVector<double> imaginary(nSites,0);
    static QVector<double> probability(nSites,0);

    for (int site = 0; site < nSites; ++site)
    {
        real[site] = std::real(m_wavefunction[site]);
        imaginary[site] = std::imag(m_wavefunction[site]);
        probability[site] = std::norm(m_wavefunction[site]);
    }

    // Plot
    m_window.plotWaveFunction(m_env.m_grid,probability,real,imaginary);
}
/******************************************************************************/
void Simulation::start()
{
    m_propagator.preparePropagation();
    m_timer.start(0); // Interval 0 means to refresh as fast as possible
    m_running = true;
}
/******************************************************************************/
void Simulation::pause()
{
    if (m_running)
    {
        m_timer.stop();
        m_running = false;
    }
}
/******************************************************************************/
void Simulation::resume()
{
    if (!m_running)
    {
        m_timer.start();
        m_running = true;
    }
}
