import random

repeats = int(input("Number of Repeats: "))
inserts = int(input("Number of Nodes to input: "))
dels = int(input("Number of Nodes to delete: "))
finds = int(input("Number of Nodes to find: "))

numList = []

input_txt = "Testing\\input.txt"
expOutput_txt = "Testing\\expectedOutput.txt"
inputFile = open(input_txt, 'w')
expOutputFile = open(expOutput_txt, 'w')

if(repeats != 0):
    inputFile.write(f"{repeats}\n")
else: # repeats == 0 (aby to aspon raz prebehlo)
    repeats += 1

for r in range(1, repeats+1):
    # Inserting
    inputFile.write(f"\n{inserts}\n")
    for num in range((inserts*r)-inserts, inserts*r):
        # print(f"insert progress = {num+1}/{inserts}")
        numList.append(num)
        expOutputFile.write(f"{num}\n")
    print("FINISHED INSERTING")

    expOutputFile.write(f"\n")

    random.shuffle(numList)
    for num in numList:
        inputFile.write(f"{num}\n")

    # Deleting
    inputFile.write(f"\n{dels}\n")
    for num in range(dels):
        delNum = random.randint((inserts*r)-inserts, inserts*r)
        print(f"dels progress = {num+1}/{dels}")
        while True:
            try:
                numList.remove(delNum)
                inputFile.write(f"{delNum}\n")
                break
            except:
                if(delNum < dels*r):
                    delNum += 1
                    print(f"exept {delNum} < {dels*r}\n")
                else:
                    delNum -= (inserts*r)-inserts
                continue
    print("FINISHED DELETING")

    numList.sort()
    for num in numList:
        expOutputFile.write(f"{num}\n")

    expOutputFile.write(f"\n")

    # Finding
    inputFile.write(f"\n{finds}\n")
    for num in range(finds):
        print(f"finds progress = {num+1}/{finds}")
        randNum = random.randint((inserts*r)-inserts, inserts*r)
        inputFile.write(f"{randNum}\n")

        if(randNum in numList):
            expOutputFile.write(f"{randNum} - true\n")
        else:
            expOutputFile.write(f"{randNum} - false\n")
    print("FINISHED FINDING")
    
    numList.clear()
    print(f"FINISHED REPEAT {r}/{repeats}")

inputFile.close()
expOutputFile.close()

print("FINISHED GENERATING!")