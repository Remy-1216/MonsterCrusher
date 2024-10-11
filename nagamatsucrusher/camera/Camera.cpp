#include "Camera.h"
#include "Pad.h"
#include <cmath>
#include "EffekseerForDXLib.h"

namespace
{
	//�J�����̕`��͈�
	constexpr float kMinimumDrawingrange = 100.0f;
	constexpr float kMaximumDrawingrange =10000.0f;

	//�J�����̍��W
	constexpr float kCameraPosY = 500.0f;
	constexpr float kCameraPosZ = -800.0f;

	// ���񑬓x
	constexpr float kAngleSpeed = 0.05f;	

	//�����_�̍��W
	constexpr float kCameraTargetY = 100.0f;

	//�J�����̋���
	constexpr float kCameraDist = 1000.0f;
	//constexpr float kCameraHeight = 100;

	// ����p
	constexpr float kFov = (DX_PI_F / 180.0f) * 60.0f;
	// ��O�N���b�v����
	constexpr float kNear = 0.1f;
	// ���N���b�v����
	constexpr float kFar = 1000.0f;
	// �J�����̏��������_
	constexpr float kTargetX = 100.0f;        // X��
	constexpr float kTargetY = 400.0f;    // Y��
	constexpr float kTargetZ = -870.0f;    // Z��
	// �J�����̒��S���W
	constexpr float kScreenCenterX = 50.0f;    // X���W
	constexpr float kScreenCenterY = 1000.0f;    // Y���W

	constexpr float AngleSpeed = 0.05f;	// ���񑬓x
}


Camera::Camera():m_cameraAngle(0.0f)
{
		

}


Camera::~Camera()
{
	// �����Ȃ�.
}

void Camera::Init()
{
	//���s100�`5000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(kMinimumDrawingrange, kMaximumDrawingrange);


	//������
	m_cameraPos = VGet(0.0f, 0.0f, 0.0f);
	
	m_cameraTarget = VGet(0.0f, 0.0f, 0.0f);

	m_cameraAngle = 0.0f;

	m_pad = 0;
}


void Camera::Update(VECTOR mplayerPos)
{
	m_pad = GetJoypadDirectInputState(DX_INPUT_PAD1,&m_input);
		
	//�E�ɃX�e�B�b�N���X���Ă���
	if (m_input.Rx > 0)
	{
		m_cameraAngle += AngleSpeed;
	}
	//�E�ɃX�e�B�b�N���X���Ă���
		//���ɃX�e�B�b�N���X���Ă���
	if (m_input.Rx < 0)
	{
		m_cameraAngle -= kAngleSpeed;
	}
	
	// �J�������W
	m_cameraPos = VAdd(mplayerPos, VTransform({ 0.0f,  kCameraPosY,kCameraPosZ } ,MGetRotY(m_cameraAngle)));

	// �����_
	m_cameraTarget = VAdd(mplayerPos, VGet(0.0f, kCameraTargetY, 0.0f));

	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();

}

void Camera::Draw()
{

#ifdef _DEBUG
	DrawFormatString(160, 200, GetColor(255, 255, 255), "�J�����̍��W(%.2f,%.2f,%.2f)", m_cameraPos.x, m_cameraPos.y, m_cameraPos.z);

	DrawFormatString(250, 250, GetColor(255, 255, 255), "%d", m_input.Rx);
#endif
}
