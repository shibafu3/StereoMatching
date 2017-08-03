#ifdef _DEBUG
//Debug���[�h�̏ꍇ
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_world300d.lib")            // opencv_core
#else
//Release���[�h�̏ꍇ
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_world300.lib")

#pragma comment(lib, "C:\\Program Files\\Anaconda3\\libs\\python35.lib")
#endif

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace std;

int main(){
	//���E�摜�ǂݍ���
	cv::Mat image_l = cv::imread("C:\\Users\\0133752\\Desktop\\tsukuba_l.png");
	cv::Mat image_r = cv::imread("C:\\Users\\0133752\\Desktop\\tsukuba_r.png");

	//�e��p�����[�^
	int window_size = 3;
	int minDisparity = 0;
	int numDisparities = 32;
	int blockSize = 3;
	int P1 = 8 * 3 * window_size * window_size;
	int P2 = 32 * 3 * window_size * window_size;
	int disp12MaxDiff = 1;
	int preFilterCap = 0;
	int uniquenessRatio = 10;
	int speckleWindowSize = 100;
	int speckleRange = 32;

	//�X�e���IBM�̃C���X�^���X��
	//StereoBM��create���\�b�h�Ń|�C���^���擾
	//���̍ۂɊe��p�����[�^�������ɓ���
	//StereoSGBM::create(
	//	minDisparity---------- - int�A�ŏ������l�A�V�t�g�i���s�ړ��j�̂Ȃ��ꍇ�ɂ�0�ŗǂ�
	//	numDisparities-------- - int�A�ő压���l�ƍŏ������l�̍��A16�̔{���ɂ���
	//	blockSize------------ - int�AOpenCV2.4�ł́ASADWindowSize�ƌĂ΂�Ă����A3�`11�̊�AStereoBM�ƈقȂ�A3����g�p�ł���
	//	P1 = 0 --------------int�A�f�t�H���g�̂܂܂ŗǂ�����
	//	P2 = 0 -------------- - int�A ����
	//	disp12MaxDiff = 0 ------int�A ���E�̎����̋��e�ő�l�A�f�t�H���g�̓`�F�b�N�Ȃ��A�f�t�H���g�ŗǂ�����
	//	preFilterCap = 0 ------ - int�A ���O�Ƀt�B���^�ő傫�Ȏ������N���b�v����A�f�t�H���g�ŗǂ�����
	//	uniquenessRatio = 0 ----int�A�ړI�֐��l�̎��_�Ƃ̍��́��䗦�A0�͔�r���Ȃ��ӁA�f�t�H���g�ŏ\���ł�����
	//	speckleWindowSize = 0 --int�A���������_��m�C�Y�������t�B���^�̃T�C�Y�A�f�t�H���g��0�͎g�p���Ȃ��ӁA����͎g���Ĕ��Ɍ��ʂ��������B
	//	speckleRange = 0 ------int�A��L�t�B���^���g�p����Ƃ��́A�����̍ő�l�A1�`2�������ŁA16�{�����A1���ǂ�����
	//	mode = StereoSGBM::MODE_SGBM----int�AOpenCV2.4�ł́AfullDP = false�Ƃ���Ă������́A�t���X�P�[���̂Q�p�X�E�_�C�i�~�b�N�v���O���~���O�����s������ɂ́AStereoSGBM::MODE_HH���g�p
	//)
	cv::Ptr<cv::StereoSGBM> ssgbm = cv::StereoSGBM::create(
		minDisparity,
		numDisparities,
		blockSize,
		P1,
		P2,
		disp12MaxDiff,
		preFilterCap,
		uniquenessRatio,
		speckleWindowSize, speckleRange, cv::StereoSGBM::MODE_SGBM);
	//set���\�b�h���g���Ă��ݒ�ł���B
	ssgbm->setSpeckleWindowSize(100);
	ssgbm->setSpeckleRange(1);

	//���E�摜����[�x�}�b�v���쐬
	cv::Mat disparity;    //((cv::MatSize)leftImg.size, CV_16S);
	ssgbm->compute(image_l, image_r, disparity);
	
	//�[�x�}�b�v�����o�I�ɕ�����悤�Ƀs�N�Z���l��ϊ�
	cv::Mat disparity_map;
	double min, max;
	//�[�x�}�b�v�̍ŏ��l�ƍő�l�����߂�
	cv::minMaxLoc(disparity, &min, &max);
	//CV_8UC1�ɕϊ��A�ő僌���W��0�`255�ɂ���
	disparity.convertTo(disparity_map, CV_8UC1, 255.0 / (max - min), -255.0 * min / (max - min));
	cv::imshow("result", disparity_map);
	cv::waitKey(0);

	return 0;
}