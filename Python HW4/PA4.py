import traceback
import math

#For this assignment, you will be implementing a max-heap as a simple Python
#list.  You don't need to create a separate heap_size variable, just use
#.append every time you want to add things to the end of the list, or .pop()
#to remove the last element of the list.

#NOTE: max_heap_insert is a bit different than the textbook implementation
#Rather than taking as an argument a key, it takes as an argument the
#Task object you want to input.  Since heap_increase_key still works
#on a numerical key, you'll need to be careful that you're actually
#putting a new Task into the heap and not just overwriting an existing
#Task's priority number.
    
#Takes as input a heap (list) of Task objects task_heap, and a Task object task.
#Inserts task into the heap task_heap, while maintaining the
#  max-heap property.
def insert_task(task_heap,task):
    task_heap.append(task)
    heap_increase_key(task_heap, task)

def parent(i):
    i+=1
    i = math.floor(i/2)
    i-=1
    return i
def left(i):
    return (2*i)+1
def right(i):
    return (2*i) + 2

def heap_increase_key(A, key):
    i = len(A)-1
    A[i] = key
    while(i>0 and A[parent(i)].priority < A[i].priority):
        temp = A[i]
        A[i] = A[parent(i)]
        A[parent(i)] = temp
        i = parent(i)
    
    
#Takes as input a heap (list) consisting of Task objects, task_heap
#Removes the Task of highest priority from the heap, and returns it.
#Returns None (without throwing an error) if the heap contains no Tasks
def extract_max_priority_task(task_heap):
    le = len(task_heap) -1
    if(le < 0):
        return None
    max_v = task_heap[0]
    task_heap[0] = task_heap[le]
    task_heap.pop()
    max_heapify(task_heap, 0)
    return max_v

def max_heapify(A, i):
    L = left(i)
    r = right(i)
    if(L<=(len(A)-1) and A[L].priority > A[i].priority):
        largest = L
    else:
        largest = i
    if(r<=(len(A)-1) and A[r].priority > A[largest].priority):
        largest = r
    if(largest != i):
        temp = A[i]
        A[i] = A[largest]
        A[largest] = temp
        max_heapify(A, largest)
    
            
#Takes as input a heap (list) of Task objects task_heap, an index of that
#  heap i, and a number new_priority.
#Increases the priority of the Task at index i within heap task_heap
#  to new_priority, and adjusts the heap accordingly to
#  maintain the max-heap property.
#Immediately returns None if new_priority is less than the
#  target Task's current priority, without throwing an error.
def increase_task_priority(task_heap,i,new_priority):
    if(task_heap[i].priority > new_priority):
        return None
    task_heap[i].priority = new_priority
    max_heapify(task_heap, 0)



#  DO NOT EDIT BELOW THIS LINE
        

#Task class
#Each task has two instance variables:
#   self.description is a string describing what the task is
#   self.priority is a number representing the importance of the
#      task (higher values are more important)
class Task:
    def __init__(self,description,priority):
        self.description = description
        self.priority = priority
    def __repr__(self):
        return "\n{:5}:{}".format(str(self.priority),self.description)
    def __eq__(self,other):
        return type(self) == type(other) and \
               self.description == other.description and \
               self.priority == other.priority



#Test cases:
    
t56 = Task("'Accidentally' run into love interest",56)
t61a = Task("Brood over inner darkness",61)
t61b = Task("Lunch with Powerdude and Megagal",61)
t89 = Task("Pick up Laundry",89)
t65 = Task("Go to boring normal job as alter-ego",65)
t1 = Task("Comic relief with useless sidekick",1)
t84 = Task("Help clean up collateral damage",84)
t33 = Task("Receive key to city",33)
t96 = Task("Prevent bank robbery",96)
t46 = Task("Training montage",46)

t20a = Task("Take a nap",20)
t20b = Task("Defeat King Explosion Murder", 20)
t20c = Task("Walk Ultradog the Annhilator", 20)
t97 = Task("Escape elaborate deathtrap", 97)
t16 = Task("Respond to fanmail",16)

#Duplicate objects, in case originals corrupted by student code
d56 = Task("'Accidentally' run into love interest",56)
d61a = Task("Brood over inner darkness",61)
d90 = Task("Brood over inner darkness",90)
d61b = Task("Lunch with Powerdude and Megagal",61)
d89 = Task("Pick up Laundry",89)
d65 = Task("Go to boring normal job as alter-ego",65)
d1 = Task("Comic relief with useless sidekick",1)
d84 = Task("Help clean up collateral damage",84)
d33 = Task("Receive key to city",33)
d61c = Task("Receive key to city",61)
d96 = Task("Prevent bank robbery",96)
d46 = Task("Training montage",46)

d20a = Task("Take a nap",20)
d20b = Task("Defeat King Explosion Murder", 20)
d20c = Task("Walk Ultradog the Annhilator", 20)
d30 = Task("Walk Ultradog the Annhilator", 30)
d97 = Task("Escape elaborate deathtrap", 97)
d16 = Task("Respond to fanmail",16)

count = 0



