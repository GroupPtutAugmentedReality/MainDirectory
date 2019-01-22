using System;
using System.Collections.Generic;
using System.Text;
using UnityEngine;

namespace ptut2
{
    class TerraBlend : TerraOperator
    {
       public TerraBlend(List<TerraNode> node):base(node)
        {
          
        }
        public TerraBlend(TerraNode a, TerraNode b,TerraNode c,TerraNode d) : base(a,b,c,d)
        {
      
        }
       

        protected override ScalarAlpha ElevationAlpha(ref Vector2 vec)
        {
            double w = 0.0;
            double hr = 0.0;

            double wi, hri;
            for(int i = 0; i < nodes.Count; i++)
            {
                ScalarAlpha sss = nodes[i].ElevationAlpha(ref vec);
                wi = sss.getAlpha();
                w += wi;

                hri = sss.getValue();
                hr += wi * hri;
            }

            if (w == 0.0)
            {
                return new ScalarAlpha();
            }
            hr /= w;
            //w /= nodes.size();

            return new ScalarAlpha(hr, w);
        }

        //fonction draw non nessecaire fait en c++ avec qt;

        public int Traversal(ref Vector2 p)
        {
          int n = 1;
          if (!box666.inTheBox(p.x,p.y))
            return n;

          for (int i = 0; i<nodes.Count; i++)
          {
            n += nodes[i].Traversal(ref p);
          }
          return n;
        }

    }
}
