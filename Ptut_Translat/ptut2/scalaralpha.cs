using System;
using System.Collections.Generic;
using System.Text;

namespace ptut2
{
    class ScalarAlpha
    {
        protected double value;
        protected double alpha;

        public ScalarAlpha() {
            alpha = 0.0;
            value = 0.0;
        }
        public ScalarAlpha( double x,  double a)
        {
            value = x;
            alpha = a;
        }

        public void setValue(double x)
        {
            this.value = x;
        }
        public double getValue()
        {
            return this.value;
        }
        public void setAlpha(double x)
        {
            this.alpha = x;
        }
        public double getAlpha()
        {
            return this.alpha;
        }
        public static bool operator !=(ScalarAlpha ms,ScalarAlpha msa)
        {
            if (ms.value != msa.value)
            {
                return true;
            }
            else
                return false;
        }
        public static bool operator ==( ScalarAlpha ms,ScalarAlpha msa)
        {
            if (ms.value == msa.value)
                return true;            
            else
                return false;
        }
        public static ScalarAlpha operator +(ScalarAlpha ms,ScalarAlpha msa)
        {
            return new ScalarAlpha(ms.value + msa.value, ms.alpha + msa.alpha);
        }
        public static ScalarAlpha operator -(ScalarAlpha ms, ScalarAlpha msa)
        {
            return new ScalarAlpha(ms.value - msa.value, ms.alpha - msa.alpha);
        }
        public static ScalarAlpha operator *(ScalarAlpha ms,ScalarAlpha  msa)
        {
            return new ScalarAlpha(ms.value * msa.value, ms.alpha * msa.alpha);
        }

        public override string ToString() {
            return "value : " + value + " Alpha : " + alpha;
        }

    }
}
