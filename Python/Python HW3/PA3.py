import traceback
import math

#The test sequence assumes that you will complete merge sort first.
#If you want to attempt quicksort first, change this to False and
#it will flip the order of the tests.
merge_sort_first = True

#Note: you can rename the parameter names for any of these functions
#if you don't find the textbook's p, q, and r to be helpful.  Just
#don't change the names of the functions.

#Takes in list of Emails A, and indices p, q, and r
#Assuming that the sublists A[p...q] and A[q+1...r] are sorted by
#the length of the Email's sender, from longest to shortest,
#merges the two lists together and puts them back into the list
#Returns nothing
def merge(A,p,q,r):
    numL = q-p+1
    numR = r-q
    L= []
    R = []
    i = 0

    while(i<numL):
        L.append(A[p+i])
        print(A[p+i].sender)
        print(L[i].sender)
        i+=1
    j = 0
    while(j<numR):
        R.append(A[q+1+j])
        print(A[q+1+j].sender)
        print(R[j].sender)
        j+=1
    L.append(Email("", None))
    R.append(Email("", None))
    i = 0
    j=0
    split = p
    #print(p)

    while(split<=r):
        if(len(L[i].sender) >= len(R[j].sender)):
            A[split] = L[i]
            i+=1
        else:
            A[split] = R[j]
            j+=1
        split+=1

#Takes in list of Emails A, and indices p and r
#Sorts the sublist A[p...r], in order of the length of each Email's
#sender, from longest name to shortest, using Merge Sort
#Returns nothing
def merge_sort(A,p,r):
    if(p<r):
        q = math.floor((p+r)/2)
        merge_sort(A, p, q)
        merge_sort(A, q+1, r)
        merge(A, p, q, r)


#Takes in list of Emails A, and indices p and r
#Chooses A[r] as a pivot Email and re-orders the list so that
#all Emails with a longer sender name than the pivot come
#before the pivot in the list, and all Emails with a shorter
#sender name come after.
#Returns the index where the pivot is located
def partition(A,begin,end):
    pivot = len(A[end].sender)
    split = begin-1
    for cur in range(begin, end):
        if(len(A[cur].sender) >= pivot):
            split = split+1
            temp = A[split]
            A[split] = A[cur]
            A[cur] = temp
    temp2 = A[split+1]
    A[split+1] = A[end]
    A[end] = temp2
    return split + 1

#Takes in list of Emails A, and indices p and r
#Sorts the sublist A[p...r], in order of the length of each Email's
#sender, from longest name to shortest, using Quicksort
#Returns nothing
def quicksort(A,p,r):
    if(p<r):
        split = partition(A, p, r)
        quicksort(A, p, split-1)
        quicksort(A, split+1, r)



#DO NOT CHANGE ANYTHING BELOW THIS LINE

#Takes in list of Emails A, and begins the recursive merge_sort
#process by calling merge_sort on the entire list
#Returns nothing
def merge_sort_wrapper(A):
    merge_sort(A,0,len(A)-1)


#Takes in list of Emails A, and begins the recursive quicksort
#process by calling quicksort on the entire list
#Returns nothing
def quicksort_wrapper(A):
    quicksort(A,0,len(A)-1)

#Email class
#Each email has two instance variables:
#   self.sender is a string representing the name of the person
#     or organization that sent the email
#   self.subject is a stirng representing the subject line of
#     the Email
class Email:
    def __init__(self,sender,subject):
        self.sender = sender
        self.subject = subject
    def __repr__(self):
        return "\n{:20}:{}".format(self.sender,self.subject)
    def key(self):
        return -len(self.sender)


#Setup test cases
e1 = Email("Inigo Montoya","You killed my father. Prepare to die.")
e2 = Email("IRS", "THIS IS YOUR FINAL WARNING")
e3 = Email("Your Grandson", "Need Money for College")
e4 = Email("Prince of Nigeria", "Help transferring funds")
e5 = Email("Jackpot Lottery", "YOU'RE WINNER!")
e6 = Email("Super Antivirus Pro", "Virus Detected! Download Now!")
e7 = Email("Anonymous", "Forward this to 20 friends and you will get $$$")
e8 = Email("The Bank", "Please confirm your password")

