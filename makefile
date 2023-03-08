# Define the symbols we might want to change:
CXX	:= g++

OBJECTS	:= constants.o data.o DrawFunctions.o RayCalculations.o structs.o

raycaster: $(OBJECTS)
	$(CXX) $(OBJECTS) raycaster.c -o raycaster -framework OpenGL -framework GLUT
	rm $(OBJECTS)
constants.o: constants.c
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c constants.c -o constants.o
data.o: data.c
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c data.c -o data.o
structs.o: structs.c
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c structs.c -o structs.o
RayCalculations.o: RayCalculations.c
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c RayCalculations.c -o RayCalculations.o
DrawFunctions.o: DrawFunctions.c
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c DrawFunctions.c -o DrawFunctions.o

clean:
	rm raycaster *.o