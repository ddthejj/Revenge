using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Revenge
{
    public class Item
    {
        #region Properties

        string name;
        ItemGround groundSprite;

        #endregion

        #region Get Set Properties

        public string Name { get { return name; } }

        #endregion

        #region Constructor

        public Item(string _name = "ERROR")
        {
            name = _name;
            //groundSprite = new InteractableTile();
        }

        #endregion

        #region Methods



        #endregion

        private class ItemGround : Tile
        {
            #region Properties



            #endregion

            #region Get Set Properties



            #endregion

            #region Constructor



            #endregion

            #region Methods



            #endregion
        }
    }

}
