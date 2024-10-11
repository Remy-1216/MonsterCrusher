#pragma once

#include "DxLib.h"

class Player;
class Camera
{
public:
	Camera();
	virtual ~Camera();

    void Init();

	void Update(VECTOR mplayerPos);

    void Draw();

    //�J�����̃A���O�����擾����
    float GetCameraAngle() const noexcept { return m_cameraAngle; }
private:

    //�W���C�p�b�h�̏o��
    DINPUT_JOYSTATE m_input;

    //�W���C�p�b�h�̏o�͂̕ێ�
    int m_pad;

    //�J�����A���O���̈ʒu
    float m_cameraAngle;

    //�J�����̍��W
    VECTOR m_cameraPos;

    //�J�����̒����_
    VECTOR m_cameraTarget;

    //�v���C���[�̍��W
    VECTOR m_playerTarget;

  
};

