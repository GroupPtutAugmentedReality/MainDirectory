using System;
using System.Collections.Generic;
using System.Text;
using UnityEngine;

namespace ptut2
{
	class TerraSkeleton : TerraPrimitive
	{
		protected double r; // Radius.
		protected double alpha; //!< Amplitude weight.

		public TerraSkeleton(ref double r, ref double a)
		{
			this.r = r;
			this.alpha = a;
		}
	}
}
