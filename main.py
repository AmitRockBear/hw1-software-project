from math import sqrt
import sys
eps = 0.0001


def validate_params():
    if not (len(sys.argv) != 5 and len(sys.argv) != 6):
        K = int(sys.argv[1])
        N = int(sys.argv[2])
        d = int(sys.argv[3])
        iter = 200 if len(sys.argv) == 5 else int(sys.argv[4])
        if (not isinstance(N, int) or N <= 0):
            print("Invalid number of points!")
            return
        if (not isinstance(K, int) or K <= 1 or K >=N):
            print("Invalid number of clusters!")
            return
        if (not isinstance(d, int)):
            print( "Invalid dimension of point!")
            return
        if (not isinstance(iter, int) or iter <= 1 or iter >= 1000):
            print("Invalid maximum iteration!")
            return
        filePath = sys.argv[5] if len(sys.argv) == 6 else sys.argv[4]
        return(K, N, d, iter, filePath)
    else:
        return

def distance(vector1, vector2, d):
    sum = 0
    for i in range(d):
        sum+=(vector1[i]-vector2[i])**2
    return sqrt(sum)

def initialize_vectors_array(filePath):
    vectors = []
    with open(filePath, 'r') as fp:
        for line in fp:
            line = line.split(',')
            line = [float(item) for item in line]
            vectors.append(line)
    return vectors

def initialize_centroids_array(vectors):
    centroids = []
    for i in range(K):
        centroids.append(vectors[i].copy())
    return centroids

def main(K, N, d, iter, filePath): 
    vectors = initialize_vectors_array(filePath=filePath)  
    centroids = initialize_centroids_array(vectors)
    max_distance = eps + 1
    iter_counter = 0

    while(max_distance >= eps and iter_counter < iter):
        max_distance = 0
        centroids_sum = []
        centroids_sum = [[0 for _ in range(d)] for _ in range(K)]
        counters = [0 for _ in range(K)]
        for i in range(N):
            min_distance = distance(vectors[i], centroids[0], d)
            min_j = 0
            for j in range(K):
                distance_from_centroid = distance(vectors[i], centroids[j], d)
                if min_distance > distance_from_centroid:
                    min_distance = distance_from_centroid
                    min_j = j
            counters[min_j]+=1
            for p in range(d):
                centroids_sum[min_j][p]+= vectors[i][p]
        for j in range(K):
            if counters[j] != 0:
                new_centroid_j = [centroids_sum[j][p] / counters[j] for p in range(d)]
                centroids_distance = distance(centroids[j], new_centroid_j, d)
                max_distance = max(max_distance, centroids_distance)
                centroids[j] = new_centroid_j.copy()
        iter_counter += 1

    for item in centroids:
        print(','.join([str(round(float(num), 4)) for num in item]))

if __name__ == "__main__":
    result = validate_params()
    if not result == None:
        K, N, d, iter, filePath = result
        main(K, N, d, iter, filePath)
