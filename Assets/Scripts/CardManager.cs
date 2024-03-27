using JetBrains.Annotations;
/*using Microsoft.Unity.VisualStudio.Editor;*/
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System;
using System.Runtime.CompilerServices;
using TMPro;
using Unity.VisualScripting;
using UnityEditor;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.UIElements;
using static System.Net.Mime.MediaTypeNames;
using static UnityEngine.RuleTile.TilingRuleOutput;
using static UnityEngine.EventSystems.EventTrigger;
using System.Text;
/*using static UnityEditor.PlayerSettings;*/


public class CardManager : MonoBehaviour {

    public enum Ctype {
        ATTACK,
        SKILL,
        POWER,
        CURSE,
        STATUS
    };//卡牌种类

    public enum Cvalue {
        INITIAL,
        COMMON,
        VALUE,
        RARE
    };//稀有度

    public enum Ccolour {
        RED,
        GREEN,
        BLUE,
        GREY,
        BLACK
    };//颜色，黑色算诅咒卡

    private static CardManager ins;

    static public Dictionary<string, int> id_map = new Dictionary<string, int>();
    static public Dictionary<string, int> cost_map = new Dictionary<string, int>();
    static public Dictionary<string, Ctype> Ctype_map = new Dictionary<string, Ctype>();
    static public Dictionary<string, Cvalue> Cvalue_map = new Dictionary<string, Cvalue>();
    static public Dictionary<string, Ccolour> Ccolour_map = new Dictionary<string, Ccolour>();
    static public Dictionary<string, string> description_map = new Dictionary<string, string>();
    static public Dictionary<string, string> chinese_name_map = new Dictionary<string, string>();
    static public Dictionary <string, string> state_icon_map  = new Dictionary<string, string>();
    static public Dictionary<string,string> helper_map = new Dictionary<string, string>();
    static public List<BaseCards> card_library = new List<BaseCards>();

    static public int TotalCard = 32;
    static public int MaxHandCardNum = 10;
    static public float CardInterval = 1.3f;
    static public float CardHeight = -4f;
    static public List<BaseCards> card_list = new List<BaseCards>();
    public List<BaseCards> card_in_hand = new List<BaseCards>();
    public List<BaseCards> discard_list = new List<BaseCards>();
    public List<BaseCards> exhaust_list = new List<BaseCards>();
    public List<BaseCards> draw_card_list= new List<BaseCards>();
    public List<BaseCards> card_anime_list = new List<BaseCards>();
    public List <Vector3> card_destination = new List<Vector3>();

    public bool is_card_anime = false;
    private float anime_start_time = 0f;
    private BaseCards which_arrow = null;
    
    static public battleManager battle_manager;
    static public CharacterManager character_manager;
    static public Hero hero;
    static public BaseCards card_up = null;
    static public Character target = null;
    GameObject help_obj = null;
    GameObject end_turn_obj1 = null;
    GameObject end_turn_obj2 = null;
    GameObject dialog_obj = null;

    public TextMeshProUGUI card_count = null;
    public TextMeshProUGUI draw_card_count = null;
    public TextMeshProUGUI dis_card_count = null;


    private void Awake()
    {
        Debug.Log("Awake");
        if (ins != null)
        {
            Destroy(this.gameObject);
            return;
        }
        else
            ins = this;

        DontDestroyOnLoad(this);
        LoadDictionary();
    }
    // Start is called before the first frame update
    void Start()
    {
       
        transform.position = Vector3.zero;
        battle_manager = GameObject.Find("battleManager").GetComponent<battleManager> ();
        character_manager = GameObject.Find("CharacterManager").GetComponent<CharacterManager>();
        hero = GameObject.Find("Hero").GetComponent<Hero>();
        for (int i = 1; i <TotalCard+1 ; ++i)
        {
            BaseCards card = CardCreate(i);
            if (card != null)
            {
                card_list.Add(card);
            }
        }

    }




