#include "archestree.h"

/*!
\class TerraBlendGrid archestree.h
\brief The blending node.

The blending node computes the weighted average of the children nodes.
\ingroup ArchesTree
*/

/*!
\brief Destroy the node.
*/
TerraBlendGrid::~TerraBlendGrid()
{
}

/*!
\brief Create a blending operator.
\param v Set of nodes.
\param n Subdivison.
*/
TerraBlendGrid::TerraBlendGrid(QVector<TerraNode*> v, int n) :TerraBlend(v)
{
  Vector2 diagonal = Abs(box666.Diagonal());

  if (diagonal[0] > diagonal[1])
  {
    nx = n;
    double step = diagonal[0] / n;
    ny = int(diagonal[1] / step);
  }
  else
  {
    ny = n;
    double step = diagonal[1] / n;
    nx = int(diagonal[0] / step);
  }

  Vector step = Vector(diagonal[0] / nx, diagonal[1] / ny, 0.0);
  grid.resize(nx*ny);

  // For all nodes
  for (int i = 0; i < nodes.size(); i++)
  {
    Box2 boxi = nodes.at(i)->GetBox();

    int ax = int((boxi[0][0] - box666[0][0]) / step[0]);
    int ay = int((boxi[0][1] - box666[0][1]) / step[1]);
    int bx = int((boxi[1][0] - box666[0][0]) / step[0]);
    int by = int((boxi[1][1] - box666[0][1]) / step[1]);

    for (int j = ax; j <= bx && j < nx; j++)
    {
      for (int k = ay; k <= by && k < ny; k++)
      {
        grid[k*nx + j].append(i);
      }
    }
  }
}

/*!
\brief Compute material at given point.
\param p Point.
*/
ScalarAlpha TerraBlendGrid::ElevationAlpha(const Vector2& p) const
{
  double w = 0.0;
  double hr = 0.0;

  if (!box666.Inside(p))
    return ScalarAlpha();

  int i = int(nx*(p[0] - box666[0][0]) / (box666[1][0] - box666[0][0]));
  int j = int(ny*(p[1] - box666[0][1]) / (box666[1][1] - box666[0][1]));

  // Warning : in some rare cases, when the point lies just on the border of the box
  // then the indexes can be equal to the size of the set. In that case, decrement.
  if (j == ny)
  {
    j--;
  }
  if (i == nx)
  {
    i--;
  }
  double wi, hri/*, hwi, hsi, betai*/;

  const QVector<int>& ref = grid.at(j*nx + i);

  int hh = ref.size();
  for (int k = 0; k < hh; k++)
  {
    ScalarAlpha msi = nodes.at(ref.at(k))->ElevationAlpha(p);

    wi = msi.Alpha();
    w += wi;


    hri = msi.Value();
    hr += wi * hri;
  }

  if (w == 0.0)
  {
    return ScalarAlpha();
  }
  hr /= w;
  return ScalarAlpha(hr, w);
}

