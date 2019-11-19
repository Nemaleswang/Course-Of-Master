
CXX = g++

SRC_DIR = ./cpp_code/
TARGET = bpt_db
OBJ = bpt.o main.o

$(TARGET):$(OBJ)
	$(CXX) -o $(TARGET) $(OBJ)
	rm -rf $(OBJ)

bpt.o:
	$(CXX) -c $(SRC_DIR)bpt.cpp

main.o:
	$(CXX) -c $(SRC_DIR)main.cpp -std=c++11 

clean:
	rm -rf $(OBJ) $(TARGET)