    // Update is called once per frame
    void Update()
    {
        if (SceneLock.Lock == 0)
            return;
        card_count.text = card_list.Count.ToString();
        draw_card_count.text = draw_card_list.Count.ToString();
        dis_card_count.text = discard_list.Count.ToString();
        if ( is_card_anime)
        {
            is_card_anime = false;
            for(int i=0;i<card_anime_list.Count;++i)
            {
                if ((card_anime_list[i].card_obj.transform.position - card_destination[i]).magnitude > 0.1f)
                {
                    card_anime_list[i].card_obj.transform.position = Vector3.Lerp(card_anime_list[i].card_obj.transform.position, card_destination[i], 0.2f);
                    is_card_anime = true;
                }
            }
        }
        else
        {
            
            ChooseCard();
            IfUseCard();
            ShowInformation();
        }
        
    }

    private void ChooseCard()
    {
        foreach (BaseCards card in card_in_hand)
        {
            float[] bounds = card.GetCoordinates();
            bool[] result;
            if (card.isMoved)
                bounds[2] -= BaseCards.Movement.y;
            result = IsTouched(Camera.main.ScreenToWorldPoint(Input.mousePosition), bounds);

            if (result[1])
            {
                card.isChoosed = !card.isChoosed;
                if (card.isChoosed && card_up == card)
                {
                    which_arrow = card;
                }
                if (card_up == card)
                    DrawBesselArrow(card);
            }
            else if (!card.isMoved && result[0] && card_up == null)
            {
                card.Move(BaseCards.Movement);
                card.isMoved = true;
                card_up = card;
            }
            else if (card.isMoved && !card.isChoosed && !result[0])
            {
                card.Move(-BaseCards.Movement);
                card.isMoved = false;
                card_up = null;
            }

        }
    }

    private void IfUseCard()
    {
        if (target != null && card_up != null && card_up.can_be_used)
        {
            if(hero.energy < card_up._tem_cost)
            {
                ShowLackEnergy();
                target = null;
                return; 
            }
            hero.energy -= card_up._tem_cost;
            battle_manager.changeState(4);
            card_up.Use(target);
            battle_manager.changeState(5);
            if (card_up.isExhaust)
            {
                card_in_hand.Remove(card_up);
                exhaust_list.Add(card_up);
                CardAnimation();
                card_anime_list.Add(card_up);
                card_destination.Add(new Vector3(10, CardHeight, 0));
            }
            else
            {
                card_in_hand.Remove(card_up);
                discard_list.Add(card_up);
                CardAnimation();
                card_anime_list.Add(card_up);
                card_destination.Add(new Vector3(10, CardHeight, 0));
            }
            card_up.isChoosed = false;
            card_up.isMoved = false;
            card_up = null;
            which_arrow = null;
            target = null;
            DrawBesselArrow(which_arrow);
        }
        if (card_up != null && ! card_up.can_be_used)
                target = null;
        foreach (BaseCards card in card_in_hand)
        {
            card.update_card_use_state();
        }
    }
    void LoadDictionary()
    {


        const int column = 8;
        string[] data;
        TextAsset t1 = Resources.Load<TextAsset>("content/card");
        data = t1.text.Split("\r\n");


        for (int i = 1; i < data.Length; i++)
        {
            string[] one_card = data[i].Split(',');
            if (data[i].Length < column)
                break;
            string name = one_card[1];
            id_map.Add(name, int.Parse(one_card[0]));
            chinese_name_map.Add(name, one_card[2]);
            cost_map.Add(name, int.Parse(one_card[6]));
            switch (one_card[3])
            {
                case "攻击":
                    Ctype_map.Add(name, Ctype.ATTACK);
                    break;
                case "技能":
                    Ctype_map.Add(name, Ctype.SKILL);
                    break;
                case "能力":
                    Ctype_map.Add(name, Ctype.POWER);
                    break;
                case "状态":
                    Ctype_map.Add(name, Ctype.STATUS);
                    break;
                case "诅咒":
                    Ctype_map.Add(name, Ctype.CURSE);
                    break;
            }
            switch (one_card[4])
            {
                case "initial":
                    Cvalue_map.Add(name, Cvalue.INITIAL);
                    break;
                case "common":
                    Cvalue_map.Add(name, Cvalue.COMMON);
                    break;
                case "rare":
                    Cvalue_map.Add(name, Cvalue.RARE);
                    break;
                case "value":
                    Cvalue_map.Add(name, Cvalue.VALUE);
                    break;
            }
            switch (one_card[5])
            {
                case "red":
                    Ccolour_map.Add(name, Ccolour.RED);
                    break;
                case "green":
                    Ccolour_map.Add(name, Ccolour.GREEN);
                    break;
                case "blue":
                    Ccolour_map.Add(name, Ccolour.BLUE);
                    break;
                case "grey":
                    Ccolour_map.Add(name, Ccolour.GREY);
                    break;
                case "black":
                    Ccolour_map.Add(name, Ccolour.BLACK);
                    break;
            }
            description_map.Add(name, one_card[7].Replace("\\n", Environment.NewLine));
        }

        TextAsset t2 = Resources.Load<TextAsset>("content/state_icon");
        string[] data2;
        data2 = t2.text.Split("\r\n");
        for (int i=0;i<data2.Length; i++)
        {
            string[] one_state = data2[i].Split(',');
            if (data2[i].Length < 2)
                break;
            state_icon_map.Add(one_state[0], "imgs/state/" + one_state[1]);
        }

        TextAsset t3 = Resources.Load<TextAsset>("content/power");
        string[] data3;
        data3 = t3.text.Split("\r\n");
        for (int i = 0; i < data3.Length; i++)
        {
            string[] one_state = data3[i].Split(',');
            if (data3[i].Length < 2)
                break;
            helper_map.Add(one_state[0], one_state[1].Replace("\\n", Environment.NewLine));
        }
    }

