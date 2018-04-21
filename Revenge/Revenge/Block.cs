using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;


namespace Revenge
{
    public abstract class Block : GameObject
    {
        #region Properties

        /// <summary>
        /// The layer to draw the block on
        /// </summary>
        protected float layer;
        /// <summary>
        /// The height of the block
        /// </summary>
        //protected short height = 32;
        /// <summary>
        /// The width of the block
        /// </summary>
        //protected short width = 32;
        /// <summary>
        /// The texture of the block
        /// </summary>
        protected Texture2D texture;
        /// <summary>
        /// The rectangle of the block
        /// </summary>
        protected Rectangle rectangle;
        /// <summary>
        /// Whether or not the object is updating
        /// </summary>
        bool frozen;


        #endregion

        #region Constructor

        protected Block(Rectangle _rectangle, float _layer) { rectangle = _rectangle; layer = _layer; }
        
        protected Block(float _layer, int x = 0, int y = 0, int _height = 32, int _width = 32) { rectangle = new Rectangle(x, y, _height, _width); layer = _layer; }

        #endregion

        #region Get Set Properties

        /// <summary>
        /// The texture of the block
        /// </summary>
        public Texture2D Texture { get { return texture; } set { texture = value; } }
        /// <summary>
        /// The rectangle of the block
        /// </summary>
        public Rectangle Rectangle { get { return rectangle; } }
        /// <summary>
        /// The location of the block
        /// </summary>
        public Point Location { get { return rectangle.Location; } set { rectangle.Location = value; } }
        /// <summary>
        /// Whether or not the block is updating
        /// </summary>
        public bool Frozen { get { return frozen; } }

        #endregion

        #region Methods

        /// <summary>
        /// Draws the object
        /// </summary>
        public abstract void Draw();

        /// <summary>
        /// Adds the draw and update to the manager
        /// </summary>
        public override void Activate()
        {
            base.Activate();
            frozen = false;
            Manager.draw += Draw;
        }

        /// <summary>
        /// Removes the draw and update from the manager
        /// </summary>
        public override void Deactivate()
        {
            base.Deactivate();
            frozen = true;
            Manager.draw -= Draw;
        }

        /// <summary>
        /// Stops updating the object
        /// </summary>
        public virtual void Freeze()
        {
            frozen = true;
            Manager.update -= Update;
        }

        /// <summary>
        /// Resumes updating the object
        /// </summary>
        public virtual void Unfreeze()
        {
            frozen = false;
            Manager.update += Update;
        }

        public void CenterOn(Point point)
        {
            rectangle.X = point.X - rectangle.Width / 2;
            rectangle.Y = point.Y - rectangle.Height / 2;
        }

        #endregion
    }
}
