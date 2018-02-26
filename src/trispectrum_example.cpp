//------------------------------------------------------------------------------
// test bed for EFTofLSS library
//------------------------------------------------------------------------------

#include <cmath>
#include <iomanip>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <map>

#include "SPTkernels.hpp"
#include "ThreeVector.hpp"
#include "Random.hpp"
#include "Trispectrum.hpp"
#include "LinearPowerSpectrumAnalytic.hpp"
#include "LinearPowerSpectrumCAMB.hpp"
#include "InterpolatingFunction.hpp"


using namespace std;

// main routine
int main()
{
   //Analytic Power spectrum
   LinearPowerSpectrumAnalytic PL(1);

   //Alternatively, use CAMB output
   //LinearPowerSpectrumCAMB PLcamb("data/LIdata.txt");
   //PLcamb.set_kmin(0.);

   // Initialize EFT coefficients - default to zeros (i.e. ignore EFT graphs)
   EFTcoefficients coeffs;

   // Create a trispectrum object at 1 loop
   Trispectrum TS(kOneLoop, &PL, &coeffs);
   // set the maximum loop momentum magnitude
   TS.set_qmax(2.);
   // set the random number seed for VEGAS
   TS.set_seed(37);

   // Set three three-vectors
   ThreeVector k1(0.1, 1, 0.3);
   ThreeVector k2(0.3, -0.2, 0.1);
   ThreeVector k3(0.01, 0.1, 0.4);

   cout << "---------- computing Tree-level Trispectrum ----------" << endl;
   double TSresult_tree = TS.tree(k1, k2, k3);
   cout << "Tree result = " << TSresult_tree << endl;

   cout << "---------- computing One-loop Trispectrum ----------" << endl;
   IntegralResult TSresult_loop = TS.loopSPT(k1, k2, k3);
   cout << "Loop result = " << TSresult_loop.result << endl;

   return 0;
} // end of main program