    public BaseCards CardCreate(int i)
    {
        switch (i)
        {
            case 1:
                return new Anger();
            case 2:
                return new Strike_R();
            case 3:
                return new Defend_R();
            case 4:
                return new PowerThrough();
            case 5:
                return new RecklessCharge();
            case 6:
                return new IronWave();
            case 7:
                return new BodySlam();
            case 8:
                return new Corruption();
            case 9:
                return new LastStrike();
            case 10:
                return new Purity();
            case 11:
                return new Aggregate();
            case 12:
                return new Undo();
            case 13:
                return new Bane();
            case 14:
                return new HandOfGreed();
            case 15:
                return new Hunduoshizhong();
            case 16:
                return new SteamPower();
            case 17:
                return new JustLucky();
            case 18:
                return new Blur();
            case 19:
                return new DoubleSoul();
            case 20:
                return new Combust();
            case 21:
                return new MoneyIsPower();
            case 22:
                return new DemonForm();
            case 23:
                return new ShangGong();
            case 24:
                return new Linghunchongji();
            case 25:
                return new Buffer();
            case 26:
                return new Offering();
            case 27:
                return new CreativeAi();
            case 28:
                return new PommelStrike();
            case 29:
                return new Clothesline();
            case 30:
                return new Clash();
            case 31:
                return new ShrugltOff();
            case 32:
                return new Shockwave();
            default:
                return null;
        }
    }

    public BaseCards  GetCardReward(int id, Vector3 pos)
    {
        BaseCards card = CardCreate(id);
        if (card == null)
            return null;
        card.card_obj.transform.position = pos;
        return card;
    }

    public bool[] IsTouched(Vector3 p, float[] bounds)
    {
        bool touch = false, click = false;
        bool[] result = new bool[2];
        if (p.x >= bounds[0] && p.x <= bounds[1] && p.y >= bounds[2] && p.y <= bounds[3])
        {
            touch = true;
            if (Input.GetMouseButtonDown(0))
            {
                click = true;
            }
        }
        else
        {
            touch = click = false;
        }
        result[0] = touch;
        result[1] = click;
        return result;
    }
    void DrawBesselArrow(BaseCards card)
    {
        if (GetComponent<BesselArrow>())
        {
            for(int i = 0;i<transform.childCount;++i)
                Destroy(transform.GetChild(i).gameObject);
            Destroy(GetComponent<BesselArrow>());
        }
        if (card == null)
            return;
        if (card.isChoosed)
            this.AddComponent<BesselArrow>();
    }

