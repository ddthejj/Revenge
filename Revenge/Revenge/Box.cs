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
    public abstract class Box : Block
    {
        protected class Arrow : GameObject
        {
            #region Properties
            /// <summary>
            /// The texture of the arrow
            /// </summary>
            public static Texture2D texture;
            /// <summary>
            /// The rectangle of the arrow
            /// </summary>
            Rectangle rectangle = new Rectangle(0, 0, 15, 15);
            /// <summary>
            /// The layer of the arrow
            /// </summary>
            float layer = .1f;
            /// <summary>
            /// The rotation of the texture of the arrow
            /// </summary>
            float rotation = 0f;
            /// <summary>
            /// Whether or not the arrow blinks
            /// </summary>
            bool blink = false;
            /// <summary>
            /// The transparency of the arrow currently
            /// </summary>
            float transparency = 1f;
            /// <summary>
            /// The timer for the arrow to blink
            /// </summary>
            short timer = 0;

            #endregion

            #region Get Set Properties
            
            /// <summary>
            /// Moves the arrow
            /// </summary>
            /// <param name="x">The x location to move the arrow to</param>
            /// <param name="y">The y location to move the arrow to
            /// </param>
            public void Move(int x, int y) { rectangle.Location = new Point(x, y); }

            #endregion

            #region Constructor

            /// <summary>
            /// 
            /// </summary>
            /// <param name="_rectangle">The size and location of the rectangle</param>
            /// <param name="_blink">Whether or not the arrow blinks</param>
            /// <param name="_rotation">How to rotate the arrow</param>
            public Arrow(Rectangle _rectangle, bool _blink = false, float _rotation = 0f) { rectangle = _rectangle; blink = _blink; rotation = _rotation; }

            #endregion

            #region Methods

            /// <summary>
            /// Turns the arrow on
            /// </summary>
            public override void Activate()
            {
                base.Activate();
                Manager.draw += Draw;
            }

            /// <summary>
            /// Turns the arrow off
            /// </summary>
            public override void Deactivate()
            {
                base.Deactivate();
                Manager.draw -= Draw;
            }

            /// <summary>
            /// Updates the arrow every frame
            /// </summary>
            public override void Update()
            {
                if (blink)
                {
                    timer++;
                    if (timer >= 45)
                    {
                        timer = 0;
                        if (transparency == 1f)
                            transparency = 0f;
                        else
                            transparency = 1f;
                    }
                }
            }

            /// <summary>
            /// Draws the arrow 
            /// </summary>
            public void Draw()
            {
                Game1.spriteBatch.Draw(texture, rectangle, null, Color.White * transparency, rotation, new Vector2(0, 0), SpriteEffects.None, layer);
            }

            /// <summary>
            /// Resets the arrow
            /// </summary>
            public void Reset()
            {
                timer = 0;
                transparency = 1f;
            }
            #endregion
        }

        #region Properties

        /// <summary>
        /// The texture of the corner of all boxes
        /// </summary>
        static Texture2D borderCorner;
        /// <summary>
        /// The texture of the border of all boxes
        /// </summary>
        static Texture2D borderEdge;
        /// <summary>
        /// The texture of the background of all boxes
        /// </summary>
        static Texture2D background;
        /// <summary>
        /// The font to use for all boxes
        /// </summary>
        protected static SpriteFont spriteFont;

        #endregion

        #region Constructor

        /// <summary>
        /// Constructs a box given a rectangle
        /// </summary>
        /// <param name="_rectangle">The rectangle of the box</param>
        public Box(Rectangle _rectangle) : base(_rectangle, 0.1f) { }


        #endregion

        #region Get Set Properties

        /// <summary>
        /// Sets the static textures of the box 
        /// </summary>
        public static Texture2D[] SetTextures { set { borderCorner = value[0]; borderEdge = value[1]; background = value[2]; } }
        /// <summary>
        /// Sets the static font of the box
        /// </summary>
        public static SpriteFont Font { set { spriteFont = value; } }
        /// <summary>
        /// Sets the static arrow texture
        /// </summary>
        public static Texture2D SetArrowTexture { set { Arrow.texture = value; } }

        #endregion

        #region Methods

        /// <summary>
        /// Draws the border and background of the box
        /// </summary>
        public override void Draw()
        {
            Game1.spriteBatch.Draw(background, rectangle, null, Color.Black,
               0f, new Vector2(0, 0), SpriteEffects.None, .12f);

            Game1.spriteBatch.Draw(borderEdge, new Rectangle(rectangle.Left, rectangle.Top, rectangle.Width, borderEdge.Height), null, Color.White,
                0f, new Vector2(0, 0), SpriteEffects.None, .11f);
            Game1.spriteBatch.Draw(borderEdge, new Rectangle(rectangle.Left, rectangle.Bottom - borderEdge.Height, rectangle.Width, borderEdge.Height), null, Color.White,
                (float)(Math.PI), new Vector2(borderEdge.Width, borderEdge.Width), SpriteEffects.None, .11f);
            Game1.spriteBatch.Draw(borderEdge, new Rectangle(rectangle.Left, rectangle.Top, rectangle.Height, borderEdge.Height), null, Color.White,
                (float)(3 * Math.PI / 2), new Vector2(borderEdge.Width, 0), SpriteEffects.None, .11f);
            Game1.spriteBatch.Draw(borderEdge, new Rectangle(rectangle.Right - borderEdge.Height, rectangle.Top, rectangle.Height, borderEdge.Height), null, Color.White,
                (float)(Math.PI / 2), new Vector2(0, borderEdge.Width), SpriteEffects.None, .11f);

            Game1.spriteBatch.Draw(borderCorner, new Rectangle(rectangle.Left, rectangle.Top, borderCorner.Width, borderCorner.Height), null, Color.White,
                (float)0, new Vector2(0, 0), SpriteEffects.None, .1f);
            Game1.spriteBatch.Draw(borderCorner, new Rectangle(rectangle.Right - borderCorner.Width, rectangle.Top, borderCorner.Width, borderCorner.Height), null, Color.White,
                (float)(Math.PI / 2), new Vector2(0, borderCorner.Width), SpriteEffects.None, .1f);
            Game1.spriteBatch.Draw(borderCorner, new Rectangle(rectangle.Left, rectangle.Bottom - borderCorner.Height, borderCorner.Width, borderCorner.Height), null, Color.White,
                (float)(3 * Math.PI / 2), new Vector2(borderCorner.Width, 0), SpriteEffects.None, .1f);
            Game1.spriteBatch.Draw(borderCorner, new Rectangle(rectangle.Right - borderCorner.Width, rectangle.Bottom - borderCorner.Height, borderCorner.Width, borderCorner.Height), null, Color.White,
                (float)(Math.PI), new Vector2(borderCorner.Width, borderCorner.Width), SpriteEffects.None, .1f);
        }

        #endregion
    }
}
