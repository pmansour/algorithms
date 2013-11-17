using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;

namespace Project1.Fractals
{
    using SharpDX.Toolkit.Graphics;
    using Project1.Colors;

    class Tuple {
        public int i { get; set; }
        public int j { get; set; }

        public Tuple(int i, int j)
        {
            this.i = i;
            this.j = j;
        }
    }

    public interface FractalGenerator
    {
        /// <summary>
        /// Get the height range of this fractal generator.
        /// </summary>
        /// <returns></returns>
        float getRange();
        /// <summary>
        /// Get the actual height values generated (the height map).
        /// </summary>
        /// <returns></returns>
        float[,] getHeightMap();

        /// <summary>
        /// Get an array of ordered Vector3 objects, ready to be rendered on
        /// the screen.
        /// </summary>
        /// <returns></returns>
        Vector3[] getVertices();

        /// <summary>
        /// Get an array of ordered VertexPositionColor objects, ready to be
        /// rendered on the screen.
        /// </summary>
        /// <param name="gen">The color generator to use for generating colors.</param>
        /// <returns></returns>
        VertexPositionColor[] getVPCs(ColorGenerator gen);
    }

    /// <summary>
    /// Assumptions:
    ///     (1) The fractal generated will be a square of size 2^n + 1.
    ///     (2) Bottom-left corner is at (0, 0).
    /// </summary>
    public class DiamondSquare : FractalGenerator
    {

        private Random r;
        private int n, size;
        private float range;

        /// <summary>
        /// A 2D array that maps (i,j) co-ordinates to their respective height values.
        /// </summary>
        private float[,] heightMap;

        /// <summary>
        /// Constructor that takes the size of the landscape as a power of two.
        /// </summary>
        /// <param name="n">The size of the landscape will be 2^n + 1.</param>
        public DiamondSquare(int n)
        {
            // set the n to the value we've been given
            this.n = n;

            // calculate the size of the map
            size = (int) Math.Pow(2.0, n) + 1;

            // ideally, we want the height range to be less than half the size of the map
            range = ((float) size) / 6;

            // initialize the height map
            heightMap = new float[size, size];

            // initialize a new random number generator
            r = new Random();

            // generate the actual height map
            generateHeightMap();
        }

        /// <summary>
        /// Does the given tuple specify a valid (i, j) co-ordinate for the
        /// current height map?
        /// </summary>
        private bool validCoord(Tuple coord)
        {
            return  coord.i >= 0 && coord.i < size &&
                    coord.j >= 0 && coord.j < size;
        }

        /// <summary>
        /// Find the mean height from the heights at the given co-ordinates.
        /// Any invalid co-ordinates are ignored.
        /// </summary>
        /// <param name="values">The tuples specifying all the co-ordinates to consider.</param>
        /// <returns></returns>
        private float meanValue(params Tuple[] values)
        {
            float totalSum;
            int numberOfValues;

            // initialize the total to be 0
            totalSum = 0;
            numberOfValues = 0;

            // add all the heights of the given tuples
            foreach (Tuple tuple in values)
            {
                // if any of the tuple indices are invalid, ignore them
                if(!validCoord(tuple))
                {
                    continue;
                }

                // otherwise, add this height value to the total sum
                totalSum += heightMap[tuple.i, tuple.j];
                numberOfValues++;
            }

            // if there were 0 valid points, then return 0
            if (numberOfValues == 0)
            {
                return 0F;
            }

            // return the mean of these values
            return totalSum / numberOfValues;
        }

        /// <summary>
        /// Generate a random number between [-range, range].
        /// </summary>
        /// <param name="range">The range.</param>
        private float random(float range)
        {
            float min, max;

            min = -range;
            max = range;

            return min + ((float) r.NextDouble() * (max - min));
        }

        /// <summary>
        /// Generate a height value for a point based on the mean height of the
        /// given surrounding points, and a small random number within a given
        /// range.
        /// </summary>
        /// <param name="range"></param>
        /// <param name="values"></param>
        /// <returns></returns>
        private float generateValue(float range, params Tuple[] values)
        {
            float mean, rand, value;

            // find the mean of the given tuples' height values
            mean = meanValue(values);
            // generate a small random number
            rand = random(range);
            
            // add that random number to the mean value
            value = mean + rand;

            return value;
        }

