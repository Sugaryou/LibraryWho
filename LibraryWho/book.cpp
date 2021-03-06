

#include "book.h"

BorrowNode::BorrowNode() {
    rid = 0;
    nextReader = NULL;
}

Book::Book(ISBN isbnIn) {
    isbn = isbnIn;
    date.year = 2000;
    date.month = 1;
    date.day = 1;
    price = 0;
    quantity = 0;
    readers = NULL;
}

Book::~Book() {
    BorrowNode *p = readers;
    while (readers != NULL) {
        p = readers;
        readers = readers->nextReader;
        delete p;
    }
}

void Book::inputPrice(double priceIn) {
    if (priceIn < 0) {
        price = 0;
        return;
    }
    price = (unsigned)(priceIn * 100);
}

double Book::outputPrice() {
    return ((double)price)/100;
}

ISBN Book::getISBN() {
    return isbn;
}

int Book::getReaderNum() {
    BorrowNode *p = readers;
    int num = 0;
    while (p != NULL) {
        num++;
        p = p->nextReader;
    }
    return num;
}

int Book::getTotal() {
    return getReaderNum()+quantity;
}

int Book::getReaders(unsigned* &rid) {
    BorrowNode *p = readers;
    for (int i=0; i<getReaderNum(); i++) {
        rid[i] = p->rid;
        p = p->nextReader;
    }
    return getReaderNum();
}

int Book::borrowBook(unsigned rid) {
    if (quantity < 1) {
        return 1;
    }
    BorrowNode* p = readers;
    while (p != NULL) {
        if (p->rid == rid) {
            return 2;
        }
        p = p->nextReader;
    }
    p = readers;
    readers = new BorrowNode;
    readers->rid = rid;
    readers->nextReader = p;
    quantity--;
    return 0;
}

int Book::returnBook(unsigned rid) {
    BorrowNode* p = readers;
    BorrowNode* pre = NULL;
    while (p != NULL) {
        if (p->rid == rid) {
            if (pre == NULL) {
                readers = p->nextReader;
            } else {
                pre->nextReader = p->nextReader;
            }
            delete p;
            quantity++;
            return 0;
        } else {
            pre = p;
            p = p->nextReader;
        }
    }
    return 1;
}