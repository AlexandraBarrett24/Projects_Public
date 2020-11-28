import traceback

        

#SortStudents function. Takes as input a list of Student objects,
#sorted alphabetically by name (last, then first), and outputs a list of
#Student objects, sorted by the following priority:
#house, then year, then name.
def SortStudents(studentList):
    
    studentList = CountingSort_year(studentList, 8)
    studentList = CountingSort_house(studentList, 4)
    return studentList


def CountingSort_year(A,k):
    out = list(A)
    count= []
    for i in range(0, k+1):
        count.append(0)
    for j in range(0, len(A)):
        count[A[j].year] +=1
    for t in range(1, k+1):
        count[t] = count[t] + count[t-1]
    for u in reversed(range (0, len(A))):
        key = A[u]
        index = count[key.year]-1
        out[index] = key
        count[key.year] -=1
    return out

def houseToNum(a):
    if a == 'Eagletalon':
        return 0
    elif a == 'Lannister':
        return 1
    elif a == 'Pufflehuff':
        return 2
    else:
        return 3
def CountingSort_house(A,k):
    out = list(A)
    count= []
    for i in range(0, k+1):
        count.append(0)
    for j in range(0, len(A)):
        count[houseToNum(A[j].house)] +=1
    for t in range(1, k+1):
        count[t] = count[t] + count[t-1]
    for u in reversed(range (0, len(A))):
        key = A[u]
        index = count[houseToNum(key.house)]-1
        out[index] = key
        count[houseToNum(key.house)] -=1
    return out


#  DO NOT EDIT BELOW THIS LINE

#Student class
#Each task has three instance variables:
#   self.name is a string representing the name of the student
#   self.house is a string representing which house the student is in
#   self.year is an integer representing what year the student is
class Student:
    def __init__(self,csvstring):
        csvdata = csvstring.split(",")
        self.name = csvdata[0]
        self.house = csvdata[1]
        self.year = int(csvdata[2])
    def __repr__(self):
        return "\n{:25}: {:12} {}".format(self.name,self.house,self.year)
    def __eq__(self,other):
        return type(self) == type(other) and \
               self.name == other.name and \
               self.house == other.house and \
               self.year == other.year

      



#Takes a string filename as an argument, and constructs a list
#  of Students from the information in the CSV file at filename
def getStudentList(filename):
    fp = open(filename)
    fp.readline()
    studentList = []
    for line in fp:
        studentList.append(Student(line))
    return studentList


tests = ['roster1.csv','roster2.csv','roster3.csv','roster4.csv',
         'roster5.csv','roster6.csv']
correct = ['roster1sorted.csv','roster2sorted.csv',
           'roster3sorted.csv','roster4sorted.csv',
           'roster5sorted.csv','roster6sorted.csv']


#Run test cases, check whether sorted list correct
count = 0

try:
    for i in range(len(tests)):
        print("\n---------------------------------------\n")
        print("TEST #",i+1)
        print("Reading student data from:",tests[i])
        roster = getStudentList(tests[i])
        print("Reading sorted student data from",correct[i])
        rosterSorted = getStudentList(correct[i])
        print("Running: SortStudents() on data list\n")
        output = SortStudents(roster)
        print("Expected:",rosterSorted,"\n\nGot:",output)
        assert len(output) == len(rosterSorted), "Output list length "\
               +str(len(output))+\
                  ", but should be "+str(len(rosterSorted))
        for j in range(len(output)):
            assert output[j] == rosterSorted[j],"Student #"\
                       +str(j+1)+" incorrect: "+\
                    str(output[j])+" \nshould be "+str(rosterSorted[j])
        print("Test Passed!\n")
        count += 1
except AssertionError as e:
    print("\nFAIL: ",e)

except Exception:
    print("\nFAIL: ",traceback.format_exc())

#Check for less than or greater than signs anywhere in the file
cursed = False
with open(__file__) as f:
    source = f.read()
    for ch in source:
        if ord(ch) == 60:
            print("Less than sign detected: Curse activated!")
            count = 0
            cursed = True
        if ord(ch) == 62:
            print("Greater than sign detected: Curse activated!")
            count = 0
            cursed = True

print()
if cursed:
    print("You are now a newt.  Don't worry, you'll get better.")
print(count,"out of",len(tests),"tests passed.")


