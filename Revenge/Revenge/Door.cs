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
    public class Door : Tile
    {
        #region Properties

        /// <summary>
        /// The string that leads to the connected room
        /// </summary>
        string destinationRoom = "TestRoom1";
        /// <summary>
        /// The point to enter the next room at
        /// </summary>
        Point entryPoint = new Point(0, 0);

        #endregion

        #region Constructor

        public Door() : base() { }
        public Door(ProtoTile that, Point location, float layer, string roomPath, Point entryPoint) : base(that, location, layer) { destinationRoom = roomPath; this.entryPoint = entryPoint; }

        #endregion

        #region Methods

        public override void Update()
        {
            base.Update();

            if (Manager.Player.Rectangle.Intersects(rectangle))
            {
                Manager.FadeOut(destinationRoom, entryPoint);
                Freeze();
            }
        }

        #endregion
    }
}