        /// <summary>
        /// Generate the actual height map.
        /// 
        /// This just gives the four corners of the map random values,
        /// then calls a recursive function to fill in the rest.
        /// </summary>
        private void generateHeightMap()
        {
            // generate the first four corners of the map randomly
            heightMap[0, 0] = random(range);
            heightMap[0, size - 1] = random(range);
            heightMap[size - 1, 0] = random(range);
            heightMap[size - 1, size - 1] = random(range);

            // fill in the rest
            createPartOfTheMap(0, size - 1, size - 1, 0, range);
        }

        /// <summary>
        /// The recursive function where most of the logic occurs.
        /// 
        /// Repeatedly does the following:
        ///     (1) Reduce the range of allowed random numbers.
        ///     (2) Generate a value for the center based on the four corners.
        ///     (3) Generate values for the diagonal square around the center.
        ///     (4) Repeat for each mini-square until there are none left.
        /// </summary>
        /// <param name="left"></param>
        /// <param name="bottom"></param>
        /// <param name="right"></param>
        /// <param name="top"></param>
        /// <param name="range">The range of random numbers allowed.</param>
        private void createPartOfTheMap(int left, int bottom, int right, int top, float range)
        {
            int centerI, centerJ, halfLength;

            // find half of the length of the square
            halfLength = (right - left) / 2;
            
            // find the coordinates of the center point
            centerI = (left + right) / 2;
            centerJ = (top + bottom) / 2;

            // if the square half-length is less than 1, then stop
            if (halfLength < 1)
            {
                return;
            }

            // reduce the height range
            range /= 2;

            // the "diamond" step - use the average of the four corners for the center
            heightMap[centerI, centerJ] = generateValue(range,
                    new Tuple(left, top),
                    new Tuple(left, bottom),
                    new Tuple(right, top),
                    new Tuple(right, bottom)
                );

            // the "square" step - fill the diagonal square around the center
            heightMap[centerI, top] = generateValue(range,
                    new Tuple(left, top),
                    new Tuple(right, top),
                    new Tuple(centerI, centerJ),
                    new Tuple(centerI, top + halfLength)      // (outside the mini-square - might not exist)
                );
            heightMap[centerI, bottom] = generateValue(range,
                    new Tuple(left, bottom),
                    new Tuple(right, bottom),
                    new Tuple(centerI, centerJ),
                    new Tuple(centerI, bottom - halfLength)   // (outside the mini-square - might not exist)
                );
            heightMap[left, centerJ] = generateValue(range,
                    new Tuple(left, top),
                    new Tuple(left, bottom),
                    new Tuple(centerI, centerJ),
                    new Tuple(left - halfLength, centerJ)     // (outside the mini-square - might not exist)
                );
            heightMap[right, centerJ] = generateValue(range,
                    new Tuple(right, top),
                    new Tuple(right, bottom),
                    new Tuple(centerI, centerJ),
                    new Tuple(right + halfLength, centerJ)    // (outside the mini-square - might not exist)
                );

            // call this recursively on each mini square
            createPartOfTheMap(left, centerJ, centerI, top, range);
            createPartOfTheMap(centerI, centerJ, right, top, range);
            createPartOfTheMap(left, bottom, centerI, centerJ, range);
            createPartOfTheMap(centerI, bottom, right, centerJ, range);
        }

        /// <summary>
        /// Create a Vector3 for the point specified by a given co-ordinate.
        /// 
        /// This function will also translate the point as necessary to ensure
        /// that the landscape is symmetric about the origin.
        /// 
        /// Assuming that the height translates to the y-value.
        /// </summary>
        /// <param name="coord"></param>
        /// <returns></returns>
        private Vector3 getVector(Tuple coord)
        {
            return new Vector3(
                    coord.i - (size / 2),
                    heightMap[coord.i, coord.j],
                    coord.j - (size / 2)
                );
        }

        /// <summary>
        /// Generate the two triangles that describe the square whose bottom-left corner is the given coordinate.
        /// </summary>
        /// <param name="coord">The coordinates of the bottom-left corner of the square.</param>
        /// <returns></returns>
        private Vector3[] getTriangleVertices(Tuple coord)
        {
            // if the bottom-left coordinate is at the right or top borders, then there are no triangles here
            if(coord.i == size - 1 || coord.j == size - 1)
            {
                // return an empty array
                return new Vector3[0];
            }

            // otherwise, return vertices for the two triangles within that square
            return new Vector3[] {
                // the first (upper) triangle
                getVector(new Tuple(coord.i, coord.j)),
                getVector(new Tuple(coord.i, coord.j + 1)),
                getVector(new Tuple(coord.i + 1, coord.j + 1)),

                // the second (lower) triangle
                getVector(new Tuple(coord.i, coord.j)),
                getVector(new Tuple(coord.i + 1, coord.j + 1)),
                getVector(new Tuple(coord.i + 1, coord.j))
            };
        }

