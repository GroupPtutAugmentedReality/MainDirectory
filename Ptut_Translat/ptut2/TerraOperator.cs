using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

namespace ptut2
{
	abstract class TerraOperator : TerraNode
	{
		List<TerraNode> nodes;

		public TerraOperator(TerraNode a, TerraNode b, TerraNode c, TerraNode d):base()
		{
			nodes.Add(a);
			if (b != null) nodes.Add(b);
			if (c != null) nodes.Add(c);
			if (d != null) nodes.Add(d);
		}
        public TerraOperator(List<TerraNode> node):base()
        {
            this.nodes = node;
        }

        protected abstract ScalarAlpha ElevationAlpha(ref Vector2 vec);


	}
}
