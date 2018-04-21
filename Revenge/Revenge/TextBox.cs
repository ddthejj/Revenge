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
    public class TextBox : Box
    {
        protected class Line
        {
            #region Properties

            public string Text;
            public Vector2 Location;
            public SpriteFont Font;
            public Color Color = Color.White;

            #endregion

            #region Get Set Properties

            public int Width { get { return (int)Font.MeasureString(Text).X; } }
            public int Height { get { return (int)Font.MeasureString(Text).Y; } }

            #endregion

            #region Constructor

            public Line(string text, Vector2 location, SpriteFont font)
            {
                Text = text; Location = location; Font = font;
            }

            #endregion
        }

        #region Properties

        Arrow arrow;

        public string[] text;
        public double speed = 5;

        protected List<Line> currentText = new List<Line>();
        protected double whichChar = 0, previousChar = 0;
        protected int whichLine = 0;
        protected int whichBox = 0;
        protected bool started = false, waiting = false;

        #endregion

        #region Get Set Properties



        #endregion

        #region Constructor

        public TextBox(Rectangle rectangle) : base(rectangle)
        {
            arrow = new Arrow(new Rectangle(rectangle.Right - 30, rectangle.Bottom - 30, 7, 7), true, (float)(Math.PI / 2));
        }

        #endregion

        #region Methods

        public void Start(string[] newText)
        {
            text = newText;
            if (newText == null)
                text = new string[] { "ERROR" };
            Activate();
        }

        public void End()
        {
            text = null;
            Deactivate();
        }

        void TextScroll()
        {
            // First, set the double previous char to equal which char the text is up to.
            previousChar = whichChar;

            // Then we need to check if the box is first starting, and this will make sure that you can't press space on this frame. 
            if (currentText.Count == 0)
            {
                currentText.Add(new Line("", new Vector2(rectangle.X + 20, rectangle.Y + 15), spriteFont));
                started = true;
            }

            if (!waiting)
            {
                // Then we increase which char by the speed of the text being displayed.
                if (whichChar < text[whichBox].Length)
                {
                    whichChar += speed;
                }

                // Then we add the chars in between the previous char and which char to the current string.
                for (int i = (int)Math.Ceiling(previousChar); i < whichChar; i++)
                {
                    // This makes sure the char hasn't gone past the end of the box
                    if (i < text[whichBox].Length)
                    {
                        // Adds the char that corresponds to i in the for loop
                        currentText[whichLine].Text += text[whichBox][i];

                        // Make sure if there's multiple words contained in the speed it checks for collision on the right each time there is
                        if (text[whichBox][i] == ' ')
                            if (Collided(i))
                            {
                                whichChar = i + 1;
                                break;
                            }
                    }
                    else
                    {
                        i = text[whichBox].Length;
                        break;
                    }
                }

                if (!started && whichChar >= text[whichBox].Length)
                {
                    if (!arrow.Enabled)
                        arrow.Activate();

                    if (KeyOptions.KeyPressed(KeyOptions.Select))
                    {
                        arrow.Deactivate();
                        arrow.Reset();

                        if (whichBox < text.GetLength(0) - 1)
                        {
                            whichBox++;
                            currentText.Clear();
                            whichLine = 0;
                            whichChar = 0;
                            previousChar = 0;
                        }
                        else
                        {
                            Manager.CompleteText();
                            Reset();
                        }
                    }
                }

            }
            else
            {
                if (!arrow.Enabled)
                    arrow.Activate();

                if (KeyOptions.KeyPressed(KeyOptions.Select))
                {
                    currentText.Clear();
                    whichLine = 0;
                    waiting = false;
                    arrow.Deactivate();
                    arrow.Reset();
                }
            }
            started = false;
        }

        public override void Update()
        {
            TextScroll();
        }

        public override void Draw()
        {
            base.Draw();
            foreach (Line line in currentText)
            {
                Game1.spriteBatch.DrawString(line.Font, line.Text, line.Location, line.Color);
            }
        }

        public bool Collided(double currentTestChar)
        {
            string overflowTestString = currentText[whichLine].Text;
            int overflowTestInt = (int)Math.Floor(currentTestChar) + 1;

            while (overflowTestInt < text[whichBox].Length - 1 && text[whichBox][overflowTestInt] != ' ')
            {
                overflowTestString += text[whichBox][overflowTestInt];
                overflowTestInt++;
            }

            int measureX = (int)spriteFont.MeasureString(overflowTestString).X;

            if (spriteFont.MeasureString(overflowTestString).X >= rectangle.Width - 40)
            {
                if (rectangle.Y + 15 + ((whichLine + 1) * spriteFont.MeasureString("0").Y) < rectangle.Bottom - 10 - spriteFont.MeasureString("0").Y)
                {
                    whichLine++;
                    currentText.Add(new Line("", new Vector2(rectangle.X + 20, rectangle.Y + 15 + (whichLine * spriteFont.MeasureString("0").Y)), spriteFont));
                }
                else
                {
                    waiting = true;
                    //whichChar = previousChar;
                    return true;
                }
            }

            return false;
        }

        public void Reset()
        {
            whichChar = 0;
            previousChar = 0;
            whichLine = 0;
            whichBox = 0;
            currentText.Clear();
            started = false;
        }

        #endregion

    }
}
