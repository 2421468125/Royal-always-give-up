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
/*        Debug.Log("I use defend");
        BattleManager.ChangeState(BattleManager.State.BeforeUseCard);
        int real_damage = BattleManager.Power(BattleManager.POWER.DEFEND, Hero, Hero, defense);
        Hero.(real_damage);
        ChangeState(BeforeuseCard);*/
    }
}