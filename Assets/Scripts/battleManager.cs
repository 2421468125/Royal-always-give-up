using System.Collections.Generic;
using UnityEngine;
using System.Collections;
using System.Globalization;
using JetBrains.Annotations;
using UnityEngine.Timeline;
using UnityEngine.SceneManagement;
using System.Security.Cryptography;
using System;
using System.Linq;
using System.Runtime.Serialization;
using Unity.VisualScripting;
using Unity.Mathematics;
using TMPro;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

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
    private bufType bufType;
    void Start()
    {
        if (ins == null)
        {
            ins = this;
        }
        else Destroy(this);
        DontDestroyOnLoad(this);
        CM = GameObject.Find("CardManager").GetComponent<CardManager>();
        CHM = GameObject.Find("CharacterManager").GetComponent<CharacterManager>();
        Hero = GameObject.Find("Hero").GetComponent<Hero>();
        bufType = new bufType();
        Debug.Assert(CM != null);
        Debug.Assert(Hero != null);
    }
    private void Update()
    {
        if (SceneLock.Lock == 0)
            return;
    }
    private void phase1()
    { //战斗开始
        Hero.Layer++;
        Hero.soul_count = GameObject.Find("soul_text").GetComponent<TextMeshProUGUI>();
        Hero.energy_text = GameObject.Find("energy").GetComponent<TextMeshProUGUI>();
        Hero.hp_in_bar = GameObject.Find("health").GetComponent<TextMeshProUGUI>();
        Hero.gold_text = GameObject.Find("gold_num").GetComponent<TextMeshProUGUI>();
        Hero.transform.position = new Vector3(-5, -1, 0);
        //Hero.bar = GameObject.Find("bloodbar2").GetComponent<Bloodbar>();
        CM.card_count = GameObject.Find("card_num").GetComponent<TextMeshProUGUI>();
        CM.draw_card_count = GameObject.Find("drawcard_count").GetComponent<TextMeshProUGUI>();
        CM.dis_card_count = GameObject.Find("discard_count").GetComponent<TextMeshProUGUI>();
        Canvas canvas = FindObjectOfType<Canvas>();
        canvas.transform.GetChild(5).GetComponent<Button>().onClick.AddListener(CM.Turn_end);// 5: TakeAll;
        CHM = GameObject.Find("CharacterManager").GetComponent<CharacterManager>();
        CM.ClearAllList();
        CM.CopyCardList();
        Hero.AddSoul(Hero.money >= 10 ? 10 : Hero.money);
        Hero.money -= Hero.soul;
        CHM.CopyBuff();   
        phase2();
    }
    private void phase2()
    { // 回合开始
        if(Hero.dynamicBuf["hunpozaisheng"] != 0)
            for(int i=1;i<=Hero.dynamicBuf["hunpozaisheng"];i++){
                Hero.AddSoul(getSoul(Hero,Hero,10));
            }
        Hero.dynamicBuf["fangyu"] = 0;
        Hero.UpdateState();
        Hero.energy = Mathf.Max(Hero.energy, Hero.max_energy);
        CM.StartTurn();
        foreach(Enemy enemy in CHM.EnemyList){
            if(enemy.dynamicBuf["duwu"] != 0){
                Hero.dynamicBuf["zuzhou"] += enemy.dynamicBuf["duwu"];
            }
        }
        Hero.UpdateState();
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
        Hero.energy = 0;
        if (CHM.num == 0)
        {
            changeState(9);
        }
        else {
            phase6_1();
            CM.Discard();
            phase6_2(); 
        }
    }
    private void phase6_1()
    { // 我方回合结束
        if (Hero.dynamicBuf["xuruo"] != 0) { Hero.dynamicBuf["xuruo"]--; }
        if (Hero.dynamicBuf["cuiruo"] != 0) { Hero.dynamicBuf["cuiruo"]--; }
        if (Hero.dynamicBuf["yishang"] != 0) { Hero.dynamicBuf["yishang"]--; }
        if(Hero.dynamicBuf["hexinhudun"] != 0)
            for(int i=1;i<=Hero.dynamicBuf["hexinhudun"];i++){
                if(Hero.soul >= 3){
                    Hero.AddSoul(getSoul(Hero,Hero,-3));
                    Hero.dynamicBuf["fangyu"] += 6;
                    Hero.UpdateState();
                }
            }
        if (Hero.dynamicBuf["zuzhou"] != 0){
            Hero.Hurt(Hero.dynamicBuf["zuzhou"]);
            Hero.dynamicBuf["zuzhou"]--;
        }
        Hero.UpdateState();
    }
    private void phase6_2()
    { // 敌人回合开始
        foreach(Enemy enemy in CHM.EnemyList){
            enemy.dynamicBuf["fangyu"] = 0;
            if(enemy.dynamicBuf["jingji+"] != 0){
                enemy.dynamicBuf["jingji+"]--;
                enemy.UpdateState();
            }
            if(enemy.dynamicBuf["zuzhou"] != 0){
                enemy.Hurt(enemy.dynamicBuf["zuzhou"]);
                enemy.dynamicBuf["zuzhou"]--;
            }
            enemy.UpdateState();
        }
        CHM.startEnemyturn();
    }
    private void phase7()
    { // 敌人行动

    }
    private void phase8()
    { // 敌人回合结束
        foreach(Enemy enemy in CHM.EnemyList){
            if(enemy.dynamicBuf["cuiruo"] != 0) enemy.dynamicBuf["cuiruo"]--;
            if(enemy.dynamicBuf["yishang"] != 0) enemy.dynamicBuf["yishang"]--;
            if(enemy.dynamicBuf["xuruo"] != 0) enemy.dynamicBuf["xuruo"]--;
        }
        CHM.Rebuild();
        phase2();
    }
    private void phase9()
    { // 战斗结束
        Hero.money += Hero.soul;
        Hero.soul = 0;
        CM.ClearAllList();
        Hero.transform.position += new Vector3(0, 0, 10);
        SceneManager.LoadScene("SelectCardScene");

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
    public string randomDebuff(){
        string debuff;
        var filtered = bufType.dic.Where(pair => pair.Value == 1).ToList();
        System.Random random = new System.Random();
        var selectedPair = filtered[random.Next(filtered.Count)];
        debuff = selectedPair.Key;
        return debuff;
    }
    public int attack(Character src, Character dst, int val)
    { // 4操作数
        Dictionary<string, int> srcBuf = src.dynamicBuf;
        Dictionary<string, int> dstBuf = dst.dynamicBuf;
        float realAttack = (float)val + (float)srcBuf["liliang"];
        if (dstBuf["yishang"] != 0) realAttack *= 1.5f;
        if (srcBuf["xuruo"] != 0) realAttack *= 0.75f;
        if (dstBuf["jingji"] != 0) dst.Hurt(dstBuf["jingji"]);
        if (dst.name != "Hero" && dstBuf["jingji+"]!=0) changeBuf(randomDebuff(),dst,src,1);
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
    public int getSoul(Character src, Character dst, int val)
    {
        if(val > 0 && Hero.dynamicBuf["chaonengli"] != 0)
            for(int i=1;i<= Hero.dynamicBuf["chaonengli"];i++){
                Hero.dynamicBuf["liliang"]++;
                Hero.UpdateState();
            }
        if(val < 0 && Hero.dynamicBuf["emoqiyue"] != 0)
            for(int i=1;i<=Hero.dynamicBuf["emoqiyue"];i++){
                CM.Draw_card(1);
            }
        return val;
    }
    private int draw(Character src, Character dst, int val)
    {
        int realval = val;
        if(Hero.dynamicBuf["choupaijianyi"] > 0){
            realval -= Hero.dynamicBuf["choupaijianyi"];
            Hero.dynamicBuf["choupaijianyi"] = 0;
            realval = math.max(0,realval);
        }
        return realval;
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
            case 4:
                return getSoul(src,dst,val);
            default:
                return -1;
        }
    }
    public void changeBuf(String bufName,Character src,Character dst,int val){
        Dictionary<string, int> srcBuf = src.dynamicBuf;
        Dictionary<string, int> dstBuf = dst.dynamicBuf;
        if(dst.dynamicBuf["rengongzhipin"] != 0 && bufType.dic[bufName] == 1 && val > 0){
            dst.dynamicBuf["rengongzhipin"]--;
            return;
        }
        dst.AddState(bufName, val);
        if(bufType.dic[bufName] == 1 && dst.name == "Hero" && dstBuf["duoluo"] > 0)
            for(int i=1;i<=dstBuf["duoluo"];i++){
                CM.Draw_card(1);
                Hero.energy++;
                Hero.UpdateEnergy();
            }
        if(bufType.dic[bufName] == 1 && dst.name == "Hero" && dstBuf["tongguiyujin"] > 0)
            for(int i=1;i<=dstBuf["tongguiyujin"];i++){
                foreach(Enemy enemy in CHM.EnemyList){
                    enemy.dynamicBuf[bufName] += val;
                }
            }
        Hero.UpdateState();
        foreach(Enemy enemy in CHM.EnemyList){
            enemy.UpdateState();
        }
    }
}

