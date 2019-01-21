namespace ptut2
{
    class TreeArches
    {
        TerraNode bedrock;
        /*TerraNode* water; //!< Water.
        TerraNode* foam;
        TerraNode* sand;
        VegetationNode* veg_root;
         */

        public TreeArches(ref TerraNode ter)
        {
            bedrock = ter;
            /*
                water = w;
                sand = s;
                foam = f;
                veg_root = v;
             */
        }

        public ScalarAlpha GetBedrock(ref System.Numerics.Vector2  p)
        {
            return bedrock.ElevationAlpha(ref p);
        }
    }
}
