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
    public abstract class MenuBox : Box
    {

        protected class MenuItem
        {
            #region Properties

            string text;
            Vector2 location;
            int subMenu;

            #endregion

            #region Constructor

            public MenuItem(string _text, Vector2 _location, int _subMenu) { text = _text; location = _location; subMenu = _subMenu; }

            //public MenuItem(string _text, Vector2 _location) { text = _text;  location = _location; subMenu = -1; }
            #endregion

            #region Methods

            public string Text { get { return text; } }
            public Vector2 Location { get { return location; } }
            public int SubMenu { get { return subMenu; } }

            public void Draw()
            {
                Game1.spriteBatch.DrawString(spriteFont, text, location, Color.White);
            }

            #endregion
        }

        #region Properties

        protected MenuItem[,] selectableLocations;
        protected Vector2 selected = new Vector2(0, 0), defaultSelected;
        protected Arrow arrow;
        protected int previousMenu;

        #endregion

        #region Get Set Properties

        public int PreviousMenu { get { return previousMenu; } }

        #endregion

        #region Constructor
        /// <summary>
        /// Builds a menu and options with submenus
        /// </summary>
        /// <param name="_rectangle">The rectangle of the menu</param>
        /// <param name="texts">The texts for the options</param>
        /// <param name="locations">The locations of the options</param>
        /// <param name="SubMenus">The submenus for the options</param>
        /// <param name="PreviousMenu">The previous menu</param>
        public MenuBox(Rectangle _rectangle, string[,] texts, Vector2[,] locations, int[,] SubMenus, int PreviousMenu = -1) : base(_rectangle)
        {
            previousMenu = PreviousMenu;

            CreateOptions(texts, locations, SubMenus);
        }
        /// <summary>
        /// Constructor that is used by base classes that automatically place options
        /// </summary>
        /// <param name="_rectangle">The rectangle of the menu</param>
        /// <param name="_previousMenu">The previous menu</param>
        protected MenuBox(Rectangle _rectangle, int _previousMenu) : base(_rectangle) { previousMenu = _previousMenu; }

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
            arrow.Draw();
        }
        /// <summary>
        /// Lets you switch between the options on a menu.
        /// </summary>
        void SwitchSelected()
        {
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


            if (KeyOptions.KeyPressed(KeyOptions.Interact))
            {
                Select();
            }
        }
        /// <summary>
        /// Closes all open menus if the menu button is pressed
        /// </summary>
        protected void CheckClose()
        {
            if (KeyOptions.KeyPressed(KeyOptions.Menu))
            {
                Deactivate();
                Manager.CloseMenu(previousMenu, true);
            }
        }
        /// <summary>
        /// Resets the menu back to its default state
        /// </summary>
        public void Reset()
        {
            selected = defaultSelected;
            arrow.Move((int)selectableLocations[(int)selected.Y, (int)selected.X].Location.X + rectangle.X - 20, (int)selectableLocations[(int)selected.Y, (int)selected.X].Location.Y + rectangle.Y + 7);
            arrow.Reset();
        }
        /// <summary>
        /// Opens the submenu of the selected option
        /// </summary>
        protected abstract void Select();
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
                        selectableLocations[i, j] = new MenuItem(texts[i, j], locations[i, j],  SubMenus[i, j]);
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

        public override void Deactivate()
        {
            base.Deactivate();
            Reset();
        }

        #endregion 
    }

    public class MainMenu : MenuBox
    {
        #region Properties


        #endregion

        #region Get Set Properties



        #endregion

        #region Constructor

        public MainMenu(Rectangle _rectangle, string[,] texts, Vector2[,] locations, int[,] SubMenus, int PreviousMenu = -1) : base(_rectangle, texts, locations, SubMenus, PreviousMenu)
        {

        }

        #endregion

            #region Methods

        protected override void Select()
        {
            int subMenu = selectableLocations[(int)selected.Y, (int)selected.X].SubMenu;
            Freeze();
            if (subMenu != -1)
            {
                Manager.NextMenu(subMenu);
            }
            else
            {
                Manager.CloseMenu(previousMenu);
                Deactivate();
            }
        }

        #endregion
    }

    public class CharacterMenu : MenuBox
    {
        #region Properties

        List<Character> party;

        #endregion

        #region Get Set Properties



        #endregion

        #region Constructor

        public CharacterMenu (Rectangle _rectangle, int _previousMenu ) : base(_rectangle, _previousMenu)
        {
            party = Manager.Party;

            string[,] characters = new string[party.Count(), 1];
            Vector2[,] locations = new Vector2[party.Count(), 1];
            int[,] selections = new int[party.Count(), 1];

            for (int i = 0; i < party.Count(); i++)
            {
                characters[i, 0] = party[i].Name;
                locations[i, 0] = new Vector2(40, 35 + (50 * i));
                selections[i, 0] = i;
            }

            CreateOptions(characters, locations, selections);
        }

        #endregion

        #region Methods

        public override void Activate()
        {
            base.Activate();
            Init();
        }

        void Init()
        {
            party = Manager.Party;
        }
        
        protected override void Select()
        {
            int whichCharacter = selectableLocations[(int)selected.Y, (int)selected.X].SubMenu;
            Freeze();
            if (whichCharacter != -1)
            {
                CharacterDetailsMenu dets = new CharacterDetailsMenu(party[whichCharacter], (int)Manager.MenuOrder.CharacterMenu, rectangle);

            }
            else
            {
                Manager.CloseMenu(previousMenu);
                Deactivate();
            }
        }

        #endregion

        class CharacterDetailsMenu : MenuBox
        {
            #region Properties



            #endregion

            #region Get Set Properties



            #endregion

            #region Constructor

            public CharacterDetailsMenu(Character selectedCharacter, int characterMenu, Rectangle characterMenuRectangle) :base(new Rectangle(characterMenuRectangle.Right, characterMenuRectangle.Top, 200, 200), characterMenu)
            {
                
            }

            #endregion

            #region Methods

            protected override void Select()
            {
                
            }

            #endregion
        }
    }
}
