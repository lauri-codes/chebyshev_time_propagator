#include "chebyshevpropagator.hh"
#include <QDebug>

/******************************************************************************/
ChebyshevPropagator::ChebyshevPropagator(SimEnvironment &env,
                                         cx_vec &wavefunction,
                                         double factorPrecision)
    : m_env(env)
    , m_wavefunction(wavefunction)
    , m_factorPrecision(factorPrecision)
{
}
/******************************************************************************/
cx_mat ChebyshevPropagator::calculateNormalizedHamiltonian()
{
    cx_mat hamiltonian(m_env.m_nSites,m_env.m_nSites);
    hamiltonian.zeros();

    // Fill diagonal
    for (int index = 0; index < m_env.m_nSites; ++index)
    {
        double potential = m_env.m_potential[index]*2*pow(m_env.m_dx,2);

        if (index == 0 || index == m_env.m_nSites)
        {
            hamiltonian(index,index) = cx_double(1+potential,0);
        }
        else
        {
            hamiltonian(index,index) = cx_double(-2+potential,0);
        }
    }

    // Fill off-diagonals
    for (int index = 0; index < m_env.m_nSites-1; ++index)
    {
        hamiltonian(index,index+1) = cx_double(1,0);
        hamiltonian(index+1,index) = cx_double(1,0);
    }

    // Multiply the matrix with prefactor
    double prefactor = -1.0/(2.0*pow(m_env.m_dx,2));
    hamiltonian *= prefactor;

    cx_mat identity = arma::eye<cx_mat>(m_env.m_nSites,
                                        m_env.m_nSites);

    cx_mat normalizedHamiltonian = (2*hamiltonian-identity*
                                    (m_gridEMin+m_gridEMax))/m_gridEDelta;
    return normalizedHamiltonian;
}
/******************************************************************************/
void ChebyshevPropagator::calculateGridMinMaxEnergy()
{
    auto VMinMax = std::minmax_element(m_env.m_potential.begin(),
                                       m_env.m_potential.end());
    double TMax = 1.0/2.0*pow(pi/m_env.m_dx,2);
    m_gridEMin = *VMinMax.first;
    m_gridEMax = *VMinMax.second+TMax;
    m_gridEDelta = m_gridEMax-m_gridEMin;
}
/******************************************************************************/
int ChebyshevPropagator::calculatePolynomialFactors()
{
    m_chebyshevFactors.clear();
    double alpha = m_gridEDelta*m_env.m_dt/2;

    double factor = _j0(alpha);
    int term = 0;

    while (factor > m_factorPrecision)
    {
        m_chebyshevFactors.push_back(factor);
        ++term;
        factor = 2*_jn(term,alpha);
    }
    qDebug() << Q_FUNC_INFO << "Number of terms in expansion: " << term;
    return term;
}
/******************************************************************************/
void ChebyshevPropagator::calculateExpansionRecursively(int finalIndex,
                                       cx_mat &hamiltonian,
                                       int currentIndex)
{
    // Initial conditions
    if (currentIndex == 0)
    {
        m_chebyshevTerms.clear();
        cx_mat identity = arma::eye<cx_mat>(m_env.m_nSites,
                                            m_env.m_nSites);
        m_chebyshevTerms.push_back(identity);
    }
    else if (currentIndex == 1)
    {
        m_chebyshevTerms.push_back(-cx_double(0,1)*hamiltonian);
    }

    // Recurrence relation
    else
    {
        m_chebyshevTerms.push_back(-cx_double(0,2)*hamiltonian*
                                   m_chebyshevTerms[currentIndex-1]+
                                   m_chebyshevTerms[currentIndex-2]);
    }

    // The end condition
    if (currentIndex == finalIndex-1)
    {
        return;
    }
    else
    {
        calculateExpansionRecursively(finalIndex,hamiltonian,currentIndex+1);
    }
}
/******************************************************************************/
void ChebyshevPropagator::preparePropagation()
{
    calculateGridMinMaxEnergy();
    cx_mat normalizedHamiltonian = calculateNormalizedHamiltonian();
    int numberOfTerms = calculatePolynomialFactors();
    calculateExpansionRecursively(numberOfTerms,normalizedHamiltonian);
    calculatePropagationMatrix();
}
/******************************************************************************/
void ChebyshevPropagator::calculatePropagationMatrix()
{
    m_propagationMatrix = cx_mat(m_env.m_nSites,m_env.m_nSites);
    m_propagationMatrix.zeros();

    for (int term = 0; term < m_chebyshevTerms.length(); ++term)
    {
        m_propagationMatrix += m_chebyshevFactors[term]*m_chebyshevTerms[term];
    }

    cx_double prefactor = exp((cx_double(0,-0.5)*
                               (m_gridEMax+m_gridEMin))*m_env.m_dt);
    m_propagationMatrix *=prefactor;
}
/******************************************************************************/
void ChebyshevPropagator::propagate()
{
    m_wavefunction = m_propagationMatrix*m_wavefunction;
}
