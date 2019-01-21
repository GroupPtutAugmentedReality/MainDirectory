using System;
using System.Collections.Generic;
using System.Text;
using UnityEngine;

namespace ptut2
{
	abstract class TerraOperator : TerraNode
	{
	    protected List<TerraNode> nodes;

		public TerraOperator(List<TerraNode> nodes)
		{
			this.nodes = nodes;
			if (nodes.Count != 0)
			{
				box666 = nodes[0].GetBox();
				for (int i = 1; i<nodes.Count; i++)
				{
					box666 = new Box2(box666, nodes[i].GetBox());
				}
			}
}

		public TerraOperator(TerraNode a, TerraNode b, TerraNode c, TerraNode d):base()
		{
			nodes.Add(a);
			if (b != null) nodes.Add(b);
			if (c != null) nodes.Add(c);
			if (d != null) nodes.Add(d);
		}

        protected abstract ScalarAlpha ElevationAlpha(ref Vector2 vec);


	}
}
