using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace ptut2
{
    class TerraUnary : TerraNode
    {
		public TerraNode node;

		TerraUnary(TerraNode n)
		{
			node = n;
		}

		/*public TerraNode PutAt(TerraNode node, ref Vector s, ref double a, ref Vector2 t)
		{
		  return new TerraTranslate(new TerraRotate(new TerraScale(node, s), a), t);
		}*/

	}
}
