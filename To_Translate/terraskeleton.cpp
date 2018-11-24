#include "archestree.h"

/*!
\class TerraSkeleton archestree.h
\brief Generic class for skeletal primitives.

\ingroup ArchesTree
*/

/*!
\param r Radius.
\param a Alpha.
*/
TerraSkeleton::TerraSkeleton(const double& r, const double& a) :alpha(a), r(r)
{
}

/*!
\brief Compute the falloff function.
\param d Squared distance to the center.
*/
double TerraSkeleton::Alpha(const double& d) const
{
  return alpha * Quadric::SmoothCompact(d, r*r);
}
