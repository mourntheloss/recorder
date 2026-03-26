#include <opencv2/opencv.hpp>
#include <windows.h>
#include <iostream>

using namespace cv;
using namespace std;

Mat GetScreenShot() {
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    HDC hScreen = GetDC(NULL);
    HDC hDC = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, width, height);
    SelectObject(hDC, hBitmap);

    // Copy screen to bitmap
    BitBlt(hDC, 0, 0, width, height, hScreen, 0, 0, SRCCOPY);

    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;

    Mat mat(height, width, CV_8UC4);
    GetDIBits(hDC, hBitmap, 0, height, mat.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    // clean
    DeleteObject(hBitmap);
    DeleteDC(hDC);
    ReleaseDC(NULL, hScreen);

    // convert rbgfbert
    Mat bgrMat;
    cvtColor(mat, bgrMat, COLOR_BGRA2BGR);
    return bgrMat;
}

int main() {
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    
    // video writer
    VideoWriter writer;
    int codec = VideoWriter::fourcc('M', 'J', 'P', 'G'); // i hate motion jpge
    double fps = 20.0;
    
    bool recording = false;
    cout << "[F1] start | [F2] stop and save | [ESC] quit" << endl;

    while (true) {
        // START RECORDING
        if (GetAsyncKeyState(VK_F1) & 0x8000 && !recording) {
            writer.open("output.avi", codec, fps, Size(width, height), true);
            if (!writer.isOpened()) {
                cout << "Error: Could not open video file for writing!" << endl;
            } else {
                recording = true;
                cout << "● RECORDING..." << endl;
            }
        }

        // STOP RECORDING
        if (GetAsyncKeyState(VK_F2) & 0x8000 && recording) {
            recording = false;
            writer.release();
            cout << "■ SAVED to output.avi" << endl;
        }

        // EXIT
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break;

        // THE CAPTURE LOOP
        if (recording) {
            Mat frame = GetScreenShot();
            writer.write(frame);
        }

        // stop melt processor
        Sleep(1000 / fps); 
    }

    return 0;
}
