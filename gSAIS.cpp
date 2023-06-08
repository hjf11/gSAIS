#include <stdlib.h>
#include <stdbool.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string.h>
#include <memory>


using namespace std;

unsigned char mask[]={0x80, 0x40, 0x20, 0x10, 0x08,0x04, 0x02, 0x01};
#define tget(i) ( (t[(i)/8]&mask[(i)%8]) ? 1 : 0 )
#define tset(i, b) t[(i)/8]=(b)?(mask[(i)%8]|t[(i)/8]):(~(mask[(i)%8])&t[(i)/8])
#define chr(i) (cs==sizeof(int)?((int*)s)[i]:((unsigned char*)s)[i])
#define isLMS(i) (i>0 && tget(i) && !tget(i-1))
// find the start or end of each bucket
void getBuckets(unsigned char*s, int*bkt, int n,int K, int cs, bool end) {
    int i, sum=0;// clear all buckets
    for(i=0; i<=K; i++) bkt[i]=0;
    // compute the size of each bucket
    for(i=0; i<n; i++) bkt[chr(i)]++;
    for(i=0; i<=K; i++){ 
        sum+=bkt[i];
        //mijenjam sum u sum-1 jer nizovi završavaju separatorom
        bkt[i]=end ? sum-1 : sum-bkt[i]; 
    }
}
// compute SAl
void induceSAl(unsigned char*t, int*SA, unsigned char*s, int*bkt, int n, int K, int cs, bool end) {
    int i, j;
    // find starts of buckets
    getBuckets(s, bkt, n, K, cs, end);
    for(i=0; i<n; i++) {
        j=SA[i]-1;
        if(j>=0 && !tget(j)) SA[bkt[chr(j)]++]=j;
    }
}
// compute SAs
void induceSAs(unsigned char*t, int*SA,unsigned char*s, int*bkt,int n, int K, int cs, bool end) {
    int i, j;
    // find ends of buckets
    getBuckets(s, bkt, n, K, cs, end);

    for(i=n-1; i>=0; i--) {
        j=SA[i]-1;
        if(j>=0 && tget(j)) SA[bkt[chr(j)]--]=j;
    }
}

void induceSAl_gSAIS(unsigned char*t, int*SA, unsigned char*s, int*bkt, int n, int K, int cs, bool end, unsigned char separator) {
    int i, j;
    //nadi pocetke pretinaca
    getBuckets(s, bkt, n, K, cs, end);
    for(i=0; i<n; i++) {
        j=SA[i]-1;
        //isto kao u originalnom samo se provjerava je li separator
        if(j>=0 && !tget(j) && chr(j) != separator) {
            SA[bkt[chr(j)]++]=j;
        }
    }
}

void induceSAs_gSAIS(unsigned char*t, int*SA, unsigned char*s, int*bkt, int n, int K, int cs, bool end, unsigned char separator) {
    int i, j;
    //nadi krajeve pretinaca
    getBuckets(s, bkt, n, K, cs, end);
    for(i=n-1; i>=0; i--) {
        j=SA[i]-1;
        //isto kao u originalnom samo se provjerava je li separator
        if(j>=0 && tget(j) && chr(j) != separator) {
            SA[bkt[chr(j)]--]=j;
        }
    }
}


