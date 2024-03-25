using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LineConnector : MonoBehaviour
{
    // Start is called before the first frame update

   // GameObject gameObjectA = GameObject.Find("MapCreate");
    
    public List<Vector3> startPoints ; // ��ʼ���Transform����
    public List<Vector3> endPoints;
    public int cnt_Points;
    private LineRenderer[] lineRenderers;
    public Material defaultLineMaterial;
    private void Awake()
    {
        
        //  Debug.Log(endPoints[13].z);
    }
    private void Update()
    {
        MapCreate mapCreate = FindObjectOfType<MapCreate>();
        cnt_Points = mapCreate.cnt_String;
        lineRenderers = new LineRenderer[cnt_Points+3];
        startPoints = mapCreate.startPoints;
        endPoints = mapCreate.endPoints;
        for (int i = 0; i < cnt_Points; i++)
        {
            GameObject lineObj = new GameObject("Line" + i); // ����һ���µ���Ϸ����������LineRenderer���
            lineRenderers[i] = lineObj.AddComponent<LineRenderer>(); // ���LineRenderer���
            // ����LineRenderer����
            lineRenderers[i].positionCount = 2;
            lineRenderers[i].startWidth = 0.2f;
            lineRenderers[i].endWidth = 0.2f;
            lineRenderers[i].sortingOrder = 2;
            lineRenderers[i].material = defaultLineMaterial;
        }
        //Debug.Log(cnt_Points);
        //Debug.Log(startPoints.Count);
        // �������ͽ�����
        for (int i = 0; i < cnt_Points; i++)
        {
            if (startPoints[i] != null && endPoints[i] != null)
            {
                lineRenderers[i].SetPosition(0, startPoints[i]);
                lineRenderers[i].SetPosition(1, endPoints[i]);
            }
        }

    }
}
