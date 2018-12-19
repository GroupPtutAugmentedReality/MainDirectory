
#include "archestree.h"

/*!
\class TriangleStack archestree.h
\brief A terrain represented as triangle irregular network.

This class stores a set of samples recording the different layers of materials for every sample.
*/

/*!
\brief Empty.
*/
TriangleStack::TriangleStack()
{
}

/*!
\brief Create a triangle irregular network.
*/
TriangleStack::TriangleStack(const QVector<Vector2>& points, const QVector<ScalarAlpha>& stack, const QVector<int>& triangles)
{
  TriangleStack::points = points;
  TriangleStack::stack = stack;
  TriangleStack::triangles = triangles;
}

/*!
\brief
*/
double TriangleStack::Height(int i) const
{
  return stack.at(i).Value();
}
/*!
\brief
*/
Vector TriangleStack::Vertex(int i) const
{
  return points.at(i).ToVector(stack.at(i).Value());
}

/*!
\brief
*/
Vector TriangleStack::TerrainPos(int i) const
{
  return points.at(i).ToVector(stack.at(i).Value());
}


/*!
\brief
*/
void TriangleStack::AddVertex(const Vector2& p, const ScalarAlpha& m)
{
  points.append(p);
  stack.append(m);
}
/*!
\brief
*/
void TriangleStack::AddTriangle(int a, int b, int c)
{
  triangles.append(a);
  triangles.append(b);
  triangles.append(c);
}
/*!
\brief
*/
Box TriangleStack::GetBox() const
{
  Box b(Vertex(0));
  for (int i = 1; i < stack.size(); i++)
  {
    b.Extend(Vertex(i));
  }
  return b;
}

/*!
\brief
*/
MayaGeometry TriangleStack::CreateGeometry(const QString& name) const
{
  QVector<Vector> p;
  p.reserve(stack.size());

  QVector<Vector> n;
  n.resize(stack.size());
  n.fill(Vector(0.0, 0.0, 0.0));

  QVector<MayaIndexVertexData> indexes;
  indexes.reserve(triangles.size() * 3);

  // Vertices
  for (int i = 0; i < stack.size(); i++)
  {
    p.append(Vertex(i));
  }
  // Normals
  for (int i = 0; i < triangles.size(); i += 3)
  {
    int a = triangles.at(i);
    int b = triangles.at(i + 1);
    int c = triangles.at(i + 2);
    Vector normal = Triangle(Vertex(a), Vertex(b), Vertex(c)).AreaNormal();
    n[a] += normal;
    n[b] += normal;
    n[c] += normal;
  }
  for (int i = 0; i < n.size(); i++)
  {
    Normalize(n[i]);
  }
  // Indexes
  for (int i = 0; i < triangles.size(); i++)
  {
    MayaIndexVertexData mi1(triangles.at(i), triangles.at(i));
    indexes.append(mi1);
  }

  MayaMaterial mo = MayaMaterial(ShaderTerrainIGN, Color(0.3, 0.3, 0.3, 1.0), Color(0.5, 0.4, 0.2, 1.0), Color(0.1, 0.1, 0.1, 1.0), 50.0);
  MayaGeometry mg(name, p, n, indexes, mo);

  return mg;
}

/*!
\brief 
*/
MayaGeometryAll TriangleStack::CreateGeometryTerrain(ArchesTree* arches, const QString& name) const
{
  MayaMaterial moTerrain = MayaMaterial(ShaderNormalWireframe, Color(0.3, 0.3, 0.3, 1.0), Color(0.5, 0.4, 0.2, 1.0), Color(0.1, 0.1, 0.1, 1.0), 50.0);
  MayaMaterial moWater = MayaMaterial(ShaderPhong, Color(0.6, 0.6, 0.8, 1.0), Color(0.5, 0.5, 0.8, 1.0), Color(0.1, 0.1, 0.1, 1.0), 50.0);

  MayaGeometryAll mga;
  MayaGeometry mgTerrain("Terrain", moTerrain);
  MayaGeometry mgWater("Water", moWater);

  QVector<Vector> triangleTerrain;
  QVector<Vector> triangleWater;

  for (int i = 0; i < triangles.size(); i += 3)
  {
    int a = triangles.at(i);
    int b = triangles.at(i + 1);
    int c = triangles.at(i + 2);

    arches->GetTriangles(Vertex(a), Vertex(b), Vertex(c), triangleTerrain, triangleWater);
  }

  for (int i = 0; i < triangleTerrain.size(); i = i + 3)
  {
    mgTerrain.AddTriangle(triangleTerrain[i], triangleTerrain[i + 1], triangleTerrain[i + 2]);
  }

  for (int i = 0; i < triangleWater.size(); i = i + 3)
  {
    mgWater.AddTriangle(triangleWater[i], triangleWater[i + 1], triangleWater[i + 2]);
  }

  mga.Append(mgTerrain);
  mga.Append(mgWater);

  return mga;
}