    private void ShowInformation()
    {
        if (card_up != null)
        {
            if (help_obj == null)
            {
                help_obj = new GameObject("HelpInformation");
                Texture2D reward = Resources.Load<Texture2D>("imgs/reward/rewardListItemPanel");
                Sprite help_sp = Sprite.Create(reward, new Rect(0, 0, reward.width, reward.height), new(0, 1));
                help_obj.transform.localPosition = new Vector3(-9.06f, 4.45f, 0);
                help_obj.transform.localScale = new Vector3(2f, 1.8f, 1f);
                SpriteRenderer help_render = help_obj.AddComponent<SpriteRenderer>();
                help_render.sprite = help_sp;

                TextMeshPro information = null;
                GameObject information_obj = new GameObject("information_text");
                /*information_obj.transform.localPosition = help_obj.transform.localPosition + new Vector3 (1, -1, 0);*/
                information_obj.transform.localPosition = new Vector3(-4.7f, 3.6f, 0);
                information = information_obj.AddComponent<TextMeshPro>();
                information.text = card_up.GetHelper();
                information.font = BaseCards.font;
                information.fontStyle = FontStyles.Bold;
                information.fontSize = 3f;
                information.autoSizeTextContainer = true;
                information.color = new Color32(21, 58, 102, 255);
                information.outlineWidth = 0.2f;
                information.outlineColor = Color.white;
                information.transform.SetParent(help_obj.transform);
            }
        }
        else if (help_obj)
        {
            Destroy(help_obj);
        }
    }

    void ShowLackEnergy()
    {
        dialog_obj = new GameObject("Dialog");
        Texture2D reward = Resources.Load<Texture2D>("imgs/event/dialog_box");
        Sprite dialog_sp = Sprite.Create(reward, new Rect(0, 0, reward.width, reward.height), new(0.5f, 0.5f));
        dialog_obj.transform.localPosition = new Vector3(-2f, 2.23f, 0);
        dialog_obj.transform.localScale = new Vector3(0.4f, 0.4f, 1f);
        SpriteRenderer dialog_render = dialog_obj.AddComponent<SpriteRenderer>();
        dialog_render.sprite = dialog_sp;

        TextMeshPro dialog_text = null;
        GameObject dialog_text_obj = new GameObject("dialog_text");
        /*information_obj.transform.localPosition = help_obj.transform.localPosition + new Vector3 (1, -1, 0);*/
        dialog_text_obj.transform.localPosition = new Vector3(-1.95f, 2.131f, 0);
        dialog_text = dialog_text_obj.AddComponent<TextMeshPro>();
        dialog_text.text = "我没有足够的能量。";
        dialog_text.font = BaseCards.font;
        dialog_text.fontStyle = FontStyles.Bold;
        dialog_text.fontSize = 3f;
        dialog_text.autoSizeTextContainer = true;
        dialog_text.color = new Color32(18, 23, 30, 255);
        dialog_text.outlineWidth = 0.1f;
        dialog_text.outlineColor = Color.white;
        dialog_text.transform.SetParent(dialog_obj.transform);
        Destroy(dialog_obj, 1f);
    }

    public void StartTurn()
    {
        int real_num = battle_manager.power(3, hero, hero, 5);
        Draw_card(real_num);
        foreach(BaseCards card in card_in_hand)
        {
            card.update_card_use_state();
        }
    }

    public void CopyCardList()
    {
        foreach(BaseCards card in card_list)
        {
            int id = card._id;
            draw_card_list.Add(CardCreate(id));
        }
        Shuffle(draw_card_list);
        
    }

    public void ClearAllList()
    {
        card_in_hand.Clear();
        discard_list.Clear();
        exhaust_list.Clear();
        draw_card_list.Clear();
        card_anime_list.Clear();
        card_destination.Clear();
}

