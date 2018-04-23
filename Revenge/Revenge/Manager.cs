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
    public static class Manager
    {
        #region Event Management

        public delegate void VoidDelegate();

        /// <summary>
        /// Holds all the drawing events
        /// </summary>
        public static event VoidDelegate draw;
        /// <summary>
        /// Holds all the updating events
        /// </summary>
        public static event VoidDelegate update;

        /// <summary>
        /// Draw all the active elements in the game
        /// </summary>
        public static void Draw()
        {
            if (draw != null)
                draw();
        }
        /// <summary>
        /// Update all the active and unfrozen elements in the game
        /// </summary>
        public static void Update()
        {
            if (update != null)
                update();
        }

        #endregion

        #region Properties

        /// <summary>
        /// The current player character
        /// </summary>
        static Player player = null;
        /// <summary>
        /// The center of the screen
        /// </summary>
        static Point centerScreen;
        /// <summary>
        /// The current room the player is in
        /// </summary>
        static Room currentRoom = null;
        /// <summary>
        /// The box at the bottom of the screen where text will be displayed
        /// </summary>
        static TextBox dialogueBox;
        /// <summary>
        /// The menus that appear when the player pauses
        /// </summary>
        static MenuBox[] pauseMenus;
        /// <summary>
        /// The order of the menus
        /// </summary>
        public enum MenuOrder
        {
            MainPause,
            CharacterMenu,
            Inventory,
            Options,
            CharacterDetails,
            MaxInt
        }

        /// <summary>
        /// The current group of characters 
        /// </summary>
        static List<Character> party;
        /// <summary>
        /// The inventory of the party
        /// </summary>
        static List<Item> inventory = new List<Item>();

        #region Door Properties

        /// <summary>
        /// The point the player enters a room at when going throgh a door
        /// </summary>
        static Point playerEntry;
        /// <summary>
        /// The file path to get the room from
        /// </summary>
        static string roomPath;

        #endregion

        #endregion

        #region Get Set Properties

        /// <summary>
        /// The current player character
        /// </summary>
        public static Player Player { get { return player; } }
        /// <summary>
        /// The center of the screen
        /// </summary>
        public static Point CenterScreen { get { return centerScreen; } }
        /// <summary>
        /// Centers the current room on the player
        /// </summary>
        public static void CenterRoom() { currentRoom.CenterRoom(); }
        /// <summary>
        /// The current room being displayed
        /// </summary>
        public static Room CurrentRoom { get { return currentRoom; } }
        /// <summary>
        /// The box at the bottom of the screen where text will be displayed
        /// </summary>
        public static TextBox DialogueBox { get { return dialogueBox; } }
        /// <summary>
        /// The menu that appears when the player pauses
        /// </summary>
        public static MenuBox PauseMenu { get { return pauseMenus[(int)MenuOrder.MainPause]; } }
        /// <summary>
        /// The current group of characters
        /// </summary>
        public static List<Character> Party { get { return Party; } }
        /// <summary>
        /// The inventory of the party
        /// </summary>
        public static List<Item> Inventory { get { return inventory; } }
        #endregion

        #region Methods

        /// <summary>
        /// Initialize the manager
        /// </summary>
        public static void Init()
        {

            #region Player
            // Sets up the test player

            /// <summary>
            /// The player in the game, just used for testing currently
            /// </summary>
            Player testPlayer = new Player("TEST");
            testPlayer.Location = new Point(100, 100);
            testPlayer.Texture = Game1.ContentManager.Load<Texture2D>("PlayerDown1");
            testPlayer.Activate();
            player = testPlayer;

            party = new List<Character>() { testPlayer, new Player("JOHN CENA") };

            #endregion

            // Sets up the test room

            /// <summary>
            /// The initial test room, testing for collision, layering, interaction, and doors with tiles
            /// </summary>
            Room testRoom = new Room();
            testRoom.BuildRoom("TestRooms\\Test1", new Point(100, 100));
            testRoom.Activate();
            currentRoom = testRoom;
            centerScreen = new Point(Game1.graphics.PreferredBackBufferWidth / 2, Game1.graphics.PreferredBackBufferHeight / 2);

            dialogueBox = new TextBox(new Rectangle(20, Game1.graphics.PreferredBackBufferHeight - 130, Game1.graphics.PreferredBackBufferWidth - 40, 110));

            pauseMenus = new MenuBox[(int)MenuOrder.MaxInt];

            pauseMenus[(int)MenuOrder.MainPause] = new MenuBox(new Rectangle(50, 50, 150, 250),
                                    new string[,] { { "Characters" }, { "Inventory" }, { "Equip" }, { "Exit" } },
                                    new Vector2[,] { { new Vector2(40, 35) }, { new Vector2(40, 85) }, { new Vector2(40, 135) }, { new Vector2(40, 185) } },
                                    new int[,] {
                                                { (int) MenuOrder.CharacterMenu },
                                                { (int) MenuOrder.Inventory },
                                                { -1 },
                                                { -1 }
                                               }
                                    );
            pauseMenus[(int)MenuOrder.CharacterMenu] = new MenuBox(MenuBox.BoxType.CharactersMenu);
            pauseMenus[(int)MenuOrder.Inventory] = new MenuBox(MenuBox.BoxType.InventoryMenu);
            pauseMenus[(int)MenuOrder.CharacterDetails] = new MenuBox();
            //pauseMenus[(int)MenuOrder.Inventory] 
        }

        /// <summary>
        /// Moves the current player
        /// </summary>
        /// <param name="location"></param>
        public static void MovePlayer(Point location)
        {
            player.Location = location;
        }

        #region Fade

        /// <summary>
        /// Starts the fading out of the current room and will switch rooms when done
        /// </summary>
        /// <param name="newRoomPath">The path of the room that the player is entering (provided by the door the player collides with)</param>
        /// <param name="entryPoint">The point the player will enter the new room (provided by the door the player collides with)</param>
        public static void FadeOut(string newRoomPath, Point entryPoint)
        {
            player.Freeze();
            currentRoom.FadeOut();
            roomPath = newRoomPath;
            playerEntry = entryPoint;
        }
        /// <summary>
        /// Fades in the new room. Automatically called by a room when it is moved into.
        /// </summary>
        public static void FadeIn()
        {
            currentRoom.Deactivate();
            Room newRoom = new Room(roomPath, playerEntry);
            newRoom.Activate();
            currentRoom = newRoom;
            CenterRoom();
            currentRoom.FadeIn();
        }

        #endregion

        #region Interact

        /// <summary>
        /// Interacts with the tile specified
        /// </summary>
        /// <param name="i">The x location of the tile</param>
        /// <param name="j">The y location of the tile</param>
        public static void Interact(int i, int j)
        {
            if (currentRoom.Tiles[1][i, j] is InteractableTile)
            {
                InteractableTile tile = currentRoom.Tiles[1][i, j] as InteractableTile;
                WriteText(tile.Text);
            }
        }
        /// <summary>
        /// Writes the text given in a textbox as dialogue
        /// </summary>
        /// <param name="text">The array of strings to write</param>
        public static void WriteText(string[] text)
        {
            player.Freeze();
            currentRoom.Freeze();

            dialogueBox.Start(text);
        }
        /// <summary>
        /// Completes the dialogue sequence
        /// </summary>
        public static void CompleteText()
        {
            player.Unfreeze();
            currentRoom.Unfreeze();

            dialogueBox.End();
        }

        #endregion

        #region Menuing

        /// <summary>
        /// Opens the pause menu
        /// </summary>
        public static void OpenMenu()
        {
            player.Freeze();
            currentRoom.Freeze();

            pauseMenus[(int)MenuOrder.MainPause].Activate();
        }
        /// <summary>
        /// Closes the pause menu
        /// </summary>
        public static void CloseMenu(int previousMenu = -1, bool closeAllMenus = false)
        {
            if (previousMenu != -1)
            {
                pauseMenus[previousMenu].Unfreeze();
                if (closeAllMenus)
                    pauseMenus[previousMenu].CheckClose();
            }
            else
            {
                pauseMenus[(int)MenuOrder.MainPause].Deactivate();

                player.Unfreeze();
                currentRoom.Unfreeze();
            }
        }
        /// <summary>
        /// Opens a menu based on the previous menu and an int
        /// </summary>
        /// <param name="selection">The option selected from the previous menu</param>
        /// <param name="boxType">The type of menu box calling the function</param>
        public static void NextMenu(int selection, MenuBox.BoxType boxType)
        {
            if (boxType == MenuBox.BoxType.BaseMenu)
            {
                if (selection == (int)MenuOrder.CharacterMenu)
                {
                    string[,] characters = new string[party.Count(), 1];
                    Vector2[,] locations = new Vector2[party.Count(), 1];
                    int[,] selections = new int[party.Count() + 1, 1];

                    for (int i = 0; i < party.Count(); i++)
                    {
                        characters[i, 0] = party[i].Name;
                        locations[i, 0] = new Vector2(40, 35 + (50 * i));
                        selections[i, 0] = i;
                    }

                    Rectangle rectangle = new Rectangle(pauseMenus[(int)MenuOrder.MainPause].Rectangle.Right, pauseMenus[(int)MenuOrder.MainPause].Rectangle.Top, 150, (50 + (50 * party.Count())));

                    pauseMenus[(int)MenuOrder.CharacterMenu].Init(rectangle, characters, locations, selections, (int)MenuOrder.MainPause);
                }
                else if (selection == (int)MenuOrder.Inventory)
                {
                    string[] itemNamesRaw = new string[inventory.Count()];
                    for (int i = 0; i < itemNamesRaw.Length; i++)
                    {
                        itemNamesRaw[i] = inventory[i].Name;
                    }

                    int itemsPerColumn = (int)Math.Ceiling((double)itemNamesRaw.Length / 3.0);

                    string[,] itemNames = new string[itemsPerColumn, 3];
                    Vector2[,] itemLocations = new Vector2[itemsPerColumn, 3];

                    int row = 0;
                    for (int i = 0; i < itemNamesRaw.Length; i++)
                    {
                        int column = i % 3;

                        itemNames[row, column] = itemNamesRaw[i];
                        itemLocations[row, column] = new Vector2(35 + (50 * row), 35 + (75 * column));

                        if (column == 2)
                            row++;
                    }

                    Rectangle rectangle = new Rectangle(pauseMenus[(int)MenuOrder.MainPause].Rectangle.Right, pauseMenus[(int)MenuOrder.MainPause].Rectangle.Top, 300, 300);

                    pauseMenus[(int)MenuOrder.Inventory].Init(rectangle, itemNames, itemLocations, new int[,] { }, (int)MenuOrder.MainPause);
                }
                pauseMenus[selection].Activate();
            }
            else if (boxType == MenuBox.BoxType.CharactersMenu)
            {
                Character character = party[selection];

                Item[] equipArray = character.EquipArray;
                string[] equipNames = new string[equipArray.Length];

                for (int i = 0; i < equipArray.Length; i++)
                {
                    if (equipArray[i] != null)
                        equipNames[i] = equipArray[i].Name;
                    else
                        equipNames[i] = "None";
                }

                string[] texts = { character.Name, (character.Harmony.ToString() + ':' + character.Chaos.ToString()),
                    "HP", character.BaseHitPoints.ToString(), character.EffectiveHitPoints.ToString(), "MP", character.BaseMagicPoints.ToString(), character.EffectiveMagicPoints.ToString(),
                    "AT", character.BaseAttack.ToString(), character.EffectiveAttack.ToString(), "DF", character.BaseDefense.ToString(),character.EffectiveDefense.ToString(),
                    "MG", character.BaseMagic.ToString(), character.EffectiveMagic.ToString(), "MD", character.BaseMagicDefense.ToString(), character.EffectiveMagicDefense.ToString(),
                    "DX", character.BaseDexterity.ToString(), character.EffectiveDexterity.ToString(), "SK", character.BaseSkill.ToString(), character.EffectiveSkill.ToString(),
                    "Weapon", equipNames[0], equipNames[1], "Armor", equipNames[2], equipNames[3], equipNames[4], equipNames[5]};

                Vector2[] locations = { new Vector2(30, 30), new Vector2(200, 30),
                    new Vector2(40, 60), new Vector2(80, 60), new Vector2(110, 60), new Vector2(40, 80), new Vector2(80, 80), new Vector2(110, 80),
                    new Vector2(40, 100), new Vector2(80, 100), new Vector2(110, 100), new Vector2(40, 120), new Vector2(80, 120), new Vector2(110, 120),
                    new Vector2(40, 140), new Vector2(80, 140), new Vector2(110, 140), new Vector2(40, 160), new Vector2(80, 160), new Vector2(110, 160),
                    new Vector2(40, 180), new Vector2(80, 180), new Vector2(110, 180), new Vector2(40, 200), new Vector2(80, 200), new Vector2(110, 200),
                    new Vector2(150, 75), new Vector2(220, 75), new Vector2(220, 95),
                    new Vector2(150, 125), new Vector2(220, 125), new Vector2(220, 145), new Vector2(220, 165), new Vector2(220, 185)
                };
                pauseMenus[(int)MenuOrder.CharacterDetails].Init(new Rectangle(pauseMenus[(int)MenuOrder.CharacterMenu].Rectangle.Right, pauseMenus[(int)MenuOrder.CharacterMenu].Rectangle.Top, 400, 270),
                                                                          texts, locations, (int)MenuOrder.CharacterMenu);
                pauseMenus[(int)MenuOrder.CharacterDetails].Activate();
            }
        }

        #endregion

        /// <summary>
        /// Centers text horizontally
        /// </summary>
        /// <param name="rectangle">The text is within</param>
        /// <param name="text">The string to center</param>
        /// <param name="font">The font the string is using</param>
        /// <returns></returns>
        public static int CenterTextHorizontal(Rectangle rectangle, string text, SpriteFont font)
        {
            int length = (int)font.MeasureString(text).X;

            return (rectangle.Width / 2 - length / 2);
        }

        #endregion
    }
}
