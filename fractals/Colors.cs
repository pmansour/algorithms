using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using SharpDX;
using SharpDX.Toolkit;

namespace Project1.Colors
{
    using SharpDX.Toolkit.Graphics;

    using Project1.Fractals;

    public interface ColorGenerator
    {
        /// <summary>
        /// Generate a color for a given height value.
        /// </summary>
        /// <param name="height"></param>
        /// <returns></returns>
        Color getColor(float height);
    }

    /// <summary>
    /// A rule that specifies what height percentages use what color.
    /// </summary>
    class Rule
    {
        private Color color;
        private float min, max;

        /// <summary>
        /// Create a new color rule with the given minimum and maximum height requirements.
        /// </summary>
        /// <param name="min"></param>
        /// <param name="max"></param>
        /// <param name="color"></param>
        public Rule(float min, float max, Color color)
        {
            this.min = min;
            this.max = max;
            this.color = color;
        }

        /// <summary>
        /// Does this rule apply to the given percentage value?
        /// Check if it's within the [min, max] range (inclusive).
        /// </summary>
        /// <param name="percentage"></param>
        /// <returns></returns>
        public bool applies(float percentage)
        {
            return min <= percentage && percentage <= max;
        }

        /// <summary>
        /// Generate an adjusted version of the color in this rule, according
        /// to the height percentage.
        /// </summary>
        /// <param name="percentage"></param>
        /// <returns></returns>
        public Color getColor(float percentage)
        {
            return Color.AdjustSaturation(
                    color,
                    (percentage - min) / (max - min)
                );
        }
    }

    /// <summary>
    /// Classify colors based on the ratio of the height to the possible height range,
    /// according to some defined set of rules.
    /// </summary>
    public class SaturatedColorGenerator : ColorGenerator
    {
        private float range;
        private Rule[] rules;

        public SaturatedColorGenerator(FractalGenerator fractal)
        {
            // set the height range to whatever the fractal is using
            this.range = fractal.getRange();
            // setup all the rules
            setupRules();
        }

        /// <summary>
        /// Setup the rules that will be used in this color generator.
        /// </summary>
        private void setupRules()
        {
            rules = new Rule[] {
                // 0-20%: lava
                new Rule(0F, 0.20F, Color.Red),
                // 20-30%: deep ocean
                new Rule(0.20F, 0.30F, Color.DarkBlue),
                // 30-40%: ocean
                new Rule(0.30F, 0.40F, Color.Blue),
                // 40-50%: sea
                new Rule(0.40F, 0.50F, Color.SeaGreen),
                // 50-60%: sand
                new Rule(0.50F, 0.60F, Color.SandyBrown),
                // 60-70%: forests
                new Rule(0.60F, 0.70F, Color.ForestGreen),
                // 70-80%: rocks
                new Rule(0.70F, 0.80F, Color.DarkSlateGray),
                // 80-90%: mountains
                new Rule(0.80F, 0.90F, Color.Brown),
                // 90-100% mountain peaks
                new Rule(0.90F, 1F, Color.WhiteSmoke)
            };
        }

        /// <summary>
        /// Work out the ratio of the given height to the possible range of heights.
        /// 
        /// Basically a range-independent way of working out height for the purpose of colors.
        /// </summary>
        /// <param name="height"></param>
        /// <returns></returns>
        private float getPercentage(float height)
        {
            return (range + height) / (2 * range);
        }

        /// <summary>
        /// Find the most suitable color for this height value.
        /// </summary>
        /// <param name="height"></param>
        /// <returns></returns>
        public Color getColor(float height)
        {
            float percentage;

            // work out the percentage of this height w.r.t. the range
            percentage = getPercentage(height);

            // go through all the rules
            foreach (Rule r in rules)
            {
                // if one of them applies
                if (r.applies(percentage))
                {
                    // return the color it generates
                    return r.getColor(percentage);
                }
            }

            // if none of them applied (which shouldn't happen), return dark gray
            return Color.DarkGray;
        }
    }
}