        /// <summary>
        /// Get the vertices for the flat bottom of the landscape.
        /// 
        /// Just here as a reference, but not really used.
        /// </summary>
        /// <returns></returns>
        private Vector3[] getBottom()
        {
            return new Vector3[] {
                new Vector3(size / 2, - range, - size / 2),
                new Vector3(size / 2, - range, size / 2),
                new Vector3(- size / 2, - range, size / 2),
                
                new Vector3(size / 2, - range, - size / 2),
                new Vector3(- size / 2, - range, size / 2),
                new Vector3(- size / 2, - range, - size / 2),
            };
        }

        /// <summary>
        /// Get the vertices for the flat sides of the landscape.
        /// 
        /// Just here as a reference, but not really used.
        /// </summary>
        /// <returns></returns>
        private Vector3[] getSides()
        {
            return new Vector3[] {
                new Vector3(- size / 2, - range, size / 2),
                new Vector3(- size / 2, range, size / 2),
                new Vector3(- size / 2, range, - size / 2),

                new Vector3(- size / 2, - range, size / 2),
                new Vector3(- size / 2, range, - size / 2),
                new Vector3(- size / 2, - range, - size / 2),


                new Vector3(- size / 2, - range, - size / 2),
                new Vector3(- size / 2, range, - size / 2),
                new Vector3(size / 2, range, - size / 2),

                new Vector3(- size / 2, - range, - size / 2),
                new Vector3(size / 2, range, - size / 2),
                new Vector3(size / 2, - range, - size / 2),


                new Vector3(size / 2, - range, - size / 2),
                new Vector3(size / 2, range, - size / 2),
                new Vector3(size / 2, range, size / 2),

                new Vector3(size / 2, - range, - size / 2),
                new Vector3(size / 2, range, size / 2),
                new Vector3(size / 2, - range, size / 2),


                new Vector3(size / 2, - range, size / 2),
                new Vector3(size / 2, range, size / 2),
                new Vector3(- size / 2, range, size / 2),

                new Vector3(size / 2, - range, size / 2),
                new Vector3(- size / 2, range, size / 2),
                new Vector3(- size / 2, - range, size / 2)
            };
        }

        /// <summary>
        /// Get the range of allowed height values in this fractal.
        /// </summary>
        /// <returns></returns>
        public float getRange()
        {
            return range;
        }

        /// <summary>
        /// Get the actual height map that's been generated by this algorithm.
        /// </summary>
        /// <returns></returns>
        public float[,] getHeightMap()
        {
            return heightMap;
        }

        public Vector3[] getVertices()
        {
            int index, numVertices;
            Vector3[] vertices, triangles;

            // after a lot of maths, I figured out that the number of vertices is
            // going to be v = 6 * 4^n using the initial n.
            numVertices = 6 * (int)Math.Pow(4, n);

            vertices = new Vector3[numVertices];

            // the index into the vertices array
            index = 0;

            // add the vertices for the landscape
            for (int i = 0; i < size - 1; i++)
            {
                for (int j = 0; j < size - 1; j++)
                {
                    // get the triangle vertices for the square whose bottom corner is at (i, j)
                    triangles = getTriangleVertices(new Tuple(i, j));

                    // add them to the vertices array
                    foreach (Vector3 v in triangles)
                    {
                        vertices[index] = v;
                        index++;
                    }
                }
            }

            return vertices;
        }

        public VertexPositionColor[] getVPCs(ColorGenerator gen)
        {
            Vector3[] vertices;
            VertexPositionColor[] VPCs;

            // get the vertices for the landscape
            vertices = getVertices();

            // initialize the VPCs array to that same size
            VPCs = new VertexPositionColor[vertices.Length];

            // add color to each vertex
            for (int i = 0; i < vertices.Length; i++)
            {
                VPCs[i] = new VertexPositionColor(vertices[i], gen.getColor(vertices[i].Y));
            }

            // return the VPCs array
            return VPCs;
        }
    }
}
