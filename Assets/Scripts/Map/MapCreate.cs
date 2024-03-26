using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Unity.VisualScripting;
using static UnityEditor.Experimental.GraphView.GraphView;

public struct data
{
    public int x, y;
}
public class MapCreate : MonoBehaviour
{
    

    // Start is called before the first frame update
    const int Max_Point = 7; //ÿ�еĵ���
    const int Min_Point = 2;
    const int Point_Layer = 15;
  
    const float monster_poss = 0.60f;
    const float elite_poss = 0.18f;
    const float rest_poss = 0.10f;
    const float shop_poss = 0.1f;
    const float event_poss = 0.15f;
    public int[] cnt_layer = new int[20];
    public data[,] position_per_layer = new data[20,10];
    public int[,] type_per = new int[20,10];  // 记录这个位置要用哪个种类的
    int lower_index = 0; // ������Ѿ������ڼ�����
    int linked = 1;//less�ڵ�����������Ŀ
    float aver ; //ƽ��һ���ڵ��������
    public int cnt_String = 0;
    public List<Vector3> startPoints;
    public List<Vector3> endPoints;
    public int[,,] If_connectted = new int[20, 10, 10]; //��i��j���Ƿ�͵�i+1��k������
    public int[,] If_addressable = new int[20, 10];
    private float chances;

    public static int[,] type_per0 = new int[20,10];
    public static int[] cnt_layer0 = new int[20];
    public static data[,] position_per_layer0 = new data[20,10]; // 用这个来存每个的位置
    public static int lower_index0 = 0; // 下面的已经连到第几个了
    public static int linked0 = 1;//less节点已相连的数目
    public static float aver0 ; //平均一个节点的连线数
    public static int cnt_String0 = 0;
    public static List<Vector3> startPoints0;
    public static List<Vector3> endPoints0;
    public static int[,,] If_connectted0 = new int[20, 10, 10]; //第i层j个是否和第i+1层k个相连
    public static int[,] If_addressable0 = new int[20, 10];


    Vector3 P;