    public void Draw_card(int number)
    {
        battle_manager.changeState(3);
        //int real_number = battle_manager.power(3,hero,hero,number);
        int real_number = number;
        real_number = Math.Min(real_number, MaxHandCardNum - card_in_hand.Count);
        int first_draw_num = Math.Min(real_number, draw_card_list.Count);
        for (int i = 0; i < first_draw_num ; ++i, --real_number)
        {
            BaseCards card = draw_card_list[0];
            draw_card_list.RemoveAt(0);
            card_in_hand.Add(card);
            card.DrawCard();
        }
        if (real_number > 0)
        {
            ShuffleCard();
            int second_draw_num = Math.Min(real_number, draw_card_list.Count);
            for (int i = 0; i < second_draw_num; ++i)
            {
                BaseCards card = draw_card_list[0];
                draw_card_list.RemoveAt(0);
                card_in_hand.Add(card);
                card.DrawCard();
            }
        }
        CardAnimation();
    }

    public void Discard()
    {
        CardAnimation(true);
        while (card_in_hand.Count > 0)
        {
            BaseCards card = card_in_hand[0];
            card_in_hand.RemoveAt(0);
            if (card.Discard())
                discard_list.Add(card);
        }
        
    }


    public void ShuffleCard()
    {
        battle_manager.changeState(10);
        while(discard_list.Count > 0)
        {
            BaseCards card = discard_list[0];
            discard_list.RemoveAt(0);
            draw_card_list.Add(card);
            card.card_obj.transform.position = new Vector3(-10, CardHeight, 0);
        }
        Shuffle(draw_card_list);
    }

    private void Shuffle<T>(List<T> list)
    {
        int count = list.Count;
        while (count > 1)
        {
            count--;
            int randomIndex = UnityEngine.Random.Range(0, count + 1);
            T temp = list[randomIndex];
            list[randomIndex] = list[count];
            list[count] = temp;
        }
    }

    public void CardAnimation(bool discard=false)
    {
        card_destination = new List<Vector3>();
        card_anime_list = new List<BaseCards>();
        float first_pos = CardInterval * (card_in_hand.Count - 1) / 2;
        if (! discard)
            for (int i = 0; i < card_in_hand.Count; ++i)
            {
                card_destination.Add(new Vector3(first_pos - 1.5f * i, CardHeight, 0));
                card_anime_list.Add(card_in_hand[i]);
            }
        else
            for (int i = 0; i < card_in_hand.Count; ++i)
            {
                card_destination.Add(new Vector3(10, CardHeight, 0));
                card_anime_list.Add(card_in_hand[i]);
            }
        is_card_anime = true;
        anime_start_time = Time.time;
    }

    public void Turn_end()
    {
        if (card_up == null)
        {
            battle_manager.changeState(6);
            Debug.Log("why");
        }
            
    }
}



public class BaseCards {
    public GameObject card_obj;
    private Texture2D background, foreground;
    private SpriteRenderer bg_render, fg_render;
    private TextMeshPro description;
    private TextMeshPro card_name;
    private TextMeshPro card_type;
    private TextMeshPro card_cost;

    private float pos_x, pos_y, pos_z;
    public int _id;
    public int _cost, _tem_cost;
    private string _name, _chinese_name, _type_des, _desription;
    public CardManager.Ctype _type;
    public CardManager.Cvalue _value;
    public CardManager.Ccolour _colour;
    
    static public TMP_FontAsset font = Resources.Load<TMP_FontAsset>("fonts/MSYH SDF");

    static public float BG_z = 1f;
    static public float FG_z = -1f;
    static public float font_size = 1;
    static public float card_name_font_size = 1.5f;
    static public float card_type_font_size = 0.9f; // 单独定义卡牌类型字体大小 适配显示 
    static public float card_cost_font_size = 3f;
    static public float description_font_size = 1.15f;
    
    static private float DeltaForeGround = 0.352f;
    static private float DeltaCardName = 0.894f;
    static private float DeltaDescription = -0.5f;
    public CardManager card_manager;
    public battleManager battle_manager;

    private static Vector2 DeltaCardType = new Vector2(0.01f, -0.119f);
    private static Vector2 DeltaCardCost = new Vector2(-0.674f, 0.968f);
    public static Vector3 Movement = new Vector3(0, 0.5f, 0);
    public bool isMoved = false;
    public bool isChoosed = false;

