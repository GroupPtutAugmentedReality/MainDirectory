using System;

public class TerraDisc : TerraSkeleton
{
    /*!
	\class TerraDisc archestree.h
	\brief A disc primitive.
	\ingroup ArchesTree
	*/

    /*!
    \brief Create a disc primitive.
    \param d Disc.
    \param r Radius.
    \param z Elevation.
    \param a Alpha.
    */

    public TerraDisc(/*const*/ ref Disc2 d, /*const*/ ref double z, /*const*/ ref double r, /*const*/ ref double a)
    {

        TerraSkeleton::r = r;
        TerraSkeleton::a = a;
        Disc2::disc = d;
        TerraDisc::z = z;
        box666 = d.GetBox().Extended(r);
    }



    /*!
    \brief Compute the fields.
    \param p Point.
    */
    ScalarAlpha ElevationAlpha(/*const*/ ref Vector2 p) /*const*/
    {
		if (!box666.Inside(p))
			return ScalarAlpha();

		double d = disc.R(p);

		return ScalarAlpha(z, Alpha(d));
	}

	/*!
	\brief Compute the size of a node.
	*/
	int Memory() /*const*/
	{
        return sizeof(TerraDisc);
	}

	/*!
	\brief Renders the node into a QGraphicsScene.
	\param scene The scene.
	*/

	/*void (ref QGraphicsScene scene) const
	{
		disc.Draw(scene);
		disc.Draw(scene);
		disc.Extended(r).Draw(scene);
	}*/
}


