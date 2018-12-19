#include "archestree.h"

/*!
\class TerraReplace archestree.h
\brief A replacement operator.

This operator replaces the first primitive with the other according to the alpha channel.
\ingroup ArchesTree
*/

/*!
\brief Create a replacement operator.
\param a, b Argument nodes.
*/
TerraReplace::TerraReplace(TerraNode* a, TerraNode* b) :TerraBinary(a, b)
{
}

/*!
\copydoc TerraNode::ElevationAlpha
*/
ScalarAlpha TerraReplace::ElevationAlpha(const Vector2& p) const
{
  double w = 0.0;
  double hr = 0.0;
  double hw = 0.0;
  double hs = 0.0;
  double beta = 0.0;

  if (!box666.Inside(p))
    return ScalarAlpha();

  ScalarAlpha msaa = a->ElevationAlpha(p);
  ScalarAlpha msab = b->ElevationAlpha(p);

  double wa = msaa.Alpha();
  double wb = msab.Alpha();

  w = wb * wb + (1.0 - wb)*wa;
  double hra = msaa.Value();
  double hrb = msab.Value();
  hr = (1.0 - wb)*hra + wb * hrb;

  return ScalarAlpha(hr, w);
}

/*!
\brief Create a list of replacement nodes.
\param v %Array of sub-trees.
*/
TerraNode* TerraReplace::ReplaceArray(const QVector<TerraNode*>& v)
{
  if (v.size() == 1)
    return v.at(0);
  TerraNode* node = v.at(0);
  for (int i = 1; i < v.size(); i++)
  {
    node = new TerraReplace(node, v.at(i));
  }
  return node;
}



/*!
\class Terra2015Replace archestree.h
\brief A replacement operator.

This operator replaces the first primitive with the other according to the alpha channel.
\ingroup ArchesTree
*/

/*!
\brief Create a replacement operator.
\param a, b Argument nodes.
*/
Terra2015Replace::Terra2015Replace(TerraNode* a, TerraNode* b) :TerraBinary(a, b)
{
}

#include "quintic.h"

/*!
\brief Evaluate the fields at a given point in the plane.
\param p Point.
*/
ScalarAlpha Terra2015Replace::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha::Empty;

  ScalarAlpha vw_a = a->ElevationAlpha(p);

  ScalarAlpha vw_b = b->ElevationAlpha(p);

  double value_a = vw_a.Value();
  double value_b = vw_b.Value();
  double weight_a = vw_a.Alpha();
  double weight_b = vw_b.Alpha();

  if (weight_b == 0.0)
    return vw_a;

  if (weight_b == 1.0)
    return vw_b;

  double res_value = Math::Lerp(value_a, value_b, Quintic::Smooth(weight_b));
  double res_weight = weight_a;

  return ScalarAlpha(res_value, res_weight);
}

/*!
\brief Create a list of replacement nodes.
\param v %Array of sub-trees.
*/
TerraNode* Terra2015Replace::ReplaceArray(const QVector<TerraNode*>& v)
{
  if (v.size() == 1)
    return v.at(0);

  TerraNode* node = v.at(0);
  for (int i = 1; i < v.size(); i++)
  {
    node = new Terra2015Replace(node, v.at(i));
  }
  return node;
}



