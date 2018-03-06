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
    public class Player : Character
    {
        #region Properties

        #endregion

        #region Constructor

        public Player(string _name) : base(31, 31, _name)
        {
        }

        #endregion

        #region Methods

        /// <summary>
        /// Moves the player based on player input
        /// </summary>
        public override void Move()
        {
            KeyboardState key = Game1.key;
            KeyboardState oldKey = Game1.oldKey;

            Tile[,] currentLayer = Manager.CurrentRoom.Tiles[1];
            List<Tile> upList = new List<Tile>(), downList = new List<Tile>(), leftList = new List<Tile>(), rightList = new List<Tile>();

            #region Movement and Tile Collision

            // This sets up i and j to be the top left tile the player is in

            int i, j;
            Point IJ = SetIJ(currentLayer);
            i = IJ.X; j = IJ.Y;

            int iOffset = 0;
            int jOffset = 0;
            bool collideUp = false, collideDown = false, collideLeft = false, collideRight = false;

            
            #region Up Tiles
            // Finds the tiles above the player

            if (i != 0)
            {
                while (j + jOffset < currentLayer.GetLength(1) &&
                       currentLayer[i - 1, j + jOffset].Rectangle.Left < Manager.Player.Rectangle.Right)
                {
                    if (currentLayer[i - 1, j + jOffset].Collidable)
                        upList.Add(currentLayer[i - 1, j + jOffset]);

                    jOffset++;
                }
            }

            #endregion

            #region Move Up
            // Moves the player up

            foreach (Tile tile in upList)
            {
                Rectangle futurePlayerRectangle = new Rectangle
                    (Manager.Player.Rectangle.X, Manager.Player.Rectangle.Y - speed, Manager.Player.Rectangle.Width, Manager.Player.Rectangle.Height);

                if (futurePlayerRectangle.Intersects(tile.Rectangle))
                    collideUp = true;
            }
            if (key.IsKeyDown(KeyOptions.Up))
            {
                if (!collideUp)
                    rectangle.Y -= speed;
                else
                    rectangle.Y = upList[0].Rectangle.Bottom + 1;

                wayFacing = WayFacing.Up;
            }
            IJ = SetIJ(currentLayer);
            i = IJ.X; j = IJ.Y;

            #endregion 

            #region Down Tiles
            // Finds the tiles below the player

            iOffset = 0;

            while (i + iOffset < currentLayer.GetLength(0) - 1 &&
                  currentLayer[i + iOffset, j].Rectangle.Top < Manager.Player.Rectangle.Bottom)
            {
                iOffset++;
            }

            jOffset = 0;

            while (j + jOffset < currentLayer.GetLength(1) &&
                   currentLayer[i + iOffset, j + jOffset].Rectangle.Left < Manager.Player.Rectangle.Right)
            {
                if (currentLayer[i + iOffset, j + jOffset].Collidable)
                    downList.Add(currentLayer[i + iOffset, j + jOffset]);

                jOffset++;
            }

            #endregion

            #region Move Down
            // Moves the player down

            foreach (Tile tile in downList)
            {
                Rectangle futurePlayerRectangle = new Rectangle
                    (Manager.Player.Rectangle.X, Manager.Player.Rectangle.Y + speed, Manager.Player.Rectangle.Width, Manager.Player.Rectangle.Height);

                if (futurePlayerRectangle.Intersects(tile.Rectangle))
                    collideDown = true;
            }
            if (key.IsKeyDown(KeyOptions.Down))
            {
                if (!collideDown)
                    rectangle.Y += speed;
                else
                    rectangle.Y = downList[0].Rectangle.Top - 1 - rectangle.Height;

                wayFacing = WayFacing.Down;
            }
            IJ = SetIJ(currentLayer);
            i = IJ.X; j = IJ.Y;

            #endregion

            #region Tiles Left
            // Finds the tiles to the left of the player

            iOffset = 0; jOffset = 0;

            if (j != 0)
            {
                while (i + iOffset < currentLayer.GetLength(0) &&
                    currentLayer[i + iOffset, j - 1].Rectangle.Top < Manager.Player.Rectangle.Bottom)
                {
                    if (currentLayer[i + iOffset, j - 1].Collidable)
                        leftList.Add(currentLayer[i + iOffset, j - 1]);

                    iOffset++;
                }
            }

            #endregion

            #region Move Left
            // Moves the player left

            foreach (Tile tile in leftList)
            {
                Rectangle futurePlayerRectangle = new Rectangle
                    (Manager.Player.Rectangle.X - speed, Manager.Player.Rectangle.Y, Manager.Player.Rectangle.Width, Manager.Player.Rectangle.Height);

                if (futurePlayerRectangle.Intersects(tile.Rectangle))
                    collideLeft = true;
            }
            if (key.IsKeyDown(KeyOptions.Left))
            {
                if (!collideLeft)
                    rectangle.X -= speed;
                else
                    rectangle.X = leftList[0].Rectangle.Right + 1;

                wayFacing = WayFacing.Left;
            }
            IJ = SetIJ(currentLayer);
            i = IJ.X; j = IJ.Y;

            #endregion

            #region Right Tiles
            // Finds the tiles to the right of the player

            jOffset = 0;

            while (j + jOffset < currentLayer.GetLength(1) - 1 &&
                  currentLayer[i, j + jOffset].Rectangle.Left < Manager.Player.Rectangle.Right)
            {
                jOffset++;
            }

            iOffset = 0;

            while (i + iOffset < currentLayer.GetLength(0) &&
                   currentLayer[i + iOffset, j + jOffset].Rectangle.Top < Manager.Player.Rectangle.Bottom)
            {
                if (currentLayer[i + iOffset, j + jOffset].Collidable)
                    rightList.Add(currentLayer[i + iOffset, j + jOffset]);

                iOffset++;
            }

            #endregion

            #region Move Right
            // Moves the player right

            foreach (Tile tile in rightList)
            {
                Rectangle futurePlayerRectangle = new Rectangle
                    (Manager.Player.Rectangle.X + speed, Manager.Player.Rectangle.Y, Manager.Player.Rectangle.Width, Manager.Player.Rectangle.Height);

                if (futurePlayerRectangle.Intersects(tile.Rectangle))
                    collideRight = true;
            }
            if (key.IsKeyDown(KeyOptions.Right))
            {
                if (!collideRight)
                    rectangle.X += speed;
                else
                    rectangle.X = rightList[0].Rectangle.Left - 1 - rectangle.Width;

                wayFacing = WayFacing.Right;
            }
            #endregion

            #endregion
        }
        /// <summary>
        /// Lets the player interact with objects in front of them
        /// </summary>
        /// 
        public override void Draw()
        {
            base.Draw();
        }
        public void Interact()
        {
            if (KeyOptions.KeyPressed(KeyOptions.Interact))
            {
                Point interactPoint = new Point(rectangle.Center.X, rectangle.Center.Y);

                #region Set Interact Point

                if (wayFacing == WayFacing.Up)
                {
                    interactPoint.Y -= (rectangle.Height / 2) + 10;
                }
                else if (wayFacing == WayFacing.Down)
                {
                    interactPoint.Y += (rectangle.Height / 2) + 10;
                }
                else if (wayFacing == WayFacing.Left)
                {
                    interactPoint.X -= (rectangle.Width / 2) + 10;
                }
                else
                {
                    interactPoint.X += (rectangle.Width / 2) + 10;
                }

                #endregion

                int roomX = Manager.CurrentRoom.Tiles[1][0, 0].Rectangle.X, roomY = Manager.CurrentRoom.Tiles[1][0, 0].Rectangle.Y;
                int i = 1, j = 1;
                for (; i < Manager.CurrentRoom.Height; i++)
                    if (i * 32 + roomY >= interactPoint.Y)
                    {
                        i--;
                        break;
                    }
                for (; j < Manager.CurrentRoom.Width; j++)
                    if (j * 32 + roomX >= interactPoint.X)
                    {
                        j--;
                        break;
                    }

                Manager.Interact(i, j);
            }
        }
        /// <summary>
        /// Lets the player open the menu
        /// </summary>
        public void Menu()
        {
            if (KeyOptions.KeyPressed(KeyOptions.Menu))
                Manager.OpenMenu();
        }

        public override void Update()
        {
            Move();
            Menu();
            Interact();
            Manager.CenterRoom();
        }
        /// <summary>
        /// Finds the tile that the player is on
        /// </summary>
        /// <param name="currentLayer">The layer of tiles that the player is on</param>
        /// <returns></returns>
        private Point SetIJ(Tile[,] currentLayer)
        {
            int i = 0, j = 0;
            for (; i < currentLayer.GetLength(0) - 1; i++)
            {
                if (currentLayer[i, 0].Rectangle.Bottom > Manager.Player.Rectangle.Y)
                {
                    break;
                }
            }

            for (; j < currentLayer.GetLength(1) - 1; j++)
            {
                if (currentLayer[0, j].Rectangle.Right > Manager.Player.Rectangle.X)
                {
                    break;
                }
            }

            return new Point(i, j);
        }

        #endregion
    }
}
