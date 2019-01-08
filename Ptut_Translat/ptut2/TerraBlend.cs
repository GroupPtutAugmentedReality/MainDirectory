using System;
using System.Collections.Generic;
using System.Numerics;
using System.Text;

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

        int Traversal(ref Vector2 p)
        {
          int n = 1;
          if (!box666.Inside(p))
            return n;

          for (int i = 0; i<nodes.size(); i++)
          {
            n += nodes.at(i)->Traversal(p);
          }
          return n;
        }

    }
}
