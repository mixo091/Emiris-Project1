output = input("Give me the output file name?")
print("Calculating accuracy...")
file = open(output, "r")
Queries = []
Distance_LSH = []
Distance_True = []
for row in file:
    value = row.split(":")
    if value[0] == 'Query':
        Queries.append(value[1])
    if value[0] == 'distanceLSH':
        Distance_LSH.append(value[1])
    if value[0] == 'distanceTrue':
        Distance_True.append(value[1])


result = 0
resultNN = 0
for i in range(len(Queries)):
    division = float(Distance_True[i])/float(Distance_LSH[i])
    result = result + division
    if int(division) == 1:
        resultNN = resultNN + 1

file.close()
accuracy = round(result / float(len(Queries)) * 100)
accuracyNN = round(resultNN / len(Queries) * 100)
print(f"AccuracyBased on Distance : {accuracy} %")
print(f"Number of true NN's hitted by Lsh is {resultNN}.Accuracy Based on finding the true NN : {accuracyNN} %")


