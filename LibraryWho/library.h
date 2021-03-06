

#ifndef __Library__library__
#define __Library__library__

#include <iostream>
#include "common.h"
#include "reader.h"
#include "book.h"
#include <fstream>

struct ReaderNode {
    Reader *reader;
    ReaderNode *nextReader;
    ReaderNode (unsigned ridIn){
        reader = new Reader(ridIn);
        nextReader = NULL;
    }
    ~ReaderNode(){
        delete reader;
    }
};

struct BookNode {
    Book *book;
    BookNode *nextBook;
    BookNode(ISBN isbnIn){
        book = new Book(isbnIn);
        nextBook = NULL;
    }
    ~BookNode() {
        delete book;
    }
};

class Library {
    unsigned bookNumber;
    unsigned readerNumber;
    unsigned ridCount;
    ReaderNode* readerPool;
    BookNode* bookPool;
    
    int buildReaderPool(string filePath);   //Import the reader pool from *.lwr.
    int buildBookPool(string filePath);     //Import the book pool from *.lwb.
    int exportBookPool(string filePath);    //Export the book pool into *.lwb.
    int exportReaderPool(string filePath);  //Export the reader pool into *.lwr.
    
public:
    Book* bookNow;
    Reader* readerNow;
    
    Library();
    ~Library();
    int importBooks(string filePath, int num);       //-1.Fail, 0.Success
    int printAllBooks(string filePath);     //-1.Fail, 0.Success need to change
    int printAllReaders(string filePath);   //-1.Fail, 0.Success
    int buildALibrary(string filePath);     //0.Success, 1.Book fail, 2.Reader fail.
    int addReaders(int num, int level);     //0.Success, 1-3, Fail.
	bool deleteReaders();  //删除读者
	bool deleteBooks();  //删除书籍
	bool addBooks(); //增加书本
	bool addquality(); //增加库存
    bool ISBN_search();                     //0.Fail, 1.Success
    void BOOKNAME_search();                 
	void AUTHOR_search();
    bool printBookInfo();                   //Print the information of the book.
    bool printReaderInfo();                 //Print the information of the reader.
    int borrowBook();                       //Reader now borrow the book now.
    int returnBook();                       //Reader now return the book now.
    int exportTheLibrary(string filepath);  //0.Success, 1.Book fail, 2.Reader fail.
	bool RID_search();                      //0.Fail, 1.Success
    bool READERNAME_search();               //0.Fail, 1.Success
    bool EMAIL_search();                    //0.Fail, 1.Success
	void Time_now();                        //显示当前时间
	int RenewBook();      //续借
};

#endif /* defined(__Library__library__) */