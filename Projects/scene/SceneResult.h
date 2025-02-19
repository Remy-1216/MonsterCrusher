#pragma once
#include "SceneBase.h"
class SceneResult :public SceneBase
{
public:
	SceneResult();
	~SceneResult();

	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();

private:

	//�A�j���[�V����
	void Animation();

	//�t�F�[�h�C���A�t�F�[�h�A�E�g
	void Fade();

private:
	//�w�i�̃n���h��
	int m_handle;
	
	//�i�C�g�̃��f��
	int m_knightHandle;

	//�n�`�̃n���h��
	int m_beeHandle;

	//�X���C���̃n���h��
	int m_slimeHandle;

	//�X�e�[�W
	int m_stageHandle;

	//BGM
	int m_bgm;

	//�A�^�b�`�����A�j���[�V�����̍Đ����Ԃ�ۑ�����
	int m_totalTime;

	//�A�^�b�`�����n�`�̃A�j���[�V�����̍Đ����Ԃ�ێ�����
	int m_beeTotalTime;

	//�A�^�b�`�����X���C���̃A�j���[�V�����̍Đ����Ԃ�ێ�����
	int m_slimeTotalTime;

	//�i�C�g�̃A�j���[�V�����i���o�[
	int m_attachIndex;

	//�n�`�̃A�j���[�V�����i���o�[
	int m_beeAttachIndex;

	//�X���C���A�j���[�V�����i���o�[
	int m_slimeAttachIndex;

	//�t�F�[�h�C���A�A�E�g
	int m_fadeAlpha;

	//�i�C�g�̍Đ�����
	float m_playTime;

	//�n�`�̍Đ�����
	float m_beePlayTime;

	//�X���C���̍Đ�����
	float m_slimePlayTime;

	//�^�C�g����ʂɖ߂邩
	bool m_isSceneEnd;
	

	//�X�e�[�W�̈ʒu
	VECTOR m_pos;

	//�i�C�g�̈ʒu
	VECTOR m_knightPos;

	//�n�`�̍��W
	VECTOR m_beePos;

	//�X���C���̍��W
	VECTOR m_slimePos;

	//�J�����̍��W
	VECTOR m_cameraPos;
	VECTOR m_cameraTarget;

};

