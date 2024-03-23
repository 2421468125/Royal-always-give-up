using JetBrains.Annotations;
using Microsoft.Unity.VisualStudio.Editor;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using TMPro;
using UnityEditor;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.UIElements;
using static System.Net.Mime.MediaTypeNames;
using static UnityEngine.RuleTile.TilingRuleOutput;

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

    public BaseCards a;

    static public Dictionary<string, int> id_map = new Dictionary<string, int>();
    public static Dictionary<string, int> cost_map = new Dictionary<string, int>();
    static public Dictionary<string, Ctype> Ctype_map = new Dictionary<string, Ctype>();
    static public Dictionary<string, Cvalue> Cvalue_map = new Dictionary<string, Cvalue>();
    static public Dictionary<string, Ccolour> Ccolour_map = new Dictionary<string, Ccolour>();
    static public Dictionary<string, string> description_map = new Dictionary<string, string>();
    static public Dictionary<string, string> chinese_name_map = new Dictionary<string, string>();

    // Start is called before the first frame update
    void Start()
    {
        transform.position = Vector3.zero;
        LoadDictionary();
        a = new BaseCards("anger");
        create_an_enemy();
    }


    private Texture2D enemy;
    private SpriteRenderer enemy_render;
    void create_an_enemy()
    {
        GameObject enemy_obj = new GameObject("Enemy");
        enemy = Resources.Load<Texture2D>("imgs/monsters/cultist");
        // Vector2 pivot = new Vector2(0.5f, 0); // 将锚点放置在底部中心
        Vector2 pivot = new Vector2(0.5f, 0.5f);
        Sprite enemy_sp = Sprite.Create(enemy, new Rect(0, 0, enemy.width, enemy.height), pivot);
        // Sprite enemy_sp = Resources.Load<Sprite>("cultist_0"); // 使用裁剪后的图片作为精灵
        enemy_obj.transform.SetParent(enemy_obj.transform);
        enemy_render = enemy_obj.AddComponent<SpriteRenderer>();
        enemy_render.sprite = enemy_sp;
        enemy_obj.transform.position = new Vector3(1f, 1f, 1f);
    }



    // Update is called once per frame
    void Update()
    {
        a.touched(Camera.main.ScreenToWorldPoint(Input.mousePosition));
        a.ChangeCost();
    }

    void LoadDictionary()
    {

        TextAsset txt = Resources.Load("content/card") as TextAsset;
        const int column = 8;
        string[] data = txt.text.Split("\r\n");
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
            description_map.Add(name, one_card[7]);
        }
    }

}

public class BaseCards {
    private Texture2D background, foreground;
    private SpriteRenderer bg_render, fg_render;
    private GameObject card_obj;
    private TextMeshPro description;
    private TextMeshPro card_name;
    private TextMeshPro card_type;
    private TextMeshPro card_cost;

    private float pos_x, pos_y, pos_z;
    private int _id;
    private int _cost, _tem_cost;
    private string _name, _chinese_name, _type_des, _desription;
    private CardManager.Ctype _type;
    private CardManager.Cvalue _value;
    private CardManager.Ccolour _colour;

    static public TMP_FontAsset font = Resources.Load<TMP_FontAsset>("fonts/MSYHBD SDF");

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
    private static Vector2 DeltaCardType = new Vector2(0.01f, -0.119f);
    private static Vector2 DeltaCardCost = new Vector2(-0.674f, 0.968f);

    private bool isMoved = false;
    private bool isChoosed = false;

    public BaseCards(string name)
    {
        _name = name;
        card_obj = new GameObject(_name);
        card_obj.transform.position = Vector3.zero;
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
        set_texture();

        GameObject bg_obj = new GameObject("BackGround");
        Sprite bg_sp = Sprite.Create(background, new Rect(0, 0, background.width, background.height), Vector2.one * 0.5f);
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

    public void touched(Vector3 p)
    {
        Vector3 movement = new Vector3(0, 1, 0);
        float[] card_pos_bound = this.GetCoordinates();
        if (p.x >= card_pos_bound[0] &&
            p.x <= card_pos_bound[1] &&
            p.y >= card_pos_bound[2] &&
            p.y <= card_pos_bound[3] )
        {
            if (!isMoved)
            {
                this.Move(movement);
                isMoved = true;
            }
            if (Input.GetMouseButtonDown(0))
            {
                isChoosed = !isChoosed;
            }
            // Debug.Log("touched");
        }
        else
        {
            if (isMoved && !isChoosed)
            {
                this.Move(-movement);
                isMoved = false;
            }
            // Debug.Log("leave");
        }
    }
}