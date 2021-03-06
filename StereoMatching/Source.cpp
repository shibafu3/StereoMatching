#ifdef _DEBUG
//Debugモードの場合
#pragma comment(lib,"C:\\opencv\\build\\x86\\vc12\\lib\\opencv_world300d.lib")            // opencv_core
#else
//Releaseモードの場合
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
	//左右画像読み込み
	cv::Mat image_l = cv::imread("C:\\Users\\0133752\\Desktop\\tsukuba_l.png");
	cv::Mat image_r = cv::imread("C:\\Users\\0133752\\Desktop\\tsukuba_r.png");

	//各種パラメータ
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

	//ステレオBMのインスタンス化
	//StereoBMのcreateメソッドでポインタを取得
	//その際に各種パラメータを引数に入力
	//StereoSGBM::create(
	//	minDisparity---------- - int、最小視差値、シフト（平行移動）のない場合には0で良い
	//	numDisparities-------- - int、最大視差値と最小視差値の差、16の倍数にする
	//	blockSize------------ - int、OpenCV2.4では、SADWindowSizeと呼ばれていた、3〜11の奇数、StereoBMと異なり、3から使用できる
	//	P1 = 0 --------------int、デフォルトのままで良かった
	//	P2 = 0 -------------- - int、 同上
	//	disp12MaxDiff = 0 ------int、 左右の視差の許容最大値、デフォルトはチェックなし、デフォルトで良かった
	//	preFilterCap = 0 ------ - int、 事前にフィルタで大きな視差をクリップする、デフォルトで良かった
	//	uniquenessRatio = 0 ----int、目的関数値の次点との差の％比率、0は比較しない意、デフォルトで十分であった
	//	speckleWindowSize = 0 --int、小さい斑点やノイズを消すフィルタのサイズ、デフォルトの0は使用しない意、これは使って非常に効果があった。
	//	speckleRange = 0 ------int、上記フィルタを使用するときの、視差の最大値、1〜2が推奨で、16倍される、1が良かった
	//	mode = StereoSGBM::MODE_SGBM----int、OpenCV2.4では、fullDP = falseとされていたもの、フルスケールの２パス・ダイナミックプログラミングを実行させるには、StereoSGBM::MODE_HHを使用
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
	//setメソッドを使っても設定できる。
	ssgbm->setSpeckleWindowSize(100);
	ssgbm->setSpeckleRange(1);

	//左右画像から深度マップを作成
	cv::Mat disparity;    //((cv::MatSize)leftImg.size, CV_16S);
	ssgbm->compute(image_l, image_r, disparity);
	
	//深度マップを視覚的に分かるようにピクセル値を変換
	cv::Mat disparity_map;
	double min, max;
	//深度マップの最小値と最大値を求める
	cv::minMaxLoc(disparity, &min, &max);
	//CV_8UC1に変換、最大レンジを0〜255にする
	disparity.convertTo(disparity_map, CV_8UC1, 255.0 / (max - min), -255.0 * min / (max - min));
	cv::imshow("result", disparity_map);
	cv::waitKey(0);

	return 0;
}