// find the suffix array SA of s[0..n-1] in {1..K}ˆn
// require s[n-1]=0 (the sentinel!), n>=2
// use a working space (excluding s and SA) of
// at most 2.25n+O(1) for a constant alphabet
void SAIS(unsigned char*s, int*SA, int n,int K, int cs) {
    // LS-type array in bits
    unsigned char*t=(unsigned char*)malloc(n/8+1);
    
    int i, j;
    // classify the type of each character
    // the sentinel must be in s1, important!!!
    tset(n-2, 0); 
    tset(n-1, 1);
    for(i=n-3; i>=0; i--) 
        tset(i, (chr(i)<chr(i+1)|| (chr(i)==chr(i+1)&& tget(i+1)==1))?1:0);

    // stage 1: reduce the problem by at least 1/2
    // sort all the S-substrings
    // bucket array
    int*bkt = (int*)malloc(sizeof(int)*(K+1));
    // find ends of buckets
    getBuckets(s, bkt, n, K, cs, true);
    for(i=0; i<n; i++) SA[i]=-1;
    for(i=1; i<n; i++)
        if(isLMS(i)) SA[bkt[chr(i)]--]=i;

    // for(i=n-2; i>=0; i--)
    // if(isLMS(i)) SA[bkt[chr(i)]--]=i;
    // SA[0]=n-1;

    induceSAl(t, SA, s, bkt, n, K, cs, false);
    induceSAs(t, SA, s, bkt, n, K, cs, true);
    free(bkt);
    // compact all the sorted substrings into
    // the first n1 items of SA
    // 2*n1 must be not larger than n (proveable)
    int n1=0;
    for(i=0; i<n; i++)
        if(isLMS(SA[i])) SA[n1++]=SA[i];

    // find the lexicographic names of substrings
    // init the name array buffer
    for(i=n1; i<n; i++) SA[i]=-1;
    int name=0, prev=-1;
    for(i=0; i<n1; i++) {
        int pos=SA[i]; 
        bool diff=false;
        for(int d=0; d<n; d++) {
            if(prev==-1  || chr(pos+d)!=chr(prev+d)|| tget(pos+d)!=tget(prev+d)){ 
                diff=true; 
                break; 
            }else if(d>0 && (isLMS(pos+d) ||isLMS(prev+d)))
                break;
        }    
        if(diff) { 
            name++;
            prev=pos;
        }
        pos=(pos%2==0)?pos/2:(pos-1)/2;
        SA[n1+pos]=name-1;
    }
    for(i=n-1, j=n-1; i>=n1; i--)
        if(SA[i]>=0) SA[j--]=SA[i];
    
    // stage 2: solve the reduced problem
    // recurse if names are not yet unique
    int*SA1=SA,*s1=SA+n-n1;
    if(name<n1) SAIS((unsigned char*)s1, SA1, n1, name, sizeof(int));
    else 
    // generate the suffix array of s1 directly
    for(i=0; i<n1; i++) SA1[s1[i]] = i;
    // stage 3: induce the result for
    // the original problem

    // bucket array
    bkt = (int*)malloc(sizeof(int)*(K+1));
    // put all the LMS characters into their buckets
    // find ends of buckets
    getBuckets(s, bkt, n, K, cs, true);
    for(i=1, j=0; i<n; i++)
        if(isLMS(i)) s1[j++]=i; // get p1
    // get index in s
    for(i=0; i<n1; i++) SA1[i]=s1[SA1[i]];
    // init SA[n1..n-1]
    for(i=n1; i<n; i++) SA[i]=-1;
    for(i=n1-1; i>=0; i--) {
        j=SA[i];
        SA[i]=-1;
        SA[bkt[chr(j)]--]=j;
    }
    induceSAl(t, SA, s, bkt, n, K, cs, false);
    induceSAs(t, SA, s, bkt, n, K, cs, true);
    free(bkt);
    free(t);
}

