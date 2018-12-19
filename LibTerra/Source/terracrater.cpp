#include "archestree.h"
#include "cubic.h"

/*!
\class TerraCrater archestree.h
\brief Craters.
\ingroup ArchesTree
*/

/*!
\brief Create a crater.
\param center Center.
\param radius Crest radius.
\param radius_external External radius.
\param depth Depth of the crater.
\param uplift Crest height.
*/
TerraCrater::TerraCrater(const Vector2& center, const double& radius, const double& radius_external, const double& depth, const double& uplift) :
  m_f_weight(Disc2(center, radius), radius_external, 1.0), m_noise_distanceQXXQ(radius / 5, radius, 0.5, 0.5, 3), m_noiseQXXQ((depth / 5), (radius / 3), 0.5, 0.5, 3)
{
  TerraCrater::m_center = center;
  TerraCrater::m_radius = radius;
  TerraCrater::m_radius_external = radius_external;
  TerraCrater::m_depth = depth;
  TerraCrater::m_uplift = uplift;

  box666 = m_f_weight.GetBox();
}

/*!
\brief Empty.
*/
TerraCrater::~TerraCrater()
{
}

/*!
\brief Compute the weight and the height of at a given point in the plane.
\param p Point.
*/
ScalarAlpha TerraCrater::ElevationAlpha(const Vector2& p) const
{
  if (!box666.Inside(p))
    return ScalarAlpha::Empty;

  double weight = m_f_weight.Value(p);

  if (weight == 0.0)
    return ScalarAlpha::Empty;

  weight = Cubic::Smooth(weight);

  double res_materials = XValue(p);

  return ScalarAlpha(res_materials, weight);
}

/*!
\brief Compute a value between 1 (the center) and 0 (the border).
\param pt Point.
*/
double TerraCrater::XValue(const Vector2& pt) const
{
  double d = Norm(pt - m_center);
  double diff = m_noise_distanceQXXQ.Value(pt);
  double d_modified = d + diff * 0.9;

  double z = ProfileCrater(d_modified);
  double noise = m_noiseQXXQ.Value(pt);

  double wnoise = NoiseCrater(d);
  noise = noise * wnoise;

  double z_res = z + noise;

  return z_res;
}


double TerraCrater::ProfileCrater(const double& d) const
{
  if (d <= m_radius)
  {
    double t = d / m_radius;
    return Cubic::Hermite(-m_depth, m_uplift, 0.0, (m_depth + m_uplift)*2.8)(t);
  }
  else
  {
    if (d <= m_radius_external)
    {
      double t = Linear::Affine(d, m_radius, m_radius_external);
      return Cubic::Hermite(m_uplift, 0.0, -m_uplift, 0.0)(t);
    }
    else
    {
      return 0.0;
    }
  }
}

double TerraCrater::NoiseCrater(const double& d) const
{
  double wnoise = 0.0;
  if (d >= m_radius)
  {
    wnoise = 1.0 - Linear::Affine(d, m_radius, m_radius_external);
  }
  else
  {
    wnoise = d / m_radius;
  }
  return wnoise;
}

/*!
\brief Compute the size of the node.
*/
int TerraCrater::Memory() const
{
  return sizeof(TerraCrater);
}















