#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
using namespace std;
using namespace cv;

int edgeThick = 3;

Mat based(500 + edgeThick * 2, 500 + 400 + edgeThick * 2, CV_8UC3, Scalar(255, 255, 255));
Point ptOld;
void on_mouse(int evnet, int x, int y, int flags, void* userdata);
void edgeLine(Mat img);
void drawText(Mat img, const string text);

// 그리기 영역
Rect rectDrawing(0, 0, 500 + edgeThick * 2, 500 + edgeThick * 2);
Rect drawingSpace(edgeThick, edgeThick, 500, 500);

// 1열 메뉴
Rect rectSave(500 + edgeThick * 2, 0, 200, 100);
Rect rectLoad(500 + edgeThick * 2, 100, 200, 100);
Rect rectClear(500 + edgeThick * 2, 200, 200, 100);
Rect rectRun(500 + edgeThick * 2, 300, 200, 100 + edgeThick * 2);
Rect rectExit(500 + edgeThick * 2, 400 + edgeThick * 2, 200, 100);

// 2열 메뉴
Rect rectContours(500 + 200 + edgeThick * 2, 0, 200, 100);
Rect rectCenter(500 + 200 + edgeThick * 2, 100, 200, 100);
Rect rectMidLine(500 + 200 + edgeThick * 2, 200, 200, 100);
Rect rectRatio(500 + 200 + edgeThick * 2, 300, 200, 100 + edgeThick * 2);
Rect rectHalf(500 + 200 + edgeThick * 2, 400 + edgeThick * 2, 200, 100);

int main(void)
{
    Mat numberDrawing(500 + edgeThick * 2, 500 + edgeThick * 2, CV_8UC3, Scalar(255, 255, 255));
    edgeLine(numberDrawing);
    numberDrawing.copyTo(based(rectDrawing));

    Mat numberSave(100, 200, CV_8UC3, Scalar(255, 255, 255));
    edgeLine(numberSave);
    drawText(numberSave, "Save");
    numberSave.copyTo(based(rectSave));

    Mat numberLoad(100, 200, CV_8UC3, Scalar(255, 255, 255));
    edgeLine(numberLoad);
    drawText(numberLoad, "Load");
    numberLoad.copyTo(based(rectLoad));

    Mat numberClear(100, 200, CV_8UC3, Scalar(255, 255, 255));
    edgeLine(numberClear);
    drawText(numberClear, "Clear");
    numberClear.copyTo(based(rectClear));

    Mat numberRun(100 + edgeThick * 2, 200, CV_8UC3, Scalar(255, 255, 255));
    edgeLine(numberRun);
    drawText(numberRun, "Run");
    numberRun.copyTo(based(rectRun));

    Mat numberExit(100, 200, CV_8UC3, Scalar(255, 255, 255));
    edgeLine(numberExit);
    drawText(numberExit, "Exit");
    numberExit.copyTo(based(rectExit));

    Mat numberContours(100, 200, CV_8UC3, Scalar(255, 255, 255));
    edgeLine(numberContours);
    drawText(numberContours, "Contours");
    numberContours.copyTo(based(rectContours));

    Mat numberCenter(100, 200, CV_8UC3, Scalar(255, 255, 255));
    edgeLine(numberCenter);
    drawText(numberCenter, "Center");
    numberCenter.copyTo(based(rectCenter));

    Mat numberMidLine(100, 200, CV_8UC3, Scalar(255, 255, 255));
    edgeLine(numberMidLine);
    drawText(numberMidLine, "MidLine");
    numberMidLine.copyTo(based(rectMidLine));

    Mat numberRatio(100 + edgeThick * 2, 200, CV_8UC3, Scalar(255, 255, 255));
    edgeLine(numberRatio);
    drawText(numberRatio, "Ratio");
    numberRatio.copyTo(based(rectRatio));

    Mat numberHalf(100, 200, CV_8UC3, Scalar(255, 255, 255));
    edgeLine(numberHalf);
    drawText(numberHalf, "Half");
    numberHalf.copyTo(based(rectHalf));

    Mat clearBased = based.clone();

    namedWindow("numberWindow");
    setMouseCallback("numberWindow", on_mouse, &clearBased);
    imshow("numberWindow", based);
    waitKey(0);

    return 0;
}

