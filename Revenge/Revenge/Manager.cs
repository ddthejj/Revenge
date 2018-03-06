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

        public static event VoidDelegate draw;
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
        static Player player;
        /// <summary>
        /// The center of the screen
        /// </summary>
        static Point centerScreen;
        /// <summary>
        /// The current room the player is in
        /// </summary>
        static Room currentRoom;
        /// <summary>
        /// The box at the bottom of the screen where text will be displayed
        /// </summary>
        static TextBox dialogueBox;
        /// <summary>
        /// The menu that appears when the player pauses
        /// </summary>
        static MenuBox pauseMenu;
        /// <summary>
        /// The current group of characters 
        /// </summary>
        static Character[] party;

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

        #region Methods

        /// <summary>
        /// The current player character
        /// </summary>
        public static Player Player { get { return player; } set { player = value; } }
        /// <summary>
        /// The center of the screen
        /// </summary>
        public static Point CenterScreen { get { return centerScreen; } set { centerScreen = value; } }
        /// <summary>
        /// Centers the current room on the player
        /// </summary>
        public static void CenterRoom() { currentRoom.CenterRoom(); }
        /// <summary>
        /// The current room being displayed
        /// </summary>
        public static Room CurrentRoom { get { return currentRoom; } set { currentRoom = value; } }
        /// <summary>
        /// The box at the bottom of the screen where text will be displayed
        /// </summary>
        public static TextBox DialogueBox { get { return dialogueBox; } set { dialogueBox = value; } }
        /// <summary>
        /// The menu that appears when the player pauses
        /// </summary>
        public static MenuBox PauseMenu { get { return pauseMenu; } set { pauseMenu = value; } }
        /// <summary>
        /// The current group of characters
        /// </summary>
        public static Character[] Party { get { return party; } set { party = value; } }

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
            if (currentRoom.Tiles[1][i, j].Interactable)
            {
                WriteText(currentRoom.Tiles[1][i, j].Text);
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
        /// <summary>
        /// Opens the pause menu
        /// </summary>
        public static void OpenMenu()
        {
            player.Freeze();
            currentRoom.Freeze();

            pauseMenu.Activate();
        }
        /// <summary>
        /// Closes the pause menu
        /// </summary>
        public static void CloseMenu()
        {
            pauseMenu.Deactivate();

            player.Unfreeze();
            currentRoom.Unfreeze();
        }

        #endregion
    }
}