funcs = [insert_task,insert_task,insert_task,insert_task,insert_task,
         insert_task,extract_max_priority_task,increase_task_priority,
         insert_task,insert_task,insert_task,extract_max_priority_task,
         extract_max_priority_task,increase_task_priority,extract_max_priority_task,
         insert_task,extract_max_priority_task,
         extract_max_priority_task,insert_task,insert_task,
         increase_task_priority,increase_task_priority,
         extract_max_priority_task,insert_task,extract_max_priority_task]


day1_tasks = []
day2_tasks = []
params1 = [day1_tasks,t33]
params2 = [day1_tasks,t61a]
params3 = [day1_tasks,t61b]
params4 = [day1_tasks,t46]
params5 = [day1_tasks,t1]
params6 = [day1_tasks,t65]
params7 = [day1_tasks]
params8 = [day1_tasks,2,90]
params9 = [day1_tasks,t96]
params10 = [day1_tasks,t56]
params11 = [day1_tasks,t89]
params12 = [day1_tasks]
params13 = [day1_tasks,t84]
params14 = [day1_tasks,5,61]
params15 = [day1_tasks]
params16 = [day2_tasks,t20a]
params17 = [day2_tasks]
params18 = [day2_tasks]
params19 = [day2_tasks,t20b]
params20 = [day2_tasks,t20c]
params21 = [day2_tasks,1,10]
params22 = [day2_tasks,1,30]
params23 = [day2_tasks]
params24 = [day2_tasks,t97]




func_params = [params1,params2,params3,params4,params5,params6,params7,
               params8,params9,params10,params11,params12,params13,
               params14,params15,params16,params17,params18,params19,
               params20,params21,params22,params23,params24]



corr_heap1 = [d33]
corr_heap2 = [d61a,d33]
corr_heap3 = [d61a,d33,d61b]
corr_heap4 = [d61a,d46,d61b,d33]
corr_heap5 = [d61a,d46,d61b,d33,d1]
corr_heap6 = [d65,d46,d61a,d33,d1,d61b]
corr_heap7 = [d61b,d46,d61a,d33,d1]
corr_heap8 = [d90,d46,d61b,d33,d1]
corr_heap9 = [d96,d46,d90,d33,d1,d61b]
corr_heap10 = [d96,d46,d90,d33,d1,d61b,d56]
corr_heap11 = [d96,d89,d90,d46,d1,d61b,d56,d33]
corr_heap12 = [d90,d89,d61b,d46,d1,d33,d56]
corr_heap13 = [d89,d56,d61b,d46,d1,d33]
corr_heap14 = [d89,d56,d61b,d46,d1,d61c]
corr_heap15 = [d61c,d56,d61b,d46,d1]
corr_heap16 = [d20a]
corr_heap17 = []
corr_heap18 = []
corr_heap19 = [d20b]
corr_heap20 = [d20b,d20c]
corr_heap21 = [d20b,d20c]
corr_heap22 = [d30,d20b]
corr_heap23 = [d20b]
corr_heap24 = [d97,d20b]



corr_heaps = [corr_heap1,corr_heap2,corr_heap3,corr_heap4,corr_heap5,
              corr_heap6,corr_heap7,corr_heap8,corr_heap9,corr_heap10,
              corr_heap11,corr_heap12,corr_heap13,corr_heap14,corr_heap15,
              corr_heap16,corr_heap17,corr_heap18,corr_heap19,corr_heap20,
              corr_heap21,corr_heap22,corr_heap23,corr_heap24]
corr_outs = [None,None,None,None,None,None,d65,None,None,None,None,
             d96,d90,None,d89,None,d20a,None,None,None,None,
             None,d30,None]

def call_function(func,params,corr_heap,corr_output=None):
    if func == insert_task:
        print("Inserting task:",params[1])
        func(params[0],params[1])
    elif func == extract_max_priority_task:
        print("Epicperson seeks a task to complete!")
        print("Extracting maximum priority task:")
        out = func(params[0])
        print("Expected:",corr_output,"\n\nGot:",out)
        assert out == corr_output, "Extract max output incorrect"
    else:
        print("Increasing priority of",params[0][params[1]],"\nto",params[2])
        func(params[0],params[1],params[2])
    print("\nExpected Heap:",corr_heap,"\n\nGot:",params[0])
    assert params[0] == corr_heap, "Heap incorrect"



try:
    print("Day 1: Initial Task Queue Empty")
    for i in range(15):
        print("\n---------------------------------------\n")
        print("TEST #",i+1,":")
        call_function(funcs[i],func_params[i],corr_heaps[i],corr_outs[i])
        count = count + 1
    print("\n Day 1 Complete.\n")
    print("\n Day 2: Initial Task Queue Empty")
    for i in range(15,24):
        print("\n---------------------------------------\n")
        print("TEST #",i+1,":")
        call_function(funcs[i],func_params[i],corr_heaps[i],corr_outs[i])
        count = count + 1
except AssertionError as e:
    print("\nFAIL: ", e)
except Exception:
    print("\nFAIL: ", traceback.format_exc())

    


print(count,"out of 24 tests passed.")

