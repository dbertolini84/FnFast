//------------------------------------------------------------------------------
/// \file Momentum.hpp
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
//    Interface of class DiagramMomenta
//------------------------------------------------------------------------------

#ifndef MOMENTUM_HPP
#define MOMENTUM_HPP

#include <vector>
#include <map>

#include "Labels.hpp"
#include "ThreeVector.hpp"

using namespace std;

//------------------------------------------------------------------------------
/**
 * \class DiagramMomenta
 *
 * \brief class for momenta of loop, external lines
 *
 * DiagramMomenta(map<MomentumLabel, ThreeVector> momenta)
 *
 * Container for the loop and external momenta.
 * Instantiated with a map from MomentumLabel to ThreeVector
 *
 * Provides functions for:
 * - Accessing momenta
 * - Modifying momenta
 */
//------------------------------------------------------------------------------

class DiagramMomenta
{
   private:
      map<Momenta::MomentumLabel, ThreeVector> _momenta;        ///< ThreeVector for a given MomentumLabel

   public:
      /// constructor
      DiagramMomenta(map<Momenta::MomentumLabel, ThreeVector> momenta);
      /// destructor
      virtual ~DiagramMomenta() {}

      /// accessors
      map<Momenta::MomentumLabel, ThreeVector> momenta() { return _momenta; }
      ThreeVector& operator[](Momenta::MomentumLabel label) { return _momenta[label]; }
};

////////////////////////////////////////////////////////////////////////////////
// Inline Declarations
////////////////////////////////////////////////////////////////////////////////

#endif // MOMENTUM_HPP
