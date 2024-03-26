using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class VolumeControl : MonoBehaviour
{
    public AudioSource audioSource;
    public Slider volumeSlider;

    private void Start()
    {
        // ��ȡ��ƵԴ���
        //audioSource = GameObject.FindWithTag("AudioSource").GetComponent<AudioSource>();

        // ��ȡSlider���
        volumeSlider = GetComponentInChildren<Slider>();

        // ����Slider�ĳ�ʼֵΪ��ƵԴ�ĵ�ǰ����
        audioSource.volume = 0.8f;
        volumeSlider.value = 0.8f;

        // ����Slider��OnValueChanged�¼�����
        volumeSlider.onValueChanged.AddListener(OnVolumeChanged);
    }

    private void Update()
    {
        audioSource.volume = volumeSlider.value;
    }

    private void OnVolumeChanged(float volume)
    {
        // ����������Ϊת�����ֵ
        audioSource.volume = volume;
    }
}
