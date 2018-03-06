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
        public static Keys Up = Keys.W;
        public static Keys Down = Keys.S;
        public static Keys Left = Keys.A;
        public static Keys Right = Keys.D;
        public static Keys Menu = Keys.E;
        public static Keys Interact = Keys.Space;

        public static bool KeyPressed(Keys key)
        {
            if (Game1.key.IsKeyDown(key) && Game1.oldKey.IsKeyUp(key))
                return true;
            else
                return false;
        }
    }
}
