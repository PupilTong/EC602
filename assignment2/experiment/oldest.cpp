//Copyright 2019 haoyangWang haoyangw@bu.edu
#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
using std::vector;
using std::string;
struct filestruct{
    string fileName;
    int mtime;
    //refrence::operator reload
    //https://blog.csdn.net/qq_16949707/article/details/51579926
    bool operator< (const filestruct r)const { return mtime < r.mtime; }
    bool operator<= (const filestruct r)const { return mtime <= r.mtime; }
    bool operator> (const filestruct r)const { return mtime > r.mtime; }
    bool operator>= (const filestruct r)const { return mtime >= r.mtime; }
};




//refrence::fast sort
//https://zh.wikipedia.org/wiki/%E5%BF%AB%E9%80%9F%E6%8E%92%E5%BA%8F#C++
template <typename T>
void quick_sort_recursive(vector<T> & arr, int start, int end) {
    if (start >= end)
        return;
    T mid = arr[end];
    int left = start, right = end - 1;
    while (left < right) { //在整个范围内搜寻比枢纽元值小或大的元素，然后将左侧元素与右侧元素交换
        while (arr[left] < mid && left < right) //试图在左侧找到一个比枢纽元更大的元素
            left++;
        while (arr[right] >= mid && left < right) //试图在右侧找到一个比枢纽元更小的元素
            right--;
        std::swap(arr[left], arr[right]); //交换元素
    }
    if (arr[left] >= arr[end])
        std::swap(arr[left], arr[end]);
    else
        left++;
    quick_sort_recursive(arr, start, left - 1);
    quick_sort_recursive(arr, left + 1, end);
}
template <typename T> //整數或浮點數皆可使用,若要使用物件(class)時必須設定"小於"(<)、"大於"(>)、"不小於"(>=)的運算子功能
void quick_sort(vector<T> & arr, int len) {
    quick_sort_recursive(arr, 0, len - 1);
}

//reference::stat() function 
//https://linux.die.net/man/2/stat
//reference::dirent.h
//https://www.cnblogs.com/Harley-Quinn/p/6367425.html

int main(int argc, char **argv){
    auto directory = opendir(".");
    vector<filestruct> fileinfoArray;
    while(directory!=nullptr){
        dirent64 * dirinfo = readdir64(directory);
        if(dirinfo==nullptr){
            break;
        }
        struct stat fileinfo;
        if(dirinfo->d_type==8){
            stat(dirinfo->d_name, &fileinfo);
            filestruct tempFileStruct;
            tempFileStruct.fileName=dirinfo->d_name;
            tempFileStruct.mtime=fileinfo.st_mtim.tv_sec;
            fileinfoArray.push_back(tempFileStruct);
        }
    }
    closedir(directory);
    quick_sort(fileinfoArray,fileinfoArray.size());
    
    try
    {
        if(argc==2){
            for(int i=0;i<std::stoi(argv[1]) && i< fileinfoArray.size();i++){
            std::cout<< fileinfoArray[i].fileName <<"\n";
            }
        }
        else{
            std::cerr <<  "Too much or less input!"<<"\n";
            auto e = std::exception();
            throw e;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "An error Occured!"<<"\n";
    }
    
    return 0;
}

