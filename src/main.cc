#include "mainwindow.hh"
#include "simulation.hh"
#include <QApplication>

////////////////////////////////////////////////////////////////////////////////
/**
 * @brief main
 *
 * Setups the GUI and the simulation.
 *
 * Libraries used:
 *  -Qt for UI and other stuff
 *  -Armadillo for matrix calculus (http://arma.sourceforge.net/)
 *  -QCustomPlot for realtime plotting and plot exporting to .pdf
 *  (http://www.qcustomplot.com/
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    Simulation simulation(window);

    simulation.setEvenGrid(300,10);               //sites,length
    simulation.setTimeStep(0.0003);
    simulation.setPotentialWall(5,1000,0.5);      //center,height,width
    simulation.setGaussianWavePacket(2,0.6,20);   //mean,std,p
    simulation.start();

    window.show();

    return a.exec();
}
