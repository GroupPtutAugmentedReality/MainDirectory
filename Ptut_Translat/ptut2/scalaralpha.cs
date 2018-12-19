using System;
using System.Collections.Generic;
using System.Text;

namespace ptut2
{
    class Scalaralpha
    {
        protected double value;
        protected double alpha;

        public Scalaralpha() {
            alpha = 0.0;
            value = 0.0;
        }
        public Scalaralpha( double x,  double a)
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
        public static bool operator !=(Scalaralpha ms,Scalaralpha msa)
        {
            if (ms.value != msa.value)
            {
                return true;
            }
            else
                return false;
        }
        public static bool operator ==( Scalaralpha ms,Scalaralpha msa)
        {
            if (ms.value == msa.value)
                return true;            
            else
                return false;
        }
        public static Scalaralpha operator +(Scalaralpha ms,Scalaralpha msa)
        {
            return new Scalaralpha(ms.value + msa.value, ms.alpha + msa.alpha);
        }
        public static Scalaralpha operator -(Scalaralpha ms, Scalaralpha msa)
        {
            return new Scalaralpha(ms.value - msa.value, ms.alpha - msa.alpha);
        }
        public static Scalaralpha operator *(Scalaralpha ms,Scalaralpha  msa)
        {
            return new Scalaralpha(ms.value * msa.value, ms.alpha * msa.alpha);
        }

        public override string ToString() {
            return "value : " + value + " Alpha : " + alpha;
        }

    }
}
