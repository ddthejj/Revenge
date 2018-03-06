using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;

namespace Revenge
{
    public abstract class Character : Block
    {
        #region Properties

        protected string name;
        protected short speed = 3;

        protected enum WayFacing { Up, Down, Left, Right }
        protected WayFacing wayFacing = WayFacing.Down;

        #endregion

        #region Constructor

        public Character(int width = 32, int height = 32, string _name = "ERROR") : base(.49f, 0, 0, width, height)
        {
            name = _name;
            //rectangle = new Rectangle(0, 0, width, height);
            //layer = .49F;
        }

        #endregion

        #region Methods

        public string Name { get { return name; } }

        public void CenterOn(Point point)
        {
            rectangle.X = point.X - rectangle.Width / 2;
            rectangle.Y = point.Y - rectangle.Height / 2;
        }

        public abstract void Move();

        public override void Draw()
        {
            Game1.spriteBatch.Draw(texture, Rectangle, null, Color.White, 0, new Vector2(0, 0), SpriteEffects.None, layer);
        }

        #endregion
    }
}
