using System;
using System.Collections.Generic;
using System.Text;

namespace ptut2.Properties
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
        public ScalarAlpha(ref double x,  ref double na){
            value = x;
            alpha = a;
        }

        /*!
        \brief Return the height value.
        */
        public double Value(){
            return value;
        }

        /*!
        \brief Return the alpha value.
        */
        public double Alpha(){
             return alpha;
        }

        /*!
        \brief Set material layer thickness.
        \param v Value.
        */
        public void ScalarAlpha(ref double v)
        {
            value = v;
        }

        /*!
        \brief Set alpha layer value.
        \param a Alpha coefficient.
        */
        public void SetAlpha(ref double a)
        {
            alpha = a;
        }


        public ScalarAlpha operator +(ref ScalarAlpha msa)
        {
            return ScalarAlpha(value + msa.value, alpha + msa.alpha);
        }

        public ScalarAlpha operator -(ref ScalarAlpha msa)
        {
            return ScalarAlpha(value - msa.value, alpha - msa.alpha);
        }




        public bool operator!=(ref ScalarAlpha msa)
        {
            if (value != msa.value)
                return true;
            else
                return false;
        }

        public bool operator==(ref ScalarAlpha msa)
        {
            if (value == msa.value)
                return true;
            else
                return false;
        }

        public ScalarAlpha operator+=(ref ScalarAlpha msa)
        {
            return ScalarAlpha(value + msa.value, alpha + msa.alpha);
        }

        public ScalarAlpha operator *(ref double a)
        {
            return ScalarAlpha(value * a, alpha * a);
        }

        public ScalarAlpha operator*=(ref double a)
        {
            return ScalarAlpha(value * a, alpha * a);
        }
    }
}