    public bool can_be_used = true;
    public bool isExhaust = false;
    public bool isArrogant = false;
    public bool isEthereal = false;
    public string[] helper;
    public BaseCards(string name)
    {
        _name = name;
        card_obj = new GameObject(_name);
        card_obj.transform.position = new Vector3(-10,CardManager.CardHeight,0);
        pos_x = card_obj.transform.position.x;
        pos_y = card_obj.transform.position.y;
        pos_z = card_obj.transform.position.z;
        _id = CardManager.id_map[name];
        _cost = CardManager.cost_map[name];
        _type = CardManager.Ctype_map[name];
        _value = CardManager.Cvalue_map[name];
        _colour = CardManager.Ccolour_map[name];
        _desription = CardManager.description_map[name];
        _chinese_name = CardManager.chinese_name_map[name];
        _tem_cost = _cost;
        battle_manager = GameObject.Find("battleManager").GetComponent<battleManager>();
        card_manager = GameObject.Find("CardManager").GetComponent<CardManager>();
        set_texture();

        GameObject bg_obj = new GameObject("BackGround");
        Sprite bg_sp = Sprite.Create(background, new Rect(0, 0, background.width, background.height), Vector2.one * 0.5f);
        bg_obj.transform.localPosition = new Vector3(pos_x, pos_y, pos_z);
        bg_obj.transform.SetParent(card_obj.transform);
        ChangeZ(bg_obj, BG_z);
        bg_render = bg_obj.AddComponent<SpriteRenderer>();
        bg_render.sprite = bg_sp;

        GameObject fg_obj = new GameObject("ForeGround");
        Sprite fg_sp = Sprite.Create(foreground, new Rect(0, 0, foreground.width, foreground.height), Vector2.one * 0.5f);
        fg_obj.transform.localPosition = new Vector3(pos_x, pos_y + DeltaForeGround, pos_z);
        fg_obj.transform.SetParent(card_obj.transform);
        ChangeZ(fg_obj, FG_z);
        fg_render = fg_obj.AddComponent<SpriteRenderer>();
        fg_render.sprite = fg_sp;

        set_text();
    }