/*!
\brief
*/
MayaGeometry TriangleStack::CreateGeometryWater(const QString& name) const
{
  QVector<Vector> p;
  p.reserve(stack.size());

  QVector<Vector> n;
  n.resize(stack.size());
  n.fill(Vector(0.0, 0.0, 0.0));

  QVector<MayaIndexVertexData> indexes;
  indexes.reserve(triangles.size() * 6);
  /*
    // Vertices
    for (int i = 0; i < stack.size(); i++)
    {
      Vector pt1 = stack.at(i).GetValue();
      Vector pt2 = stack.at(i).GetValue();

      if (pt2[2] - pt1[2] > 0)
      {
        cout << "pt1 " << pt1 << endl;
        cout << "pt2 " << pt2 << endl;
        cout << endl;
      }


      p.append(pt1);
    }
    // Normals
    for (int i = 0; i < triangles.size(); i += 3)
    {
      int a = triangles.at(i);
      int b = triangles.at(i + 1);
      int c = triangles.at(i + 2);
      Vector normal = Triangle(Vertex(a), Vertex(b), Vertex(c)).AreaNormal();
      n[a] += normal;
      n[b] += normal;
      n[c] += normal;
    }
    for (int i = 0; i < n.size(); i++)
    {
      Normalize(n[i]);
    }
    // Indexes
    for (int i = 0; i < triangles.size(); i++)
    {
      MayaIndexVertexData mi1(triangles.at(i), triangles.at(i));
      indexes.append(mi1);
    }
    */
  MayaMaterial mo(ShaderPhong, Color(0.3, 0.3, 0.8, 1.0), Color(0.2, 0.2, 0.8, 1.0), Color(0.1, 0.1, 0.1, 1.0), 50.0);
  MayaGeometry mg(name, p, n, indexes, mo);

  return mg;
}


/*!
\brief
*/
inline int Next(int k)
{
  return (k - (k % 3)) + ((k % 3) + 1) % 3;
}

/*!
\brief
*/
MayaGeometry TriangleStack::CreateBorder(const QString& name, const double& z) const
{
  // Array of edges
  QVector<int> edges;
  edges.reserve(triangles.size() * 2);

  for (int i = 0; i < triangles.size(); i++)
  {
    // Analyse edge [ab]
    int a = triangles.at(i);
    int b = triangles.at(Next(i));
    bool detected = false;
    for (int k = i + 1; k < triangles.size(); k++)
    {
      if ((edges.at(k) == b) && (edges.at(Next(k)) == a))
      {
        detected = true;
        break;
      }
    }
    if (detected == false)
    {
      edges.append(a);
      edges.append(b);
    }
  }

  MayaMaterial mo = MayaMaterial(ShaderTerrainIGN, Color(0.3, 0.3, 0.3, 1.0), Color(0.5, 0.4, 0.2, 1.0), Color(0.1, 0.1, 0.1, 1.0), 50.0);
  MayaGeometry mg(name, mo);


  for (int i = 0; i < edges.size(); i += 2)
  {
    // Vertices
    Vector a = Vertex(edges.at(i));
    Vector b = a;
    b[2] = z;
    Vector c = Vertex(edges.at(i + 1));
    Vector d = c;
    d[2] = z;
    mg.AddQuadrangle(a, b, d, c);
  }

  return mg;
}
