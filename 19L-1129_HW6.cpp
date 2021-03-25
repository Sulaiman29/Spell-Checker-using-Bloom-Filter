#include <iostream>
#include <cmath>
#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <random>
#include <chrono>
#include <Windows.h>;

using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

class BitVector {
    unsigned char* bytes;
    int nbits;//how many bit present logically
public:
    BitVector(int nb) {
        nbits = nb;
        int n = ceil(nbits / 8.0);
        bytes = new unsigned char[n];
        for (int i = 0;i < n;i++)
            bytes[i] = 0;//clear everything at start
    }

    void set(int bno) {
        int byten = bno / 8;
        bno = bno % 8;
        unsigned char mask = 0x80;//10000000
        mask = mask >> bno;
        bytes[byten] = bytes[byten] | mask;
    }

    void clear(int bno) {
        int byten = bno / 8;
        bno = bno % 8;
        unsigned char mask = 0x80;
        mask = ~(mask >> bno);//000001000 = 111110111
        bytes[byten] = bytes[byten] & mask;
    }

    bool get(int bno) {
        int byten = bno / 8;
        bno = bno % 8;
        unsigned char mask = 0x80;
        mask = mask >> bno;
        int x = bytes[byten] & mask;
        return (x != 0);
    }

};
int power(int num, int pow)
{
    unsigned long long n1 = num;
    int p = 16777213;
    if (pow == 0)
        return 1;
    else if (pow == 1)
        return num;
    else
    {
        for (int i = 0; i < pow - 1;i++)
        {
            n1 = ((n1 % p) * (num % p)) % p;
        }
        return n1;
    }
}
int my_rand()
{
    int num = 0;
    for (int i = 0;i < 23;i++)
    {
        num = num + ((rand() % 2) * power(10, i));
    }
    return num;
}

class BloomFilter
{
private:
    BitVector bitv;
    int hash[6] = { 0 };
    int p = 16777213;

public:
    BloomFilter():bitv(728000)
    {
        //bitv.BitVector(91000);
        for (int i = 0;i < 6;i++)
        {
            hash[i] = uniform_real_distribution<double>(0, 16777212)(rng);
            //cout << hash[i]<<endl;
        }
    }

    int Hash_func(string& word, int& a)
    {
        int num = 0;
        int size = word.length() - 1;
        int j = 0;


            for (int i = size;i >= 0;i--)
            {
                num += (word[j] * power(a, i)) % p;
                j++;
            }
            int rem = (num % p) % 728000;
            return rem;
        
    }

    void insert(string& word)
    {
        for (int x = 0;x < 6;x++)
        {
            int a = hash[x];
            int num = Hash_func(word, a);
            bitv.set(num);
        }
        
    }
    
    bool lookup(string& word)
    {
        for (int x = 0; x < 6; x++)
        {
            if (!bitv.get(Hash_func(word, hash[x])))
            {
                return false;
            }
        }
        return true;
    }
    
};

string format(string& k)
{
    string temp = "";
    
    for(int i = 0;k[i] != '\0';i++)
    {
        if (k[i] >= 'a' && k[i] <= 'z')
        {
            temp = temp + k[i];
        }
        else if (k[i] >= 'A' && k[i] <= 'Z')
        {
            temp += (k[i] + 32);
        }
        
    }

    return temp;
}



int main()
{
    BloomFilter myfilter;
    fstream myfile;
    string w;

    double done = 0; int n = 0;
    char a = 219;
    system("color 0e");
    cout << "LOADING......|";

    myfile.open("dict.txt");
    while (!myfile.eof())
    {
        if (n % 910 == 0)
        {
            Sleep(5);
            cout << a;
        }
        getline(myfile, w);
        //cout << w << endl;
        myfilter.insert(w);
       
        //done = (n / 91000.0) * 100;
       // cout << "LOADING......" << done << "%";
        //cout << "LOADING....";
        n++;
    }
    cout << "|\n";
    myfile.close();
    
    //-----------------------------------------------------
    string key;
    cout << "\nCheck Spelling Of:";
    getline(cin, key);

    string temp = "", temp2 = "";

    for(int i = 0;key[i] != '\0';i++)
    {
        if (key[i] == ' ' || key[i + 1] == '\0')
        {
            if (key[i + 1] == '\0')
            {
                temp = temp + key[i];
            }
            temp2 = format(temp);


            //check now if present
            if (myfilter.lookup(temp2))
            {
                cout << temp << "  correct";
            }
            else
            {
                cout << "[" << temp2 << "] Incorrect";
            }
            temp2 = "",temp = "";
        }
        else
        {
            temp = temp + key[i];
        }
    }


    return 0;
}