    public void set_texture()
    {
        string bg_1 = null, bg_2 = null, fg_1 = null;
        switch (_colour)
        {
            case CardManager.Ccolour.RED:
                bg_1 = "red";
                break;
            case CardManager.Ccolour.GREEN:
                bg_1 = "green";
                break;
            case CardManager.Ccolour.BLUE:
                bg_1 = "blue";
                break;
            case CardManager.Ccolour.GREY:
                bg_1 = "grey";
                break;
            case CardManager.Ccolour.BLACK:
                bg_1 = "black";
                break;
        }
        switch (_value)
        {
            case CardManager.Cvalue.INITIAL:
            case CardManager.Cvalue.COMMON:
                bg_2 = "com";
                break;
            case CardManager.Cvalue.RARE:
                bg_2 = "rare";
                break;
            case CardManager.Cvalue.VALUE:
                bg_2 = "value";
                break;
        }
        switch (_type)
        {
            case CardManager.Ctype.ATTACK:
                fg_1 = "attack";
                _type_des = "攻击";
                break;
            case CardManager.Ctype.SKILL:
                fg_1 = "skill";
                _type_des = "技能";
                break;
            case CardManager.Ctype.POWER:
                fg_1 = "power";
                _type_des = "能力";
                break;
            case CardManager.Ctype.STATUS:
                fg_1 = null;
                bg_1 = "status";
                _type_des = "状态";
                break;
            case CardManager.Ctype.CURSE:
                fg_1 = bg_2 = null;
                bg_1 = "curse";
                _type_des = "诅咒";
                break;
        }
        if (null != bg_2)
            background = Resources.Load<Texture2D>(string.Format("imgs/cards/{0}_{1}_card", bg_1, bg_2));
        else
            background = Resources.Load<Texture2D>(string.Format("imgs/cards/{0}_card", bg_1));

        if (null != fg_1)
            foreground = Resources.Load<Texture2D>(string.Format("imgs/cardimg/{0}/{1}/{2}", bg_1, fg_1, _name));
        else
            foreground = Resources.Load<Texture2D>(string.Format("imgs/cardimg/{0}/{2}", bg_1, _name));
    }
    public virtual void set_text()
    {

        GameObject card_name_obj = new GameObject("CardName");
        card_name_obj.transform.localPosition = new Vector3(pos_x, pos_y + DeltaCardName, pos_z);
        card_name = card_name_obj.AddComponent<TextMeshPro>();
        card_name.text = _chinese_name;
        card_name.font = font;
        card_name.fontStyle = FontStyles.Bold;
        card_name.fontSize = card_name_font_size;
        card_name.autoSizeTextContainer = true;
        card_name.transform.SetParent(card_obj.transform);



        GameObject dscp_obj = new GameObject("Description");
        dscp_obj.transform.localPosition = new Vector3(pos_x, pos_y + DeltaDescription, pos_z);
        description = dscp_obj.AddComponent<TextMeshPro>();
        description.text = _desription;
        description.font = font;
        description.fontSize = description_font_size;
        description.autoSizeTextContainer = false;
        description.GetComponent<RectTransform>().sizeDelta = new Vector2(1.2f, 1f);
        description.alignment = TextAlignmentOptions.Center;
        description.transform.SetParent(card_obj.transform);


        GameObject card_type_obj = new GameObject("CardType");
        card_type_obj.transform.localPosition = new Vector3(pos_x + DeltaCardType.x, pos_y + DeltaCardType.y, pos_z);
        card_type = card_type_obj.AddComponent<TextMeshPro>();
        card_type.text = _type_des;
        card_type.font = font;
        card_type.fontSize = card_type_font_size;
        card_type.autoSizeTextContainer = true;
        card_type.transform.SetParent(card_obj.transform);
        card_type.color = new Color32(72, 72, 72, 255); // 颜色设置

        GameObject card_cost_obj = new GameObject("CardCost");
        card_cost_obj.transform.localPosition = new Vector3(pos_x + DeltaCardCost.x, pos_y + DeltaCardCost.y, pos_z);
        card_cost = card_cost_obj.AddComponent<TextMeshPro>();
        card_cost.text = _tem_cost.ToString();
        card_cost.font = font;
        card_cost.fontSize = card_cost_font_size;
        card_cost.fontStyle = FontStyles.Bold;
        card_cost.outlineWidth = 0.25f;
        card_cost.outlineColor = Color.black;

        card_cost.autoSizeTextContainer = true;
        card_cost.transform.SetParent(card_obj.transform);
    }

    public virtual void DrawCard()
    {

    }
    public virtual void Use(Character target)
    {
        Debug.Log("Use card");
    }

    // 加入弃牌堆返回true;
    public virtual bool Discard()
    {
        Debug.Log("Discard");
        return ! isEthereal;
    }

    public virtual void update_card_use_state()
    {
        foreach(BaseCards card in card_manager.card_in_hand)
        {
            if (this == card)
                continue;
            if (card.isArrogant)
            {
                can_be_used = false;
                return;
            }
        }
        can_be_used= true;
    }


    public void ChangeZ(GameObject obj, float z)
    {
        Vector3 pre = obj.transform.position;
        pre.z = z;
        obj.transform.position = pre;
    }

    public void Move(Vector3 Movement)
    {
        card_obj.transform.position += Movement;
    }

    public void ChangeCost()
    {
        card_cost.text = _tem_cost.ToString();
    }

    public float[] GetCoordinates()
    {
        float[] coordinates = new float[4];

        Vector2 cardPosition = card_obj.transform.position;
        float offsetX = 0.7f;  // X轴偏移量
        float offsetY = 1.0f;  // Y轴偏移量

        coordinates[0] = cardPosition.x - offsetX; // xmin
        coordinates[1] = cardPosition.x + offsetX; // xmax
        coordinates[2] = cardPosition.y - offsetY; // ymin
        coordinates[3] = cardPosition.y + offsetY; // ymax
        if (isMoved) coordinates[2] -= 1f;

        return coordinates;
    }

    public string GetHelper()
    {
        string result = "";
        if (helper == null)
            return result;
        foreach(string info  in helper)
        {
            result = result + CardManager.helper_map[info]+Environment.NewLine;
        }
        return result;
    }
}