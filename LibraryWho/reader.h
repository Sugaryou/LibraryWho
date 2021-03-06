

#ifndef __Library__reader__
#define __Library__reader__

#include "common.h"


class BookBorrowed {
public:
    ISBN theBook;
    Date borrowDate;
    bool renew;
    BookBorrowed* nextBook;
    
    BookBorrowed() {
        nextBook = NULL;
		//借书日期默认为现在
		time_t rawtime;
        tm* now;
        time(&rawtime);
        now = localtime(&rawtime);//
		borrowDate.year = now-> tm_year + 1900;
		borrowDate.month = now->tm_mon + 1;
        borrowDate.day = now->tm_mday;
        renew = 0;
    }
    Date returnDate();
};

class Reader {
private:
    BookBorrowed *bookBorrowed;
    unsigned rid;
    int borrowed;
    
public:
    string name;
    int level;
    string email;
    
    Reader(unsigned rid);
    ~Reader();
    void importData(ISBN* isbnIn, Date* dateIn, bool* renewIn, int bookNum, int borrowedIn);           //Used to import backup data.
    unsigned getRid();
    int getNumber();
    int getBorrowed();
    int getBook(ISBN* &borrowedList);
    int getDate(Date* &borrowedList);
    int getRenew(bool* &borrowedList);
    int borrowNew(ISBN newBook);           //0.Success, 1.Level limited, 2.Already borrowed.
	int renewBook(ISBN oldBook);            //返回值：0.续借成功； 1.该读者没有借过这本书； 2.已经续借过了 4.逾期
    int returnOld(ISBN oldBook);           //0:Success, 1.No such book, 2.Broken the rule.
};

#endif /* defined(__Library__reader__) */
