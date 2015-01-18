#ifndef CHEBYSHEVPROPAGATOR_HH
#define CHEBYSHEVPROPAGATOR_HH

#include "simenvironment.hh"

////////////////////////////////////////////////////////////////////////////////
/**
 * @brief The ChebyshevPropagator class
 * This class wraps the details of the Chebyshev propagation method. It provides
 * a simple interface for driving propagating a wavefunction with the functions
 * preparePropagation() and propagate().
 */
class ChebyshevPropagator
{
/******************************************************************************/
public:
    /**
     * @brief ChebyshevPropagator constructor
     * @param env reference to the simulation environment
     * @param wavefunction reference to the simulated wavefuntion
     * @param factorPrecision the wanted precision
     */
    ChebyshevPropagator(SimEnvironment &env,
                        cx_vec &wavefunction,
                        double factorPrecision = 1.0e-10);
    void preparePropagation();
    void propagate();

/******************************************************************************/
private:
    int calculatePolynomialFactors();
    void calculateExpansionRecursively(int finalIndex,
                                       cx_mat &hamiltonian,
                                       int currentIndex = 0);
    void calculatePropagationMatrix();
    cx_mat calculateNormalizedHamiltonian();
    void calculateGridMinMaxEnergy();

    SimEnvironment &m_env;                  ///< Simulation environment
    cx_vec &m_wavefunction;                 ///< Propagated wavefunction
    QVector<cx_mat> m_chebyshevTerms;
    QVector<cx_double> m_chebyshevFactors;
    cx_mat m_propagationMatrix;
    double m_gridEMax;                      ///< Maximum energy of the grid
    double m_gridEMin;                      ///< Minimum energy of the grid
    double m_gridEDelta;                    ///< m_gridEMax - m_gridEMin
    int m_terms;                            ///< Number of terms used
    double m_factorPrecision;               ///< The wanted precision for terms
};

#endif // CHEBYSHEVPROPAGATOR_HH
