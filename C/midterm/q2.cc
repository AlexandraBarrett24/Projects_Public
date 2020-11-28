#include <iostream>
using namespace std;

class Duck {
public:
    virtual void quack() {
        cout << "Quack" << endl;
    }
};

class RubberDuck : public Duck {
public:
    RubberDuck() {
        array = NULL;
    }

    RubberDuck(int size) {
        array = new int[size];
        array[0] = 1;
        array[1] = 2;
        array[2] = 3;
    }

    ~RubberDuck() {
        delete array;
    }

    void quack() {
        cout << "Squeak! ";
        for (int i = 0; i < arraySize; i++) {
            cout << array[0] << " ";
        }
        cout << endl;
    }

private:
    int* array;
    int arraySize;
};

int main() {
    Duck* rubberDuck;
    Duck& duck = *rubberDuck;

    duck.quack();

    delete[] rubberDuck;

    return 0;
}
