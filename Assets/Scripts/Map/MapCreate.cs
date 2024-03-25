using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Unity.VisualScripting;
/*using static UnityEditor.Experimental.GraphView.GraphView;*/

public struct data
{
    public int x, y;
}
public class MapCreate : MonoBehaviour
{
    // Start is called before the first frame update
    const int Max_Point = 7; //每行的点数
    const int Min_Point = 2;
    const int Point_Layer = 15;
  
    const float monster_poss = 0.60f;
    const float elite_poss = 0.18f;
    const float rest_poss = 0.10f;
    const float shop_poss = 0.1f;
    const float event_poss = 0.15f;
    int[] cnt_layer = new int[20];
    data[,] position_per_layer = new data[20,10];
    int lower_index = 0; // 下面的已经连到第几个了
    int linked = 1;//less节点已相连的数目
    float aver ; //平均一个节点的连线数
    public int cnt_String = 0;
    public List<Vector3> startPoints;
    public List<Vector3> endPoints;
    private float chances;
    void Start()
    {
        int Chest_Layer = UnityEngine.Random.Range(5, 10);//宝箱层在5-9层之间
        Vector3 p;
        p.z = 0;
        for (int layer = 0; layer < Point_Layer; layer++)
        {
            int point_num = UnityEngine.Random.Range(2,8);
            if (layer == 14) point_num = 1;
            cnt_layer[layer] = point_num;
            List<(int, int)> position_per = new List<(int, int)>();
            for (int point = 0; point < point_num; point++)
            {
                p.x = -20 + 50 / point_num * point + UnityEngine.Random.Range(-2, 2);
                p.y = -113 + 140 / Point_Layer * layer + UnityEngine.Random.Range(-1, 1);
            
                if (layer == 13)
                {
                    GameObject e = Instantiate(Resources.Load("rest") as GameObject);
                    e.name = "rest:layer = " + layer;
                    e.transform.localPosition = p;
                }
                else if (layer == Chest_Layer)
                {
                    GameObject e = Instantiate(Resources.Load("chest") as GameObject);
                    e.name = "chest:layer = " + layer;
                    e.transform.localPosition = p;
                }
                else if (layer == 0)
                {
                    GameObject e = Instantiate(Resources.Load("monster") as GameObject);
                    e.name = "monster:layer = " + layer;
                    e.transform.localPosition = p;
                }
                else if (layer == 14)
                {
                    GameObject e = Instantiate(Resources.Load("boss") as GameObject);
                    p.x = 0;
                    p.y = 13;
                    e.name ="Boss:layer = " + layer;
                    e.transform.localPosition = p;

                }
                else
                {
                    float rand_num = Random.value;
                    if (layer < 3)
                    {
                        while (rand_num > monster_poss && rand_num < monster_poss + elite_poss + rest_poss)
                            rand_num = Random.value;
                    }
                    if (rand_num < monster_poss)
                    {
                        GameObject e = Instantiate(Resources.Load("monster") as GameObject);
                        e.name = "monster:layer = " + layer;
                        e.transform.localPosition = p;
                    }
                    else if(rand_num < monster_poss+elite_poss)
                    {
                        GameObject e = Instantiate(Resources.Load("elite") as GameObject); // 这部分未完成
                        e.transform.localPosition = p;
                        e.name = "elite:layer = " + layer;
                    }
                    else if(rand_num < monster_poss + elite_poss + rest_poss)
                    {
                        GameObject e = Instantiate(Resources.Load("rest") as GameObject);
                        e.name = "rest:layer = " + layer;
                        e.transform.localPosition = p;
                    }
                    else
                    {
                        GameObject e = Instantiate(Resources.Load("shop") as GameObject);
                        e.name = "shop:layer = " + layer;
                        e.transform.localPosition = p;
                    }
                }
                position_per_layer[layer,point].x = (int)p.x;
                position_per_layer[layer, point].y = (int)p.y;
                
            }
        }
        for (int layer = 1;layer < Point_Layer ;layer++)
        {
            lower_index = 0;
            linked = 1;
            aver = cnt_layer[layer] / cnt_layer[linked - 1];
            startPoints.Add(new Vector3(position_per_layer[layer, 0].x, position_per_layer[layer, 0].y, 0));
            endPoints.Add(new Vector3(position_per_layer[layer - 1, 0].x, position_per_layer[layer - 1, 0].y, 0));
            cnt_String++;// 每次都应该用多的去连少的或者增添一个概率机制
            chances = Random.value;
            for (int upper_index = 0;upper_index < cnt_layer[layer];upper_index++)
            {
                if (cnt_layer[layer] - upper_index < cnt_layer[layer - 1] - lower_index)
                {
                    lower_index++;
                    startPoints.Add(new Vector3(position_per_layer[layer, upper_index].x, position_per_layer[layer, upper_index].y, 0));
                    endPoints.Add(new Vector3(position_per_layer[layer - 1, lower_index].x, position_per_layer[layer - 1, lower_index].y, 0));
                    cnt_String++;
                    linked = 1;
                    if(layer == 1)
                    {
                        Debug.Log(cnt_layer[layer - 1] - lower_index);
                    }
                }
                else {
                    float rand_num = Random.value + (aver - linked) * 0.5f;
                    if(rand_num > 0.75 || lower_index == cnt_layer[layer-1]-1)
                    {
                        startPoints.Add(new Vector3(position_per_layer[layer, upper_index].x, position_per_layer[layer, upper_index].y, 0));
                        endPoints.Add(new Vector3(position_per_layer[layer - 1, lower_index].x, position_per_layer[layer - 1, lower_index].y, 0));
                        cnt_String++;
                        linked++;
                    }
                    else
                    {
                        lower_index++;
                        startPoints.Add(new Vector3(position_per_layer[layer, upper_index].x, position_per_layer[layer, upper_index].y, 0));
                        endPoints.Add(new Vector3(position_per_layer[layer - 1, lower_index].x, position_per_layer[layer - 1, lower_index].y, 0));
                        cnt_String++;
                        linked = 1;
                    }
                }
                if (cnt_layer[layer] < cnt_layer[layer-1])  // 增加机制 可能还是考虑这个点
                {
                    if (chances > 0.7 && cnt_layer[layer] - upper_index < cnt_layer[layer - 1] - lower_index)
                    {
                        upper_index--;
                        chances *= 0.9f;
                    }
                    else if (upper_index == cnt_layer[layer] - 1 && cnt_layer[layer] - upper_index < cnt_layer[layer - 1] - lower_index)
                        upper_index--;
                    else if (chances < 0.7)
                        chances = Random.value;
                }
            }
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