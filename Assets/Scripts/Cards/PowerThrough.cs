using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PowerThrough : BaseCards
{
    public int defend, frail;
    public PowerThrough() : base("powerThrough")
    {
        defend = 15;
        frail = 2;
        helper = new string[2] { "fangyu","cuiruo" };
    }

    public override void Use(Character target)
    {

        int real_defend = battle_manager.power(2, CardManager.hero, target, defend);
        target.Defend(real_defend);
        target.AddState("cuiruo", 2);
    }
}
