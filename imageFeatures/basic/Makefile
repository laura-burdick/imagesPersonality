CPPFLAGS =		-I/home/wenlaura/local/opencv-3.1.0/modules/objdetect/include \
				-I/home/wenlaura/local/opencv-3.1.0/modules/highgui/include \
				-I/home/wenlaura/local/opencv-3.1.0/modules/imgproc/include \
				-I/home/wenlaura/local/opencv-3.1.0/modules/core/include \
				-I/home/wenlaura/local/opencv-3.1.0/modules/imgcodecs/include \
				-I/home/wenlaura/local/opencv-3.1.0/modules/videoio/include  \
				-I/home/wenlaura/local/opencv_contrib/modules/xfeatures2d/include \
				-I/home/wenlaura/local/opencv-3.1.0/modules/features2d/include \
				-I/home/wenlaura/local/opencv-3.1.0/modules/flann/include \
				-I/home/wenlaura/local/boost_1_60_0 \

LDFLAGS =		-L/home/wenlaura/local/opencv-3.1.0/release/lib \
				-L/home/wenlaura/local/boost_1_60_0/libs \
				-L/home/wenlaura/local/lib

LDLIB =			-lopencv_highgui -lopencv_core -lopencv_xfeatures2d \
				-lopencv_imgproc -lopencv_objdetect -lopencv_imgcodecs \
				-lopencv_videoio -lopencv_features2d -lopencv_flann \
				-lboost_filesystem -lboost_system \

CXX =			g++
CXXFLAGS =		-std=c++0x

EXECUTABLE =	imageProcessing
SOURCES =		$(wildcard *.cpp)
OBJECTS =		$(SOURCES:%.cpp=%.o)
MAIN_FILE =		main.cpp

release: CXXFLAGS += -O3 -DNDEBUG
release: all

debug: CXXFLAGS += -g3 -DDEBUG
debug: clean all

profile: CXXFLAGS += -pg
profile: clean all

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $(OBJECTS) $(LDLIB) -o $(EXECUTABLE)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $*.cpp

main.o: main.cpp featuresFile.o
image.o: image.cpp image.h color.o
featuresFile.o: featuresFile.cpp featuresFile.h image.o
color.o: color.cpp color.h

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) 
