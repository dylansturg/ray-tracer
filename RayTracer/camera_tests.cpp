#include "GenVector.h"
#include "Camera.h"

void testFail(int);
bool testSimpleBasis();
bool testComplexBasis();

int main2(int argc, char **argv)
{
	printf("Testing the camera class...\n");

	if(!testSimpleBasis()){
		testFail(0);
	}

	if(!testComplexBasis()){
		testFail(1);
	}

	printf("All tests pass!\n");
}

void testFail(int n){
	printf("Test %i failed.\n", n);
	exit(0);
}

bool testSimpleBasis(){
	Camera c = Camera(Vector3(0, 0, 0), Vector3(0, 0, -1), Vector3(0, 1, 0), 90);

	if(c.getW() != Vector3(0, 0, 1)){
		return false;
	}

	if(c.getU() != Vector3(1, 0, 0)){
		return false;
	}

	if(c.getV() != Vector3(0, 1, 0)){
		return false;
	}
	return true;
}

bool testComplexBasis(){
	Camera c = Camera(Vector3(4, 3, 1), Vector3(5, -1, 2), Vector3(1.5, 1, 2), 90);

	if(c.getW() != Vector3(-1, 4, -1)){
		return false;
	}

	if(c.getU() != Vector3(-9, -0.5, 7)){
		return false;
	}

	if(c.getV() != Vector3(27.5, 16, 36.5)){
		return false;
	}
	return true;
}