//------------------------------------------------------------------------------
/// \file Trispectrum.hpp
//
// Author(s):
//    Jon Walsh
//
// Copyright:
//    Copyright (C) 2015  LBL
//
//    This file is part of the EFTofLSS library. EFTofLSS is distributed under the
//    terms of the GNU General Public License version 3 (GPLv3), see the COPYING
//    file that comes with this distribution for details.
//    Please respect the academic usage guidelines in the GUIDELINES file.
//
// Description:
//    Interface of class Trispectrum
//------------------------------------------------------------------------------

#ifndef TRISPECTRUM_HPP
#define TRISPECTRUM_HPP

#include <vector>
#include <map>

#include "Momentum.hpp"
#include "Diagram.hpp"
#include "SPTkernels.hpp"
#include "EFTkernels.hpp"
#include "WindowFunctionBase.hpp"
#include "Integration.hpp"

using namespace std;

//------------------------------------------------------------------------------
/**
 * \class Trispectrum
 *
 * \brief class to calculate the trispectrum
 *
 * Trispectrum(Order)
 *
 * Contains the trispectrum at various levels:
 * - tree level:
 *    - differential in k_i
 *    - integrated over k_i
 * - one loop (only available if called with Order oneLoop)
 *    - differential in k_i, q
 *    - integrated over q, differential in k_i
 *    - integrated over q, k_i
 *
 * Provides functions for access to the trispectrum at these levels
 */
//------------------------------------------------------------------------------

class Trispectrum
{
   public:
      enum Graphs {
         T3111,
         T2211,
         T5111,
         T4211a,
         T4211b,
         T3311a,
         T3311b,
         T3221a,
         T3221b,
         T3221c,
         T2222,
         T5111x,
         T4211ax,
         T3311ax,
         T3221ax
      };

   private:
      Order _order;                                   ///< order of the calculation
      LinearPowerSpectrumBase* _PL;                   ///< the linear power spectrum used in the calculation
      SPTkernels* _SPTkernels;                        ///< SPT kernels instance
      EFTkernels* _EFTkernels;                        ///< EFT kernels instance
      vector<Diagram*> _tree;                         ///< tree level diagrams
      vector<Diagram*> _loop;                         ///< loop diagrams
      vector<Diagram*> _cterms;                       ///< counterterms
      map<Graphs, Diagram*> _diagrams;                ///< container for diagrams
      EFTcoefficients* _eftcoefficients;              ///< EFT coefficients
      vector<Momenta::MomentumLabel> _labels;         ///< external momenta labels
      double _UVcutoff;                               ///< UV cutoff for loop integrations
      double _kBin;                                   ///< size of k bins
      WindowFunctionBase* _W;                         ///< Window function
      int _seed;                                      ///< random number seed for VEGAS

      /// phase space for the loop momentum
      class LoopPhaseSpace {
         private:
            double _qmax;           ///< upper limit on q integral
            double _jacobian;       ///< jacobian for the phase space point
            ThreeVector _q;         ///< loop momentum value

         public:
            static constexpr double pi = 3.14159265358979;      ///< pi

         public:
            /// constructor
            LoopPhaseSpace(double qmax) : _qmax(qmax) {}
            /// destructor
            virtual ~LoopPhaseSpace() {}

            /// set the loop phase space, returns the jacobian
            double set_loopPS(double qpts[3], double x12);

            /// returns the loop momentum
            ThreeVector q() { return _q; }
      };
   
      /// phase space for the Angular Integration
      class AngularPhaseSpace {
         private:
      
            double _jacobian;       ///< jacobian for the phase space point
            ThreeVector _qHat;      ///< loop momentum value
      
         public:
            static constexpr double pi = 3.14159265358979;      ///< pi
      
         public:
            /// constructor
            AngularPhaseSpace() {}
            /// destructor
            virtual ~AngularPhaseSpace() {}
      
            /// set the loop phase space, returns the jacobian
            double set_angularPS(double qpts[2]);
      
            /// returns the loop momentum
            ThreeVector qHat() { return _qHat; }
      };


      /// container for the integration options
      struct LoopIntegrationOptions {
         double k;
         double kp;
         double costheta;
         Trispectrum* trispectrum;
         LoopPhaseSpace* loopPS;
      };

      /// container for the integration options
      struct AngularIntegrationOptions {
         double k;
         double kp;
         Trispectrum* trispectrum;
      };

      /// container for the integration options
      struct GeneralAngularIntegrationOptions {
         double k;
         double kp;
         double kpp;
         Trispectrum* trispectrum;
         AngularPhaseSpace* angularPS;
      };
   
      /// container for the integration options
      struct AngularLoopIntegrationOptions {
         double k;
         double kp;
         double kpp;
         Trispectrum* trispectrum;
         AngularPhaseSpace* angularPS;
         LoopPhaseSpace* loopPS;
      };
   
   /// container for the integration options
   struct TriLoopIntegrationOptions {
      ThreeVector k1;
      ThreeVector k2;
      ThreeVector k3;
      Trispectrum* trispectrum;
      LoopPhaseSpace* loopPS;
   };

   
   
