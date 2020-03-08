#include<iostream>
#include<vector>
#include<list>

using namespace std;

#define OPT 1

class Sort {
    public:
        Sort(bool autoinit=true);
        virtual void Exec() = 0;
        void AddElem(int elem);
        void Print();
        void Swap(int a, int b);
    public:
        int cnt;
        vector<int> data;
};

Sort::Sort(bool autoinit) {
    cnt = 0;
    data.clear();
    if (autoinit) {
        data.push_back(1);
        data.push_back(8);
        data.push_back(3);
        data.push_back(6);
        data.push_back(5);
        data.push_back(4);
        data.push_back(7);
        data.push_back(2);
        data.push_back(9);
    }  
    return;
}
void Sort::AddElem(int elem) {
    data.push_back(elem);
    return;
}
void Sort::Print() {
    for (vector<int>::iterator it = data.begin(); it != data.end(); it++) {
        cout << *it << " ";
    }
    cout<<",cnt: " << cnt << endl;
    return;
}
void Sort::Swap(int a, int b) {
    int tmp = data[a];
    data[a] = data[b];
    data[b] = tmp;
    return;
}

/*
    比较类排序
*/
//交换排序(具体执行方式是元素间两两比较)
////冒泡排序
/*
比较相邻的元素。如果前面比后面大，就交换它们两个
对每一对相邻元素作同样的工作，从开始第一对到结尾的最后一对，这样每次遍历在最后的元素应该会是最大的数
针对所有的元素重复以上的步骤(每次都对少做最后一次，因为最后的元素已经排好序)
*/
class Bubble : public Sort {
    void Exec() {
        for (size_t i = 0; i < data.size(); i++) {
            bool flag = false;
            for (size_t j = 0; j < data.size() - i - 1; j++) {
                if (data[j] > data[j+1]) {
                    Swap(j, j+1);
                    cnt++;
                    flag = true;
                }
            }
            if (!flag) {
                break;
            }
        }
    }
};
////快速排序
/*
分治法
从数列中挑出一个元素作为基准(pivot),所有比基准值小的摆放在基准前面，所有比基准值大的摆在基准的后面.此时该基准就处于数列的中间位置(partition).
后续递归地在小于基准值元素的子数列和大于基准值元素的子数列再做类似处理
*/
class Quick : public Sort {
    public:
        int partition(int left, int right) {
            int pivot = data[left];
            int i = left + 1;
            int j = right;
            while(true) {
                while(i<=j && data[i] <= pivot) i++;    //从右边扫描直至找到比pivot大的数值
                while(i<=j && data[j] >= pivot) j--;    //从左边扫描直至找到比pivot小的数值
                if (i >= j) //交汇了，说明正好扫描完，找到pivot对应的位置，退出
                    break;
                Swap(i, j); //在扫描过程中，要把不符合pivot两端要求的元素换一下
                cnt++;
            }
            data[left] = data[j];
            data[j] = pivot;
            cnt++;
            return j;
        }

        void quickSort(int left, int right) {
            if (left < right) {
                int mid = partition(left, right);
                quickSort(left, mid - 1);   //这里要留意元素如果太多，造成的栈空间资源
                quickSort(mid + 1, right);
            }
            return;
        }
        
        void Exec() {
            quickSort(0, data.size() - 1);
        }
        Quick(bool autoinit=true):Sort(autoinit) {}
};

//插入排序
////简单插入排序（一般不会用）
/*
思路简单，通过构建有序序列，对于未排序数据，在已排序序列中从后向前扫描，找到相应位置并插入
从第一个元素开始，该元素可以认为已经被排序；
取出下一个元素，在已经排序的元素序列中从后向前扫描；
如果该元素大于新元素，将该元素移到下一位置；
重复步骤3，直到找到已排序的元素小于或者等于新元素的位置；
将新元素插入到该位置后
重复上述步骤
*/
class SimplyInsert : public Sort {
    void Exec() {
        for (int i = 1; i < data.size(); i++) {
            int tmp = data[i];
            for (int j = i - 1; j >= 0; j--) {
                cnt++;
                if ( data[j] > tmp) {
                    data[j+1] = data[j];
                } else {
                    if (j+1 != i)
                        data[j+1] = tmp;
                    break; 
                }
            }
        }
    }
};
////希尔排序（会有用）
/*
与插入排序的不同之处在于，它会优先比较距离较远的元素(缩小增量排序)
先让数组中任意间隔为 h 的元素有序，刚开始 h 的大小可以是 h = n / 2
接着让 h = n / 4，让 h 一直缩小，当 h = 1 时，也就是此时数组中任意间隔为1的元素有序，此时的数组就是有序
1.选择一个增量序列t1，t2，…，tk，其中ti>tj，tk=1；
2.按增量序列个数k，对序列进行k 趟排序；
3.每趟排序，根据对应的增量ti，将待排序列分割成若干长度为m 的子序列，分别对各子表进行直接插入排序(这样排序下来能有个粗略的排序，后续排序触发的次数会少很多)。
仅增量因子为1 时，整个序列作为一个表来处理（此时触发插入的次数就少很多），表长度即为整个序列的长度
*/
class Shell : public Sort {
    void insert(int h, int loc) {
        int val = data[loc];
        int k;
        for (k = loc - h; k>=0 && val<data[k]; k-=h) {
            data[k+h] = data[k];
        }
        data[k + h] = val;
    }
    void Exec() {
        size_t len = data.size();
        for (size_t h = len/3; h > 0; h /=3) {
            for (int i = h; i < len; i++) {
                insert(h, i);   //从整个数组来看，是间隔性切割成n个数组来进行插入排序
                //if(data[i] < data[i - h])
                //  Swap(i, i-h);
                //  cnt++;
            }
        }
    }
};

