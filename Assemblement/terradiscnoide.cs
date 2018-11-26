using System;

public class TerraVertexNoise : TerraVertex
{
    /*!
	\class TerraVertexNoise archestree.h
	\brief Procedural noise over a disc.
	\ingroup ArchesTree
	*/

    /*!
    \brief Create a noise disc primitive.
    \param c Center.
    \param a Amplitude of the noise.
    \param lambda Fundamental wavelength.
    \param r Radius.
    \param al Alpha.
    */
    public TerraVertexNoise(/*const*/ ref Vector c, /*const*/ ref double a, /*const*/ ref double lambda, /*const*/ ref double r, /*const*/ ref double al) 
    {
        this.c = c;
        this.alpha = al;
        this.a = a;
        this.lambda = lambda;
        this.r = r;
		t0 = turbulence.At(c / lambda);
	}

    /*!
	\brief Compute material at a given point.
	\param p Point.
	*/
    ScalarAlpha ElevationAlpha(/*const*/ ref Vector2 p) /*const*/
	{
		if (!box666.Inside(p))
			return ScalarAlpha();

		double d = SquaredNorm(p - Vector2(c));

		if (d > r* r)
			return ScalarAlpha();

		double w = Alpha(d);
		double z = c[2] + a * (turbulence.At(p / lambda) - t0);

		return ScalarAlpha(z, w);
	}

	/*!
	\brief Compute the size of the node.
	*/	
	int Memory() /*const*/
	{
		return sizeof(TerraVertexNoise);
	}
}
