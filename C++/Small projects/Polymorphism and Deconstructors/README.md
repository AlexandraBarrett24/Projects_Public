## C++ Polymorphism and Class Destructors

## In this project
1. Polymorphism and dynamic memory allocation.
2. Class static variables.
2. When to use destructors.
3. When to use the copy constructor.

### What's in this folder:

- main.cc:
    There are 3 helper functions in main. You need to pass a command line argument when you run the executable to run 1 of these 3. The 3 helper functions are:
    - RunStaticDemo()
    - RunPointerDemo()
    - RunMemoryDemo()

- delivery.cc:
    - The base class for different types of deliveries.

- overnight.cc, twoday.cc, and ground.cc:
    - Files containing classes derived from Delivery

### Executing the Code

To get started, you can compile and run each of the helper functions by passing an argument to the executable. Follow the directions below to change the code and observe the different behavior.

```
make
./deliver static
./deliver pointer
./deliver memory
```

> It is important to note that the makefile does not list headers as dependencies. This means that if you only change a header file, `make` will assume everything is up-to-date and not recompile and link. To fix this, use `make clean` to remove all object files and then call `make`.

<hr>

### Static Demo

The intent of inheritance is to create a base class that holds code for the common elements of a collection of similar classes. The common elements of the base class are inherited by the derived class, thereby reusing code, yet in each of the derived classes, base class functions can be overridden to implement the special functionality of that subclass. This becomes particularly powerful when we manage objects from all derived classes as if they were base class objects, but the overridden functions of the derived class are called instead. In C++, this happens through _dynamic binding_ and we call this _polymorphism_.

In this example, we are creating an array of base class Delivery objects, but storing derived class objects in the array. In C++, we use the keyword _virtual_ in front of a class method to let the compiler know that it should call the derived class version of that method.

Go to main.cc and look at the helper function `RunStaticDemo()`. Notice how the array is of type Delivery, but we are assigning objects of the derived class to those elements. Notice that each type of delivery differs by its Transport() function (the keyword _virtual_ is used in Delivery.h), which impacts the number of days it takes to deliver the package. Notice that Deliver() is NOT overridden.