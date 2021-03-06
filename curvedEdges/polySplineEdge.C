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

Description
    polySplineEdge class : representation of a spline via a polyLine

\*---------------------------------------------------------------------------*/

#include "error.H"

#include "polySplineEdge.H"
#include "BSpline.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(polySplineEdge, 0);

// Add the curvedEdge constructor functions to the hash tables
curvedEdge::addIstreamConstructorToTable<polySplineEdge>
    addPolySplineEdgeIstreamConstructorToTable_;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// intervening : returns a list of the points making up the polyLineEdge
// which describes the spline. nbetweenKnots is the number of points
// placed between each knot : this ensures that the knot locations
// are retained as a subset of the polyLine points.

// note that the points are evenly spaced in the parameter mu, not
// in real space

pointField polySplineEdge::intervening
(
    const pointField& otherknots,
    const label nbetweenKnots,
    const vector& fstend,
    const vector& sndend
)
{
    BSpline spl(knotlist(points_, start_, end_, otherknots), fstend, sndend);

    label nSize(nsize(otherknots.size(), nbetweenKnots));

    pointField ans(nSize);

    label N = spl.nKnots();
    scalar init = 1.0/(N - 1);
    scalar interval = (N - 3)/N;
    interval /= otherknots.size() + 1;
    interval /= nbetweenKnots + 1;

    ans[0] = points_[start_];

    register scalar index(init);
    for (register label i=1; i<nSize-1; i++)
    {
        index += interval;
        ans[i] = spl.realPosition(index);
    }

    ans[nSize-1] = points_[end_];

    return ans;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
polySplineEdge::polySplineEdge
(
    const PtrList<vector>& points,
    const label start,
    const label end,
    const pointField& otherknots,
    const label nInterKnots
)
:
    curvedEdge(points, start, end),
    polyLine
    (
        intervening
        (
            otherknots,
            nInterKnots,
            vector::zero,
            vector::zero
        )
    ),
    otherKnots_(otherknots)
{}


// Construct from Istream
polySplineEdge::polySplineEdge
(
    const PtrList<vector>& points,
    Istream& is
)
:
    curvedEdge(points, readLabel(is), readLabel(is)),
    polyLine(pointField(0)),
    otherKnots_(is)
{
    label nInterKnots(20);
    vector fstend(is);
    vector sndend(is);

    controlPoints_.setSize(nsize(otherKnots_.size(), nInterKnots));
    distances_.setSize(controlPoints_.size());

    controlPoints_ = intervening(otherKnots_, nInterKnots, fstend, sndend);
    calcDistances();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Return the position of a point on the curve given by
//  the parameter 0 <= lambda <= 1
vector polySplineEdge::position(const scalar mu) const
{
    return polyLine::position(mu);
}


//- Return the length of the curve
scalar polySplineEdge::length() const
{
    return polyLine::length();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
