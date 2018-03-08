#include <assert.h>
#include <iostream>
using namespace std;
//#include <fstream>
const int DefaultSize = 1000;
template <class T>
class bitSet {
    //用位向量来存储集合元素, 集合元素的范围在0到
    //setSize-1之间。数组采用16位无符号短整数实现
public:
    bitSet (int sz = DefaultSize);                      //构造函数
    bitSet (const bitSet<T>& R);                        //复制构造函数
    ~bitSet () { delete [ ]bitVector; }               //析构函数
    unsigned short getMember (const T x);               //读取集合元素x
    void putMember (const T x, unsigned short v);       //将值v送入集合元素x
    void makeEmpty () {                                 //置空集合
        for (int i = 0; i < vectorSize; i++)
            bitVector[i] = 0;
    }                             //置空

    bool addMember (const T x);                         //加入新成员x
    bool AND_addMember (const T x);                     //与的方式加新成员
    bool OR_addMember (const T x);                      //或的方式加新成员
    bool delMember (const T x);                         //删除老成员x
    bitSet<T>&  operator = (const bitSet<T>& R);
    bitSet<T>&  operator + (const bitSet<T>& R);
    bitSet<T>&  operator * (const bitSet<T>& R);
    bitSet<T>&  operator - (const bitSet<T>& R);
    bool Contains (const T x);						    //判x是否集合this的成员

private:
    int setSize;                                        //集合大小
    int vectorSize;                                     //位数组大小
    unsigned short *bitVector;                          //存储集合元素的位数组
};

template <class T>
bitSet<T>::bitSet (int sz) : setSize (sz + 1) {    //构造函数
    assert (setSize > 0);                           //检查参数的合理性
    vectorSize = (setSize+15) >> 4;                 //存储数组大小
    bitVector = new unsigned short[vectorSize];     //分配空间
    assert (bitVector != NULL);						//检查存储分配是否成功
    for (int i = 0; i < vectorSize; i++)
        bitVector[i] = 0;                           //初始化
};

template<class T>
unsigned short bitSet<T>:: getMember (const T x) {
    //读取集合元素x，x从0开始
    int ad = x/16;
    int id = x%16;
    unsigned short elem = bitVector[ad];
    return ((elem >> (15 - id)) %2);
};

template<class T>
bool bitSet<T>:: delMember (const T x) {
    assert(x >= 0 && x < setSize);
    if(getMember(x) == 1){
        putMember(x, 0);
        return true;
    }

    return false;
};

template<class T>
bool bitSet<T>:: addMember (const T x) {
    assert(x >= 0 && x < setSize);
    if(getMember(x) == 0){
        putMember(x, 1);
        return true;
    }

    return false;
};

template<class T>
bool bitSet<T>:: AND_addMember (const T x) {
    assert(x >= 0 && x < setSize);
    if(getMember(x) == 1){
        putMember(x, 1);
        return true;
    }

    return false;
};

template<class T>
bool bitSet<T>:: OR_addMember (const T x) {
    assert(x >= 0 && x < setSize);

    putMember(x, 1);

    return true;
};



template<class T>
void bitSet<T>:: putMember (const T x, unsigned short v) {  //将值v送入集合元素x
    int ad = x/16;
    int id = x%16;
    unsigned short elem = bitVector[ad];
    unsigned short temp = elem >> (15-id);
    elem = elem << (id+1);
    if (temp%2==0 && v ==1) temp = temp +1;
    else if (temp%2==1 && v==0) temp = temp -1;
    bitVector[ad] = (temp<<(15-id)) | (elem >> (id+1));
};

template<class T>
bitSet<T>& bitSet<T>::operator = (const bitSet<T>& R) {

    assert (vectorSize == R.vectorSize);
    bitSet temp (setSize);

    for (int i = 0; i < vectorSize; i++)
        this->bitVector[i] = R.bitVector[i];

    return *this;
}

template<class T>
bitSet<T>& bitSet<T>::operator + (const bitSet<T>& R) {
    assert (vectorSize == R.vectorSize);
    bitSet temp (setSize);

    for (int i = 0; i < vectorSize; i++)
        this->bitVector[i] = this->bitVector[i] | R.bitVector[i];

    return *this;  //按位求"或", 由第三个集合返回
};          //求集合this与R的并

template <class T>
bitSet<T>&  bitSet<T>::operator * (const bitSet<T>& R) {
    assert (vectorSize == R.vectorSize);
    //bitSet temp (setSize);

    for (int i = 0; i < vectorSize; i++)
        this->bitVector[i] = this->bitVector[i] & R.bitVector[i];

    return *this;  //按位求“与”, 由temp返回
};          //求集合this与R的交

template <class T>
bitSet<T>&  bitSet<T>::operator - (const bitSet<T>& R) {
    assert (vectorSize == R.vectorSize);
    bitSet temp (setSize);

    for (int i = 0; i < vectorSize; i++)
        temp.bitVector[i] =  bitVector[i] & (~R.bitVector[i]);

    return temp;			//由第三个集合返回
};          //求集合this与R的差
