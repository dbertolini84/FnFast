#ifndef INTERPOLATING_FUNCTION_HPP
#define INTERPOLATING_FUNCTION_HPP

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <math.h>
#include "LinearPowerSpectrumBase.hpp"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_fit.h>

using namespace std;


class InterpolatingFunction : public LinearPowerSpectrumBase
{
   private:
      string _input_file;                                ///< input file
      vector<double> _kvec;                              ///< data storage vectors
      vector<double> _Pvec;                              ///< data storage vectors
      gsl_interp_accel* _accel_ptr;                      ///< interpolation objects in gsl
      gsl_spline* _spline_ptr;                           ///< interpolation objects in gsl
   
   public:
      /// constructors
     InterpolatingFunction(const string& input_file);
      /// destructor
      virtual ~InterpolatingFunction()
      {
         gsl_spline_free(_spline_ptr);
         gsl_interp_accel_free(_accel_ptr);
      }
   
   /// returns the linear power spectrum
   double operator()(double x);
};

////////////////////////////////////////////////////////////////////////////////
// Inline Declarations
////////////////////////////////////////////////////////////////////////////////

#endif