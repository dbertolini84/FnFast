#include "InterpolatingFunction.hpp"

using namespace std;

//------------------------------------------------------------------------------
InterpolatingFunction::InterpolatingFunction(const string& input_file): _input_file(input_file), _accel_ptr(NULL), _spline_ptr(NULL)
{
    ifstream file;
    file.open(_input_file);
    
    if(!file.good()) { cout << "InterpolatingFunction: I can't open the requested file, " << input_file << endl; }
    
    else {
        
        // Read file into vectors
        int npts = 0;
        while(!file.eof()) {
            double k,P;
            file >> k >> P;
            _kvec.push_back(k);
            _Pvec.push_back(P);
            npts++;
        }
        
        // Arrays for interpolation
        double *k_vals;
        double *P_vals;
        
        k_vals = new double[npts];
        P_vals = new double[npts];
       
        // Fill in arrays
        copy(_kvec.begin(),_kvec.end(),k_vals);
        copy(_Pvec.begin(),_Pvec.end(),P_vals);
       
        // Allocate interpolation pointers and initialize interpolation
        _accel_ptr = gsl_interp_accel_alloc();
        _spline_ptr = gsl_spline_alloc (gsl_interp_cspline, npts);
        gsl_spline_init(_spline_ptr, k_vals, P_vals, npts);
    }
   
    file.close();
}

//------------------------------------------------------------------------------
double InterpolatingFunction::operator()(double x)
{
   double res = 0;
   if(x >= _kvec.front() && x <= _kvec.back() ) res = gsl_spline_eval(_spline_ptr, x, _accel_ptr); // Interpolated function
   return res;
}