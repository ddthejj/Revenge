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

            string text;
            Vector2 location;
            Box subMenu;

            #endregion

            #region Constructor

            public MenuItem(string _text, Vector2 _location, Box _subMenu = null) { text = _text; location = _location; subMenu = _subMenu; }

            #endregion

            #region Methods

            public string Text { get { return text; } }
            public Vector2 Location { get { return location; } }
            public Box SubMenu { get { return subMenu; } }

            public void Draw()
            {
                Game1.spriteBatch.DrawString(spriteFont, text, location, Color.White);
            }

            #endregion
        }
        
        #region Properties

        MenuItem[,] selectableLocations;
        Vector2 selected = new Vector2(0, 0), defaultSelected;
        Arrow arrow;
        Box previousMenu;

        #endregion

        #region Get Set Properties



        #endregion

        #region Constructor

        public MenuBox(Rectangle _rectangle, string[,] texts, Vector2[,] locations, Box[,] SubMenus, Box PreviousMenu) : base(_rectangle)
        {
            previousMenu = PreviousMenu;
            selectableLocations = new MenuItem[texts.GetLength(0), texts.GetLength(1)];

           for (int i = 0; i < texts.GetLength(0); i++)
                for (int j = 0; j < texts.GetLength(1); j++)
                {
                    if (texts[i, j] != null)
                        selectableLocations[i, j] = new MenuItem(texts[i, j], locations[i, j], SubMenus[i,j]);
                    else
                        selectableLocations[i, j] = null;
                }

            arrow = new Arrow(new Rectangle(0, 0, 10, 10));

            for (int i = 0; i < selectableLocations.GetLength(0); i++)
            {
                for (int j = 0; j < selectableLocations.GetLength(1); j++)
                {
                    if (selectableLocations[i,j] != null)
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

        protected MenuBox(Rectangle _rectangle, string[,] texts, Vector2[,] locations) : base(_rectangle)
        {
            selectableLocations = new MenuItem[texts.GetLength(0), texts.GetLength(1)];

            for (int i = 0; i < texts.GetLength(0); i++)
                for (int j = 0; j < texts.GetLength(1); j++)
                {
                    if (texts[i, j] != null)
                        selectableLocations[i, j] = new MenuItem(texts[i, j], locations[i, j]);
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

        #endregion  

        #region Methods

        public override void Update()
        {
            Choose();
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

        void Choose()
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

        public void Reset()
        {
            selected = defaultSelected;
            arrow.Reset();
        }

        protected virtual void Select()
        {
            Box subMenu = selectableLocations[(int)selected.Y, (int)selected.X].SubMenu;
            Freeze();
            if (subMenu != null)
                subMenu.Activate();
            else
            {
                Reset();

                if (previousMenu != null)
                {
                    previousMenu.Activate();
                    Deactivate();
                }
                else
                    Manager.CloseMenu();
            }
        }

        #endregion 
    }

    public class CharacterMenu : MenuBox
    {
        #region Properties

        List<Character> party;

        #endregion

        #region Constructor

        public CharacterMenu (Rectangle _rectangle, Vector2[,] locations, string[,] texts ):base(_rectangle, texts, locations)
        {
            party = Manager.Party;
        }

        #endregion

        #region Get Set Properties



        #endregion

        #region Methods

        protected override void Select()
        {

        }

        #endregion
    }
}
