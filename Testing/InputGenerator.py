import random

repeats = int(input("Number of Repeats: "))
inserts = int(input("Number of Nodes to input: "))
dels = int(input("Number of Nodes to delete: "))
finds = int(input("Number of Nodes to find: "))

numList = [[] for _ in range(repeats)]
print(numList)

input_txt = "Testing\\input.txt"
expOutput_txt = "Testing\\expectedOutput.txt"
inputFile = open(input_txt, 'w')
expOutputFile = open(expOutput_txt, 'w')

if(repeats != 0):
    inputFile.write(f"{repeats}\n")
else: # repeats == 0 (aby to aspon raz prebehlo)
    repeats = 1

# ---------- Inserting ---------- 
for r in range(1, repeats+1):
    inputFile.write(f"\n{inserts}\n")
    for num in range((inserts*r)-inserts, inserts*r):
        # print(f"insert progress = {num+1}/{inserts}")
        numList[r-1].append(num)
        expOutputFile.write(f"{num}\n")
    # print("FINISHED INSERTING")

    expOutputFile.write(f"\n")

    random.shuffle(numList[r-1])
    for num in numList[r-1]:
        inputFile.write(f"{num}\n")

# ---------- Deleting ---------- 
for r in range(1, repeats+1):
    inputFile.write(f"\n{dels}\n")
    for num in range(dels):
        delNum = numList[r-1].pop(random.randrange(len(numList[r-1])))
        inputFile.write(f"{delNum}\n")
    # print("FINISHED DELETING")

    numList[r-1].sort()
    for num in numList[r-1]:
        expOutputFile.write(f"{num}\n")

    expOutputFile.write(f"\n")

# ---------- Finding ----------
for r in range(1, repeats+1):
    inputFile.write(f"\n{finds}\n")
    for num in range(finds):
        print(f"finds progress = {num+1}/{finds}")
        randNum = random.randint((inserts*r)-inserts, inserts*r)
        inputFile.write(f"{randNum}\n")

        if(randNum in numList[r-1]):
            expOutputFile.write(f"{randNum} - true\n")
        else:
            expOutputFile.write(f"{randNum} - false\n")
    # print("FINISHED FINDING")
    
    numList[r-1].clear()
    print(f"FINISHED REPEAT {r}/{repeats}")

inputFile.close()
expOutputFile.close()

print("FINISHED GENERATING!")