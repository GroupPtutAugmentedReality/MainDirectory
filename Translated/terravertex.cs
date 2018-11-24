using System;



public class TerraVertex : TerraSkeleton
{
    /*!
	\class TerraVertex archestree.h
	\brief A disc primitive.
	\ingroup ArchesTree
	*/

    /*!
    \brief Create a disc primitive.
    \param c Center.
    \param r Radius.
    \param a Alpha.
    */

    public TerraVertex(/*const*/ ref Vector c, /*const*/ ref double r, /*const*/ ref double a)
    {
        r = r;
        alpha = a;
        this.c = c;
        box666 = Box2(c, r);
    }

    /*!
	\brief Compute the fields.
	\param p Point.
	*/
    ScalarAlpha ElevationAlpha(/*const*/ ref Vector2 p) /*const*/
    {
		if (!box666.Inside(p))
			return ScalarAlpha();

		double d = SquaredNorm(p - Vector2(c));

		return ScalarAlpha(c[2], Alpha(d));
}

	/*!
	\brief Compute the size of a node.
	*/
	int Memory() /*const*/
	{
		return sizeof(TerraVertex);
	}

	/*!
	\brief Renders the node into a QGraphicsScene.
	\param scene The scene.
	*/
	/*void Draw(ref QGraphicsScene scene) const
	{
		QPen pen(QColor(200, 200, 200), 0.5);

		Circle2(c, r).Draw(scene, pen);
	}*/
}
