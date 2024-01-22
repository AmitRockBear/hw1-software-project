from math import sqrt
import sys
eps = 0.0001

def distance(vector1, vector2, d):
    sum = 0
    for i in range(d):
        sum+=((vector1[i]-vector2[i])**2)
    return sqrt(sum)


def main():
    if(len(sys.argv) != 5 and len(sys.argv) != 6):
        return 1    
    K = int(sys.argv[1])
    N = int(sys.argv[2])
    d = int(sys.argv[3])
    iter =  200 if len(sys.argv) == 4 else int(sys.argv[4])
    if(iter <= 1 or iter >= 1000):
        print("Invalid maximum iteration!")
    if (K <= 1 or K >=N):
        print("Invalid number of clusters!")

    
    vectors = []
    centroids = []

    filePath = sys.argv[5] if len(sys.argv) == 6 else sys.argv[4]
    with open(filePath, 'r') as fp:
        for line in fp:
            line = line.split(',')
            line = [float(item) for item in line]
            vectors.append(line)

    for i in range(K):
        centroids.append(vectors[i].copy())

    max_distance = eps + 1
    iter_counter = 0

    while(max_distance >= eps and iter_counter <= iter):
        max_distance = 0
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
                    centroids[j] = new_centroid_j
            iter_counter += 1

    for item in centroids:
        print(','.join([str(num) for num in item]))

            



            

            



if __name__ == "__main__":
    main()
