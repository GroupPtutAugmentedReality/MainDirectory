#include "ecosystem.h"
#include "maya.h"

/*
\class Tree ecosystem.h
\brief %Trees.
*/

/*!
\brief Create an tree.
*/
Tree::Tree(const Vector& p, int a)
{
  Tree::p = p;
  Tree::a = a;
  spawn = 10;
}


/*
\class Ecosystem ecosystem.h
\brief %Ecosystem featuring different types of plants.
*/

/*!
\brief Create an empty ecosystem.
\param box Rectangle domain of the ecosystem.
\param nx, ny Samples.
*/
Ecosystem::Ecosystem(const Box2& box, int nx, int ny) :Array2(box, nx, ny)
{

}

/*!
\brief Create an empty ecosystem.
\param box Rectangle domain of the ecosystem.
\param nx, ny Samples.
\param t Set of trees.
*/
Ecosystem::Ecosystem(const Box2& box, int nx, int ny, const QVector<Tree>& t) :Array2(box, nx, ny), trees(t)
{

}
/*!
\brief
*/
Vector Tree::Position() const
{
  return p;
}

/*!
\brief
*/
double Tree::Radius() const
{
  double b = a / 30.0;
  return 3.0*(b / sqrt(1.0 + b*b));
}

/*!
\brief
*/
double Tree::Height() const
{
  double b = a / 30.0;
  return 25.0*(b / sqrt(1.0 + b*b));
}


/*!
\brief
*/
MayaGeometryAll Ecosystem::CreateGeometry() const
{

  MayaGeometryStack mgs;

  mgs.AddGeometry(MayaGeometry::CreateCone(Vector(0, 0, 2.0), Vector(0, 0, 10.0), 2.0, 12));
  mgs.AddGeometry(MayaGeometry::CreateCylinder(Vector(0, 0, 0), Vector(0, 0, 2.0), 0.20, 12));

  MayaGeometry mg = mgs.Get();
  mg.SetMaterial(MayaMaterial::Normal);
  mg.SetName("Tree");
  MayaGeometrySet mge(mg);
  mge.SetName("Trees");

  for (int i = 0; i < trees.size(); i++)
  {
    Vector p = trees.at(i).Position();
    double r = trees.at(i).Radius();
    double h = trees.at(i).Height();

    // Create 16 instances
    mge.Append(FrameScaled(Matrix::Identity, p, Vector(h / 10.0)));
  }

  MayaGeometryAll mga(mge);

  return mga;
}


/*!
\brief
*/
bool Ecosystem::WithinRange(const Vector& p) const
{
  for (int i = 0; i < trees.size(); i++)
  {
    if (Norm(Vector2(p - trees.at(i).Position())) < Tree(Vector(0.0), 1).Radius() + trees.at(i).Radius())
    {
      return true;
    }
  }

  return false;
}

/*
\brief
*/
double Ecosystem::Condition(const Vector& p) const
{
  double c = 0.0;
  for (int i = 0; i < trees.size(); i++)
  {
    double d = SquaredNorm(Vector2(p - trees.at(i).Position()));
    double r = 2.0*trees.at(i).Radius();
    c += Cubic::SmoothCompact(d, r*r);
  }
  return 1.0 - c;
}

#include "random.h"
/*!
\brief
*/
void Ecosystem::Seed( LayerStack& t, int n)
{
  ScalarField2 v=t.GetVegetation();
  static RandomFast random;
  for (int i = 0; i < n; i++)
  {
    // Random seed inside domain
    Vector2 p = t.Array2::GetBox().RandomInside();
    double veg= v.Value(p);
    if (5.0*random.Uniform()<v.Value(p))
    {
    // Create seed tree
    Tree tree(t.Vertex(p),50); // 50 years
    trees.append(tree);
    }
  }
}

/*!
\brief
*/
void Ecosystem::Step(const HeightField& te, int n)
{
  for (int i = 0; i < n; i++)
  {
    Step(te);
  }
}

/*!
\brief
*/
void Ecosystem::Step(const HeightField& te)
{
  // Age all trees
  for (int i = 0; i < trees.size(); i++)
  {
    if (trees[i].Age() == false)
    {
      cout << "death at age=" << trees[i].GetAge() << endl;
      trees[i] = trees[trees.size() - 1];
      trees.erase(trees.end() - 1);
    }
  }

  // Growth
  /*
  for (int i=0;i<tree.size();i++)
  {
  EcosystemTree& ta=tree[i];
  for (int j=0;j<tree.size();j++)
  {
  if (j==i) continue;
  }
  }
  */

  /*
  for (int i=0;i<tree.size();i++)
  {
  tree[i].Grow();
  }
  */
  for (int i = 0; i < trees.size(); i++)
  {
    if (trees[i].CanSpawn())
    {
      Vector p = trees[i].Spawn(te);

      // Escape if not within terrain
      if (!te.Inside(Vector2(p))) continue;

      // Modify height 
      p = te.Vertex(p);

      // Proximity check
      //if (WithinRange(p)) continue;

      // Environment
      double e = ra.Uniform();
      double other = Condition(p);
      if /*(e < te.Resource(p)*other)*/ (true)
      {
        // Add new tree
        trees.append(Tree(p, 1));
      }
    }
  }

}

Random Ecosystem::ra;

Random Tree::ra;


/*!
\brief
*/
double Tree::SpawnRadius() const
{
  return 4.0*Radius();
}

/*!
\brief
*/
bool Tree::Age()
{
  a += 1;
  double death = Death();
  if (ra.Uniform() < death)
  {
    return false;
  }
  return true;
}

/*!
\brief
*/
Vector Tree::Spawn(const HeightField& te) const
{
  double e = ra.Uniform(0, 2.0*Math::Pi);
  double tt = Radius();
  double rr = ra.Uniform(SpawnRadius());
  return te.Vertex(p + (tt + rr)*Vector(cos(e), sin(e), 0.0));
}

/*!
\brief
*/
bool Tree::CanSpawn() const
{
  if (a > spawn)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/*!
\brief
*/
double Tree::Death() const
{
  return 0.01;
}
