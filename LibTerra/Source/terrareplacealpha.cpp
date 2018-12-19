#include "archestree.h"

/*!
\class Terra2015ReplaceAlpha archestree.h
\brief A replacement operator.

This operator replaces the first primitive with the new one according to the alpha channel.
\ingroup TerrainGroup
*/

/*!
\brief Create a replace node with two children nodes.
\param a, b Argument children.
*/
Terra2015ReplaceAlpha::Terra2015ReplaceAlpha(TerraNode* a, TerraNode* b) :TerraBinary(a, b)
{
}

/*!
\brief Recursively destroys the complete sub-tree.
*/
Terra2015ReplaceAlpha::~Terra2015ReplaceAlpha()
{
}

/*!
\brief Evaluate the materials at a given point in the plane.
\param p Point.
*/
ScalarAlpha Terra2015ReplaceAlpha::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha::Empty;

  ScalarAlpha valw_base = a->ElevationAlpha(p);

  ScalarAlpha valw_i = b->ElevationAlpha(p);
  valw_base = WeightedReplace(valw_base, valw_i);


  return valw_base;
}

#include "quintic.h"

/*!
\brief Evaluate the materials at a given point in the plane.
\param p Point.
*/
ScalarAlpha Terra2015ReplaceAlpha::WeightedReplace(const ScalarAlpha& vw_a, const ScalarAlpha& vw_b) const
{
  double xa = vw_a.Value();
  double xb = vw_b.Value();
  double weight_a = vw_a.Alpha();
  double weight_b = vw_b.Alpha();

  if (weight_b == 0.0)
    return vw_a;

  if (weight_b == 1.0)
    return vw_b;

  double nt = Quintic::Smooth(weight_b);

  double res_value = Math::Lerp(xa, xb, nt);
  double res_weight = 0.0;

  res_weight = weight_a;

  return ScalarAlpha(res_value, res_weight);
}

/*!
\class Terra2015ReplaceMaxAlpha archestree.h
\brief A replacement operator.

This operator replaces the first primitive with the new one according to the alpha channel.
\ingroup TerrainGroup
*/

/*!
\brief Create a replace node with two children nodes.
\param a, b, Argument children.
*/
Terra2015ReplaceMaxAlpha::Terra2015ReplaceMaxAlpha(TerraNode* a, TerraNode* b) :TerraBinary(a, b)
{
}

/*!
\brief Recursively destroys the complete sub-tree.
*/
Terra2015ReplaceMaxAlpha::~Terra2015ReplaceMaxAlpha()
{
}

/*!
\brief Evaluate the materials at a given point in the plane.
\param p Point.
*/
ScalarAlpha Terra2015ReplaceMaxAlpha::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha::Empty;

  ScalarAlpha valw_base = a->ElevationAlpha(p);
  ScalarAlpha valw_i = b->ElevationAlpha(p);
  valw_base = WeightedReplace(valw_base, valw_i);


  return valw_base;
}

#include "quintic.h"

/*!
\brief Evaluate the materials at a given point in the plane.
\param p Point.
*/
ScalarAlpha Terra2015ReplaceMaxAlpha::WeightedReplace(const ScalarAlpha& vw_a, const ScalarAlpha& vw_b) const
{
  double xa = vw_a.Value();
  double xb = vw_b.Value();
  double weight_a = vw_a.Alpha();
  double weight_b = vw_b.Alpha();

  if (weight_b == 0.0)
    return vw_a;

  if (weight_b == 1.0)
    return vw_b;

  double nt = Quintic::Smooth(weight_b);

  return ScalarAlpha(Math::Lerp(xa, xb, nt), Math::Max(weight_a, weight_b));
}