all:
	g++ -std=c++11 HVA_2013_ImageProcessing.cpp -I /usr/include/opencv2/  -Wall -lopencv_core -lopencv_imgproc -lopencv_highgui -o ImageProcessing