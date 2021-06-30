#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


int isConverged(double **clusters1, double **clusters2, int k, int numOfCords);
void copyArr1ToArr2(double **arr1, double **arr2, int k, int numOfCords);
void makeAllZero(double **currClusters, int k , int numOfCords);
void freeSpace (double **arr, int i);
int checkIfInteger(char n[]);
void callError();
void freeAndExit(char *arr1, char *arr2);


/*
* Function: fit
* -----------------------------
* Returns prints kmeans according to values provided from Python.
*
* DPs - all the data points.
* initialCentroids - the first k centroids for the algorithm.
* k - number of centroids.
* maxIter - maximum iterations done by the algorithm.
*/
static double** fit(double **DPs, double **centroids, int k, int maxIter, int numOfCords, int numOfDPs)
{
    int t, a, b, iterCounter=0, *countersArr;
    double **currClusters, **oldClusters;
    currClusters = (double **) malloc(k * sizeof(double *));
    oldClusters = (double **) malloc(k * sizeof(double *));
    assert(currClusters != NULL);
    assert(oldClusters != NULL);
    for (t = 0; t < k; t++)
    {
        currClusters[t] = (double *) malloc(numOfCords * sizeof(double));
        oldClusters[t] = (double *) malloc(numOfCords * sizeof(double));
        assert(currClusters[t] != NULL);
        assert(oldClusters[t] != NULL);
    }
    copyArr1ToArr2(centroids, currClusters, k, numOfCords);
    makeAllZero(oldClusters, k, numOfCords);
    /*now to compute the exact clusters, by the algo*/
    countersArr = (int*) malloc(k * sizeof(int));
    while((!isConverged(currClusters, oldClusters, k, numOfCords)) && (iterCounter<maxIter))
    {
        copyArr1ToArr2(currClusters, oldClusters, k, numOfCords);
        iterCounter++;
        countersArr = (int*) malloc(k * sizeof(int));
        for(t=0; t<k; t++)
        {
            countersArr[t]=0;
        }
        makeAllZero(currClusters, k , numOfCords);
        for(t=0; t<numOfDPs; t++)
        {
            double *DPoint = DPs[t];  /*the point ill search min for*/
            double minDistance=__FLT_MAX__;
            int closestClusterIndx = 0;
            for(a=0; a<k; a++)
            {
                double *cluster = oldClusters[a];
                double minCandidate = 0;
                for(b=0; b<numOfCords; b++)
                {
                    double x = DPoint[b]-cluster[b];
                    minCandidate+=x*x;
                }
                if(minCandidate<minDistance)
                {
                    closestClusterIndx = a;
                    minDistance = minCandidate;
                }
            }
            countersArr[closestClusterIndx]++;
            for(a=0; a<numOfCords;a++)
            {
                currClusters[closestClusterIndx][a]+=DPs[t][a];
            }

        }
        /*now to divide by |s_i|*/
        for(t=0;t<k;t++)
        {
            for(a=0;a<numOfCords;a++)
            {
                currClusters[t][a] = (currClusters[t][a]/countersArr[t]);
            }
        }
        free(countersArr);
    }
    freeSpace(oldClusters,k);
    copyArr1ToArr2(currClusters, centroids, k, numOfCords);
    freeSpace(currClusters,k);
    return centroids;
}


/*
* Function: isConverged
* -----------------------------
* Check if clusters1 equals to clusters2, return True if so, or False if not.
*
* clusters1 - first cluster.
* clusters1 - second cluster.
* k - number of data points in each cluster.
* numOfCords - number of coordinates in each data point.
*/
int isConverged(double **clusters1, double **clusters2, int k, int numOfCords)
{
    int i;
    int j;
    for(i=0; i<k; i++)
    {
        for(j=0; j<numOfCords; j++)
        {
            if(clusters1[i][j]!=clusters2[i][j])
            {
                return 0;
            }
        }
    }
    return 1;
}


/*
* Function: copyArr1ToArr2
* -----------------------------
* Copy the contents of arr1 into arr2.
*
* arr1 - first array (the one which we will copy from).
* arr2 - second array (the one which we will copy to).
* k - number of data points in each array (cluster).
* numOfCords - number of coordinates in each data point.
*/
void copyArr1ToArr2(double **arr1, double **arr2, int k, int numOfCords)
{
    int i;
    int j;
    for(i=0; i<k; i++)
    {
        for(j=0; j<numOfCords; j++)
        {
            arr2[i][j] = arr1[i][j];
        }
    }
}


