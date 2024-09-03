#pragma once
#include "DxLib.h"
#include "SceneBase.h"


class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	virtual ~SceneTitle();

	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();


private:

	//�A�j���[�V�����֘A
	void  Animation();

	//�A�j���[�V�����̐i�s
	//���[�v�������ǂ�����Ԃ�
	bool UpdateAnim(int attachNo);

	//�A�j���[�V�����̕ύX
	void ChangeAnim(int animIndex);
private:
	enum state
	{
		kWait,			//�ҋ@��
	};
private:

	//�A�j���[�V�������
	int m_currentAnimNo;	//���݂̃A�j���[�V����
	int m_prevAnimNo;		//�ύX�O�̃A�j���[�V����
	float m_animBlendRate;	//�A�j���[�V������������

	//���
	int m_state;

	//�n���h��
	int m_handle;

	//�t�F�[�h�C���A�A�E�g
	int m_fadeAlpha;

	//���艹
	int m_decisionSE;

	//���f��
	int m_modelHandle;

	//BGM
	int m_bgm;

	//�V�[���I���t���O
	bool m_isSceneEnd;	

	//���f���̍��W
	VECTOR m_pos;
};