void gSAIS(unsigned char*s, int*SA, int n,int K, int cs, unsigned char separator) {
    // polje LS tipova
    unsigned char*t=(unsigned char*)malloc(n/8+1);
    
    int i, j;
    
    //klasifikacija tipova
    tset(n-2, 0); 
    tset(n-1, 1);
    for(i=n-3; i>=0; i--) 
        tset(i, (chr(i)<chr(i+1)|| (chr(i)==chr(i+1)&& tget(i+1)==1))?1:0);

    
    //polje pretinaca, odnosno pokazivaca na dijelove pretinaca
    int*bkt = (int*)malloc(sizeof(int)*(K+1));
    //nadi kraj pretinaca
    getBuckets(s, bkt, n, K, cs, true);
    
    //inicijaliziraj SA
    for(i=0; i<n; i++) SA[i]=-1;

    //dodaj zadnji i predzadnji (rezerviraj mjesto za zadnji separator)
    SA[0] = n-1;
    SA[bkt[separator]--] = n-2;

    int prev = n-2; //prethodno dodani
    //LMS sufiksi se dodaju zdesna nalijevo
    for(i=n-2; i>=0; i--) {
        if(isLMS(i)) {
            //provjeri pocinje li sa separatorom
            if(chr(i) == separator) {
                SA[++bkt[chr(prev)]] = -1; //izbrisi prethodno dodanog
            }
            SA[bkt[chr(i)]--]=i;
            prev = i;
        } 
    }

    //rucno dodaj zadnji sufiks koji pocinje separatorom
    // getBuckets(s, bkt, n, K, cs, true);
    // SA[bkt[separator]] = n-2;

    induceSAl_gSAIS(t, SA, s, bkt, n, K, cs, false, separator);
    induceSAs_gSAIS(t, SA, s, bkt, n, K, cs, true, separator);

    //rucno se dodaju svi separatori
    bkt[separator] = 1;
    for(int i=0; i<n; i++) {
        if(chr(i) == separator) {
            SA[bkt[separator]++] = i;
        }
    }
    free(bkt);

    //pronadi novi smanjeni niz s1
    //sve isto kao u originalnom samo se provjeravaju i separatori
    //ako oba sufiksa zapocinju separatorom oni su nuzno razliciti
    int n1=0;
    for(i=0; i<n; i++) {
        if(isLMS(SA[i])) {
            SA[n1++]=SA[i];
        }
    }
        

    for(i=n1; i<n; i++) {
        SA[i]=-1;
    }
    int name = 0;
    prev = -1;
    for(i=0; i<n1; i++) {
        int pos=SA[i]; 
        bool diff=false;
        for(int d=0; d<n; d++) {
            //dodaj uvjet ako su oba separatori
            if(prev==-1 || chr(pos+d)!=chr(prev+d)||
                (chr(pos+d) == separator && chr(prev+d) == separator) ||
                tget(pos+d)!=tget(prev+d)){ 
                diff=true; 
                break; 
            }else if(d>0 && (isLMS(pos+d) || isLMS(prev+d)))
                break;
        }    
        if(diff) { 
            name++;
            prev=pos;
        }
        pos=(pos%2==0)?pos/2:(pos-1)/2;
        SA[n1+pos]=name-1;
    }
    for(i=n-1, j=n-1; i>=n1; i--) {
        if(SA[i]>=0) SA[j--]=SA[i];
    }

    //novi reducirani problem
    int *SA1 = SA;
    int *s1 = SA + n - n1;
    //ako nisu sva imena jedinstvena pozovi originalni SAIS
    //sada su brojevi umjesto slova pa se koristi normalan SAIS
    if(name<n1) {
        SAIS((unsigned char*)s1, SA1, n1, name, sizeof(int));
    } else {
        for(i=0; i<n1; i++) SA1[s1[i]] = i;
    }
    
    //isto kao u originalnom SAIS
    bkt = (int*)malloc(sizeof(int)*(K));
    //nadi krajeve pretinca
    getBuckets(s, bkt, n, K, cs, true);
    for(i=1, j=0; i<n; i++) {
        if(isLMS(i)) s1[j++]=i; // nadi p1
    }
    //nadi indeks u s
    for(i=0; i<n1; i++) {
        SA1[i]=s1[SA1[i]];
    }
    // inicijaliziraj SA
    for(i=n1; i<n; i++) SA[i]=-1;

    //promjena: rezerviraj mjesto za zadnji separator
    int temp = bkt[separator]--;
    for(i=n1-1; i>=0; i--) {
        j=SA[i];
        SA[i]=-1;
        SA[bkt[chr(j)]--]=j;
    }

    //ubaci zadnji separator
    // getBuckets(s, bkt, n, K, cs, true);
    SA[temp] = n-2;

    induceSAl_gSAIS(t, SA, s, bkt, n, K, cs, false, separator);
    induceSAs_gSAIS(t, SA, s, bkt, n, K, cs, true, separator);
    free(bkt);
    free(t);    
}