void on_mouse(int event, int x, int y, int flags, void* userdata)
{
    Mat* clearBased = static_cast<Mat*>(userdata);

    switch (event)
    {
    case EVENT_LBUTTONDOWN:
        if (drawingSpace.contains(Point(x, y))) {
            ptOld = Point(x, y);
        }
        else if (rectSave.contains(Point(x, y)))
        {
            string filename;
            cout << "저장할 파일명 입력 : ";
            getline(cin, filename);
            if (filename == "cancel") break;
            bool saving = imwrite(filename, based(drawingSpace));
            if (saving) cout << filename << " 파일이 저장됨." << endl;
            else cout << filename << " 파일 저장실패." << endl;
        }
        else if (rectLoad.contains(Point(x, y)))
        {
            string loadFileName;
            cout << "불러올 파일명을 입력 : ";
            getline(cin, loadFileName);
            if (loadFileName == "cancel") break;
            Mat temp = imread(loadFileName, IMREAD_COLOR);
            if (temp.empty()) cout << loadFileName << " 불러오기 실패." << endl;
            else cout << loadFileName << " 불러옴." << endl;
            temp.copyTo(based(drawingSpace));
            imshow("numberWindow", based);
        }
        else if (rectClear.contains(Point(x, y)))
        {
            clearBased->copyTo(based);
            imshow("numberWindow", based);
            cout << "입력창 삭제됨." << endl;
        }
        else if (rectExit.contains(Point(x, y)))
        {
            cout << "프로그램 종료" << endl;
            destroyAllWindows();
        }
        else if (rectContours.contains(Point(x, y)))
        {
            Mat gray;
            cvtColor(based(drawingSpace), gray, COLOR_BGR2GRAY);
            gray = ~gray;
            Mat bi;
            threshold(gray, bi, 0, 255, THRESH_BINARY | THRESH_OTSU);
            vector<vector<Point>> contours;
            findContours(bi, contours, RETR_TREE, CHAIN_APPROX_NONE);
            cout << "외각선의 개수 : " << contours.size() << endl;
        }
        else if (rectCenter.contains(Point(x, y)))
        {
            Mat gray;
            cvtColor(based(drawingSpace), gray, COLOR_BGR2GRAY);

            Mat binary;
            threshold(gray, binary, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

            vector<vector<Point>> contours;
            findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

            if (contours.size() > 0)
            {
                int largest_contour_index = 0;
                double largest_area = 0.0;

                for (int i = 0; i < contours.size(); i++)
                {
                    double area = contourArea(contours[i]);
                    if (area > largest_area)
                    {
                        largest_area = area;
                        largest_contour_index = i;
                    }
                }
                RotatedRect rotated_rect = minAreaRect(contours[largest_contour_index]);

                Point2f vertices[4];    //왼쪽부터 시계방향
                rotated_rect.points(vertices);
                Mat dst;
                cvtColor(binary, dst, COLOR_GRAY2BGR);
                for (int i = 0; i < 4; i++)
                {
                    line(dst, vertices[i], vertices[(i + 1) % 4], Scalar(0, 0, 255), 2);
                }

                Rect bounding_box = boundingRect(contours[largest_contour_index]);
                rectangle(dst, bounding_box, Scalar(0, 255, 0), 2);

                imshow("Bounding Box", dst);

                // Calculate the slope (angle) using vertices[0] and vertices[1]
                float dx = vertices[1].x - vertices[0].x;
                float dy = vertices[1].y - vertices[0].y;
                float angle = atan2(dy, dx) * 180.0 / CV_PI;

                // Normalize the angle to be within 0 to 180 degrees
                if (angle < -45) {
                    angle += 90;
                    swap(rotated_rect.size.width, rotated_rect.size.height);
                }

                // Get the rotation matrix
                Mat M = getRotationMatrix2D(rotated_rect.center, angle, 1.0);

                // Rotate the image
                Mat rotated;
                warpAffine(based(drawingSpace), rotated, M, based(drawingSpace).size(), INTER_CUBIC, BORDER_REFLECT);

                Mat rotated_gray;
                cvtColor(rotated, rotated_gray, COLOR_BGR2GRAY);
                Mat rotated_binary;
                threshold(rotated_gray, rotated_binary, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

                //회전된 이미지 외곽선 검출
                vector<vector<Point>> rotated_contours;
                findContours(rotated_binary, rotated_contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

                if (rotated_contours.size() > 0)
                {
                    int largest_contour_index_rotated = 0;
                    double largest_area_rotated = 0.0;

                    for (int i = 0; i < rotated_contours.size(); i++)
                    {
                        double area = contourArea(rotated_contours[i]);
                        if (area > largest_area_rotated)
                        {
                            largest_area_rotated = area;
                            largest_contour_index_rotated = i;
                        }
                    }

                    Rect bounding_box_rotated = boundingRect(rotated_contours[largest_contour_index_rotated]);
                    if (bounding_box_rotated.width / bounding_box_rotated.height > 1.75) {
                        int tmp = 0;
                        // Rotate 90 degrees
                        Mat rotated_90;
                        rotate(rotated, rotated_90, ROTATE_90_CLOCKWISE);
                        rotated = rotated_90;
                        rotated_binary = rotated_binary.t();
                        flip(rotated_binary, rotated_binary, 1);
                        findContours(rotated_binary, rotated_contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
                        bounding_box_rotated = boundingRect(rotated_contours[0]);
                    }
                    Mat cop(bounding_box_rotated.width, bounding_box_rotated.height, rotated.type());



                    cvtColor(rotated(bounding_box_rotated), cop, COLOR_BGR2GRAY);
                    cop = ~cop;
                    threshold(cop, cop, 0, 255, THRESH_BINARY | THRESH_OTSU);
                    findContours(cop, rotated_contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
                    Mat cop_color;
                    cvtColor(cop, cop_color, COLOR_GRAY2BGR);

                    // 외곽선이 두개일때 내부 외곽선 무게중심 검출
                    Moments mu;
                    Point2f mc;
                    if (rotated_contours.size() == 2)
                    {
                        // 내부 외곽선의 무게 중심 계산
                        mu = moments(rotated_contours[1]);
                        mc = Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00);

                        // 빨간 점으로 무게 중심 표시
                        circle(cop_color, mc, 5, Scalar(0, 0, 255), -1);
                    }

                    // 외부 외곽선의 무게 중심 계산
                    Moments mu_outer = moments(rotated_contours[0]);
                    Point2f mc_outer(mu_outer.m10 / mu_outer.m00, mu_outer.m01 / mu_outer.m00);
                    cout << mc_outer << endl;

                    // 빨간 점으로 외부 외곽선의 무게 중심 표시
                    circle(cop_color, mc_outer, 5, Scalar(0, 255, 0), -1);

                    imshow("rotated", rotated);
                    imshow("cop", cop_color);
                }
            }
        }
        else if (rectMidLine.contains(Point(x, y)))
        {
            Mat gray;
            cvtColor(based(drawingSpace), gray, COLOR_BGR2GRAY);

            Mat binary;
            threshold(gray, binary, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

            vector<vector<Point>> contours;
            findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

            if (contours.size() > 0)
            {
                int largest_contour_index = 0;
                double largest_area = 0.0;

                for (int i = 0; i < contours.size(); i++)
                {
                    double area = contourArea(contours[i]);
                    if (area > largest_area)
                    {
                        largest_area = area;
                        largest_contour_index = i;
                    }
                }
                RotatedRect rotated_rect = minAreaRect(contours[largest_contour_index]);

                Point2f vertices[4];    //왼쪽부터 시계방향
                rotated_rect.points(vertices);
                Mat dst;
                cvtColor(binary, dst, COLOR_GRAY2BGR);
                for (int i = 0; i < 4; i++)
                {
                    line(dst, vertices[i], vertices[(i + 1) % 4], Scalar(0, 0, 255), 2);
                }

                Rect bounding_box = boundingRect(contours[largest_contour_index]);
                rectangle(dst, bounding_box, Scalar(0, 255, 0), 2);

                imshow("Bounding Box", dst);

                // Calculate the slope (angle) using vertices[0] and vertices[1]
                float dx = vertices[1].x - vertices[0].x;
                float dy = vertices[1].y - vertices[0].y;
                float angle = atan2(dy, dx) * 180.0 / CV_PI;

                // Normalize the angle to be within 0 to 180 degrees
                if (angle < -45) {
                    angle += 90;
                    swap(rotated_rect.size.width, rotated_rect.size.height);
                }

                // Get the rotation matrix
                Mat M = getRotationMatrix2D(rotated_rect.center, angle, 1.0);

                // Rotate the image
                Mat rotated;
                warpAffine(based(drawingSpace), rotated, M, based(drawingSpace).size(), INTER_CUBIC, BORDER_REFLECT);

                Mat rotated_gray;
                cvtColor(rotated, rotated_gray, COLOR_BGR2GRAY);
                Mat rotated_binary;
                threshold(rotated_gray, rotated_binary, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

                //회전된 이미지 외곽선 검출
                vector<vector<Point>> rotated_contours;
                findContours(rotated_binary, rotated_contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

                if (rotated_contours.size() > 0)
                {
                    int largest_contour_index_rotated = 0;
                    double largest_area_rotated = 0.0;

                    for (int i = 0; i < rotated_contours.size(); i++)
                    {
                        double area = contourArea(rotated_contours[i]);
                        if (area > largest_area_rotated)
                        {
                            largest_area_rotated = area;
                            largest_contour_index_rotated = i;
                        }
                    }

                    Rect bounding_box_rotated = boundingRect(rotated_contours[largest_contour_index_rotated]);
                    if (bounding_box_rotated.width > bounding_box_rotated.height) {
                        int tmp = 0;
                        // Rotate 90 degrees
                        Mat rotated_90;
                        rotate(rotated, rotated_90, ROTATE_90_CLOCKWISE);
                        rotated = rotated_90;
                        rotated_binary = rotated_binary.t();
                        flip(rotated_binary, rotated_binary, 1);
                        findContours(rotated_binary, rotated_contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
                        bounding_box_rotated = boundingRect(rotated_contours[0]);
                    }
                    Mat cop(bounding_box_rotated.width, bounding_box_rotated.height, rotated.type());

                    cvtColor(rotated(bounding_box_rotated), cop, COLOR_BGR2GRAY);
                    cop = ~cop;
                    threshold(cop, cop, 0, 255, THRESH_BINARY | THRESH_OTSU);
                    int crossCount = 0;
                    int PreValue = 0;
                    int CrnValue = 0;
                    for (int y = 1; y < cop.rows; y++)
                    {
                        CrnValue = cop.at<uchar>(y, cop.cols / 2);
                        //cout << CrnValue << endl;
                        if (CrnValue == 255 && PreValue == 0) crossCount++;
                        PreValue = CrnValue;
                    }
                    cvtColor(cop, cop, COLOR_GRAY2BGR);
                    //line(cop, Point(cop.cols - 1, 0), Point(0, cop.rows - 1), Scalar(0, 255, 0));
                    line(cop, Point(cop.cols / 2, 0), Point(cop.cols / 2, cop.rows - 1), Scalar(0, 0, 255));
                    cout << "중앙에 직선을 그었을때 교차하는 횟수 : " << crossCount << endl;
                    imshow("cop", cop);
                }
            }
        }
        else if (rectRatio.contains(Point(x, y)))
        {
            Mat gray;
            cvtColor(based(drawingSpace), gray, COLOR_BGR2GRAY);

            Mat binary;
            threshold(gray, binary, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

            vector<vector<Point>> contours;
            findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

            if (contours.size() > 0)
            {
                int largest_contour_index = 0;
                double largest_area = 0.0;

                for (int i = 0; i < contours.size(); i++)
                {
                    double area = contourArea(contours[i]);
                    if (area > largest_area)
                    {
                        largest_area = area;
                        largest_contour_index = i;
                    }
                }
                RotatedRect rotated_rect = minAreaRect(contours[largest_contour_index]);

                Point2f vertices[4];    //왼쪽부터 시계방향
                rotated_rect.points(vertices);
                Mat dst;
                cvtColor(binary, dst, COLOR_GRAY2BGR);
                for (int i = 0; i < 4; i++)
                {
                    line(dst, vertices[i], vertices[(i + 1) % 4], Scalar(0, 0, 255), 2);
                }

                Rect bounding_box = boundingRect(contours[largest_contour_index]);
                rectangle(dst, bounding_box, Scalar(0, 255, 0), 2);

                imshow("Bounding Box", dst);

                // Calculate the slope (angle) using vertices[0] and vertices[1]
                float dx = vertices[1].x - vertices[0].x;
                float dy = vertices[1].y - vertices[0].y;
                float angle = atan2(dy, dx) * 180.0 / CV_PI;

                // Normalize the angle to be within 0 to 180 degrees
                if (angle < -45) {
                    angle += 90;
                    swap(rotated_rect.size.width, rotated_rect.size.height);
                }

                // Get the rotation matrix
                Mat M = getRotationMatrix2D(rotated_rect.center, angle, 1.0);

                // Rotate the image
                Mat rotated;
                warpAffine(based(drawingSpace), rotated, M, based(drawingSpace).size(), INTER_CUBIC, BORDER_REFLECT);

                Mat rotated_gray;
                cvtColor(rotated, rotated_gray, COLOR_BGR2GRAY);
                Mat rotated_binary;
                threshold(rotated_gray, rotated_binary, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

                //회전된 이미지 외곽선 검출
                vector<vector<Point>> rotated_contours;
                findContours(rotated_binary, rotated_contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

                if (rotated_contours.size() > 0)
                {
                    int largest_contour_index_rotated = 0;
                    double largest_area_rotated = 0.0;

                    for (int i = 0; i < rotated_contours.size(); i++)
                    {
                        double area = contourArea(rotated_contours[i]);
                        if (area > largest_area_rotated)
                        {
                            largest_area_rotated = area;
                            largest_contour_index_rotated = i;
                        }
                    }

                    Rect bounding_box_rotated = boundingRect(rotated_contours[largest_contour_index_rotated]);
                    if (bounding_box_rotated.width > bounding_box_rotated.height) {
                        int tmp = 0;
                        // Rotate 90 degrees
                        Mat rotated_90;
                        rotate(rotated, rotated_90, ROTATE_90_CLOCKWISE);
                        rotated = rotated_90;
                        rotated_binary = rotated_binary.t();
                        flip(rotated_binary, rotated_binary, 1);
                        findContours(rotated_binary, rotated_contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
                        bounding_box_rotated = boundingRect(rotated_contours[0]);
                    }
                    Mat cop(bounding_box_rotated.width, bounding_box_rotated.height, rotated.type());

                    cvtColor(rotated(bounding_box_rotated), cop, COLOR_BGR2GRAY);
                    cop = ~cop;
                    threshold(cop, cop, 0, 255, THRESH_BINARY | THRESH_OTSU);
                    double cop_ratio = (double)cop.rows / (double)cop.cols;
                    cout << cop_ratio << endl;
                }
            }
        }
        else if (rectHalf.contains(Point(x, y)))
        {
            Mat gray;
            cvtColor(based(drawingSpace), gray, COLOR_BGR2GRAY);

            Mat binary;
            threshold(gray, binary, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

            vector<vector<Point>> contours;
            findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

            if (contours.size() > 0)
            {
                int largest_contour_index = 0;
                double largest_area = 0.0;

                for (int i = 0; i < contours.size(); i++)
                {
                    double area = contourArea(contours[i]);
                    if (area > largest_area)
                    {
                        largest_area = area;
                        largest_contour_index = i;
                    }
                }
                RotatedRect rotated_rect = minAreaRect(contours[largest_contour_index]);

                Point2f vertices[4];    //왼쪽부터 시계방향
                rotated_rect.points(vertices);
                Mat dst;
                cvtColor(binary, dst, COLOR_GRAY2BGR);
                for (int i = 0; i < 4; i++)
                {
                    line(dst, vertices[i], vertices[(i + 1) % 4], Scalar(0, 0, 255), 2);
                }

                Rect bounding_box = boundingRect(contours[largest_contour_index]);
                rectangle(dst, bounding_box, Scalar(0, 255, 0), 2);

                imshow("Bounding Box", dst);

                // Calculate the slope (angle) using vertices[0] and vertices[1]
                float dx = vertices[1].x - vertices[0].x;
                float dy = vertices[1].y - vertices[0].y;
                float angle = atan2(dy, dx) * 180.0 / CV_PI;

                // Normalize the angle to be within 0 to 180 degrees
                if (angle < -45) {
                    angle += 90;
                    swap(rotated_rect.size.width, rotated_rect.size.height);
                }

                // Get the rotation matrix
                Mat M = getRotationMatrix2D(rotated_rect.center, angle, 1.0);

                // Rotate the image
                Mat rotated;
                warpAffine(based(drawingSpace), rotated, M, based(drawingSpace).size(), INTER_CUBIC, BORDER_REFLECT);

                Mat rotated_gray;
                cvtColor(rotated, rotated_gray, COLOR_BGR2GRAY);
                Mat rotated_binary;
                threshold(rotated_gray, rotated_binary, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

                //회전된 이미지 외곽선 검출
                vector<vector<Point>> rotated_contours;
                findContours(rotated_binary, rotated_contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

                if (rotated_contours.size() > 0)
                {
                    int largest_contour_index_rotated = 0;
                    double largest_area_rotated = 0.0;

                    for (int i = 0; i < rotated_contours.size(); i++)
                    {
                        double area = contourArea(rotated_contours[i]);
                        if (area > largest_area_rotated)
                        {
                            largest_area_rotated = area;
                            largest_contour_index_rotated = i;
                        }
                    }

                    Rect bounding_box_rotated = boundingRect(rotated_contours[largest_contour_index_rotated]);
                    if (bounding_box_rotated.width > bounding_box_rotated.height) {
                        int tmp = 0;
                        // Rotate 90 degrees
                        Mat rotated_90;
                        rotate(rotated, rotated_90, ROTATE_90_CLOCKWISE);
                        rotated = rotated_90;
                        rotated_binary = rotated_binary.t();
                        flip(rotated_binary, rotated_binary, 1);
                        findContours(rotated_binary, rotated_contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
                        bounding_box_rotated = boundingRect(rotated_contours[0]);
                    }
                    Mat cop(bounding_box_rotated.width, bounding_box_rotated.height, rotated.type());

                    cvtColor(rotated(bounding_box_rotated), cop, COLOR_BGR2GRAY);
                    cop = ~cop;
                    threshold(cop, cop, 0, 255, THRESH_BINARY | THRESH_OTSU);

                    Mat cop_half;
                    cop.copyTo(cop_half);
                    for (int y = 0; y < cop_half.rows; y++) {
                        for (int x = cop_half.cols / 2; x < cop_half.cols; x++)
                        {
                            cop_half.at<uchar>(y, x) = 0;
                        }
                    }
                    vector<vector<Point>> half_contours;
                    findContours(cop_half, half_contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
                    imshow("cop_half", cop_half);
                    cout << half_contours.size() << endl;
                }
            }
        }
        else if (rectRun.contains(Point(x, y)))
        {
            int number = 0;
            vector<vector<Point>> rotated_contours;
            vector<vector<Point>> half_contours;
            int crossCount = 0;
            double cop_ratio = 0;

            Moments mu;
            Point2f mc(0, 0);
            Moments mu_outer;
            Point2f mc_outer(0, 0);

            double per_x = 0.0;
            double per_y = 0.0;

            Mat gray;
            cvtColor(based(drawingSpace), gray, COLOR_BGR2GRAY);

            Mat binary;
            threshold(gray, binary, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

            vector<vector<Point>> contours;
            findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

            if (contours.size() > 0)
            {
                int largest_contour_index = 0;
                double largest_area = 0.0;

                for (int i = 0; i < contours.size(); i++)
                {
                    double area = contourArea(contours[i]);
                    if (area > largest_area)
                    {
                        largest_area = area;
                        largest_contour_index = i;
                    }
                }
                RotatedRect rotated_rect = minAreaRect(contours[largest_contour_index]);

                Point2f vertices[4];    //왼쪽부터 시계방향
                rotated_rect.points(vertices);
                Mat dst;
                cvtColor(binary, dst, COLOR_GRAY2BGR);
                for (int i = 0; i < 4; i++)
                {
                    line(dst, vertices[i], vertices[(i + 1) % 4], Scalar(0, 0, 255), 2);
                }

                Rect bounding_box = boundingRect(contours[largest_contour_index]);
                rectangle(dst, bounding_box, Scalar(0, 255, 0), 2);

                //imshow("Bounding Box", dst);

                // Calculate the slope (angle) using vertices[0] and vertices[1]
                float dx = vertices[1].x - vertices[0].x;
                float dy = vertices[1].y - vertices[0].y;
                float angle = atan2(dy, dx) * 180.0 / CV_PI;

                // Normalize the angle to be within 0 to 180 degrees
                if (angle < -45) {
                    angle += 90;
                    swap(rotated_rect.size.width, rotated_rect.size.height);
                }

                // Get the rotation matrix
                Mat M = getRotationMatrix2D(rotated_rect.center, angle, 1.0);

                // Rotate the image
                Mat rotated;
                warpAffine(based(drawingSpace), rotated, M, based(drawingSpace).size(), INTER_CUBIC, BORDER_REFLECT);

                Mat rotated_gray;
                cvtColor(rotated, rotated_gray, COLOR_BGR2GRAY);
                Mat rotated_binary;
                threshold(rotated_gray, rotated_binary, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

                //회전된 이미지 외곽선 검출
                findContours(rotated_binary, rotated_contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

                if (rotated_contours.size() > 0)
                {
                    int largest_contour_index_rotated = 0;
                    double largest_area_rotated = 0.0;

                    for (int i = 0; i < rotated_contours.size(); i++)
                    {
                        double area = contourArea(rotated_contours[i]);
                        if (area > largest_area_rotated)
                        {
                            largest_area_rotated = area;
                            largest_contour_index_rotated = i;
                        }
                    }

                    Rect bounding_box_rotated = boundingRect(rotated_contours[largest_contour_index_rotated]);
                    if (bounding_box_rotated.width / bounding_box_rotated.height > 1.75) {
                        // Rotate 90 degrees
                        Mat rotated_90;
                        rotate(rotated, rotated_90, ROTATE_90_CLOCKWISE);
                        rotated = rotated_90;
                        rotated_binary = rotated_binary.t();
                        flip(rotated_binary, rotated_binary, 1);
                        findContours(rotated_binary, rotated_contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
                        bounding_box_rotated = boundingRect(rotated_contours[0]);
                    }
                    Mat cop(bounding_box_rotated.width, bounding_box_rotated.height, rotated.type());


                    cvtColor(rotated(bounding_box_rotated), cop, COLOR_BGR2GRAY);
                    cop = ~cop;
                    threshold(cop, cop, 0, 255, THRESH_BINARY | THRESH_OTSU);

                    Mat cop_half;
                    cop.copyTo(cop_half);
                    for (int y = 0; y < cop_half.rows; y++) {
                        for (int x = cop_half.cols / 2; x < cop_half.cols; x++)
                        {
                            cop_half.at<uchar>(y, x) = 0;
                        }
                    }
                    findContours(cop_half, half_contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

                    // 종횡비
                    cop_ratio = (double)cop.rows / (double)cop.cols;

                    findContours(cop, rotated_contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

                    int PreValue = 0;
                    int CrnValue = 0;
                    for (int y = 0; y < cop.rows; y++)
                    {
                        CrnValue = cop.at<uchar>(y, cop.cols / 2);
                        if (CrnValue == 255 && PreValue == 0) crossCount++;
                        PreValue = CrnValue;
                    }

                    Mat cop_color;
                    cvtColor(cop, cop_color, COLOR_GRAY2BGR);

                    if (rotated_contours.size() == 2)
                    {
                        // 내부 외곽선의 무게 중심 계산
                        mu = moments(rotated_contours[1]);
                        if (mu.m00 != 0) {
                            mc = Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00);
                        }

                        mu_outer = moments(rotated_contours[0]);
                        if (mu_outer.m00 != 0) {
                            mc_outer = Point2f(mu_outer.m10 / mu_outer.m00, mu_outer.m01 / mu_outer.m00);
                        }
                    }
                }

                /*cout << rotated_contours.size() << endl;
                cout << crossCount << endl;
                cout << cop_ratio << endl;
                cout << half_contours.size() << endl;*/
                if (rotated_contours.size() == 1) {
                    if (crossCount == 2) number = 4;
                    else if (crossCount == 1) {
                        if (cop_ratio > 3.0) number = 1;
                        else if (cop_ratio < 1.5) number = 4;
                        else number = 7;

                    }
                    else if (crossCount == 3)
                    {
                        if (half_contours.size() == 3) number = 3;
                        else if (half_contours.size() == 2)
                        {
                            if (cop_ratio > 1.48) number = 5;
                            else number = 2;
                        }
                    }
                    else if (crossCount == 4) number = 3;
                }
                else if (rotated_contours.size() == 2)
                {
                    double max_y = 0.0;
                    double min_y = 0.0;
                    double max_x = 0.0;
                    double min_x = 0.0;
                    if (mc.y >= mc_outer.y) { max_y = mc.y; min_y = mc_outer.y; }
                    else if (mc.y <= mc_outer.y) { max_y = mc_outer.y; min_y = mc.y; }
                    if (mc.x >= mc_outer.x) { max_x = mc.x; min_x = mc_outer.x; }
                    else if (mc.x <= mc_outer.x) { max_x = mc_outer.x; min_x = mc.x; }

                    /*cout << "내부 " << mc << endl;
                    cout << "외부 " << mc_outer << endl;
                    cout << "max_x : " << max_x << endl;
                    cout << "max_y : " << max_y << endl;
                    cout << "x차이" << max_x - min_x << endl;
                    cout << "y차이" << max_y - min_y << endl;
                    cout << "외곽선 : " << rotated_contours.size() << endl;
                    cout << "종횡비 : " << cop_ratio << endl;*/
                    if (max_y - min_y < 5.0 && max_x - min_x < 5.0) number = 0;
                    else if (mc.y < mc_outer.y) {
                        if (cop_ratio < 1.4) number = 4;
                        else number = 9;
                    }
                    else if (mc.y > mc_outer.y) {
                        if (cop_ratio < 1.4) number = 4;
                        else number = 6;
                    }
                    else cout << "인식불가" << endl;
                }
                else number = 8;

                cout << "검출된 숫자 : " << number << endl;
            }
        }
        break;
    case EVENT_MOUSEMOVE:
        if (flags && EVENT_FLAG_LBUTTON && drawingSpace.contains(Point(x, y))) {
            if (drawingSpace.contains(ptOld))
            {
                line(based, ptOld, Point(x, y), Scalar(0, 0, 0), 17);
                imshow("numberWindow", based);
            }
        }
        ptOld = Point(x, y);
        break;
    default:
        break;
    }
}

void edgeLine(Mat img)
{
    line(img, Point(0, 0), Point(img.cols - 1, 0), Scalar(0, 0, 0), edgeThick);
    line(img, Point(img.cols - 1, 0), Point(img.cols - 1, img.rows - 1), Scalar(0, 0, 0), edgeThick);
    line(img, Point(img.cols - 1, img.rows - 1), Point(0, img.rows - 1), Scalar(0, 0, 0), edgeThick);
    line(img, Point(0, img.rows - 1), Point(0, 0), Scalar(0, 0, 0), edgeThick);
}

void drawText(Mat img, const string text)
{
    int fontFace = FONT_HERSHEY_SIMPLEX;
    double fontScale = 1;
    int thickness = 3;

    Size sizeText = getTextSize(text, fontFace, fontScale, thickness, 0);
    Size sizeImg = img.size();

    Point org((sizeImg.width - sizeText.width) / 2, (sizeImg.height + sizeText.height) / 2);
    putText(img, text, org, fontFace, fontScale, Scalar(0, 0, 0), thickness);
}