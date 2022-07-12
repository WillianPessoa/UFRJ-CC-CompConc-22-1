index = 0;
for number in range(11, 15):
    filename = "input" + str(number) + ".txt"
    with open(filename, "r") as file:
        testcases = file.readlines()
        testcases = [line.rstrip() for line in testcases]
        
    print("Solving test cases files to {}".format(filename))
    testcases.pop(0)
    for i in range(1, len(testcases)+1, 2):
        if i % 2 == 1:
            index+=1
            
            textFilename = "{:02d}_text.txt".format(index)
            print("Writing '{}'".format(textFilename))
            with open(textFilename, "w") as f:
                f.write(testcases[i-1])
            
            patternFilename = "{:02d}_pattern.txt".format(index)
            print("Writing '{}'".format(patternFilename))                        
            with open(patternFilename, "w") as f:
                f.write(testcases[i])
           
            
        