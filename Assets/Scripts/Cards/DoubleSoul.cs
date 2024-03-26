using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DoubleSoul : BaseCards
{

    public DoubleSoul() : base("doubleEnergy")
    {
        helper = new string[] { "hun" };
        isExhaust = true;
    }

    public override void Use(Character target)
    {
        CardManager.hero.AddSoul(CardManager.hero.soul);
    }
}
