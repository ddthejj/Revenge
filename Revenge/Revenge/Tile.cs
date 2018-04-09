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
    public class Tile : Block
    {
        #region Properties

        /// <summary>
        /// Whether or not the tile has walls
        /// </summary>
        bool collidable;

        /// <summary>
        /// The order that properties get passed into the tile
        /// </summary>
        protected enum PropertiesOrder
        {
            Texture,
            Collidable,
            Max_Int
        }

        #endregion

        #region Get Set Properties

        /// <summary>
        /// Whether or not the tile is solid
        /// </summary>
        public bool Collidable { get { return collidable; } }

        #endregion

        #region Constructor

        /// <summary>
        /// Basic tile for the game
        /// </summary>
        //public Tile() { layer = 0; }
        /// <summary>
        /// Constructs a tile with a height and a width.
        /// </summary>
        /// <param name="_height">The height of the tile.</param>
        /// <param name="_width">The width of the tile.</param>
        /// <param name="_collidable">Whether or not the player can collide with the tile</param>
        public Tile(short _height = 32, short _width = 32, bool _collidable = false) : base(0f, 0, 0, _height, _width)
        { collidable = _collidable; }
        /// <summary>
        /// Constructs a tile based on a prototype tile
        /// </summary>
        /// <param name="that">The prototype tile</param>
        /// <param name="location">The location of the new tile</param>
        /// <param name="_layer">The layer to place the tile on</param>
        public Tile(ProtoTile that, Point location, float _layer) : base(new Rectangle(location.X, location.Y, that.Width, that.Height), _layer)
        { texture = that.Texture; collidable = that.Collidable; }
        /// <summary>
        /// Creates an empty tile
        /// </summary>
        /// <param name="location">The location of the empty tile</param>
        public Tile(Point location, int height = 32, int width = 32) : base(new Rectangle(location.X, location.Y, width, height), 0f) { }

        #endregion

        #region Methods

        /// <summary>
        /// Offset a tile's locatin by a given amount
        /// </summary>
        /// <param name="x">The x amount to move by</param>
        /// <param name="y">The y amount to move by</param>
        public void Offset(int x, int y)
        {
            rectangle.X -= x; rectangle.Y -= y;
        }
        /// <summary>
        /// Offsets a tile's location by a given amount
        /// </summary>
        /// <param name="offset">The amount to move a tile by</param>
        public void Offset(Point offset)
        {
            rectangle.X -= offset.X; rectangle.Y -= offset.Y;
        }

        public override void Draw()
        {
            if (texture != null)
                Game1.spriteBatch.Draw(texture, rectangle, null, Color.White, 0F, new Vector2(0, 0), SpriteEffects.None, layer);
        }

        public override void Update()
        {

        }

        #endregion
    }

    public class ProtoTile
    {
        #region Properties

        int width, height;
        bool collidable;
        bool interactable;
        bool door;
        Texture2D texture;

        #endregion

        #region Get Set Properties

        public Texture2D Texture { get { return texture; } set { texture = value; } }
        public bool Collidable { get { return collidable; } }
        public bool Interactable { get { return interactable; } }
        public bool IsDoor { get { return door; } }
        public int Width { get { return width; } }
        public int Height { get { return height; } }

        #endregion

        #region Constructor

        public ProtoTile(bool _collidable = false, bool _interactable = false, bool _door = false, short _height = 32, short _width = 32) { collidable = _collidable; interactable = _interactable; door = _door; width = _width; height = _height; }

        #endregion
    }


    public class InteractableTile : Tile
    {
        #region Properties

        string[] text;

        #endregion

        #region Get Set Properties

        public string[] Text { get { return text; } }

        #endregion

        #region Constructor

        public InteractableTile(ProtoTile that, Point location, float layer, string[] _text) : base(that, location, layer) { text = _text; }

        #endregion

    }
}
