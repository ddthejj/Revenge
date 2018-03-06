#define DEBUG

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
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        #region Declarations

        static Texture2D white;

        #region Essentials
        /// <summary>
        /// The manager for the graphics in the game.
        /// </summary>
        public static GraphicsDeviceManager graphics;
        /// <summary>
        /// Handles drawing things to the screen.
        /// </summary>
        public static SpriteBatch spriteBatch;
        /// <summary>
        /// The current state of the keyboard and key presses.
        /// </summary>
        public static KeyboardState key = Keyboard.GetState();
        /// <summary>
        /// The sate of the keyboard and key presses on the previous frame.
        /// </summary>
        public static KeyboardState oldKey;
        #endregion

        #region Player

        /// <summary>
        /// The player in the game, just used for testing currently
        /// </summary>
        Player testPlayer;

        #endregion

        #region Tiles

        /// <summary>
        /// An enum that defines the numbers of each of the tiles in the tile set.
        /// </summary>
        enum TileSet
        {
            GroundTest,
            WallTest,
            CeilingTest,
            DoorTest,
            InteractableTest,
            Tile_Max
        }

        /// <summary>
        /// The list of default tiles that is used to build rooms
        /// </summary>
        public static ProtoTile[] tileSet = new ProtoTile[(int)TileSet.Tile_Max];

        #endregion

        #region Rooms

        /// <summary>
        /// The initial test room, testing for collision, layering, interaction, and doors with tiles
        /// </summary>
        Room testRoom = new Room();

        #endregion

        #endregion

        #region Methods

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            graphics.PreferredBackBufferHeight = 900;
            graphics.PreferredBackBufferWidth = 900;
        }

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            white = Content.Load<Texture2D>("White");

            #region TileSet
            // Sets up the tile set for the game, maybe should be done via file io

            // This is a basic test tile for the ground.
            tileSet[(int)TileSet.GroundTest] = new ProtoTile(32, 32);
            tileSet[(int)TileSet.GroundTest].Texture = Content.Load<Texture2D>("BrownTile");
            // This is a basic test tile for collision on walls.
            tileSet[(int)TileSet.WallTest] = new ProtoTile(32, 32, true);
            tileSet[(int)TileSet.WallTest].Texture = Content.Load<Texture2D>("RedTile");
            // This is a basic test tile for the layer sorting system.
            tileSet[(int)TileSet.CeilingTest] = new ProtoTile(32, 32);
            tileSet[(int)TileSet.CeilingTest].Texture = Content.Load<Texture2D>("OrangeTile");
            // This is a basic test tile for doors.
            tileSet[(int)TileSet.DoorTest] = new ProtoTile(32, 32, false, false, true);
            tileSet[(int)TileSet.DoorTest].Texture = Content.Load<Texture2D>("GreenTile");
            // This is a basic test tile for interactability.
            tileSet[(int)TileSet.InteractableTest] = new ProtoTile(32, 32, true, true);
            tileSet[(int)TileSet.InteractableTest].Texture = Content.Load<Texture2D>("BlueTile");
            #endregion

            #region Player

            // Sets up the test player
            testPlayer = new Player("TEST");
            testPlayer.Location = new Point(100, 100);
            testPlayer.Texture = Content.Load<Texture2D>("PlayerDown1");
            testPlayer.Activate();
            // Tells the manager what the current player is
            Manager.Player = testPlayer;

            #endregion

            #region Rooms

            // Sets up the test room

            // Sets up the room with the file path
            testRoom.BuildRoom("TestRooms\\Test1", new Point(100, 100));
            testRoom.Activate();

            #endregion

            #region Manager

            //Sets up the room manager
            Manager.CenterScreen = new Point(graphics.PreferredBackBufferWidth / 2, graphics.PreferredBackBufferHeight / 2);
            Manager.CurrentRoom = testRoom;
            Manager.DialogueBox = new TextBox(new Rectangle(20, graphics.PreferredBackBufferHeight - 130, graphics.PreferredBackBufferWidth - 40, 110));
            Manager.PauseMenu = new MenuBox(new Rectangle(50, 50, 150, 250),
                                            new string[,] { { "Characters" }, { "Inventory" }, { "Equip" }, { "Exit" } },
                                            new Vector2[,] { { new Vector2(40, 35) }, { new Vector2(40, 85) }, { new Vector2(40, 135) }, { new Vector2(40, 185) } }, 
                                            new Box[,] { { null }, { null }, { null }, { null } });

            #endregion

            #region Room Statics

            Room.fadeRectangle = new Rectangle(0, 0, Manager.CenterScreen.X * 2, Manager.CenterScreen.Y * 2);
            Room.fadeTexture = white;

            #endregion

            #region Box Statics
            // Set up box statics

            Box.SetTextures = new Texture2D[] { Content.Load<Texture2D>("TextBoxCorner"), Content.Load<Texture2D>("TextBoxBorder"), white };
            Box.Font = Content.Load<SpriteFont>("OrangeKid14");
            Box.SetArrowTexture = Content.Load<Texture2D>("Arrow");

            #endregion

            base.Initialize();
        }

        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);


            // TODO: use this.Content to load your game content here
        }

        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// all content.
        /// </summary>
        protected override void UnloadContent()
        {
            // TODO: Unload any non ContentManager content here
        }

        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
#if (DEBUG)
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || key.IsKeyDown(Keys.Escape))
                this.Exit();
#endif

            key = Keyboard.GetState();

            Manager.Update();

            oldKey = key;

            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Black);

            spriteBatch.Begin(SpriteSortMode.BackToFront, BlendState.AlphaBlend);

            Manager.Draw();

            spriteBatch.End();
            // TODO: Add your drawing code here

            base.Draw(gameTime);
        }

        #endregion
    }
}
