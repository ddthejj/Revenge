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
    public abstract class Character : Block
    {
        #region Properties

        struct Stats
        {
            public int HP, MP, AT, DF, MG, MD, DX, SK, rH, rC;

            public Stats(int hp, int mp, int at, int df, int mg, int md, int dx, int sk, int rh) { HP = hp; MP = mp; AT = at; DF = df; MG = mg; MD = md; DX = dx; SK = sk; rH = rh; rC = 100 - rh; }

            public int[] StatsArray { get { return new int[] { HP, MP, AT, DF, MG, MD, DX, SK, rH, rC }; } }
        }
        struct Equips
        {
            public Item W1, W2, AH, AB, AW, AF;
        }

        protected string name;
        protected short speed = 3;
        Stats baseStats;
        Stats equipStats;
        Equips equips;

        protected enum WayFacing { Up, Down, Left, Right }
        protected WayFacing wayFacing = WayFacing.Down;

        #endregion

        #region Get Set Properties

        public int Harmony { get { return baseStats.rH; } }

        public int Chaos { get { return baseStats.rC; } }


        public int BaseHitPoints { get { return baseStats.HP; } }

        public int BaseMagicPoints { get { return baseStats.MP; } }

        public int BaseAttack { get { return baseStats.AT; } }

        public int BaseDefense { get { return baseStats.DF; } }

        public int BaseMagic { get { return baseStats.MP; } }

        public int BaseMagicDefense { get { return baseStats.MD; } }

        public int BaseDexterity { get { return baseStats.DX; } }

        public int BaseSkill { get { return baseStats.SK; } }

        public int[] BaseStats { get { return baseStats.StatsArray; } }


        public int EquipHitPoints { get { return equipStats.HP; } }

        public int EquipMagicPoints { get { return equipStats.MP; } }

        public int EquipAttack { get { return equipStats.AT; } }

        public int EquipDefense { get { return equipStats.DF; } }

        public int EquipMagic { get { return equipStats.MP; } }

        public int EquipMagicDefense { get { return equipStats.MD; } }

        public int EquipDexterity { get { return equipStats.DX; } }

        public int EquipSkill { get { return equipStats.SK; } }

        public int[] EquipStats { get { return equipStats.StatsArray; } }


        public int EffectiveHitPoints { get { return baseStats.HP + equipStats.HP; } }

        public int EffectiveMagicPoints { get { return baseStats.MP + equipStats.MP; } }

        public int EffectiveAttack { get { return baseStats.AT + equipStats.AT; } }

        public int EffectiveDefense { get { return baseStats.DF + equipStats.DF; } }

        public int EffectiveMagic { get { return baseStats.MP + equipStats.MP; } }

        public int EffectiveMagicDefense { get { return baseStats.MD + equipStats.MD; } }

        public int EffectiveDexterity { get { return baseStats.DX + equipStats.DX; } }

        public int EffectiveSkill { get { return baseStats.SK + equipStats.SK; } }

        public int[] EffectiveStats
        {
            get
            {
                return new int[] {EffectiveHitPoints, EffectiveMagicPoints, EffectiveAttack, EffectiveDefense,
                                                              EffectiveMagic, EffectiveMagicDefense, EffectiveDexterity, EffectiveSkill};
            }
        }

        public Item WeaponRight { get { return equips.W1; } }

        public Item WeaponLeft { get { return equips.W2; } }

        public Item ArmorHead { get { return equips.AH; } }

        public Item ArmorBody { get { return equips.AB; } }

        public Item ArmorWaist { get { return equips.AW; } }

        public Item ArmorFeet { get { return equips.AF; } }

        public Item[] EquipArray { get { return new Item[] { equips.W1, equips.W2, equips.AH, equips.AB, equips.AW, equips.AF }; } }

        #endregion

        #region Constructor

        public Character(int width = 32, int height = 32, string _name = "ERROR", int hp = 10, int mp = 0, int at = 3, int df = 3, int mg = 3, int md = 3, int dx = 3, int sk = 3, int rh = 50) : base(.49f, 0, 0, width, height)
        {
            name = _name;

            baseStats = new Stats(hp, mp, at, df, mg, md, dx, sk, rh);
            equipStats = new Stats(0, 0, 0, 0, 0, 0, 0, 0, 50);
            equips = new Equips();
        }

        #endregion

        #region Methods

        public string Name { get { return name; } }

        public abstract void Move();

        public override void Draw()
        {
            Game1.spriteBatch.Draw(texture, Rectangle, null, Color.White, 0, new Vector2(0, 0), SpriteEffects.None, layer);
        }

        #endregion
    }
}
