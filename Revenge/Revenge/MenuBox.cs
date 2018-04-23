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
    public class MenuBox : Box
    {

        protected class MenuItem
        {
            #region Properties

            /// <summary>
            /// The text that the menu item contains
            /// </summary>
            string text;
            /// <summary>
            /// The location to write the item's text at
            /// </summary>
            Vector2 location;
            /// <summary>
            /// The number to return to the manager when something is selected
            /// </summary>
            int option;

            #endregion

            #region Constructor

            /// <summary>
            /// Builds a menu option and assigns all values
            /// </summary>
            /// <param name="_text">The text that the menu item contains</param>
            /// <param name="_location">The location to write the text at</param>
            /// <param name="_option">The number to return to the manager when something is selected</param>
            public MenuItem(string _text, Vector2 _location, int _option = -1) { text = _text; location = _location; option = _option; }

            #endregion

            #region Methods

            /// <summary>
            /// The text that the menu item contains
            /// </summary>
            public string Text { get { return text; } }
            /// <summary>
            /// The location to write the item's text at
            /// </summary>
            public Vector2 Location { get { return location; } }
            /// <summary>
            /// The number to return to the manager when something is selected
            /// </summary>
            public int SubMenu { get { return option; } }

            /// <summary>
            /// Draws the menu item
            /// </summary>
            public void Draw()
            {
                Game1.spriteBatch.DrawString(spriteFont, text, location, Color.White);
            }

            #endregion
        }

        #region Properties

        /// <summary>
        /// The type of menu that the menu is
        /// </summary>
        public enum BoxType
        {
            BaseMenu,
            CharactersMenu,
            InventoryMenu
        }

        /// <summary>
        /// The options that the player can choose from on the menu
        /// </summary>
        protected MenuItem[,] selectableLocations = new MenuItem[,] { };
        /// <summary>
        /// The static text that the player can't choose from on the menu
        /// </summary>
        protected MenuItem[] nonSelectableLocations = new MenuItem[] { };
        /// <summary>
        /// The currently selected location
        /// </summary>
        protected Vector2 selected = new Vector2(0, 0);
        /// <summary>
        /// The location of the first option on the menu
        /// </summary>
        protected Vector2 defaultSelected;
        /// <summary>
        /// The arrow that displays what the use has selected
        /// </summary>
        protected Arrow arrow;
        /// <summary>
        /// The menu that opened this menu
        /// </summary>
        protected int previousMenu;
        /// <summary>
        /// The type of menu that this is
        /// </summary>
        protected BoxType boxType = BoxType.BaseMenu;

        #endregion

        #region Get Set Properties

        /// <summary>
        /// The menu that opened this menu
        /// </summary>
        public int PreviousMenu { get { return previousMenu; } }

        #endregion

        #region Constructor
        /// <summary>
        /// Constructs a menu box with only selectable options
        /// </summary>
        /// <param name="_rectangle">The rectangle of the box</param>
        /// <param name="texts">The 2D array of options for the box to display</param>
        /// <param name="locations">Where to write each of the options</param>
        /// <param name="SubMenus">The int that each of the options holds</param>
        /// <param name="_previousMenu">The menu that opened this one</param>
        /// <param name="_boxType">The type of box that this is</param>
        public MenuBox(Rectangle _rectangle, string[,] texts, Vector2[,] locations, int[,] SubMenus, int _previousMenu = -1, BoxType _boxType = BoxType.BaseMenu) : base(_rectangle)
        {
            previousMenu = _previousMenu; boxType = _boxType;

            CreateOptions(texts, locations, SubMenus);
        }
        /// <summary>
        /// Constructs a menu box witn only non selectable options
        /// </summary>
        /// <param name="_rectangle">The rectangle of the box</param>
        /// <param name="texts">The array of options for the box to display</param>
        /// <param name="locations">Where to write each of the options</param>
        /// <param name="_previousMenu">The menu that opened this one</param>
        /// <param name="_boxType">The type of box that this is</param>
        public MenuBox(Rectangle _rectangle, string[] texts, Vector2[] locations, int _previousMenu = -1, BoxType _boxType = BoxType.BaseMenu) : base(_rectangle)
        {
            previousMenu = _previousMenu; boxType = _boxType;

            CreateStaticOptions(texts, locations);
        }
        /// <summary>
        /// Constructs a menu box that will have to be initialized later
        /// </summary>
        /// <param name="_boxType">The type of box that this is</param>
        public MenuBox(BoxType _boxType = BoxType.BaseMenu) : base(new Rectangle(0,0,0,0))
        {
            boxType = _boxType;
        }
        /// <summary>
        /// Recreates a box without destroying it
        /// </summary>
        /// <param name="_rectangle">The rectangle of the box</param>
        /// <param name="texts">The array of options for the box to display</param>
        /// <param name="locations">Where to write each of the options</param>
        /// <param name="submenus">The int that each of the options holds</param>
        /// <param name="_previousMenu">The menu that opened this one</param>
        public void Init(Rectangle _rectangle, string[,] texts, Vector2[,] locations, int[,] submenus, int _previousMenu = -1)
        {
            rectangle = _rectangle; previousMenu = _previousMenu;
            CreateOptions(texts, locations, submenus);
        }

        public void Init(Rectangle _rectangle, string[] texts, Vector2[] locations, int _previousMenu = -1)
        {
            rectangle = _rectangle; previousMenu = _previousMenu;
            CreateStaticOptions(texts, locations);
        }

        public void Init(Rectangle _rectangle, string[,] texts, Vector2[,] locations, int[,] submenus, string[] staticText, Vector2[] staticLocations, int _previousMenu = -1)
        {
            rectangle = _rectangle; previousMenu = _previousMenu;
            CreateOptions(texts, locations, submenus);
            CreateStaticOptions(staticText, staticLocations);
        }


        #endregion  

        #region Methods

        public override void Update()
        {
            SwitchSelected();
            CheckClose();
        }

        public override void Draw()
        {
            base.Draw();

            foreach (MenuItem item in selectableLocations)
            {
                Game1.spriteBatch.DrawString(spriteFont, item.Text, new Vector2(item.Location.X + rectangle.Location.X, item.Location.Y + rectangle.Location.Y), Color.White);
            }
            foreach (MenuItem item in nonSelectableLocations)
            {
                Game1.spriteBatch.DrawString(spriteFont, item.Text, new Vector2(item.Location.X + rectangle.Location.X, item.Location.Y + rectangle.Location.Y), Color.White);
            }
            if (arrow != null)
                arrow.Draw();
        }
        /// <summary>
        /// Lets you switch between the options on a menu.
        /// </summary>
        void SwitchSelected()
        {
            if (selectableLocations.GetLength(1) <= selected.X || selectableLocations.GetLength(0) <= selected.Y)
            {
                if (arrow != null)
                    arrow.Deactivate();
                return;
            }

            int maxX = selectableLocations.GetLength(1) - 1, maxY = selectableLocations.GetLength(0) - 1;

            if (KeyOptions.KeyPressed(KeyOptions.Up))
            {
                do
                {
                    if (selected.Y != 0)
                        selected.Y--;
                    else
                        selected.Y = maxY;
                }
                while (selectableLocations[(int)selected.Y, (int)selected.X] == null);

                arrow.Move((int)selectableLocations[(int)selected.Y, (int)selected.X].Location.X + rectangle.X - 20, (int)selectableLocations[(int)selected.Y, (int)selected.X].Location.Y + rectangle.Y + 7);
            }
            else if (KeyOptions.KeyPressed(KeyOptions.Down))
            {
                do
                {
                    if (selected.Y != maxY)
                        selected.Y++;
                    else
                        selected.Y = 0;
                }
                while (selectableLocations[(int)selected.Y, (int)selected.X] == null);

                arrow.Move((int)selectableLocations[(int)selected.Y, (int)selected.X].Location.X + rectangle.X - 20, (int)selectableLocations[(int)selected.Y, (int)selected.X].Location.Y + rectangle.Y + 7);
            }
            else if (KeyOptions.KeyPressed(KeyOptions.Left))
            {
                do
                {
                    if (selected.X != 0)
                        selected.X--;
                    else
                        selected.X = maxX;
                }
                while (selectableLocations[(int)selected.Y, (int)selected.X] == null);

                arrow.Move((int)selectableLocations[(int)selected.Y, (int)selected.X].Location.X + rectangle.X - 20, (int)selectableLocations[(int)selected.Y, (int)selected.X].Location.Y + rectangle.Y + 7);
            }
            else if (KeyOptions.KeyPressed(KeyOptions.Right))
            {
                do
                {
                    if (selected.X != maxX)
                        selected.X++;
                    else
                        selected.X = 0;
                }
                while (selectableLocations[(int)selected.Y, (int)selected.X] == null);

                arrow.Move((int)selectableLocations[(int)selected.Y, (int)selected.X].Location.X + rectangle.X - 20, (int)selectableLocations[(int)selected.Y, (int)selected.X].Location.Y + rectangle.Y + 7);
            }


            if (KeyOptions.KeyPressed(KeyOptions.Select))
            {
                Select();
            }
        }
        /// <summary>
        /// Closes all open menus if the menu button is pressed
        /// </summary>
        public void CheckClose()
        {
            if (KeyOptions.KeyPressed(KeyOptions.Menu))
            {
                Deactivate();
                Manager.CloseMenu(previousMenu, true);
            }

            if (KeyOptions.KeyPressed(KeyOptions.Back))
            {
                Deactivate();
                Manager.CloseMenu(previousMenu, false);
            }
        }
        /// <summary>
        /// Resets the menu back to its default state
        /// </summary>
        public void Reset()
        {
            selected = defaultSelected;
            if (arrow != null)
            {
                if (selectableLocations.GetLength(1) > selected.X && selectableLocations.GetLength(0) > selected.Y)
                {
                    arrow.Move((int)selectableLocations[(int)selected.Y, (int)selected.X].Location.X + rectangle.X - 20, (int)selectableLocations[(int)selected.Y, (int)selected.X].Location.Y + rectangle.Y + 7);
                    arrow.Reset();
                }
            }
        }
        /// <summary>
        /// Opens the submenu of the selected option
        /// </summary>
        protected virtual void Select()
        {
            int subMenu = selectableLocations[(int)selected.Y, (int)selected.X].SubMenu;
            Freeze();

            if (subMenu != -1)
            {
                Manager.NextMenu(subMenu, boxType);
            }
            else
            {
                Manager.CloseMenu(previousMenu);
                Deactivate();
            }
        }
        /// <summary>
        /// Initially creates the options of the menu 
        /// </summary>
        /// <param name="texts">The texts for the options to display</param>
        /// <param name="locations">The locations for the options to be placed at</param>
        /// <param name="SubMenus">The submenus that the options lead to</param>
        protected virtual void CreateOptions(string[,] texts, Vector2[,] locations, int[,] SubMenus)
        {
            selectableLocations = new MenuItem[texts.GetLength(0), texts.GetLength(1)];

            for (int i = 0; i < texts.GetLength(0); i++)
                for (int j = 0; j < texts.GetLength(1); j++)
                {
                    if (texts[i, j] != null)
                        selectableLocations[i, j] = new MenuItem(texts[i, j], locations[i, j], SubMenus[i, j]);
                    else
                        selectableLocations[i, j] = null;
                }

            arrow = new Arrow(new Rectangle(0, 0, 10, 10));

            for (int i = 0; i < selectableLocations.GetLength(0); i++)
            {
                for (int j = 0; j < selectableLocations.GetLength(1); j++)
                {
                    if (selectableLocations[i, j] != null)
                    {
                        arrow.Move((int)selectableLocations[i, j].Location.X + rectangle.X - 20, (int)selectableLocations[i, j].Location.Y + rectangle.Y + 7);
                        selected = new Vector2(i, j);
                        defaultSelected = new Vector2(i, j);
                        goto Found;
                    }
                }
            }
            Found:;
        }
        /// <summary>
        /// Initially createsthe options of the menu with no submenus
        /// </summary>
        /// <param name="texts">The texts for the options to display</param>
        /// <param name="locations">The locations for the options to be placed at</param>
        protected virtual void CreateOptions(string[,] texts, Vector2[,] locations)
        {
            selectableLocations = new MenuItem[texts.GetLength(0), texts.GetLength(1)];

            for (int i = 0; i < texts.GetLength(0); i++)
                for (int j = 0; j < texts.GetLength(1); j++)
                {
                    if (texts[i, j] != null)
                        selectableLocations[i, j] = new MenuItem(texts[i, j], locations[i, j], -1);
                    else
                        selectableLocations[i, j] = null;
                }

            arrow = new Arrow(new Rectangle(0, 0, 10, 10));

            for (int i = 0; i < selectableLocations.GetLength(0); i++)
            {
                for (int j = 0; j < selectableLocations.GetLength(1); j++)
                {
                    if (selectableLocations[i, j] != null)
                    {
                        arrow.Move((int)selectableLocations[i, j].Location.X + rectangle.X - 20, (int)selectableLocations[i, j].Location.Y + rectangle.Y + 7);
                        selected = new Vector2(i, j);
                        defaultSelected = new Vector2(i, j);
                        goto Found;
                    }
                }
            }
            Found:;
        }

        protected virtual void CreateStaticOptions(string[] texts, Vector2[] locations)
        {
            nonSelectableLocations = new MenuItem[texts.Length];

            for (int i = 0; i < texts.Length; i++)
            {
                nonSelectableLocations[i] = new MenuItem(texts[i], locations[i]);
            }
        }

        public override void Deactivate()
        {
            base.Deactivate();
            Reset();
        }

        #endregion 
    }

    //public class MainMenu : MenuBox
    //{
    //    #region Properties


    //    #endregion

    //    #region Get Set Properties



    //    #endregion

    //    #region Constructor

    //    public MainMenu(Rectangle _rectangle, string[,] texts, Vector2[,] locations, int[,] SubMenus, int PreviousMenu = -1) : base(_rectangle, texts, locations, SubMenus, PreviousMenu)
    //    {

    //    }

    //    #endregion

    //    #region Methods

    //    protected override void Select()
    //    {
    //        int subMenu = selectableLocations[(int)selected.Y, (int)selected.X].SubMenu;
    //        Freeze();
    //        if (subMenu != -1)
    //        {
    //            Manager.NextMenu(subMenu);
    //        }
    //        else
    //        {
    //            Manager.CloseMenu(previousMenu);
    //            Deactivate();
    //        }
    //    }

    //    #endregion
    //}


    //public class CharacterMenu : MenuBox
    //{
    //    #region Properties

    //    List<Character> party;

    //    #endregion

    //    #region Get Set Properties



    //    #endregion

    //    #region Constructor

    //    public CharacterMenu(Rectangle _rectangle, int _previousMenu) : base(_rectangle, _previousMenu)
    //    {
    //        party = Manager.Party;

    //        string[,] characters = new string[party.Count() + 1, 1];
    //        Vector2[,] locations = new Vector2[party.Count() + 1, 1];
    //        int[,] selections = new int[party.Count() + 1, 1];

    //        for (int i = 0; i < party.Count(); i++)
    //        {
    //            characters[i, 0] = party[i].Name;
    //            locations[i, 0] = new Vector2(40, 35 + (50 * i));
    //            selections[i, 0] = i;
    //        }

    //        characters[party.Count(), 0] = "Cancel";
    //        locations[party.Count(), 0] = new Vector2(40, 35 + (50 * party.Count()));
    //        selections[party.Count(), 0] = -1;

    //        rectangle.Height = 100 + (50 * party.Count());

    //        CreateOptions(characters, locations, selections);
    //    }

    //    #endregion

    //    #region Methods

    //    public override void Activate()
    //    {
    //        base.Activate();
    //        party = Manager.Party;
    //    }

    //    protected override void Select()
    //    {
    //        int whichCharacter = selectableLocations[(int)selected.Y, (int)selected.X].SubMenu;
    //        Freeze();
    //        if (whichCharacter != -1)
    //        {
    //            CharacterDetailsMenu dets = new CharacterDetailsMenu(party[whichCharacter], (int)Manager.MenuOrder.CharacterMenu, rectangle);
    //            dets.Activate();
    //        }
    //        else
    //        {
    //            Manager.CloseMenu(previousMenu);
    //            Deactivate();
    //        }
    //    }

    //    #endregion

    //    class CharacterDetailsMenu : MenuBox
    //    {
    //        #region Properties



    //        #endregion

    //        #region Get Set Properties



    //        #endregion

    //        #region Constructor

    //        public CharacterDetailsMenu(Character character, int characterMenu, Rectangle characterMenuRectangle) : base(new Rectangle(characterMenuRectangle.Right, characterMenuRectangle.Top, 400, 270), characterMenu)
    //        {
    //            Item[] equipArray = character.EquipArray;
    //            string[] equipNames = new string[equipArray.Length];

    //            for (int i = 0; i < equipArray.Length; i++)
    //            {
    //                if (equipArray[i] != null)
    //                    equipNames[i] = equipArray[i].Name;
    //                else
    //                    equipNames[i] = "None";
    //            }

    //            string[] texts = { character.Name, (character.Harmony.ToString() + ':' + character.Chaos.ToString()),
    //                "HP", character.BaseHitPoints.ToString(), character.EffectiveHitPoints.ToString(), "MP", character.BaseMagicPoints.ToString(), character.EffectiveMagicPoints.ToString(),
    //                "AT", character.BaseAttack.ToString(), character.EffectiveAttack.ToString(), "DF", character.BaseDefense.ToString(),character.EffectiveDefense.ToString(),
    //                "MG", character.BaseMagic.ToString(), character.EffectiveMagic.ToString(), "MD", character.BaseMagicDefense.ToString(), character.EffectiveMagicDefense.ToString(),
    //                "DX", character.BaseDexterity.ToString(), character.EffectiveDexterity.ToString(), "SK", character.BaseSkill.ToString(), character.EffectiveSkill.ToString(),
    //                "Weapon", equipNames[0], equipNames[1], "Armor", equipNames[2], equipNames[3], equipNames[4], equipNames[5]};

    //            Vector2[] locations = { new Vector2(30, 30), new Vector2(200, 30),
    //                new Vector2(40, 60), new Vector2(80, 60), new Vector2(110, 60), new Vector2(40, 80), new Vector2(80, 80), new Vector2(110, 80),
    //                new Vector2(40, 100), new Vector2(80, 100), new Vector2(110, 100), new Vector2(40, 120), new Vector2(80, 120), new Vector2(110, 120),
    //                new Vector2(40, 140), new Vector2(80, 140), new Vector2(110, 140), new Vector2(40, 160), new Vector2(80, 160), new Vector2(110, 160),
    //                new Vector2(40, 180), new Vector2(80, 180), new Vector2(110, 180), new Vector2(40, 200), new Vector2(80, 200), new Vector2(110, 200),
    //                new Vector2(150, 75), new Vector2(220, 75), new Vector2(220, 95),
    //                new Vector2(150, 125), new Vector2(220, 125), new Vector2(220, 145), new Vector2(220, 165), new Vector2(220, 185)
    //            };

    //            string[,] cancel = new string[,] { { "Cancel" } };
    //            Vector2[,] cancelLocation = new Vector2[,] { { new Vector2(310, 220) } };

    //            CreateOptions(cancel, cancelLocation);
    //            CreateStaticOptions(texts, locations);
    //        }

    //        #endregion

    //        #region Methods

    //        protected override void Select()
    //        {
    //            Manager.CloseMenu(previousMenu);
    //            Deactivate();
    //        }

    //        #endregion
    //    }
    //}

    //public class InventoryMenu : MenuBox
    //{
    //    #region Properties



    //    #endregion

    //    #region Get Set Properties



    //    #endregion

    //    #region Constructor

    //    public InventoryMenu(Rectangle _rectangle, int _previousMenu) : base(_rectangle, _previousMenu)
    //    {
    //        List<Item> inventory = Manager.Inventory;

    //        string[] itemNamesRaw = new string[inventory.Count()];
    //        for (int i = 0; i < itemNamesRaw.Length; i++)
    //        {
    //            itemNamesRaw[i] = inventory[i].Name;
    //        }

    //        int itemsPerColumn = (int)Math.Ceiling((double)itemNamesRaw.Length / 3.0);

    //        string[,] itemNames = new string[itemsPerColumn, 3];
    //        Vector2[,] itemLocations = new Vector2[itemsPerColumn, 3];

    //        int row = 0;
    //        for (int i = 0; i < itemNamesRaw.Length; i++)
    //        {
    //            int column = i % 3;

    //            itemNames[row, column] = itemNamesRaw[i];
    //            itemLocations[row, column] = new Vector2(35 + (50 * row), 35 + (75 * column));

    //            if (column == 2)
    //                row++;
    //        }

    //        CreateOptions(itemNames, itemLocations);
    //    }

    //    #endregion

    //    #region Methods

    //    protected override void Select()
    //    {

    //    }

    //    #endregion
    //}
}