bool binarySAISSearch(unsigned char*s, int*SA, int n, string x) {
    int left = 0;
    int right = n-1;
    int middle;
    
    //char letter = x[0];
    while(left<=right) {
        int i = 0;
        middle = (left+right)/2;
        int index = SA[middle];
        if(s[index] == x[i]) {
            bool found = true;
            for(i=1; i<x.size(); i++) {
                index++;
                if(x[i] != s[index]) {
                    found = false;
                    break;
                }
            }
            if(found) return true;
        } 
        if(x[i] < s[index]) right = middle - 1;
        else left = middle + 1;
    }
    return false;
}

int parseLine(char* line) {
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}

int physicalMemoryUsed(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}

int main(int argc, char *argv[]) {
    auto start = chrono::high_resolution_clock::now();

    vector<string> seqFiles;
    vector<string> searchFiles;

    vector<string> sequences;

    for (int i = 1; i < argc; i++){
        if ((string)argv[i] == "-seq") {
            while((string)argv[++i] != "-s") {
                seqFiles.push_back(argv[i]);
            }
        }
        else {
            searchFiles.push_back(argv[i]);
        }
    }

    // seqFiles.push_back("seq1.txt");
    // seqFiles.push_back("seq2.txt");


    for (string s: seqFiles) {
        ifstream inputFile(s);
        stringstream buffer;
        buffer << inputFile.rdbuf();
        string seq = buffer.str();

        //makni prvi red i sve praznine
        seq = seq.substr(seq.find('\n') + 1);
        seq.erase(remove(seq.begin(), seq.end(), '\r'), seq.end());
        seq.erase(remove(seq.begin(), seq.end(), '\n'), seq.end());

        //dodaj u vektor
        sequences.push_back(seq);
    }

    string temp;
    for(string str: sequences) {
        temp.append(str);
        temp.append("1");
    }

    unsigned char s[temp.size()+1];
    strcpy(reinterpret_cast<char*>(s), temp.c_str());

    int n = sizeof(s)/sizeof(unsigned char);
    int *SA = (int*)malloc(sizeof(int)*n);
    int K = 256;
    int cs = sizeof(unsigned char);
    gSAIS(s, SA, n, K, sizeof(unsigned char), '1');

    cout << "veličina: " << n << endl;

    // for(int i=0; i<10; i++) {
    //   cout << SA[i] << " ";
    // }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Vrijeme izgradnje polja: " << duration.count() << " ms" << endl;
    cout << "Utrošak fizičke memorije: " << physicalMemoryUsed() << " KB" << endl;


    vector<string> search;
    // search.push_back("CAGCGTCGTGAGAGGTGATGGTGGA");
    // cout << binarySAISSearch(s, SA, n, search.at(0));

    for (string s: searchFiles) {
        ifstream inputFile(s);
        stringstream buffer;
        buffer << inputFile.rdbuf();
        string seq = buffer.str();

        //makni prvi red i sve praznine
        seq = seq.substr(seq.find('\n') + 1);
        seq.erase(remove(seq.begin(), seq.end(), '\r'), seq.end());
        seq.erase(remove(seq.begin(), seq.end(), '\n'), seq.end());

        stringstream ss(seq);
        string temp;
        while (getline(ss, temp, ',')) {
            search.push_back(temp);
        }
    }

    start = chrono::high_resolution_clock::now();
    for(string str: search) {
        bool found = binarySAISSearch(s, SA, n, str);

        if(!found) cout << "Not found\n";
        //else cout << "found\n";
    }

    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Vrijeme searcha: " << duration.count()/search.size() << " ms" << endl;
    
    return 0;
}