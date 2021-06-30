import mykmeanssp
import sys
import numpy as np
import pandas as pd


def call_error():
    """Print error message and exit."""
    print("Error in input format.")
    sys.exit()


def print_final_res(res, k, num_of_cords):
    """Print the coordinates as needed for the exercise."""
    for cordinate in res:
        started = True
        for cord in cordinate:
            num = np.round(cord,4)
            if started:
                print(num, end="")
                started = False
            else:
                print(",",num, end="")
        print("")


length = len(sys.argv)
total = 0
maxIter = 300
txt1 = ""
txt2 = ""
if (length != 4) and (length != 5):  # error in input format
    call_error()
elif length == 5:  # maxIter is given
    if not sys.argv[2].isdigit():
        call_error()
    maxIter = int(sys.argv[2])
    txt1 = sys.argv[3]
    txt2 = sys.argv[4]
elif length == 4:   # maxIter is not given
    txt1 = sys.argv[2]
    txt2 = sys.argv[3]
if not sys.argv[1].isdigit():
    call_error()
k = int(sys.argv[1])
if k <= 0 or maxIter <= 0:
    call_error()
randoms = [0]*k
with open(txt1) as f:
    first_line = f.readline()
f.close()
if first_line == "" or first_line == None:
    call_error()
first_line_list = first_line.split(",")
d = len(first_line_list)
arr = [0]*d
for i in range(1, d):
    arr[i] = i
arr[0] = "id"
DPs1 = pd.read_csv(txt1, names=arr)
DPs2 = pd.read_csv(txt2, names=arr)
DPs = pd.merge(DPs1, DPs2, on="id")
DPs.sort_values(by="id", inplace=True)
DPs.drop("id", axis=1, inplace=True)
centroids = DPs.to_numpy()
res = [0]*k
np.random.seed(0)
numOfCentroids = DPs1.shape[0]
if numOfCentroids <= k:
    call_error()
d = (d-1)*2
mus = [[0]*d] * k
rnd = np.random.choice(numOfCentroids)
randoms[0] = rnd
mus[0] = centroids[rnd].tolist()
distances = [-1] * numOfCentroids
probs = [0]*numOfCentroids
z = 1
j = 0
rndIndex = 1
while z < k:
    sumOfMins = 0
    t = 0
    for dp in centroids:
        minimum = distances[t]
        temp = 0
        for v in range(d):
            temp += (abs(dp[v] - mus[j][v]))**2
        if temp < minimum or minimum == -1:
            minimum = temp
            distances[t] = minimum
        sumOfMins += distances[t]
        t += 1
    j += 1
    for x in range(numOfCentroids):
        probs[x] = distances[x] / sumOfMins
    rnd = np.random.choice(numOfCentroids, 1, p=probs)
    rnd = rnd[0]
    randoms[rndIndex] = rnd
    rndIndex += 1
    mus[z] = centroids[rnd].tolist()
    z += 1
started = True
for random in randoms:
    if started:
        print(random,end="")
        started = False
    else:
        print(",",random, end="")
print("")
centroidsList = centroids.tolist()
result = mykmeanssp.fit(centroidsList, mus, k, maxIter, d, numOfCentroids)
print_final_res(result, k, numOfCentroids)