//选择排序
////简单选择排序（一般不会用）
/*
思路简单：每次遍历找到最值，然后放到对应的位置
*/
class SimpleSelect : public Sort {
    void Exec() {
        size_t len = data.size();
        for (size_t i = 0; i < len; i++) {
            int val = i;
            for (size_t j = i+1; j < len; j++) {
                if (data[i] > data[j])
                    val = j;
            }
            if (i != val)
                Swap(i, val);

        }
    }
};
////堆排序（重点）
/*
1.将初始待排序关键字序列(R1,R2….Rn)构建成大顶堆，此堆为初始的无序区
2.将堆顶元素R[1]与最后一个元素R[n]交换，得到新的无序区(R1,R2,……Rn-1)和新的有序区(Rn),且满足R[1,2…n-1]<=R[n]；
3.由于交换后新的堆顶R[1]可能违反堆的性质，因此需要对当前无序区(R1,R2,……Rn-1)调整为新堆，然后再次将R[1]与无序区最后一个元素交换，得到新的无序区(R1,R2….Rn-2)和新的有序区(Rn-1,Rn)
4.不断重复此过程直到有序区的元素个数为n-1，则整个排序过程完成
*/
class Heap: public Sort {
    void heapity(int i, int len) {
        int lchild = i * 2 + 1;
	    int rchild = i * 2 + 2;
        int max = i;
        if (lchild < len && data[lchild] > data[max])
            max = lchild;
        if (rchild < len && data[rchild] > data[max])
            max = rchild;
        if(i != max) {  //不满足大堆条件，需要调整
            cnt++;
            Swap(i, max);
            heapity(max, len);
        }
        return;
    }
    
    void buildHeap() {
        size_t len = data.size();
        for(int i = len/2 - 1; i >= 0; i--) {
            heapity(i, len);
        }
    }

    void Exec() {
        buildHeap();    //建堆
        for (int i = data.size() - 1; i > 0; i--) {
            Swap(0, i);     //这里是堆排序的一个缺陷：此时若尾部是一个小值，会反复触发交换操作
            heapity(0, i);  //重新建堆logn
        }
    }
};


//归并排序（重点）
/*
分治法
把大的数组分成两个，然后对这两个数组分别进行排序，之后在把这两个数组合并成一个有序的数组
*/
class Merge : public Sort {
    void MergeProc(int left, int mid, int right) {
        int len = right - left + 1;
        int i = left;
        int j = mid + 1;
        vector<int> tmp;    //需要额外空间来保存归并好的序列
        tmp.clear();

        while(i <= mid && j <= right)
            tmp.push_back(data[i]<=data[j] ? data[i++] : data[j++]);
        while(i <= mid)
            tmp.push_back(data[i++]);
        while(j <= right)
            tmp.push_back(data[j++]);
        for (int idx = 0; idx < len; idx++)
            data[left++] = tmp[idx];
        return;
    }
    //递归实现
    void MergeSortRecur(int left, int right) {
        if(left >= right) {
            return;
        }
        int mid = (left + right) / 2;
        MergeSortRecur(left, mid);
        MergeSortRecur(mid+1, right);
        MergeProc(left, mid, right);
        return;
    }
    //非递归实现（一般就是迭代）
    void MergeSortIteration() {
        int len = data.size();
        int mid, right, left;
        for (int i = 1; i < len; i *=2) {   //每次归并的间隔
            left = 0;   //最开始肯定从0起
            while(left + i < len) { //迭代式归并
                mid = left + i - 1;
                right = mid + i < len ? mid + i : len - 1;  //是否还足够右边的
                MergeProc(left, mid, right);
                left = right + 1;  //跳到下一个迭代
            }
        }
        return;
    }
    void Exec() {
        //MergeSortRecur(0, data.size() - 1);
        MergeSortIteration();
    }
};

