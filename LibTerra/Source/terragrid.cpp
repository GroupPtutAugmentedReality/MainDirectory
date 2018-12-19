#include "archestree.h"

/*!
\class TerraGrid archestree.h
\brief Grid operators.

\ingroup ArchesTree
*/

/*!
\brief Create a grid node.
\param bb,nxt,nyt Bounding box, x and y axis subdivison and radius of primitives.
\param neighbourt Number of neighbors
*/
TerraGrid::TerraGrid(Box2 bb, int nxt, int nyt, int neighbourt) : Array2(bb, nxt, nyt)
{
  box666 = bb;
  neighbour = neighbourt;

  gridNodes.fill(nullptr, CellSize());
}

/*!
\brief Recusively destroy the sub-trees.
*/
TerraGrid::~TerraGrid()
{
  for (int i = 0; i < gridNodes.size(); i++)
  {
    delete gridNodes[i];
  }
}


void TerraGrid::append(Vector2 p, TerraNode* n)
{
  if (box666.Inside(p)) {
    int i, j;
    CellInteger(p, i, j);
		
    if (gridNodes[(CellIndex(i, j))] != nullptr) {
      delete gridNodes[(CellIndex(i, j))];
      cout << "BUGGGSS" << endl;
    }

    gridNodes[(CellIndex(i, j))] = n;
  }
}


void TerraGrid::append(int i, int j, TerraNode* n)
{
  if (gridNodes[(CellIndex(i, j))] != nullptr) {
    delete gridNodes[(CellIndex(i, j))];
    cout << "BUGGGSS" << endl;
  }

  gridNodes[(CellIndex(i, j))] = n;
}

/*!
\brief Compute material at given point.
\param p Point.
*/
ScalarAlpha TerraGrid::ElevationAlpha(const Vector2& p) const
{

  if (!box666.Inside(p)) {
    return ScalarAlpha();
  }

  int i, j;
  CellInteger(p, i, j);

  QRect rect = AreaInteger();
  QRect env = QRect(i - neighbour, j - neighbour, 1 + 2 * neighbour, 1 + 2 * neighbour);

  double w = 0.0, hr = 0.0;
  double wi, hri;
  QRect area = rect.intersected(env);
  for (int i = area.x(); i < area.x() + area.width(); i++)
  {
    for (int j = area.y(); j < area.y() + area.height(); j++)
    {
      TerraNode* tmp = gridNodes[(CellIndex(i, j))];
      if (tmp != nullptr)
      {
        ScalarAlpha msi = tmp->ElevationAlpha(p);

        wi = msi.Alpha();
        w += wi;

        hri = msi.Value();
        hr += wi * hri;
      }
    }
  }

  if (w == 0.0)
  {
    return ScalarAlpha();
  }

  hr /= w;
  return ScalarAlpha(hr, w);
}
