using System.Collections.Generic;
using UnityEngine;
using System.Collections;
using System.Globalization;
using JetBrains.Annotations;
using UnityEngine.Timeline;
using UnityEngine.SceneManagement;
using System.Security.Cryptography;

public class battleManager : MonoBehaviour
{
    // Start is called before the first frame update
    public static battleManager ins = null;
    public CardManager CM = null;
    public CharacterManager CHM = null;
    public int cost = 0;
    public Hero Hero;
    public Dictionary<string, int> heroBuf;
    public Dictionary<string, int> enemyBuf;
    void Start()
    {
        if (ins == null)
        {
            ins = this;
        }
        else Destroy(this);
        CM = GameObject.Find("CardManager").GetComponent<CardManager>();
        CHM = GameObject.Find("CharacterManager").GetComponent<CharacterManager>();
        Hero = GameObject.Find("Hero").GetComponent<Hero>();
        Debug.Assert(CM != null);
        Debug.Assert(Hero != null);
    }
    private void phase1()
    { //战斗开始
        CHM.CopyBuff();
        CM.StartTurn();
    }
    private void phase2()
    { // 回合开始
        Hero.dynamicBuf["fangyu"] = 0;
        Hero.UpdateState();
        Hero.energy = Mathf.Max(Hero.energy, Hero.max_energy);
        CM.StartTurn();
    }
    private void phase3()
    { // 抽牌时

    }
    private void phase4()
    { // 出牌&出牌前

    }
    private void phase5()
    { // 出牌后

    }
    private void phase6()
    { // 回合结束 && 敌人回合开始
        phase6_1();
        CM.Discard();
        phase6_2();
    }
    private void phase6_1()
    { // 我方回合结束
        if (Hero.dynamicBuf["xuruo"] != 0) { Hero.dynamicBuf["xuruo"]--; }
        if (Hero.dynamicBuf["cuiruo"] != 0) { Hero.dynamicBuf["cuiruo"]--; }
        Hero.UpdateState();
    }
    private void phase6_2()
    { // 敌人回合开始
        CHM.startEnemyturn();
    }
    private void phase7()
    { // 敌人行动

    }
    private void phase8()
    { // 敌人回合结束
        phase2();
    }
    private void phase9()
    { // 战斗结束

        SceneManager.LoadScene("AfterBattleScene");
    }
    private void phase10()
    { // 洗牌

    }
    private void phase11()
    { // 主角死亡

    }
    public void changeState(int x)
    {
        switch (x)
        {
            case 1:
                phase1();
                break;
            case 2:
                phase2();
                break;
            case 3:
                phase3();
                break;
            case 4:
                phase4();
                break;
            case 5:
                phase5();
                break;
            case 6:
                phase6();
                break;
            case 7:
                phase7();
                break;
            case 8:
                phase8();
                break;
            case 9:
                phase9();
                break;
            case 10:
                phase10();
                break;
            case 11:
                phase11();
                break;
        }
    }
    public int attack(Character src, Character dst, int val)
    { // 4操作数
        Dictionary<string, int> srcBuf = src.dynamicBuf;
        Dictionary<string, int> dstBuf = dst.dynamicBuf;
        float realAttack = (float)val + (float)srcBuf["liliang"];
        if (dstBuf["yishang"] != 0) realAttack *= 1.5f;
        if (srcBuf["xuruo"] != 0) realAttack *= 0.75f;
        if (dstBuf["jinji"] != 0) dst.Hurt(dstBuf["jinji"]);
        int realAttackToInt = (int)realAttack;
        return realAttackToInt;
    }
    public int defend(Character src, Character dst, int val)
    { // 3操作数，不需要dst
        Dictionary<string, int> dstBuf = dst.dynamicBuf;
        float realDefend = (float)val + (float)dstBuf["minjie"];
        if (dstBuf["cuiruo"] != 0) realDefend *= 0.75f;
        int realDefendToInt = (int)realDefend;
        return realDefendToInt;
    }

    private int draw(Character src, Character dst, int val)
    {
        return val;
    }
    public int power(int x, Character src = null, Character dst = null, int val = -1)
    {
        switch (x)
        {
            case 1:
                return attack(src, dst, val);
            case 2:
                return defend(src, dst, val);
            case 3:
                return draw(src, dst, val);
            default:
                return -1;
        }
    }
}

