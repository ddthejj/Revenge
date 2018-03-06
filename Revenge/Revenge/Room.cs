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
    public class Room : Block
    {
        #region Properties

        #region Fade Properties

        /// <summary>
        /// The black rectangle covering the whole screen that causes the room to look like it's fading out
        /// </summary>
        public static Rectangle fadeRectangle = new Rectangle(0, 0, Manager.CenterScreen.X * 2, Manager.CenterScreen.Y * 2);
        /// <summary>
        /// A pure white texture (maybe should be static in manager if uses become needed more than 
        /// </summary>
        public static Texture2D fadeTexture;
        /// <summary>
        /// The transparency of the fade rectangle 
        /// </summary>
        float fade = 0.0f;
        /// <summary>
        /// Whether or not the room is fading
        /// </summary>
        bool fadingOut, fadingIn;

        #endregion

        /// <summary>
        /// Information attatched to a door that tells it where to send the player 
        /// </summary>
        struct DoorInfo
        {
            /// <summary>
            /// The string that points to the room the door leads to
            /// </summary>
            public string RoomPath { get; }
            /// <summary>
            /// Where to put the player in the new room
            /// </summary>
            public Point PlayerEntry { get; }

            /// <summary>
            /// Information attatched to a door that tells it where to send the player 
            /// </summary>
            /// <param name="path">The string that points to the room the door leads to</param>
            /// <param name="entry">Where to put the player in the new room</param>
            public DoorInfo(string path, Point entry) { RoomPath = path; PlayerEntry = entry; }
        }
    
        /// <summary>
        /// The layers of tiles of a room
        /// </summary>
        Tile[][,] layers;
        /// <summary>
        /// The list of door information attatched to each of the doors in the room
        /// </summary>
        List<DoorInfo> doorInfo = new List<DoorInfo>();
        /// <summary>
        /// The array of groups of strings that are associated with the interactable objects in the room
        /// </summary>
        string[][] textInfo;

        #endregion 

        #region Constructor

        public Room() :base(0) { }

        /// <summary>
        /// Creates a room with a path to its int array and an entry point for the player
        /// </summary>
        /// <param name="roomPath">The path to the location of the room's int array</param>
        /// <param name="playerEntry">The point the player is entering the room at</param>
        public Room(string roomPath, Point playerEntry) : base(.5f)
        {
            BuildRoom(roomPath, playerEntry);
        }

        #endregion

        #region Get Set Properties

        /// <summary>
        /// The layers of tiles of a room
        /// </summary>
        public Tile[][,] Tiles { get { return layers; } }
        /// <summary>
        /// The width of the room
        /// </summary>
        public int Width { get { return layers[0].GetLength(1); } }
        /// <summary>
        /// The height of the room
        /// </summary>
        public int Height { get { return layers[0].GetLength(0); } }

        #endregion

        #region Methods

        /// <summary>
        /// Creates the room based on a path given places the player at the start of the room
        /// </summary>
        /// <param name="roomPath">The file path from Content to the room being built</param>
        /// <param name="playerEntry">The point the player will enter the room into</param>
        public void BuildRoom(string roomPath, Point playerEntry)
        {
            if (File.Exists("Content\\Levels\\" + roomPath + ".txt"))
            {
                using (StreamReader reader = new StreamReader("Content\\Levels\\" + roomPath + ".txt"))
                {
                    #region Doors

                    int numDoors = Convert.ToInt32(reader.ReadLine());
                    for (int i = 0; i < numDoors; i++)
                    {
                        string[] doorData = reader.ReadLine().Split(',');
                        doorInfo.Add(new DoorInfo(doorData[0], new Point(Convert.ToInt32(doorData[1]), Convert.ToInt32(doorData[2]))));
                    }
                    reader.ReadLine();

                    #endregion

                    #region Text

                    int numText = Convert.ToInt32(reader.ReadLine());
                    textInfo = new string[numText][];
                    for (int i = 0; i < numText; i++)
                    {
                        textInfo[i] = reader.ReadLine().Split('|');
                    }
                    reader.ReadLine();

                    #endregion

                    int width, height;
                    string[] dimensions = reader.ReadLine().Split(',');
                    reader.ReadLine();
                    width = Convert.ToInt32(dimensions[0]); height = Convert.ToInt32(dimensions[1]);
                    float layer = .75F;

                    int x = 0, y = 0;
                    layers = new Tile[3][,];

                    short doorCount = 0;
                    short textCount = 0;

                    for (int l = 0; l < 3; l++)
                    {

                        layers[l] = new Tile[width, height];
                        for (int i = 0; i < height; i++)
                        {
                            string[] line = reader.ReadLine().Split(',');

                            for (int j = 0; j < width; j++)
                            {
                                if (line[j] != "-1")
                                {
                                    if (Game1.tileSet[Convert.ToInt32(line[j])].IsDoor)
                                    {
                                        layers[l][i, j] = new Door(Game1.tileSet[Convert.ToInt32(line[j])], new Point(x, y), layer, doorInfo[doorCount].RoomPath, doorInfo[doorCount].PlayerEntry);
                                        doorCount++;
                                    }
                                    else
                                        layers[l][i, j] = new Tile(Game1.tileSet[Convert.ToInt32(line[j])], new Point(x, y), layer);

                                    if (layers[l][i, j].Interactable)
                                    {
                                        layers[l][i, j].Text = textInfo[textCount];
                                        textCount++;
                                    }
                                }
                                else
                                    layers[l][i, j] = new Tile(new Point(x, y));

                                x += 32;
                            }
                            x = 0;
                            y += 32;
                        }
                        x = 0; y = 0;
                        layer -= .25F;
                        reader.ReadLine();
                    }

                }
            }

            Manager.Player.Location = playerEntry;

        }
        /// <summary>
        /// Centers the room on the player
        /// </summary>
        public void CenterRoom()
        {
            Point offset = new Point(Manager.Player.Rectangle.X - Manager.CenterScreen.X + (Manager.Player.Rectangle.Width / 2),
                                     Manager.Player.Rectangle.Y - Manager.CenterScreen.Y + (Manager.Player.Rectangle.Height / 2));
            Manager.Player.CenterOn(Manager.CenterScreen);

            for (int l = 0; l < 3; l++)
            {
                for (int i = 0; i < layers[0].GetLength(0); i++)
                {
                    for (int j = 0; j < layers[0].GetLength(1); j++)
                    {
                        if (layers[l][i, j] != null)
                            layers[l][i, j].Offset(offset);
                    }
                }
            }
        }
        /// <summary>
        /// Centers and fades the room
        /// </summary>
        public override void Update()
        {
            CenterRoom();

            if (fadingOut)
            {
                fade += .05f;
                if (fade >= 1f)
                {
                    Manager.FadeIn();
                    fadingOut = false;
                    Manager.draw -= Draw;
                }
            }
            else if (fadingIn)
            {
                fade -= .05f;
                if (fade <= 0f)
                {
                    Manager.Player.Unfreeze();
                    fadingIn = false;
                    Manager.draw -= Draw;
                }
            }

        }
        /// <summary>
        /// Activates the room and all its tiles
        /// </summary>
        public override void Activate()
        {
            Manager.update += Update;

            foreach (Tile[,] layer in layers)
            {
                for (int i = 0; i < layer.GetLength(0); i++)
                {
                    for (int j = 0; j < layer.GetLength(1); j++)
                    {
                        if (layer[i, j] != null)
                            layer[i, j].Activate();
                    }
                }
            }
        }
        /// <summary>
        /// Deactivates the room and all its tiles
        /// </summary>
        public override void Deactivate()
        {
            base.Deactivate();
            foreach (Tile[,] layer in layers)
            {
                for (int i = 0; i < layer.GetLength(0); i++)
                {
                    for (int j = 0; j < layer.GetLength(1); j++)
                    {
                        if (layer[i, j] != null)
                            layer[i, j].Deactivate();
                    }
                }
            }
        }
        /// <summary>
        /// Fades the room out to a black screen
        /// </summary>
        public void FadeOut()
        {
            if (!fadingOut)
            {
                fadingOut = true;
                fade = 0.0f;
                Manager.draw += Draw;
            }
        }
        /// <summary>
        /// Fades the room in from a black screen
        /// </summary>
        public void FadeIn()
        {
            if (!fadingIn)
            {
                fadingIn = true;
                fade = 1.0f;
                Manager.draw += Draw;
            }
        }
        /// <summary>
        /// Draws the fade rectangle
        /// </summary>
        public override void Draw()
        {
            if (fadingIn || fadingOut)
                Game1.spriteBatch.Draw(fadeTexture, fadeRectangle, null, Color.Black * fade, 0f, new Vector2(0, 0), SpriteEffects.None, 0f);
        }
        /// <summary>
        /// Freezes the room and all its tiles
        /// </summary>
        public override void Freeze()
        {
            foreach (Tile[,] layer in layers)
            {
                foreach (Tile tile in layer)
                {
                    tile.Freeze();
                }
            }
        }
        /// <summary>
        /// Unfreezes the room and all its tiles
        /// </summary>
        public override void Unfreeze()
        {
            foreach (Tile[,] layer in layers)
            {
                foreach (Tile tile in layer)
                {
                    tile.Unfreeze();
                }
            }
        }

        #endregion
    }
}
