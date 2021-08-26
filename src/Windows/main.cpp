#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include  <atlstr.h>
#include "BetterPrint.h"
using namespace cv;
using namespace std;

Mat ResizeImg(Mat s_img);
Mat ToBlack(Mat img);
void OpenComm(CString comm_name, int baud_rate = 9600);
int pic_size[] = { 64, 128 }; // 2x + 10
int frame_count = 0;
HANDLE hCom;
DCB dcb;

int main()
{
    printf("按下任意键开始\n");
    system("pause");

    OpenComm("COM4", 255000);

    VideoCapture video("zuocao.mp4");
    register Mat img;
    video.read(img);
    system("cls");

    while (!img.empty())
    {
        register std::string img_data = "";
        //每n帧输出一次
        for (int i = 0; i <= 2; i++)
        {
            video.read(img);
        }
        img = ResizeImg(img);
        img = ToBlack(img); //转黑白图像
        //imshow("test", img);
        //waitKey(33);
        for (int i = 0; i < img.rows; i++)
        {
            for (int j = 0; j < img.cols; j++)
            {
                //MoveCursor(j, i);
                if (img.at<uchar>(i, j) == 0)
                {
                    //printf("■");
                    img_data += "1";
                }
                else
                {
                    //printf(" ");
                    img_data += "0";
                }
            }
        }
        img_data[img_data.length() - 1] = 's';
        img_data[0] = 'b';
        WriteFile(hCom, img_data.c_str(), (DWORD)img_data.length(), NULL, NULL);
        //std::cout << img_data;
        //break;
        frame_count++;
        printf("\r第%d帧传输完成", frame_count);
        Sleep(33);
    }
    return 0;
}

Mat ResizeImg(Mat s_img)
{
    Mat d_img = Mat::zeros(pic_size[0], pic_size[1], CV_8U);
    resize(s_img, d_img, d_img.size());
    return d_img;
}

Mat ToBlack(Mat img)
{
    Mat gray_image;
    cvtColor(img, gray_image, CV_BGR2GRAY);
    threshold(gray_image, gray_image, 100, 200, THRESH_BINARY); //二值化图像
    return gray_image;
}

void OpenComm(CString comm_name, int baud_rate)
{
    comm_name = "\\\\.\\" + comm_name;
    START:
    hCom = CreateFile(comm_name, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hCom == (HANDLE)-1)
    {
        printf("串口打开失败，按任意键重试\n");
        system("pause");
        goto START;
    }
    else
    {
        SetupComm(hCom, 1024, 1024);
        GetCommState(hCom, &dcb);
        dcb.BaudRate = baud_rate; //波特率
        dcb.ByteSize = 8;
        dcb.Parity = NOPARITY;
        dcb.StopBits = 0;
        SetCommState(hCom, &dcb);
        PurgeComm(hCom, PURGE_TXCLEAR);
    }
}
