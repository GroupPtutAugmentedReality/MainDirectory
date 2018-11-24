using System;

public class TerraSkeleton : TerraPrimitive
{	
	/*!
	\class TerraSkeleton archestree.h
	\brief Generic class for skeletal primitives.

	\ingroup ArchesTree
	*/

    /*!
    \param r Radius.
    \param a Alpha.
    */
    public TerraSkeleton(/*const*/ ref double r, /*const*/ ref double a) /*const*/
	{
        alpha = a;
        this.r = r;
	}


    /*!
	\brief Compute the falloff function.
	\param d Squared distance to the center.
	*/
    public double Alpha(/*const*/ ref double d) /*const*/
    {
        return alpha * Quadric::SmoothCompact(d, r * r);
    }
}