    void Start()
    {
        int Chest_Layer = UnityEngine.Random.Range(5, 10);//�������5-9��֮��
        Vector3 p;
        p.z = 0;
        for(int i = 0; i < Point_Layer; i++)
        {
          for(int j = 0 ; j < 10; j ++)
          {
            position_per_layer[i,j].x = -25; // 预设一个初始值，如果是这个值说明这个位置没有被创建，否则有，则将ij赋值给他的scripts的layer 和pointnum
            position_per_layer[i,j].y = -120; 
          }
        }
        if(!SceneCreateManager.mapGenerated)
        {
          for (int laye = 0; laye < Point_Layer; laye++)
          {
            int point_num = UnityEngine.Random.Range(2,8);
            if (laye == 14) point_num = 1;
            cnt_layer[laye] = point_num;
            List<(int, int)> position_per = new List<(int, int)>();
            for (int point = 0; point < point_num; point++)
            {
                p.x = -20 + 50 / point_num * point + UnityEngine.Random.Range(-2, 2);
                p.y = -113 + 140 / Point_Layer * laye + UnityEngine.Random.Range(-1, 1);
                if(laye == 14)
                {
                  p.x = 0;
                  p.y = 13;
                }
                position_per_layer[laye,point].x = (int)p.x;
                position_per_layer[laye, point].y = (int)p.y;
                if (laye == 13)
                {
                    /*GameObject e = Instantiate(Resources.Load("rest") as GameObject);
                    SceneSwitchMaprelated sceneSwitchMaprelated = e.GetComponent<SceneSwitchMaprelated>();
                    sceneSwitchMaprelated.laye = laye;
                    sceneSwitchMaprelated.point_num = point;
                    e.transform.localPosition = p;
                    LAYE = laye;
                    POINT = point;
                    P = p;*/
                    type_per[laye,point] = 1;
                }
                else if (laye == Chest_Layer)
                {
                    /*GameObject e = Instantiate(Resources.Load("chest") as GameObject);
                    SceneSwitchMaprelated sceneSwitchMaprelated = e.GetComponent<SceneSwitchMaprelated>();
                    sceneSwitchMaprelated.laye = laye;
                    sceneSwitchMaprelated.point_num = point;
                    e.transform.localPosition = p;
                    LAYE = laye;
                    POINT = point;
                    P = p;*/
                    type_per[laye,point] = 2;
                }
                else if (laye == 0)
                {
                    /*GameObject e = Instantiate(Resources.Load("monster") as GameObject);
                    SceneSwitchMaprelated sceneSwitchMaprelated = e.GetComponent<SceneSwitchMaprelated>();
                    sceneSwitchMaprelated.laye = laye;
                    sceneSwitchMaprelated.point_num = point;
                    e.transform.localPosition = p;
                    LAYE = laye;
                    POINT = point;
                    P = p;*/
                    type_per[laye,point] = 3;
                }
                else if (laye == 14)
                {
                    /*GameObject e = Instantiate(Resources.Load("boss") as GameObject);
                    p.x = 0;
                    p.y = 13;
                    SceneSwitchMaprelated sceneSwitchMaprelated = e.GetComponent<SceneSwitchMaprelated>();
                    sceneSwitchMaprelated.laye = laye;
                    sceneSwitchMaprelated.point_num = point;
                    e.transform.localPosition = p;
                    LAYE = laye;
                    POINT = point;
                    P = p;*/
                    position_per_layer0[laye,point].x = 0; 
                    position_per_layer0[laye,point].y = -13;
                    type_per[laye,point] = 4;
                }
                else
                {
                    float rand_num = Random.value;
                    if (laye < 3)
                    {
                        while (rand_num > monster_poss && rand_num < monster_poss + elite_poss + rest_poss)
                            rand_num = Random.value;
                    }
                    if (rand_num < monster_poss)
                    {
                        /*GameObject e = Instantiate(Resources.Load("monster") as GameObject);
                        SceneSwitchMaprelated sceneSwitchMaprelated = e.GetComponent<SceneSwitchMaprelated>();
                        sceneSwitchMaprelated.laye = laye;
                        sceneSwitchMaprelated.point_num = point;
                        e.transform.localPosition = p;
                        LAYE = laye;
                        POINT = point;
                        P = p;*/
                        type_per[laye,point] = 3;
                    }
                    else if(rand_num < monster_poss+elite_poss)
                    {
                        /*GameObject e = Instantiate(Resources.Load("elite") as GameObject); 
                        SceneSwitchMaprelated sceneSwitchMaprelated = e.GetComponent<SceneSwitchMaprelated>();
                        sceneSwitchMaprelated.laye = laye;
                        sceneSwitchMaprelated.point_num = point;
                        e.transform.localPosition = p;
                        LAYE = laye;
                        POINT = point;
                        P = p;*/
                        type_per[laye,point] = 5;
                    }
                    else if(rand_num < monster_poss + elite_poss + rest_poss)
                    {
                        /*GameObject e = Instantiate(Resources.Load("rest") as GameObject);
                        SceneSwitchMaprelated sceneSwitchMaprelated = e.GetComponent<SceneSwitchMaprelated>();
                        sceneSwitchMaprelated.laye = laye;
                        sceneSwitchMaprelated.point_num = point;
                        e.transform.localPosition = p;
                        LAYE = laye;
                        POINT = point;
                        P = p;*/
                        type_per[laye,point] = 1;
                    }
                    else
                    {
                        /*GameObject e = Instantiate(Resources.Load("shop") as GameObject);
                        SceneSwitchMaprelated sceneSwitchMaprelated = e.GetComponent<SceneSwitchMaprelated>();
                        sceneSwitchMaprelated.laye = laye;
                        sceneSwitchMaprelated.point_num = point;
                        e.transform.localPosition = p;
                        LAYE = laye;
                        POINT = point;
                        P = p;*/
                        type_per[laye,point] = 6;
                    }
                }    
            }
        }
        for (int laye = 0; laye < Point_Layer; laye++) //
            for (int i = 0; i < cnt_layer[laye]; i++)
                for (int j = 0; j < cnt_layer[laye + 1]; j++)
                    If_connectted[laye, i, j] = 0;
        for (int i = 0; i < cnt_layer[0]; i++)
            If_addressable[0, i] = 1;
        for (int laye = 1;laye < Point_Layer ;laye++)
        {
            lower_index = 0;
            linked = 1;
            aver = cnt_layer[laye] / cnt_layer[linked - 1];
            startPoints.Add(new Vector3(position_per_layer[laye, 0].x, position_per_layer[laye, 0].y, 0));
            endPoints.Add(new Vector3(position_per_layer[laye - 1, 0].x, position_per_layer[laye - 1, 0].y, 0));
            If_connectted[laye - 1, 0, 0] = 1;
            cnt_String++;// ÿ�ζ�Ӧ���ö��ȥ���ٵĻ�������һ�����ʻ���
            chances = Random.value;
            for (int upper_index = 0;upper_index < cnt_layer[laye];upper_index++)
            {
                if (cnt_layer[laye] - upper_index < cnt_layer[laye - 1] - lower_index)
                {
                    lower_index++;
                    startPoints.Add(new Vector3(position_per_layer[laye, upper_index].x, position_per_layer[laye, upper_index].y, 0));
                    endPoints.Add(new Vector3(position_per_layer[laye - 1, lower_index].x, position_per_layer[laye - 1, lower_index].y, 0));
                    If_connectted[laye - 1, lower_index, upper_index] = 1;
                    cnt_String++;
                    linked = 1;
                }
                else {
                    float rand_num = Random.value + (aver - linked) * 0.5f;
                    if(rand_num > 0.75 || lower_index == cnt_layer[laye-1]-1)
                    {
                        startPoints.Add(new Vector3(position_per_layer[laye, upper_index].x, position_per_layer[laye, upper_index].y, 0));
                        endPoints.Add(new Vector3(position_per_layer[laye - 1, lower_index].x, position_per_layer[laye - 1, lower_index].y, 0));
                        If_connectted[laye - 1, lower_index, upper_index] = 1;
                        cnt_String++;
                        linked++;
                    }
                    else
                    {
                        lower_index++;
                        startPoints.Add(new Vector3(position_per_layer[laye, upper_index].x, position_per_layer[laye, upper_index].y, 0));
                        endPoints.Add(new Vector3(position_per_layer[laye - 1, lower_index].x, position_per_layer[laye - 1, lower_index].y, 0));
                        If_connectted[laye - 1, lower_index, upper_index] = 1;
                        cnt_String++;
                        linked = 1;
                    }
                }
                if (cnt_layer[laye] < cnt_layer[laye-1])  // ���ӻ��� ���ܻ��ǿ��������
                {
                    if (chances > 0.7 && cnt_layer[laye] - upper_index < cnt_layer[laye - 1] - lower_index)
                    {
                        upper_index--;
                        chances *= 0.9f;
                    }
                    else if (upper_index == cnt_layer[laye] - 1 && cnt_layer[laye] - upper_index < cnt_layer[laye - 1] - lower_index)
                        upper_index--;
                    else if (chances < 0.7)
                        chances = Random.value;
                }
            }
        }
        If_addressable[14, 0] = 1;
        //DontDestroyOnLoad(e);
        //e.SetActive(true);

        for(int i=0;i<20;i++)
        {
          cnt_layer0[i] = cnt_layer[i];
        }
        for(int i=0;i<20;i++)
        {
          for(int j=0;j<10;j++)
          {
            position_per_layer0[i,j] = position_per_layer[i,j];
          }
        }
        linked0 = linked;//less节点已相连的数目
        aver0 = aver; //平均一个节点的连线数
        cnt_String0 = cnt_String;
        startPoints0 = startPoints;
        endPoints0 = endPoints;
        for(int i=0;i<20;i++)
        {
          for(int j=0;j<10;j++)
          {
            for(int k=0;k<10;k++)
            {
              If_connectted0[i,j,k] = If_connectted[i,j,k]; //第i层j个是否和第i+1层k个相连
            }
          }
        }
        for(int i=0;i<20;i++)
        {
          for(int j=0;j<10;j++)
          {
              type_per0[i,j] = type_per[i,j]; //第i层j个是否和第i+1层k个相连
          }
        }
        for(int i=0;i<20;i++)
        {
          for(int j=0;j<10;j++)
          {
            If_addressable0[i,j] = If_addressable[i,j];
          }
        }
        
        SceneCreateManager.mapGenerated = true;
        
        //else{
        //    if (e != null)
        //    {
        //        e.SetActive(true);  
        //    }
      }
   //     Debug.Log(endPoints.Count);
        LineConnector lineConnector = FindObjectOfType<LineConnector>();
        //  lineConnector.startPoints.AddRange(startPoints);
        //  lineConnector.endPoints.AddRange(endPoints);
        // lineConnector.cnt_Points = cnt_String;
      /*  for (int layer = 1; layer < Point_Layer; layer++)
        {
            for (int point = 0; point < cnt_layer[layer] -1; point++)
            {

            }
        }*/
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}