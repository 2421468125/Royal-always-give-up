
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR;

public class Defend_R : BaseCards
{
    public int defense;
    public Defend_R() : base("defend_R")
    {
        defense = 5;
    }

    public override void Use(Character target)
    {
        int real_defend = battle_manager.power(2, CardManager.hero, target, defense);
        target.Defend(real_defend);
        
    }
}
