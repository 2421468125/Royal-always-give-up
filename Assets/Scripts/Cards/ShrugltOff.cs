
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR;

public class ShrugltOff : BaseCards {
    public int defense;
    public ShrugltOff() : base("shrugItOff")
    {
        defense = 8;
        helper = new string[1] { "fangyu" };
    }

    public override void Use(Character target)
    {
        int real_defend = battle_manager.power(2, CardManager.hero, target, defense);
         target.Defend(real_defend);
        card_manager.Draw_card(1);
    }
}