e9 = Email("Ted", "HAHAHAHAHAHAHAHAHAHA")
e10 = Email("Tedd","mine!")
e11 = Email("Teddd","be")
e12 = Email("Tedddd","will")
e13 = Email("Teddddd","Vengeance")
e14 = Email("Tedddddd","day.")
e15 = Email("Teddddddd","this")
e16 = Email("Tedddddddd","for")
e17 = Email("Teddddddddd","waited")
e18 = Email("Tedddddddddd","I")
e19 = Email("Teddddddddddd","have")
e20 = Email("Tedddddddddddd","Long")



inbox1 = []
inbox2 = [e13]
inbox3 = [e2,e6]
inbox4 = [e9,e10,e11,e12,e13,e14,e15,e16,e17,e18,e19,e20]
inbox5 = [e20,e19,e18,e17,e16,e15,e14,e13,e12,e11,e10,e9]
inbox6 = [e1,e2,e3,e4,e5,e6,e7,e8]

#Quicksort's 6th test is slightly different: QSort isn't stable,
#so we don't bother checking for the case where you have two messages
#of equal length.
inbox1q = []
inbox2q = [e13]
inbox3q = [e2,e6]
inbox4q = [e9,e10,e11,e12,e13,e14,e15,e16,e17,e18,e19,e20]
inbox5q = [e20,e19,e18,e17,e16,e15,e14,e13,e12,e11,e10,e9]
inbox6q = [e1,e2,e4,e5,e6,e7,e8]


tests = [inbox1,inbox2,inbox3,inbox4,inbox5,inbox6]
testsq = [inbox1q,inbox2q,inbox3q,inbox4q,inbox5q,inbox6q]
correct = [[],
           [e13],
           [e6,e2],
           [e20,e19,e18,e17,e16,e15,e14,e13,e12,e11,e10,e9],
           [e20,e19,e18,e17,e16,e15,e14,e13,e12,e11,e10,e9],
           [e6,e4,e5,e1,e3,e7,e8,e2]]

correctq = [[],
           [e13],
           [e6,e2],
           [e20,e19,e18,e17,e16,e15,e14,e13,e12,e11,e10,e9],
           [e20,e19,e18,e17,e16,e15,e14,e13,e12,e11,e10,e9],
            [e6,e4,e5,e1,e7,e8,e2]]

#Run test cases, check whether sorted list correct
count = 0

try:
    if merge_sort_first:
        for i in range(len(tests)):
            print("TEST #"+str(i+1))
            print("Running: merge_sort_wrapper(",tests[i],")\n")
            merge_sort_wrapper(tests[i])
            print("Expected:",correct[i],"\n\nGot:",tests[i])
            assert correct[i] == tests[i],"List sorted incorrectly"
            count=count+1
            print("\n---------------------------------------\n")
        for i in range(len(tests)):
            print("TEST #"+str(i+7))
            print("Running: quicksort_wrapper(",testsq[i],")\n")
            quicksort_wrapper(testsq[i])
            print("Expected:",correctq[i],"\n\nGot:",testsq[i])
            assert correctq[i] == testsq[i],"List sorted incorrectly"
            count=count+1
            print("\n---------------------------------------\n")
    else:
        for i in range(len(tests)):
            print("TEST #"+str(i+1))
            print("Running: quicksort_wrapper(",testsq[i],")\n")
            quicksort_wrapper(testsq[i])
            print("Expected:",correctq[i],"\n\nGot:",testsq[i])
            assert correctq[i] == testsq[i],"List sorted incorrectly"
            count=count+1
            print("\n---------------------------------------\n")
        for i in range(len(tests)):
            print("TEST #"+str(i+7))
            print("Running: merge_sort_wrapper(",tests[i],")\n")
            merge_sort_wrapper(tests[i])
            print("Expected:",correct[i],"\n\nGot:",tests[i])
            assert correct[i] == tests[i],"List sorted incorrectly"
            count=count+1
            print("\n---------------------------------------\n")
except AssertionError as e:
    print("\nFAIL: ",e)
except Exception:
    print("\nFAIL: ",traceback.format_exc())


print(count,"out of",len(tests+testsq),"tests passed.")

