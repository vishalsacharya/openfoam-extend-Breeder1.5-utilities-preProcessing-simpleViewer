/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 1991-2005 OpenCFD Ltd.
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Class
    blockDescriptor

Description
    block descriptor. Takes the description of the block and the list
    of curved edges and creates a list of points on edges together
    with the weighting factors

    This is an edited version which also generates an FXGLGroup object
    for display

SourceFiles
    blockDescriptor.C

\*---------------------------------------------------------------------------*/

#ifndef blockDescriptor_H
#define blockDescriptor_H

#include "scalar.H"
#include "label.H"
#include "point.H"
#include "cellShape.H"
#include "scalarList.H"
#include "curvedEdgeList.H"

#include "fx.h"
#include "fx3d.h"
#include "FXGLViewer.h"
#include "FXGLCurvedLine.H"

#include "fx.h"
#include "fx3d.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class Istream;
class Ostream;

/*---------------------------------------------------------------------------*\
                           Class blockDescriptor Declaration
\*---------------------------------------------------------------------------*/

class blockDescriptor
{
    // Private data

        //- Block mesh points
        const PtrList<vector>& blockMeshPoints_;

        //- block shape
        cellShape blockShape_;

        // reference to a list of curved edges
        const curvedEdgeList& curvedEdges_;

        // block edge points
        List<List<point> > edgePoints_;

        //- block edge weighting factors
        scalarListList edgeWeights_;

        //- number of point in each direction
        Vector<label> n_;

        //- expansion ratios in all directions
        
        scalarList expRatios_;
        scalarList expand_; // note to self; don't need this; could remove

        //- name of the zone (empty string if none)
        word zoneName_;

    // Private member functions

        void makeBlockEdges();

        void setEdge(label edge, label start, label end, label dim);


public:

    // Constructors

        //- Construct from components. Optional cellSet/zone name.
        blockDescriptor
        (
            const cellShape&,
            const PtrList<vector>& blockMeshPoints,
            const curvedEdgeList& edges,
            const Vector<label>& n,
            const scalarList& expand,
            const word& zoneName = ""
        );

        //- Construct from Istream
        blockDescriptor
        (
            const PtrList<vector>& blockMeshPoints,
            const curvedEdgeList& edges,
            Istream& is
        );

        //- Clone
        autoPtr<blockDescriptor> clone() const
        {
            notImplemented("blockDescriptor::clone()");
            return autoPtr<blockDescriptor>(NULL);
        }


    // Member Functions

        // Access

        const PtrList<vector>& points() const;

        const cellShape& blockShape() const;

        const List<List<point> >& blockEdgePoints() const;

        const scalarListList& blockEdgeWeights() const;

        const Vector<label>& n() const;

        const word& zoneName() const;

        FXGLGroup * displayBlock();

        void listBlock(FXComposite *p);

    // Member Operators

        void operator=(const blockDescriptor&);


    // IOstream Operators

       friend Ostream& operator<<(Ostream&, const blockDescriptor&);
};


inline Istream& operator>>(Istream& is, blockDescriptor*)
{
    notImplemented("Istream& operator>>(Istream& is, blockDescriptor*)");
    return is;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
