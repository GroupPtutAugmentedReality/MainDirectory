using System;
using System.Collections.Generic;
using System.Text;

namespace ptut2
{
    class ScalarAlpha
    {
        protected double value; //!< Real.
        protected double alpha; //!< Alpha.
        //static const ScalarAlpha Empty = new ScalarAlpha(0.0,0.0); //!< Empty value and weight

        /*!
        \brief Empty refructor.
        */
        public ScalarAlpha(){
            value = 0.0;
            alpha = 0.0;
        }

        /*!
        \brief refructor.
        \param x Real.
        \param a Alpha.
        */
        public ScalarAlpha( double x,  double na){
            value = x;
            alpha = na;
        }

        /*!
        \brief Return the height value.
        */
        public double getValue(){
            return this.value;
        }

        /*!
        \brief Return the alpha value.
        */
        public double getAlpha(){
             return this.alpha;
        }

        /*!
        \brief Set material layer thickness.
        \param v Value.
        */
        public void SetValue(ref double v)
        {
            this.value = v;
        }

        /*!
        \brief Set alpha layer value.
        \param a Alpha coefficient.
        */
        public void SetAlpha(ref double a)
        {
            this.alpha = a;
        }


        public static ScalarAlpha operator +(ScalarAlpha ms,ScalarAlpha msa)
        {
            ScalarAlpha s = new ScalarAlpha(ms.value + msa.value, ms.alpha + msa.alpha);
            return s;
        }

        public static ScalarAlpha operator -(ScalarAlpha ms, ScalarAlpha msa)
        {
            ScalarAlpha s = new ScalarAlpha(ms.value - msa.value, ms.alpha - msa.alpha);
            return s;
        }




        public static bool operator!=( ScalarAlpha ms, ScalarAlpha msa)
        {
            if (ms.value != msa.value)
                return true;
            else
                return false;
        }

        public static bool operator==(ScalarAlpha ms, ScalarAlpha msa)
        {
            if (ms.value == msa.value)
                return true;
            else
                return false;
        }
        public static ScalarAlpha operator *(ScalarAlpha s, double a)
        {
            return new ScalarAlpha(s.value * a, s.alpha * a);
        }

    }
}
