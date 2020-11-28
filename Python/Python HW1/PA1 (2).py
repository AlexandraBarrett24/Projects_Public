import platform, traceback


#See instructions for details on each of the following functions:
def longest_name(account_list):
    namecompare = len(account_list[0].name)
    counter = 0
    index = 0
    for x in account_list:
        if namecompare < len(x.name):
           namecompare = len(x.name)
           index = counter
        counter += 1
    return account_list[index]
        
    #print("TODO: Implement this and remove this print statement")

def filter_negative(account_list):
    #counter = 0
    negs = []
    for x in account_list:
        if x.balance < 0:
            negs.append(x)
           # account_list.pop(counter)
       # counter += 1
    return negs
           
   
def max_Bal (account_list):
    balcompare = account_list[0].balance
    index = 0
    counter = 0
    for x in account_list:
        if balcompare < x.balance:
            balcompare = x.balance
            index = counter
        counter += 1
    return index

def r0b1n_h00d(account_list):
    index = max_Bal(account_list)
    longest_name(filter_negative(account_list)).balance += 1337
    account_list[index].balance = account_list[index].balance - 1337
    
   # print("TODO: Implement this and remove this print statement")
    

#Account class (you are not required to change anything here, but you may want
# to add some extra methods)
class Account:
    #Initialize an Account object.  Account objects have two instance variables
    #self.name is a string, representing the name associated with the Account
    #self.balance is a float, representing the balance of the account in $.
    def __init__(self,name,balance):
        self.name = name
        self.balance = balance
    #String representation of an Account object - "name: $balance"
    def __repr__(self):
        return self.name + ": $" + str(self.balance)

# DO NOT EDIT ANYTHING BELOW THIS LINE

#Test Cases
def make_accounts():
    return [Account("Scrooge McDuck", 10000000000),
            Account("Macklemore",20),
            Account("Loch Ness Monster",-3.5),
            Account("College Student",-78000),
            Account("Bruce Wayne", 212013114),
            Account("The Shoe from Monopoly",200),
            Account("Sheriff of Nottingham",-337),
            Account("Emmitt Brown", -80),
            Account("Jesse Loude", -4.74),
            Account("Ron Weasley", 0),
            Account("Lone Starr", -1000000)]

acc = make_accounts()
bank1 = [acc[0],acc[1],acc[2],acc[3]]
bank2 = [acc[5],acc[6]]
bank3 = [acc[7],acc[8],acc[9]]
bank4 = [acc[10],acc[3],acc[7],acc[8],acc[5],acc[2],
         acc[6],acc[9],acc[0],acc[1],acc[4]]


bal1 = [9999998663,20,1333.5,-78000]
bal2 = [-1137,1000]
bal3 = [1257,-4.74,-1337]
bal4 = [-1000000,-78000,1257,-4.74,200,1333.5,
        1000,-1337,9999997326,20,212013114]

acc = make_accounts()
test_banks = [bank1,bank2,bank3,bank4]
correct_longest = [acc[2], acc[5], acc[7], acc[5]]
correct_filter = [[acc[2],acc[3]], [acc[6]], [acc[7],acc[8]],
                  [acc[10],acc[3],acc[7],acc[8],acc[2],acc[6]]]
correct_balances = [bal1,bal2,bal3,bal4]

count = 0

try:
    vers = platform.python_version()
    assert vers[0] == '3', "You must use Python 3, "+vers+" is not acceptable"
    for i in range(len(test_banks)):
        print("TEST #"+str(i+1))
        print("Running: longest_name(",test_banks[i],")\n")
        out = longest_name(test_banks[i][:])
        print("Expected:",correct_longest[i],"\nGot:",out)
        assert type(out) == Account, "Return type not Account"
        assert correct_longest[i].name == out.name, \
               "Longest account name incorrect"
        assert correct_longest[i].balance == out.balance, \
               "Balance incorrect"
        count += 1
        print("\n---------------------------------------\n")
    for i in range(len(test_banks)):
        print("TEST #"+str(i+1+len(test_banks)))
        print("Running: filter_negative(",test_banks[i],")\n")
        out = filter_negative(test_banks[i][:])
        print("Expected:",correct_filter[i],"\nGot:",out)
        assert type(out) == list, "Return type not list"
        assert len(correct_filter[i]) == len(out), \
               "Length of list does not match"
        for j in range(len(out)):
            assert type(out[j]) == Account, \
                   "Element at index "+str(j)+" is not of type Account"
            assert correct_filter[i][j].name == out[j].name, \
                   "Account at index "+str(j)+" name incorrect"
            assert correct_filter[i][j].balance == out[j].balance, \
                   "Account at index "+str(j)+" balance incorrect"
        count += 1
        print("\n---------------------------------------\n")
    for i in range(len(test_banks)):
        print("TEST #"+str(i+1+len(test_banks)*2))
        original_names = list(map(lambda x: x.name,test_banks[i]))
        print("Running: r0b1n_h00d(",test_banks[i],")\n")
        r0b1n_h00d(test_banks[i])
        correct = list(map(lambda x,y: Account(x,y),
                           original_names,correct_balances[i]))
        print("Expected:",correct,"\n\nGot:",test_banks[i])
        assert len(correct) == len(test_banks[i]), "Number of bank accounts altered"
        for j in range(len(test_banks[i])):
            acc = test_banks[i][j]
            assert acc.name == original_names[j], original_names[j] + \
                   " account name altered." 
            assert acc.balance == correct_balances[i][j], acc.name + \
                   " account balance incorrect."
        count=count+1
        print("\n---------------------------------------\n")

except AssertionError as e:
    print("\nFAIL: ",e)
except Exception:
    print("\nFAIL: ",traceback.format_exc())


print("\n---------------------------------------\n")
print(count,"out of",12,"tests passed.")
