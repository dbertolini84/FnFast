//------------------------------------------------------------------------------
/// \file Diagram.cpp
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
//    Implementation of class Diagram
//------------------------------------------------------------------------------

#include "Diagram.hpp"

/*
 * The implementation of a SPT diagram
 * in terms of the lines in the graph
 */

//------------------------------------------------------------------------------
Diagram::Diagram(vector<Line> lines, map<Vertices::VertexLabel, KernelBase*> kernels, LinearPowerSpectrumBase* PL) : _lines(lines), _kernels(kernels), _PL(PL)
{
   // iterate over lines, fill in other diagram objects
   _order = kTree;
   for (size_t i = 0; i < _lines.size(); i++) {
      // check if the line has the loop momentum in it
      // if so, it has an IR pole that must be regulated if it is away from 0
      if (_lines[i].propagator().hasLabel(Momenta::q)) {
         _order = kOneLoop;
         Propagator pole = _lines[i].propagator().IRpole(Momenta::q);
         if (!pole.isNull()) {
            _IRpoles.push_back(pole);
         }
      }
      // for the start and end vertices, add the momentum to the vertex factor list
      _vertexmomenta[_lines[i].start()].push_back(_lines[i].propagator());
      _vertexmomenta[_lines[i].end()].push_back(_lines[i].propagator().reverse());
   }
}

//------------------------------------------------------------------------------
double Diagram::value_base(DiagramMomenta mom)
{
   // the diagram value is:
   // symmetry factor * propagators * vertices
   double value = _symfac;
   // iterate over lines
   for (size_t i = 0; i < _lines.size(); i++) {
      value *= (*_PL)(_lines[i].propagator().p(mom).magnitude());
   }
   // now do vertex factore
   for (size_t i = 0; i < Vertices::vertexlabels.size(); i++) {
      Vertices::VertexLabel vertex = Vertices::vertexlabels[i];
      vector<ThreeVector> p;
      for (size_t j = 0; j < _vertexmomenta[vertex].size(); j++) {
         p.push_back(_vertexmomenta[vertex][j].p(mom));
      }
      value *= _kernels[vertex]->Fn_sym(p);
   }
   return value;
}

//------------------------------------------------------------------------------
double Diagram::value_base_IRreg(DiagramMomenta mom)
{
   // To regulate the diagram in the IR, we map each region with
   // an IR pole at q = qIR != 0 onto coordinates with the pole at q = 0
   double value = 0;
   // each IRpole gets a set of PS cuts to isolate it
   // first we do the pole at q = 0
   double PSregion = 1;
   for (size_t i = 0; i < _IRpoles.size(); i++) {
      // require theta(|q| < |q - pole|) for each non-null IR pole
      ThreeVector pole = _IRpoles[i].p(mom);
      PSregion *= theta(mom[Momenta::q], mom[Momenta::q] - pole);
      // add diagram value times the PS factor
      value += PSregion * value_base(mom);
   }
   // now loop over all the IR poles
   for (size_t i = 0; i < _IRpoles.size(); i++) {
      // for these poles we change variables: q -> q + pole
      // so that the pole maps to 0 and we exclude all other poles
      ThreeVector pole = _IRpoles[i].p(mom);
      // first the PS cuts from mapping the pole onto q = 0 (and shifting the pole at 0)
      PSregion = 1;
      PSregion *= theta(mom[Momenta::q], mom[Momenta::q] + pole);
      // loop over all other poles and make PS cuts for each
      for (size_t j = 0; j < _IRpoles.size(); i++) {
         if (j != i) {
            ThreeVector pole_j = _IRpoles[j].p(mom);
            PSregion *= theta(mom[Momenta::q], mom[Momenta::q] + pole - pole_j);
         }
      }
      // copy and shift the diagram momentum for the pole
      DiagramMomenta mom_shift = mom;
      mom_shift[Momenta::q] = mom[Momenta::q] + pole;
      // add the diagram value for this shifted momentum, times the PS factor
      value += PSregion * value_base(mom_shift);
   }

   return value;
}

//------------------------------------------------------------------------------
double Diagram::value_IRreg(DiagramMomenta mom)
{
   // To return the IR regulated diagram symmetrized over external momenta,
   // we symmetrize the IR regulated diagram with the input momentum routing
   // over external momentum configurations.
   // To make the symmetrization more efficient, we compute only the
   // momentum configurations giving distinct diagram values,
   // and multiply each by the appropriate symmetry factor
   return 0;
}
