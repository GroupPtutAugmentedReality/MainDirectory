using System;
using UnityEngine;

namespace ptut2
{
	class Box2
	{
		public double x1, y1, x2, y2;
		public Box2()
		{
			x1 = 0.0;
			x2 = 0.0;
			y1 = 0.0;
			y2 = 0.0;
		}

		public Box2(double x1, double x2, double y1, double y2)
		{
			this.x1 = x1;
			this.x2 = x2;
			this.y1 = y1;
			this.y2 = y2;
		}

		public Box2(Box2 b1, Box2 b2)
		{
				double lowestCoordX, lowestCoordY;

				lowestCoordX = Math.Min(b1.x1, b1.x2);
				lowestCoordX = Math.Min(lowestCoordX, b2.x1);
				lowestCoordX = Math.Min(lowestCoordX, b2.x2);

				lowestCoordY = Math.Min(b1.x1, b1.x2);
				lowestCoordY = Math.Min(lowestCoordY, b2.x1);
				lowestCoordY = Math.Min(lowestCoordY, b2.x2);

				double highestCoordX, highestCoordY;

				highestCoordX = Math.Max(b1.x1, b1.x2);
				highestCoordX = Math.Max(highestCoordX, b2.x1);
				highestCoordX = Math.Max(highestCoordX, b2.x2);

				highestCoordY = Math.Max(b1.x1, b1.x2);
				highestCoordY = Math.Max(highestCoordY, b2.x1);
				highestCoordY = Math.Max(highestCoordY, b2.x2);

				Box2 tmp = new Box2(lowestCoordX, lowestCoordY, highestCoordX, highestCoordY);
		}

		public bool inTheBox(double x, double y)
		{
			if (this.y1 > this.y2)
			{
				if (this.x1 > this.x2)
				{
					if (y > this.y2 && y < this.y1)
					{
						if (x > this.x2 && x < this.x1)
						{
							return true;
						}
						else
						{
							return false;
						}
					}
					else
					{
						return false;
					}
				}
				else if (this.x1 < this.x2)
				{
					if (x < this.x2 && x > this.x1)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			else if (this.y1 < this.y2)
			{
				if (this.x1 > this.x2)
				{
					if (y < this.y2 && y > this.y1)
					{
						if (x > this.x2 && x < this.x1)
						{
							return true;
						}
						else
						{
							return false;
						}
					}
					else
					{
						return false;
					}
				}
				else if (this.x1 < this.x2)
				{
					if (x < this.x2 && x > this.x1)
					{
						return true;
					}
					else
					{
						return false;
					}
				}

			}

			return true;
		}

	}

}