   public:
      /// constructor
      Trispectrum(Order order, LinearPowerSpectrumBase* PL, EFTcoefficients* eftcoefficients);
      /// destructor
      virtual ~Trispectrum() {}

      /// access diagrams
      Diagram* operator[](Graphs graph) { return _diagrams[graph]; }
   
      /// set size of k bins
      void set_kBinSize(double kBin) { _kBin = kBin;}
      /// set window function
      void set_windowFunction(WindowFunctionBase* W) { _W = W; }

      /// set the loop momentum cutoff
      void set_qmax(double qmax);

      /// set the random number seed
      void set_seed(int seed) { _seed = seed; }

      /// get results
      /// covariance limit, differential in k
      /// tree level
      double cov_tree(double k, double kp, double costheta);
      /// one loop integrated in q
      IntegralResult cov_loopSPT(double k, double kp, double costheta);
      /// one loop counterterms
      double cov_ctermsEFT(double k, double kp, double costheta);

      /// covariance limit integrated over angles
      /// tree level
      IntegralResult cov_tree(double k, double kp);
      /// full one loop integrated in q
      IntegralResult cov_loopSPT(double k, double kp);
      /// one loop counterterms
      IntegralResult cov_ctermsEFT(double k, double kp);

      /// Averaged over k bins
      /// tree level
      double cov_tree_kbin(double k2, double k3, double k4);
      /// one loop integrated in q
      double cov_loopSPT_kbin(double k2, double k3, double k4);
      /// one loop counterterms
      double cov_ctermsEFT_kbin(double k2, double k3, double k4);

      /// Averaged over k bins + convolution with window function
      /// tree level
      double cov_tree_kbin_win(double k2, double k3, double k4);
      /// one loop integrated in q
      double cov_loopSPT_kbin_win(double k2, double k3, double k4);
      /// one loop counterterms
      double cov_ctermsEFT_kbin_win(double k2, double k3, double k4);

      /// full trispectrum, differential in k
      /// tree level
      double tree(ThreeVector k1, ThreeVector k2, ThreeVector k3);
      /// one loop differential in q and integrated in q
      double loopSPT_excl(ThreeVector k1, ThreeVector k2, ThreeVector k3, ThreeVector q);
      IntegralResult loopSPT(ThreeVector k1, ThreeVector k2, ThreeVector k3);
      /// one loop counterterms
      double ctermsEFT(ThreeVector k1, ThreeVector k2, ThreeVector k3);
   
      // DANIELE
      // select a single loop diagram
      // right now only for testing purpose
      void set_loop_diagram(Trispectrum::Graphs graph_label);
   
   
      // ANGULAR AVERAGED TRIANGLE
      // <T(k, kp, -k-kp, 0)>
      IntegralResult tri_triangle_tree(double k, double kp);
      IntegralResult tri_triangle_loopSPT(double k, double kp);
   
      // ANGULAR AVERAGED TRAPEZOID
      IntegralResult tri_trapezoid_tree(double k, double kp, double kpp);
      IntegralResult tri_trapezoid_loop(double k, double kp, double kpp);
   
      // EQUILATERAL TRIANGLE
      IntegralResult tri_equi_tree(double k, double kp);
      IntegralResult tri_equi_loop(double k, double kp);

   

   private:
      /// tree level: polar angle integrand function, covariance limit
      static int tree_angular_integrand(const int *ndim, const double xx[], const int *ncomp, double ff[], void *userdata);
      /// loop integrand function, covariance limit
      static int loop_integrand(const int *ndim, const double xx[], const int *ncomp, double ff[], void *userdata);
      /// polar angle integrand function, covariance limit
      static int angular_loop_integrand(const int *ndim, const double xx[], const int *ncomp, double ff[], void *userdata);
      /// polar angle integrand function, covariance limit, EFT counterterms
      static int cterms_angular_integrand(const int *ndim, const double xx[], const int *ncomp, double ff[], void *userdata);
   
      static int triangle_tree_integrand(const int *ndim, const double xx[], const int *ncomp, double ff[], void *userdata);
      static int triangle_loop_integrand(const int *ndim, const double xx[], const int *ncomp, double ff[], void *userdata);
   
      static int general_angular_integrand_tree(const int *ndim, const double xx[], const int *ncomp, double ff[], void *userdata);
      static int general_angular_integrand_loop(const int *ndim, const double xx[], const int *ncomp, double ff[], void *userdata);
   
      static int equilateral_integrand_tree(const int *ndim, const double xx[], const int *ncomp, double ff[], void *userdata);
      static int equilateral_integrand_loop(const int *ndim, const double xx[], const int *ncomp, double ff[], void *userdata);

      static int loopSPT_integrand(const int *ndim, const double xx[], const int *ncomp, double ff[], void *userdata);


   
   
      double _tri_triangle_tree(double k, double kp, double costheta);

};

////////////////////////////////////////////////////////////////////////////////
// Inline Declarations
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
inline void Trispectrum::set_qmax(double qmax) {
   for (size_t c = 0; c < _loop.size(); c++) {
      _loop[c]->set_qmax(qmax);
   }
}

#endif // TRISPECTRUM_HPP
