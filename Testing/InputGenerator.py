import random

inserts = int(input("Number of Nodes to input: "))
dels = int(input("Number of Nodes to delete: "))
finds = int(input("Number of Nodes to find: "))

numList = []

input_txt = "Testing\\input.txt"
expInput_txt = "Testing\\expectedOutput.txt"
inputFile = open(input_txt, 'w')
expInputFile = open(expInput_txt, 'w')

# Inserting
inputFile.write(f"{inserts}\n")
for num in range(inserts):
    numList.append(num)
    expInputFile.write(f"{num}\n")

expInputFile.write(f"\n")

random.shuffle(numList)
for num in numList:
    inputFile.write(f"{num}\n")

# Deleting
inputFile.write(f"\n{dels}\n")
for num in range(dels):
    delNum = random.randint(0, inserts)
    while True:
        try:
            numList.remove(delNum)
            inputFile.write(f"{delNum}\n")
            break
        except:
            if(delNum < dels):
                delNum += 1
            else:
                delNum -= delNum
            continue

for num in numList:
    expInputFile.write(f"{num}\n")

expInputFile.write(f"\n")

# Finding
inputFile.write(f"\n{finds}\n")
for num in range(finds):
    randNum = random.randint(0, inserts)
    inputFile.write(f"{randNum}\n")

    if(randNum in numList):
        expInputFile.write(f"{randNum} - true\n")
    else:
        expInputFile.write(f"{randNum} - false\n")

inputFile.close()
expInputFile.close()

print("FINISHED GENERATING!")