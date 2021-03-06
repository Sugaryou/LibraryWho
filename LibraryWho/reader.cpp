

#include "reader.h"


Reader::Reader(unsigned ridIn){
    bookBorrowed = NULL;
    rid = ridIn;
    borrowed = 0;
    level = 1;
}

Reader::~Reader(){
    while (bookBorrowed != NULL) {
        BookBorrowed* p = bookBorrowed;
        bookBorrowed = bookBorrowed->nextBook;
        delete p;
    }
}

unsigned Reader::getRid(){
    return rid;
}

int Reader::getNumber(){
    BookBorrowed *p = bookBorrowed;
    int num = 0;
    while ( p!= NULL) {
        num++;
        p = p->nextBook;
    }
    return num;
}

int Reader::borrowNew(ISBN newBook){
    if (getNumber() == level) {
        return 1;
    }
    if (bookBorrowed == NULL) {
        bookBorrowed = new BookBorrowed;
        bookBorrowed->theBook = newBook;
    } else {
        BookBorrowed* p = bookBorrowed;
        while (p->nextBook != NULL) {
            if (p->theBook == newBook) {
                return 2;
            }
            p = p->nextBook;
        }
        p->nextBook = new BookBorrowed;
        p->nextBook->theBook = newBook;
    }
    borrowed++;
    return 0;
}

int Reader::renewBook(ISBN oldBook){
    BookBorrowed* p = bookBorrowed;
    while (p != NULL) {
        if (p->theBook == oldBook) {
            break;
        }
    }
    if (p == NULL) {
        return 1;
    }
    if (p->renew == 1) {
        return 2;
    }
    time_t rawtime;
    tm* now;
    time(&rawtime);
    now = localtime(&rawtime);
    Date expect = p->returnDate();
    if (expect.year < (now->tm_year + 1900)) {
        return 4;
    }
	else if (expect.year > (now->tm_year + 1900)){
		p->renew = 1;
		return 0;
	}
    else if (expect.month < now->tm_mon+1){
        return 4;
    }
	else if (expect.month > now->tm_mon+1){
        p->renew = 1;
		return 0;
    }
    else if (expect.day < now->tm_mday) {
        return 4;
    }
    else {
        p->renew = 1;
        return 0;
    }
}

int Reader::returnOld(ISBN oldBook) {

    BookBorrowed *pre = NULL;
    BookBorrowed *p = bookBorrowed;
    while (p!=NULL && p->theBook!=oldBook) {
        pre = p;
        p = p->nextBook;
    }
    if (p == NULL) {
        return 1;
    } else {
        int returnNum = 1;
        time_t rawtime;
        tm* now;
        time(&rawtime);
        now = localtime(&rawtime);
        Date expect = p->returnDate();
        if (expect.year < (now->tm_year + 1900) ) {
            returnNum = 2;
        }
		else if(expect.year > (now->tm_year + 1900) ) {
            returnNum = 0;
        }
        else if (expect.month < now->tm_mon+1){
            returnNum = 2;
        }
		else if (expect.month > now->tm_mon+1){
            returnNum = 0;
        }
        else if (expect.day < now->tm_mday) {
            returnNum = 2;
        }
        else {
            returnNum = 0;
        }
        if (pre == NULL) {
            bookBorrowed = p->nextBook;
        } else {
            pre->nextBook = p->nextBook;
        }
        delete p;
        return returnNum;
    }
}

int Reader::getBook(ISBN* &borrowedList) {
    BookBorrowed *p = bookBorrowed;
    int i = 0;
    while (p != NULL) {
        borrowedList[i] = p->theBook;
        p = p->nextBook;
        i++;
    }
    return i;
}

int Reader::getDate(Date* &borrowedList) {
    BookBorrowed* p = bookBorrowed;
    int i = 0;
    while (p != NULL) {
        borrowedList[i] = p->borrowDate;
        p = p->nextBook;
        i++;
    }
    return i;
}

int Reader::getRenew(bool* &borrowedList) {
    BookBorrowed* p = bookBorrowed;
    int i = 0;
    while (p != NULL) {
        borrowedList[i] = p->renew;
        p = p->nextBook;
        i++;
    }
    return i;
}

Date BookBorrowed::returnDate(){
    Date returnDay = borrowDate;
    returnDay.day += 5;//返还日期
    switch (borrowDate.month) {
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
            if (returnDay.day > 31) {
                returnDay.day -= 31;
                returnDay.month += 1;
            }
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            if (returnDay.day > 30) {
                returnDay.day -= 30;
                returnDay.month += 1;
            }
            break;
        case 1:
            if (returnDay.day > 31) {
                returnDay.day -= 31;
                returnDay.month += 1;
            }
        case 2:
            short feb;
            if (returnDay.year%4 == 0) {
                if (returnDay.year%100 == 0) {
                    if (returnDay.year%400 == 0) {
                        feb = 29;
                    } else {
                        feb =28;
                    }
                } else {
                    feb = 29;
                }
            } else {
                feb = 28;
            }
            if (returnDay.day > feb) {
                returnDay.day -= feb;
                returnDay.month += 1;
            }
            break;
        case 12:
            if (returnDay.day > 31) {
                returnDay.day -= 31;
                returnDay.month = 1;
                returnDay.year += 1;
            }
            break;
            
        default:
            returnDay.year = 2000;
            returnDay.month = 1;
            returnDay.day = 1;
            break;
    }
    if (renew) {
        returnDay.day += 30;
        switch (borrowDate.month) {
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
                if (returnDay.day > 31) {
                    returnDay.day -= 31;
                    returnDay.month += 1;
                }
                break;
            case 4:
            case 6:
            case 9:
            case 11:
                if (returnDay.day > 30) {
                    returnDay.day -= 30;
                    returnDay.month += 1;
                }
                break;
            case 1:
                if (returnDay.day > 31) {
                    returnDay.day -= 31;
                    returnDay.month += 1;
                }
            case 2:
                short feb;
                if (returnDay.year%4 == 0) {
                    if (returnDay.year%100 == 0) {
                        if (returnDay.year%400 == 0) {
                            feb = 29;
                        } else {
                            feb =28;
                        }
                    } else {
                        feb = 29;
                    }
                } else {
                    feb = 28;
                }
                if (returnDay.day > feb) {
                    returnDay.day -= feb;
                    returnDay.month += 1;
                }
                break;
            case 12:
                if (returnDay.day > 31) {
                    returnDay.day -= 31;
                    returnDay.month = 1;
                    returnDay.year += 1;
                }
                break;
                
            default:
                returnDay.year = 2000;
                returnDay.month = 1;
                returnDay.day = 1;
                break;
        }
    }
    return returnDay;
}

int Reader::getBorrowed(){
    return borrowed;
}

void Reader::importData(ISBN* isbnIn, Date* dateIn, bool* renewIn, int bookNum, int borrowedIn){
    borrowed = borrowedIn;
    for (int i=0; i<bookNum; i++) {
        BookBorrowed* p = bookBorrowed;
        bookBorrowed = new BookBorrowed;
        bookBorrowed->theBook = isbnIn[i];
        bookBorrowed->borrowDate = dateIn[i];
        bookBorrowed->renew = renewIn[i];
        bookBorrowed->nextBook = p;
    }
}