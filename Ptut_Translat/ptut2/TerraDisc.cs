using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace ptut2
{
	class TerraDisc : TerraSkeleton
	{
		Disc2 disc; //!< Disc.
		double z;     //!< Elevation.

		public TerraDisc(/*const*/ ref Disc2 d, /*const*/ ref double z, /*const*/ ref double r, /*const*/ ref double a):base(ref r,ref a)
		{
			this.disc = d;
			this.z = z;
			//box666 = d.GetBox().Extended(r);
		}

		public ScalarAlpha ElevationAlpha(/*const*/ ref Vector2 p) /*const*/
		{
			if (!box666.Inside(p))
				return new ScalarAlpha();

			double d = disc.R(p);

			return new ScalarAlpha(z, Alpha(d));
		}




	}
}
