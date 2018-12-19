using System;
using System.Collections.Generic;
using System.Text;

namespace ptut2
{
	class TerraBinary : TerraNode
	{
		TerraNode a; //!< Left sub-tree.
		TerraNode b; //!< Right sub-tree.

		public TerraBinary(TerraNode a, TerraNode b)
		{
			this.a = a;
			this.b = b;
		}

		/*public int Traversal(ref Vector2 p)
		{
			int n = 1;
			if (!box666.Inside(p))
				return n;


			n += a.Traversal(p) + b.Traversal(p);
			return n;
		}*/

		/*public int Memory()
		{
			return sizeof(TerraBinary) + a->Memory() + b->Memory();
		}*/


}
}
