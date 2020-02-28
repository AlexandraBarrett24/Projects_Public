import turtle, traceback

#Sets the amount of time to delay after each turtle drawing operation.
# Adjust this if you want, but be sure to set it back to 0 before submitting.
draw_delay = 0

#Set this to False to turn off turtle drawing entirely.
enable_turtle = True

#Insertion_Sort: takes one parameter, a list of Rectangle objects
#Uses insertion sort to sort the list in place from highest depth value
# to lowest.
#Doesn't return anything.
def insertion_sort(shape_list):
    x = 1
    while x < len(shape_list):
        key = shape_list[x]
        i = x -1
        while i >=0 and shape_list[i].depth < key.depth:
            shape_list[i+1] = shape_list[i]
            i = i -1
        shape_list[i+1] = key
        x += 1
        
    #TODO: Implement this function.
    

#  DO NOT EDIT BELOW THIS LINE

t = None
if enable_turtle:
    t = turtle.Turtle()

#Takes a list of Rectangle objects shape_list, and a turtle object t.
#Draw a single scene using turtle graphics.  
def draw_scene(shape_list,t):
    insertion_sort(shape_list)
    if enable_turtle:
        turtle.resetscreen()
        turtle.hideturtle()
        t.speed(0)
        s = turtle.getscreen()
        s.tracer(1,draw_delay)
        for shape in shape_list:
            shape.draw(t)
        t.hideturtle()

#Rectangle class.  Represents a single rectangle on the turtle canvas
class Rectangle:
    #self.x: x coord of the Rectangle's lower left corner
    #self.y: y coord of the Rectangle's lower left corner.
    #self.depth: depth of the Rectangle: higher values represent Rectangles
    #   further in the background
    #self.width: extent of the Rectangle in the x direction
    #self.length: extent of the Rectangle in the y direction
    #self.color: String representing the color of the Rectangle
    #self.id: Unique string identifier for object
    def __init__(self,x,y,depth,width,length,color,name):
        self.x = x
        self.y = y
        self.depth = depth
        self.width = width
        self.length = length
        self.color = color
        self.id = name
    #Draw method: takes in a Turtle object and draws the Rectangle
    def draw(self,turt):
        turt.penup()
        turt.setpos(self.x,self.y)
        turt.color(self.color)
        turt.begin_fill()
        turt.pendown()
        for i in range(2):
            turt.forward(self.width)
            turt.left(90)
            turt.forward(self.length)
            turt.left(90)
        turt.end_fill()
    #String representation of Rectangle object: depth:identifier
    def __repr__(self):
        return str(self.depth)+":"+self.id


#Create Rectangles in scene
background = Rectangle(-400,-300,60,800,600,"lime green","background")
building = Rectangle(-150,-50,41,300,150,"ivory4","building")
door = Rectangle(50,-50,40,35,70,"burlywood4","door")
line1 = Rectangle(-200,-145,20,50,5,"yellow","line1")
line2 = Rectangle(150,-145,20,50,5,"yellow","line2")
leaves1 = Rectangle(-200,10,30,110,80,"dark green","leaves1")
leaves2 = Rectangle(90,-70,10,220,150,"dark green","leaves2")
river = Rectangle(-400,70,50,800,100,"blue","river")
road = Rectangle(-400,-200,21,800,100,"black","road")
trunk1 = Rectangle(-160,-70,31,30,110,"brown","trunk1")
trunk2 = Rectangle(170,-230,11,60,220,"brown","trunk2")
window = Rectangle(-110,-10,40,80,60,"grey20","window")


#Create test case lists
scene1 = []
scene2 = [river]
scene3 = [trunk2,leaves2,road]
scene4 = [leaves1,trunk1,window,door,building]
scene5 = [road,background,line2,river,line1]
scene6 = [building,background,door,line1,line2,leaves1,leaves2,
             river,road,trunk1,trunk2,window]

#List of test cases and their correct sorted versions
tests = [scene1,scene2,scene3,scene4,scene5,scene6]
correct = [[],
           [river],
           [road,trunk2,leaves2],
           [building,window,door,trunk1,leaves1],
           [background,river,road,line2,line1],
           [background,river,building,door,window,trunk1,leaves1,
          road,line1,line2,trunk2,leaves2]]


#Run test cases, check whether sorted list correct
count = 0

try:
    for i in range(len(tests)):
        print("\n---------------------------------------\n")
        print("TEST #",i+1)
        print("Running: Insertion_Sort(",tests[i],")\n")
        draw_scene(tests[i],t)
        print("Expected:",correct[i],"\n\nGot:",tests[i])
        assert (correct[i] == tests[i]), "Sorting incorrect"
        print("Test Passed!\n")
        count+=1
except AssertionError as e:
    print("\nFAIL: ",e)
except Exception:
    print("\nFAIL: ",traceback.format_exc())

print(count,"out of",len(tests),"tests passed.")