/*
    非比较排序
*/
//计数排序
/*
将输入的数据值转化为键存储在额外开辟的数组空间中。 作为一种线性时间复杂度的排序，计数排序要求输入的数据必须是有确定范围的整数（适合元素范围不是很大并且序列比较集中时）
1.找出待排序的数组中最大和最小的元素；
2.统计数组中每个值为i的元素出现的次数，存入数组C的第i项；
3.对所有的计数累加（从C中的第一个元素开始，每一项和前一项相加）；
4.反向填充目标数组：将每个元素i放在新数组的第C(i)项，每放一个元素就将C(i)减去1
*/
class CntSort : public Sort {
    void Exec() {
        //找出数组中的极致
        int min = data[0],max = data[0];
        for (size_t i = 1; i < data.size(); i++) {
            if (min > data[i])
                min = data[i];
            if (max < data[i])
                max = data[i];
        }
        //开始计数
        vector<int> count(max-min+1, 0);
        for (size_t i = 1; i < data.size(); i++) {
            count[data[i] - min]++;
        }
        //根据计数反向填充
        int loc = 0;
        for (size_t i = 0; i < count.size(); i++) {
            if (count[i] > 0) {
                data[loc++] = i;
                count[i]--;
            }
        }
    }
};

//桶排序
/*
将数组中的最大值和最小值之间的数进行瓜分，分成 n 个区间，n个区间对应n个桶，把各元素放到对应区间的桶中去，再对每个桶中的数进行排序（快排或堆排）
之后每个桶里面的数据就是有序的，继而整个数组也是有序的
桶划分的越小，各个桶之间的数据越少，排序所用的时间也会越少。但相应的空间消耗就会增大
*/
class BucketSort : public Sort {
    void Exec() {
        int min = data[0],max = data[0];
        for (size_t i = 1; i < data.size(); i++) {
            if (min > data[i])
                min = data[i];
            if (max < data[i])
                max = data[i];
        }
        //分区,第 i 桶存放  bucketSize*i ~ bucketSize*i+bucketSize-1范围的数
        int bucketNum = (max - min)/bucketSize + 1;
        vector<Quick> bucketList(bucketNum, Quick(false));  //每个桶里的元素采用递归桶（直至桶大小为1，此时退化成计数排序）或者其他排序算法排序
        //元素放入桶中
        for (size_t i = 0; i < data.size(); i++) {
            bucketList[(data[i] - min)/bucketSize].AddElem(data[i]);
        }
        //对每个桶中的元素进行排序
        for (size_t i = 0; i < bucketList.size(); i++) {
            if (0 != bucketList[i].data.size()) {
                bucketList[i].Exec();
            }
        }
        //汇总（回写）
        int loc = 0;
        for (size_t i = 0; i < bucketList.size(); i++) {      
            for (size_t j = 0; j < bucketList[i].data.size(); j++) {
                 data[loc++] = bucketList[i].data[j];
            }
        }
    }
    public:
        static const int bucketSize = 5;
};

//基数排序
/*
基数排序是按照低位先排序，然后收集；再按照高位排序，然后再收集；依次类推，直到最高位
（通用说法）有时候有些属性是有优先级顺序的，先按低优先级排序，再按高优先级排序。最后的次序就是高优先级高的在前，高优先级相同的低优先级高的在前
1.取得数组中的最大数，并取得位数；
2.arr为原始数组，从最低位开始取每个位组成radix数组；
    -->先以个位数的大小来对数据进行排序，接着以十位数的大小来多数进行排序，接着以百位数的大小...
       用10个桶（10进制）归类，这样每次得到的都是以某一位为基准的有序列
3.对radix进行计数排序（利用计数排序适用于小范围数的特点）；
*/
class RaidxSort : public Sort {
    public:
        RaidxSort():Sort(false) {
            data.push_back(3);
            data.push_back(44);
            data.push_back(38);
            data.push_back(5);
            data.push_back(47);
            data.push_back(15);
            data.push_back(36);
            data.push_back(26);
            data.push_back(27);
            data.push_back(2);
            data.push_back(46);
            data.push_back(4);
            data.push_back(19);
            data.push_back(50);
            data.push_back(48);
        }
        void Exec() {
            //找到最大值，看看位数
            int max = data[0];
            for (size_t i = 1; i < data.size(); i++) {
                if (max < data[i])
                    max = data[i];
            }
            int num = 0;
            while (max) {
                max /= 10;
                num++;
            }
            if (!num) {
                //玩屁
                return;
            }
            vector<Quick> bucketList(10, Quick(false));             //10进制，所以10个桶
            for (int i = 1, radio = 1; i <= num; i++, radio *= 10) { //依次按位来进行排序
                for (size_t j = 0; j < data.size(); j++) {
                    bucketList[(data[j]/radio)%10].AddElem(data[j]);  //按位归类
                }
                //回写
                int loc = 0;
                for (size_t i = 0; i < bucketList.size(); i++) {      
                    for (size_t j = 0; j < bucketList[i].data.size(); j++) {
                        data[loc++] = bucketList[i].data[j];
                    }
                    bucketList[i].data.clear();    //清空
                }
            }
        }
};

/*
    测试
*/
void test(Sort* obj) {
    obj->Exec();
    obj->Print();
}

int main() {
    Bubble v1;
    test(&v1);
    Quick v2;
    test(&v2);
    SimplyInsert v3;
    test(&v3);
    Heap v4;
    test(&v4);
    Merge v5;
    test(&v5);
    SimpleSelect v6;
    test(&v6);
    Shell v7;
    test(&v7);

    CntSort v8;
    test(&v8);
    BucketSort v9;
    test(&v9);
    RaidxSort v10;
    test(&v10);
    return 0;
}