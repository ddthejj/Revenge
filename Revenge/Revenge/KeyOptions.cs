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
    public static class KeyOptions
    {
        public static Keys Up = Keys.Up;
        public static Keys Down = Keys.Down;
        public static Keys Left = Keys.Left;
        public static Keys Right = Keys.Right;
        public static Keys Select = Keys.Z;
        public static Keys Back = Keys.X;
        public static Keys Menu = Keys.C;

        public static bool KeyPressed(Keys key)
        {
            if (Game1.key.IsKeyDown(key) && Game1.oldKey.IsKeyUp(key))
                return true;
            else
                return false;
        }
    }
}
