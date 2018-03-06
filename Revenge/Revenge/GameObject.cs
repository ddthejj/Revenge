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
    public abstract class GameObject
    {
        /// <summary>
        /// Whether or not the object is active
        /// </summary>
        bool enabled;

        /// <summary>
        /// Lets the object perform actions
        /// </summary>
        public abstract void Update();

        /// <summary>
        /// Adds update to the manager
        /// </summary>
        public virtual void Activate()  
        {
            Manager.update += Update;
            enabled = true;
        }
        /// <summary>
        /// Removes update from the manager
        /// </summary>
        public virtual void Deactivate()
        {
            Manager.update -= Update;
            enabled = false;
        }

        /// <summary>
        /// Whether or not the object is active
        /// </summary>
        public bool Enabled { get { return enabled; } }
    }
}
