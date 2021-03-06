//------------------------------------------------------------------------------
/// \file Propagator.hpp
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
//    Interface of class Propagator
//------------------------------------------------------------------------------

#ifndef PROPAGATOR_HPP
#define PROPAGATOR_HPP

#include <vector>
#include <map>

#include "LabelMap.hpp"
#include "ThreeVector.hpp"

namespace fnfast {

//------------------------------------------------------------------------------
/**
 * \class Propagator
 *
 * \brief class for propagators
 *
 * Propagator(vector<pair<Momentum, double> > components)
 *
 * Container that is an abstract representation of a linear combination
 * of loop and external momenta.  A method is provided to input actual
 * 3-momenta and obtain a ThreeVector of the momentum of the propagator
 * Instantiated with a vector of pairs, where:
 * - first item: label for which momentum it is
 * - second item: factor (int) multiplying the momentum
 *
 * Provides functions for:
 * - ThreeVector for the momentum given values for the loop and external momenta
 */
//------------------------------------------------------------------------------

class Propagator
{
   public:
      /// label determining the sign of the momentum in a propagator
      enum class LabelFlow : int {
         kMinus = -1,
         kNull = 0,
         kPlus = 1
      };

   private:
      LabelMap<Momentum, LabelFlow> _components;     ///< components of the momenta and their scale factors

   public:
      /// constructor
      Propagator(LabelMap<Momentum, LabelFlow> components);
      /// destructor
      virtual ~Propagator() {}

      /// accessors
      LabelMap<Momentum, LabelFlow> components() const { return _components; }

      /// get the momentum given values for the loop, external momenta
      ThreeVector p(LabelMap<Momentum, ThreeVector> mom) const;

      /// get the list of labels with coefficients != kNull in the propagator
      std::vector<Momentum> labels() const;

      /// check if a given label is present in the propagator
      bool hasLabel(Momentum label) const;

      /// check if the propagator is null (has any nonzero contributions)
      bool isNull() const;

      /// returns a propagator with the momentum reversed
      Propagator reverse() const;

      /*
       * return a momentum object from the current instance where we
       * solve for the momentum of label given the total momentum = 0
       * e.g. if we have the momentum -q + k2 + k3, then solveNull(q) = k2 + k3
       */
      Propagator IRpole(Momentum label) const;

      /// Propagator stream insertion operator
      friend std::ostream& operator<<(std::ostream& out, const Propagator& prop);

   private:
      /// helper function to reverse sign of a propagator coefficient
      static LabelFlow reverse_flow(LabelFlow sign);

   protected:
      /// Streams a string to \a out.
      std::ostream& print(std::ostream& out) const;
};

////////////////////////////////////////////////////////////////////////////////
// Inline Declarations
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& out, const Propagator& prop)
{
   return prop.print(out);
}

} // namespace fnfast

#endif // PROPAGATOR_HPP
