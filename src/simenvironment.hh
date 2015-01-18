#ifndef SIMENVIRONMENT_HH
#define SIMENVIRONMENT_HH

//#define ARMA_NO_DEBUG   // Optimization for armadillo
#include <armadillo>
#include <QVector>

// Use these abbreviations
using arma::cx_rowvec;
using arma::cx_double;
using arma::cx_vec;
using arma::cx_mat;
using arma::mat;

const double pi = 3.1415926535897;

////////////////////////////////////////////////////////////////////////////////
/**
 * @brief The SimEnvironment struct
 */
struct SimEnvironment {
    QVector<double> m_grid;
    QVector<double> m_potential;
    int m_nSites;
    double m_dx;
    double m_dt;
};

#endif // SIMENVIRONMENT_HH
