/*******************************************************************************
Grid physics library, www.github.com/paboyle/Grid 

Source file: programs/Hadrons/SRBPrecCG.cc

Copyright (C) 2016

Author: Antonin Portelli <antonin.portelli@me.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

See the full license in the file "LICENSE" in the top level distribution 
directory.
*******************************************************************************/

#include <Hadrons/SRBPrecCG.hpp>

using namespace Grid;
using namespace QCD;
using namespace Hadrons;

/******************************************************************************
*                       SRBPrecCG implementation                              *
******************************************************************************/
// constructor /////////////////////////////////////////////////////////////////
SRBPrecCG::SRBPrecCG(const std::string name)
: Module(name)
{}

// parse parameters
void SRBPrecCG::parseParameters(XmlReader &reader, const std::string name)
{
   read(reader, name, par_);
}

// dependency relation
std::vector<std::string> SRBPrecCG::getInput(void)
{
    return std::vector<std::string>();
}

std::vector<std::string> SRBPrecCG::getOutput(void)
{
    std::vector<std::string> out = {getName()};
    
    return out;
}

// execution ///////////////////////////////////////////////////////////////////
void SRBPrecCG::execute(Environment &env)
{
    auto &mat   = *(env.getFermionAction(par_.action)->getFMat());
    auto solver = [&mat, this](LatticeFermion &sol,
                               const LatticeFermion &source)
    {
        ConjugateGradient<LatticeFermion>           cg(par_.residual, 10000);
        SchurRedBlackDiagMooeeSolve<LatticeFermion> schurSolver(cg);
        
        schurSolver(mat, source, sol);
    };
    
    env.addSolver(getName(), solver, par_.action);
}