/*
* Function: makeAllZero
* -----------------------------
* Turn all slots of currClusters to zero.
*
* currClusters - the given cluster which will turn to zero-cluster.
* k - number of data points in currClusters.
* numOfCords - number of coordinates in each data point.
*/
void makeAllZero(double **currClusters, int k , int numOfCords)
{
    int i;
    int j;
    for(i=0; i<k; i++)
    {
        for(j=0; j<numOfCords; j++)
        {
            currClusters[i][j] = 0;
        }
    }
}


/*
* Function: freeSpace
* -----------------------------
* Free a given 2D array that was created using malloc, in which wach line was also created using malloc.
*
* arr - the array which will be free.
* i - number of rows in arr.
*/
void freeSpace(double **arr, int i)
{
    int a;
    for(a=0; a<i; a++)
    {
        free(arr[a]);
    }
    free(arr);
}


/*
* Function: checkIfInteger
* -----------------------------
* Check if n is non-negative integer.
*
* n - parameter that may be an integer
*/
int checkIfInteger(char n[])
{
    int i;
    int len = strlen(n);
    for(i=0; i<len; i++ )
    {
        if((n[i] < '0') || (n[i] > '9'))
        {
            return 0;
        }
    }
    return 1;
    
}


/*
* Function: callError
* -----------------------------
* Print error message and return 0.
*/
void callError()
{
    printf("error in input format.\n");
}


/*
* Function: freeAndExit
* -----------------------------
* Free allocated memory for arr1 and arr2, then exit program
*/
void freeAndExit(char *arr1, char *arr2)
{
    callError();
    free(arr1);
    free(arr2);
}


static PyObject* fit_capi(PyObject *self, PyObject *args)
{
    double **centroids, **DPs, **almostRes, *c_cord;
    int k, maxIter, numOfCords, numOfDPs, t, j;
    PyObject *DataPoints, *initialCentroids, *prevDataPoint, *prevCord, *res;
    if(!PyArg_ParseTuple(args, "OOiiii", &DataPoints, &initialCentroids, &k, &maxIter, &numOfCords, &numOfDPs))
    {
        return NULL;
    }
    DPs = (double **) malloc(numOfDPs * sizeof(double *));
    centroids = (double **) malloc(k * sizeof(double *));
    assert(DPs != NULL);
    assert(centroids != NULL);
    /*initialize DPs:*/
    for (t = 0; t < numOfDPs; t++)
    {
        prevDataPoint = PyList_GetItem(DataPoints, t);
        double *DataPoint = (double *) malloc(numOfCords * sizeof(double));
        assert(DataPoint != NULL);
        for (j=0; j < numOfCords; j++)
        {
            prevCord = PyList_GetItem(prevDataPoint, j);
            DataPoint[j] = PyFloat_AsDouble(prevCord);
        }
        DPs[t] = DataPoint;
    }
    /*up until here, all the Data Points are stored in DP*/
    /*initialize initialCentroids:*/
    for (t = 0; t < k; t++)
    {
        prevDataPoint = PyList_GetItem(initialCentroids, t);
        double *DataPoint = (double *) malloc(numOfCords * sizeof(double));
        assert(DataPoint != NULL);
        for (j=0; j < numOfCords; j++)
        {
            prevCord = PyList_GetItem(prevDataPoint, j);
            DataPoint[j] = PyFloat_AsDouble(prevCord);
        }
        centroids[t] = DataPoint;
    }
    /*up until here, all the initial centroids are stored in initialCentroids*/
    almostRes = fit(DPs, centroids, k, maxIter, numOfCords, numOfDPs);
    res = PyList_New(k);
    for (t=0; t < k; t++)
    {
        PyObject *cord = PyList_New(numOfCords); 
        c_cord = almostRes[t];
        for (j=0; j < numOfCords; j++)
        {
            PyObject *cordinate = PyFloat_FromDouble(c_cord[j]);
            PyList_SET_ITEM(cord, j, cordinate);
        }
        PyList_SET_ITEM(res, t, cord);
    }
    freeSpace(almostRes,k);
    return res;
}


static PyMethodDef capiMethods[] = {
    {"fit",
    (PyCFunction) fit_capi,
    METH_VARARGS,
    PyDoc_STR("list of Data Points and k initial centroids for kmeans algorithm.")},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp",
    NULL,
    -1,
    capiMethods
};


PyMODINIT_FUNC
PyInit_mykmeanssp(void)
{
    PyObject *m;
    m = PyModule_Create(&moduledef);
    if (!m)
    {
        return NULL;
    }
    return m;
}
