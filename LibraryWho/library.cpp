
#include "library.h"
#include <iostream>
using namespace std;

Library::Library() {
    bookNumber = 0;
    readerNumber = 0;
    ridCount = 0;
    readerPool = NULL;
    bookPool = NULL;
    readerNow = NULL;
    bookNow = NULL;
}

Library::~Library() {
    while (readerPool != NULL) {
        ReaderNode *p = readerPool;
        readerPool = readerPool->nextReader;
        delete p;
    }
    while (bookPool != NULL) {
        BookNode *p = bookPool;
        bookPool = bookPool->nextBook;
        delete p;
    }
}

int Library::importBooks(string filePath, int num) {
    ifstream fin(filePath);
    if (!fin) return -1;
    string reading;
    string buffer;
    double price;
    BookNode* p;
    fin >> buffer;                          //Read the No.
    reading = buffer;
    fin >> buffer;                          //Read name into the buffer.
    while (reading != buffer) {
        ISBN tmpISBN;
        fin >> reading;                     //Read the next string.
        while (!(tmpISBN << reading)) {     //If this string is not an ISBN code.
            buffer += " ";                  //Add this string to the name.
            buffer += reading;
            fin >> reading;                 //Read another string.
        }
        p = bookPool;                       //If this string is an ISBN code.
        bookPool = new BookNode(tmpISBN);   //Create a new book node with this ISBN code.
        bookPool->nextBook = p;
        bookPool->book->name = buffer;      //Input its name.
        fin >> buffer;                      //Input the author.
        Date tmpDate;
        fin >> reading;                     //Input another string.
        while (!(tmpDate << reading)) {     //If this string is not a Date string.
            buffer += " ";                  //Add this string to the author.
            buffer += reading;
            fin >> reading;                 //Read another string.
        }
        bookPool->book->date = tmpDate;     //If this string is a Date code.
        bookPool->book->author = buffer;    //Input the author and the date to the new node.
        fin >> price;                       //Read the price.
        bookPool->book->inputPrice(price);  //Input the price to the new node.
        bookPool->book->quantity = num;     //Input the quantity of the new node.
        bookNumber++;
        tmpDate.year = 0;
        tmpISBN.group_a = "0";
        fin >> buffer;                          //Read the No.
        reading = buffer;
        fin >> buffer;                          //Read name into the buffer.
    }
    fin.close();
    return 0;
}

int Library::printAllBooks(string filePath) {
    ofstream fout(filePath);
    if (!fout) return -1;
    BookNode *p = bookPool;
    int i = 1;
    while (i <= bookNumber) {
        fout << i << ".  ";
        fout << p->book->name;
        fout << p->book->getISBN().group_a << '-';
        fout << p->book->getISBN().group_b << '-';
        fout << p->book->getISBN().group_c << '-';
        fout << p->book->getISBN().group_d << '-';
        fout << p->book->getISBN().group_e << ' ';
        fout << p->book->author << ' ';
        fout << p->book->date.year << '-';
        fout << p->book->date.month << '-';
        fout << p->book->date.day << ' ';
        fout << p->book->outputPrice() << '\n';
        p = p->nextBook;
        i++;
    }
    fout.close();
    return 0;
}

bool Library::ISBN_search() {
    string look_for;
    printf("请输入将要查找的书籍的ISBN，形如(978-x-xxx-xxxx-x): ");
    cin  >> look_for;
	cout<<endl;
    ISBN fuck;
    if (!(fuck << look_for)){
        cout << "无效的 ISBN " << look_for << endl;
		bookNow = NULL;
        return 0;
    }
    BookNode * test;
    for( test = bookPool; test != NULL; test = test->nextBook ){
        if(test->book->getISBN() == fuck){
            break;
        }
    }
    if(test == NULL){
        cout << "抱歉，未能找的ISBN为 " << look_for <<" 的书籍。"<< endl<< endl;
		bookNow = NULL;
        return 0;
    } else {
        bookNow = test->book;
        return 1;
    }
}

void Library::BOOKNAME_search(){
    string look_for;
    printf("请输入将要查找的书籍的书名: ");
    cin >> look_for;
	cout<<endl;
    BookNode * test;
	int count = 0;
    for( test = bookPool; test != NULL; test = test->nextBook ){
		if(test->book->name.find(look_for) != test->book->name.npos){
			bookNow = test->book;
			printBookInfo();
			cout<<endl;
			count ++;
        }
    }
	cout<<"总共找到 "<<count<<" 本包含 "<<look_for<<" 字样的书籍。"<<endl<<endl;    
}

bool Library::printBookInfo() {
    if (bookNow == NULL) {
        return false;
    }
    cout << "书名:        " << bookNow->name << endl;
    cout << "ISBN:        ";
    cout << bookNow->getISBN().group_a << '-';
    cout << bookNow->getISBN().group_b << '-';
    cout << bookNow->getISBN().group_c << '-';
    cout << bookNow->getISBN().group_d << '-';
    cout << bookNow->getISBN().group_e << endl;
    cout << "作者:        " << bookNow->author << endl;
    cout << "印刷日期:    ";
    cout << bookNow->date.year << '-';
    cout << bookNow->date.month << '-';
    cout << bookNow->date.day << endl;
    cout << "价格:        " << bookNow->outputPrice() << endl;
    cout << "库存:        " << bookNow->quantity << endl;
    int readerNum = bookNow->getReaderNum();
    unsigned* ridList = new unsigned[readerNum];
    bookNow->getReaders(ridList);
    cout << "已借阅:      " << readerNum << endl;
	if(readerNum >0 ){
	cout << "借阅的读者列表：" <<endl; 
	}
    for (int i=0; i<readerNum; i++) {
		ReaderNode* target;
		for (target=readerPool; target!=NULL; target = target->nextReader) {
			if (target->reader->getRid() == ridList[i]) {
				cout << "ID: " << target->reader->getRid() << "  ";
				cout << "姓名: " << target->reader->name << "  ";
				cout << "E-mail: " << target->reader->email << endl;
        }
    }
    }
    cout << endl;
    cout << endl;
	return true;
}

bool Library::addBooks(){
	cout << "请输入要添加的书籍的书名和ISBN（用空格分开）： "<<endl;
	cout << "[Zeyong]: ";
	string ab_name = "";
	string ab_n;
	ISBN ab_isbn;
	cin >> ab_n;
	if(!(ab_isbn<<ab_n)){
		ab_name = ab_n;
		cin >> ab_n;
	}
	cout << ab_n;
	while(!(ab_isbn<<ab_n)){
		ab_name = ab_name + ' ' + ab_n ;
		cin >> ab_n;
	}

	BookNode * bname;
    for( bname = bookPool; bname != NULL; bname = bname->nextBook ){
		if(bname->book->name.compare(ab_name) == 0){
			cout << "这本书已经在现有的书籍信息中，如果要增加库存，请使用命令AQ。" <<endl;
			return false;
			}
		}
	BookNode * bisbn;
    for( bisbn = bookPool; bisbn != NULL; bisbn = bisbn->nextBook ){
        if(bisbn->book->getISBN() == ab_isbn){
			cout << "抱歉，您输入的ISBN是《"<< bisbn->book->name <<"》的" <<endl;
			return false;
        }
    }
	BookNode * ab_temp = new BookNode(ab_isbn);
	ab_temp->book->name = ab_name;
	cout << "请输入书价： (例如：10.50，则输入1050)"<<endl;
	cout << "[Zeyong]: ";
	unsigned ab_price;
	cin >> ab_price;
	if(ab_price < 0){
		cout<<"书价输入有误！";
			return false;
	}
	ab_temp->book->price = ab_price;
	cout << "请输入库存： " <<endl;
	cout << "[Zeyong]: ";
	int ab_quality = 0;
	cin >> ab_quality;
	if(ab_quality < 0){
		cout << "库存有误，默认为0，要修改，可使用命令AQ。"<<endl;
		ab_quality = 0;
	}
	ab_temp->book->quantity = ab_quality;
	cout << "请输入作者和印刷日期: (用空格分开，例如：列夫托尔斯泰 著 2012-1-1)" <<endl;
	cout << "[Zeyong]: ";
	string ab_author = "";
	/*cin >> ab_author;
	*/
	string ab_date;
	Date abda;
	cin >> ab_date;
	if(!(abda << ab_date)){
		ab_author = ab_date;
		cin >> ab_date;
	}
	while(!(abda << ab_date)){
		/*cout << "错误的印刷日期！"<<endl;
		return false;*/
		ab_author = ab_author  + ' ' + ab_date; 
		cin >> ab_date;
	}
	ab_temp->book->date << ab_date; 
	cout << ab_temp->book->date.year <<" "<<ab_date<< endl;
	ab_temp->book->author = ab_author;
	cout<<"请确认书籍信息："<<endl;
	cout << "书名:        " << ab_temp->book->name << endl;
    cout << "ISBN:        ";
    cout << ab_temp->book->getISBN().group_a << '-';
    cout << ab_temp->book->getISBN().group_b << '-';
    cout << ab_temp->book->getISBN().group_c << '-';
    cout << ab_temp->book->getISBN().group_d << '-';
    cout << ab_temp->book->getISBN().group_e << endl;
    cout << "作者:        " << ab_temp->book->author << endl;
    cout << "印刷日期:    ";
    cout << ab_temp->book->date.year << '-';
    cout << ab_temp->book->date.month << '-';
    cout << ab_temp->book->date.day << endl;
    cout << "价格:        " << ab_temp->book->outputPrice() << endl;
    cout << "库存:        " << ab_temp->book->quantity << endl;
	cout << "y/n ?"<< endl <<endl;
	cout << "[Zeyong]: ";
	char ab_test;
	cin >> ab_test;
	while (ab_test != 'y' && ab_test != 'Y' && ab_test != 'N' && ab_test != 'n'){
		cout<<"y/n ?"<<endl;
		cout << "[Zeyong]: ";
		cin >> ab_test;
	}
	if (ab_test == 'N' || ab_test == 'n'){
		return false;
	}
	if(bookPool == NULL){
		ab_temp->nextBook = NULL;
	}
	else {
		ab_temp->nextBook = bookPool;
	}
	bookPool = ab_temp;
	bookNumber ++;
	cout <<"添加成功"<<endl;
	return true;
}

bool Library::addquality(){
	if(bookNow == NULL){
		cout << "当前没有要增加库存的书" <<endl;
		return false;
	}
	cout << "将要增加书籍《"<< bookNow->name << "》的库存吗？(y/n)" << endl;
	cout << "当前库存为： "<<bookNow->quantity <<"  已借阅： "<<bookNow->getReaderNum()<<endl;
	cout << "[Zeyong]: ";
	char aq;
	cin >> aq;
	while (aq != 'y' && aq != 'Y' && aq != 'N' && aq != 'n'){
		cout<<"y/n ?"<<endl;
		cout << "[Zeyong]: ";
		cin >> aq;
	}
	cout << "请输入要增加的数量："<<endl;
	int aq_num=0;
	cout << "[Zeyong]: ";
	cin >> aq_num;
	while (aq_num < 0){
		cout << "抱歉，增加数量不能为负数,请重新输入：" <<endl;
		cout << "[Zeyong]: ";
		cin >> aq_num;
	}
	bookNow->quantity = bookNow->quantity + aq_num;
	cout << "增加库存成功，现在该书库存为： "<<bookNow->quantity<<endl;
	return true;
}

int Library::addReaders(int num, int levelIn) {
    char code = 0;
    if (num < 1) {
        code = code+1;
    }
    if (levelIn < 1) {
        code = code+2;
    }
    if (code > 0) {
        return code;
    }
    string reading;
    for(int i=0; i<num; i++){
        ReaderNode* tmp = new ReaderNode(ridCount);
        cout << "No. " << tmp->reader->getRid() << endl;;
        cout << "姓名: ";
        cin >> reading;
        tmp->reader->name = reading;
        tmp->reader->level = levelIn;
        cout << "最大借阅量: " << tmp->reader->level << endl;
        cout << "邮箱: ";
        cin >> reading;
        tmp->reader->email = reading;
        tmp->nextReader = readerPool;
        readerPool = tmp;
        readerNumber++;
        ridCount++;
        cout << endl;
    }
    return 0;
}

bool Library::deleteBooks() {
	if(bookNow == NULL){
		cout << "当前没有要删除的书籍" << endl;
		return false;
	}
	cout << "将要删除书籍《"<< bookNow->name << "》吗？(y/n)" << endl;
	cout << "[Zeyong]: ";
	char db;
	cin >>db;
		while (db != 'y' && db != 'Y' && db != 'N' && db != 'n'){
		cout<<"y/n ?"<<endl;
		cout << "[Zeyong]: ";
		cin >> db;
	}
	int readernum = bookNow->getReaderNum();
	if(db == 'y' || db == 'Y'){
			if(bookNow->quantity <= 0 ){
				cout << "抱歉，该书已经全部借阅出去，暂时不能删除" <<endl;
				return false;
			}
			int db_num = 0;//删除数量
			cout << "请问要删除多少本？" <<endl;
			cout << "库存： "<<bookNow->quantity <<endl;
			cout << "[Zeyong]: ";
			cin >> db_num;
			while (db_num > bookNow->quantity){
				cout<<"要删除的数量大于最大删除数量，请重新输入删除量："<<endl;
				cout << "[Zeyong]: ";
				cin >> db_num;
			}
			while (db_num < 0){
				cout << "抱歉，增加数量不能为负数,请重新输入：" <<endl;
				cout << "[Zeyong]: ";
				cin >> db_num;
			}
			bookNow->quantity = bookNow->quantity - db_num;			
			cout << "删除成功。"<<endl;
			return true;
	}
	return false;
}

bool Library::deleteReaders() {
	if(readerNow == NULL){
		cout << "当前没有要删除的读者" << endl;
		return false;
	}
	cout << "将要删除读者 " << readerNow->name << " 的信息吗？(y/n)" << endl;
	cout << "( *ID: " << readerNow->getRid() << " )" <<endl;
	cout << "[Zeyong]: ";
	char dr;
	cin >> dr;
	while (dr != 'y' && dr != 'Y' && dr != 'N' && dr != 'n'){
		cout<<"y/n ?"<<endl;
		cout << "[Zeyong]: ";
		cin >> dr;
	}
	if(dr == 'y' || dr == 'Y'){
		if(readerNow->getNumber() > 0){
			cout << "抱歉，这位作者还有没有归还的书籍，因此不能删除该读者的信息。" << endl;
			return false;
		}
		//开始删除信息并处理
		unsigned dr_rid;
		dr_rid = readerNow->getRid();
		ReaderNode* target;
		for (target=readerPool; target != NULL; target = target->nextReader)
		{
			if(target->nextReader != NULL){ 
				if(target->nextReader->reader->getRid() == dr_rid){
					break;
				}
			}
		}
		if(target == NULL){target = readerPool;
			if(target->nextReader == NULL){delete target;readerPool = NULL;}
			else {ReaderNode *p = target;readerPool = readerPool->nextReader;delete p;}
		}
		else {
			ReaderNode* p;
			p = target->nextReader;//目标读者
			if(p->nextReader != NULL){
				target->nextReader = p->nextReader;
			    delete p;
		}
		else { delete p; target->nextReader = NULL;}
	}
	cout<<"删除读者信息成功"<<endl;
	readerNumber -- ;
	return true;
}
	return false;
}


int Library::printAllReaders(string filePath) {
    ofstream fout(filePath);
    if (!fout) return -1;
    ReaderNode *p = readerPool;
    int i = 1;
    while (i <= readerNumber) {
        fout << p->reader->getRid() << "  ";
        fout << p->reader->name << ' ';
        fout << p->reader->level << ' ';
        fout << p->reader->email << ' ';
        fout << p->reader->getBorrowed() << '\n';
        p = p->nextReader;
        i++;
    }
    fout.close();
    return 0;
}

bool Library::printReaderInfo() {
    if (readerNow == NULL) {
        return false;
    }
    cout << "ID:            " << readerNow->getRid() << endl;
    cout << "姓名:          " << readerNow->name << endl;
    cout << "E-mail:        " << readerNow->email << endl;
    cout << "最大借阅量:    " << readerNow->level << endl;
    cout << "借书记录: " << readerNow->getBorrowed() << endl;
    int bookNum = readerNow->getNumber();
    cout << "现在已借阅的:  " << bookNum << endl;
    ISBN* booksBorrowed = new ISBN[bookNum];
    Date* booksDate = new Date[bookNum];
    bool* booksRenew = new bool[bookNum];
    readerNow->getBook(booksBorrowed);
    readerNow->getDate(booksDate);
    readerNow->getRenew(booksRenew);
    for (int j=0; j<bookNum; j++) {
		cout << "No." << j+1 <<endl;
		cout << "ISBN:     ";
        cout << booksBorrowed[j].group_a << '-';
        cout << booksBorrowed[j].group_b << '-';
        cout << booksBorrowed[j].group_c << '-';
        cout << booksBorrowed[j].group_d << '-';
        cout << booksBorrowed[j].group_e << endl;
		//bookname
		cout << "书名:     ";
	    BookNode * test;
        for( test = bookPool; test != NULL; test = test->nextBook ){
			if(test->book->getISBN() == booksBorrowed[j]){
            break;
            }
		}
		if(test == NULL){
			cout << endl;
			cout << "借书池发生错误" <<endl;
			return false;
		}
		cout << "《" << test->book->name << "》" <<endl;
		//
		cout << "借书日期: ";
        cout << booksDate[j].year << '-';
        cout << booksDate[j].month << '-';
        cout << booksDate[j].day << endl;
		cout << "续借:     ";
        cout << booksRenew[j] << '\n' << endl;
    }
    cout << endl;
	return true;
}

int Library::exportReaderPool(string filePath) {
    ofstream fout(filePath);
    if (!fout) return 2;
    ReaderNode *p = readerPool;
    int i = 1;
    while (i <= readerNumber) {
        fout << p->reader->getRid() << ' ';
        fout << p->reader->name << ' ';
        fout << p->reader->level << ' ';
        fout << p->reader->email << ' ';
        fout << p->reader->getBorrowed() << ' ';
        int bookNum = p->reader->getNumber();
        fout << bookNum << ' ';
        ISBN* booksBorrowed = new ISBN[bookNum];
        Date* booksDate = new Date[bookNum];
        bool* booksRenew = new bool[bookNum];
        p->reader->getBook(booksBorrowed);
        p->reader->getDate(booksDate);
        p->reader->getRenew(booksRenew);
        for (int j=0; j<bookNum; j++) {
            fout << booksBorrowed[j].group_a << '-';
            fout << booksBorrowed[j].group_b << '-';
            fout << booksBorrowed[j].group_c << '-';
            fout << booksBorrowed[j].group_d << '-';
            fout << booksBorrowed[j].group_e << ' ';
            fout << booksDate[j].year << '-';
            fout << booksDate[j].month << '-';
            fout << booksDate[j].day << ' ';
            fout << booksRenew[j] << ' ';
        }
        fout << '\n';
        delete [] booksRenew;
        delete [] booksDate;
        delete [] booksBorrowed;
        p = p->nextReader;
        i++;
    }
    fout.close();
    return 0;
}

int Library::exportBookPool(string filePath) {
    ofstream fout(filePath);
    if (!fout) return 1;
    BookNode *p = bookPool;
    unsigned i = 1;
    while (i <= bookNumber) {
        fout << i << ".  ";
        fout << p->book->name << ' ';
        fout << p->book->getISBN().group_a << '-';
        fout << p->book->getISBN().group_b << '-';
        fout << p->book->getISBN().group_c << '-';
        fout << p->book->getISBN().group_d << '-';
        fout << p->book->getISBN().group_e << ' ';
        fout << p->book->author << ' ';
        fout << p->book->date.year << '-';
        fout << p->book->date.month << '-';
        fout << p->book->date.day << ' ';
        fout << p->book->outputPrice() << ' ';
        fout << p->book->quantity << ' ';
        int bookNum = p->book->getReaderNum();
        unsigned* ridList = new unsigned[bookNum];
        p->book->getReaders(ridList);
        fout << bookNum << ' ';
        for (int j=0; j<bookNum; j++) {
            fout << ridList[j] << ' ';
        }
        fout << '\n';
        p = p->nextBook;
        i++;
    }
    fout.close();
    return 0;
}

int Library::exportTheLibrary(string filePath) {
    string path = filePath;
    if (path.back() != '\\') {
        path += "\\";
    }
    string pathLWR = path;
    string pathLWB = path;
    pathLWR += "backup.lwr";
    pathLWB += "backup.lwb";
    int runResult = 0;
    runResult += exportBookPool(pathLWB);
    runResult += exportReaderPool(pathLWR);
    return runResult;
}

int Library::buildALibrary(string filePath) {
    string path = filePath;
    if (path.back() != '\\') {
        path += "\\";
    }
    string pathLWR = path;
    string pathLWB = path;
    pathLWR += "backup.lwr";
    pathLWB += "backup.lwb";
    int runResult = 0;
    runResult += buildReaderPool(pathLWR);
    runResult += buildBookPool(pathLWB);
    return runResult;
}

int Library::buildBookPool(string filePath) {
    ifstream fin(filePath);
    if (!fin) return 1;
    string reading;
    string buffer;
    string lastReading;
    double price;
    int num = 0;
    unsigned rid;
    BookNode* p;
    fin >> buffer;                          //Read the No.
    reading = buffer;
    fin >> buffer;                          //Read name into the buffer.
    while (reading != buffer) {
        ISBN tmpISBN;
        fin >> reading;                     //Read the next string.
        while (!(tmpISBN << reading)) {     //If this string is not an ISBN code.
            buffer += " ";                  //Add this string to the name.
            buffer += reading;
            fin >> reading;                 //Read another string.
        }
        p = bookPool;                       //If this string is an ISBN code.
        bookPool = new BookNode(tmpISBN);   //Create a new book node with this ISBN code.
        bookPool->nextBook = p;
        bookPool->book->name = buffer;      //Input its name.
        fin >> buffer;                      //Input the author.
        Date tmpDate;
        fin >> reading;                     //Input another string.
        while (!(tmpDate << reading)) {     //If this string is not a Date string.
            buffer += " ";                  //Add this string to the author.
            buffer += reading;
            fin >> reading;                 //Read another string.
        }
        bookPool->book->date = tmpDate;     //If this string is a Date code.
        bookPool->book->author = buffer;    //Input the author and the date to the new node.
        fin >> price;                       //Read the price.
        bookPool->book->inputPrice(price);  //Input the price to the new node.
        fin >> num;
        bookPool->book->quantity = num;     //Input the quantity of the new node.
        fin >> num;
        for (int i = 0; i<num; i++) {       //Input the readers.
            fin >> reading;
            rid = (unsigned)stoul(reading);
            bookPool->book->borrowBook(rid);
			bookPool->book->quantity++;
        }
        bookNumber++;
        tmpDate.year = 0;
        tmpISBN.group_a = "0";
        lastReading = reading;
        fin >> buffer;                       //Read the No.
        reading = buffer;
        fin >> buffer;                       //Read name into the buffer.
    }
    fin.close();
    return 0;
}

int Library::buildReaderPool(string filePath) {
    ifstream fin(filePath);
    if(!fin) return 2;
    string reading;
    string buffer;
    unsigned ridIn;
    int tmpCount;
    int borrowedIn;
    ReaderNode* p;
    fin >> buffer;
    reading = buffer;
    fin >> buffer;
    while (reading != buffer) {
        ridIn = (unsigned)stoul(reading);
        p = readerPool;
        readerPool = new ReaderNode(ridIn);
        readerNumber++;
        if(ridCount <= ridIn)
            ridCount = ridIn+1;
        readerPool->nextReader = p;
        readerPool->reader->name = buffer;
        fin >> reading;
        readerPool->reader->level = stoi(reading);
        fin >> reading;
        readerPool->reader->email = reading;
        fin >> borrowedIn;
        fin >> tmpCount;
        ISBN* booksBorrowed = new ISBN[tmpCount];
        Date* booksDate = new Date[tmpCount];
        bool* booksRenew = new bool[tmpCount];
        for (int i=0; i<tmpCount; i++) {
            fin >> reading;
            booksBorrowed[i] << reading;
            fin >> reading;
            booksDate[i] << reading;
            fin >> booksRenew[i];
        }
        readerPool->reader->importData(booksBorrowed, booksDate, booksRenew, tmpCount, borrowedIn);
        delete [] booksRenew;
        delete [] booksDate;
        delete [] booksBorrowed;
        fin >> buffer;
        reading = buffer;
        fin >> buffer;
    }
    return 0;
}

int Library::borrowBook(){
    int returnNum = 0;
    if (bookNow == NULL) {
        returnNum += 1;
    }
    if (readerNow == NULL) {
        returnNum += 2;
    }
    if (returnNum > 0) {
        return returnNum;
    }
    returnNum = bookNow->borrowBook(readerNow->getRid());
    if (returnNum > 0) {
        returnNum = returnNum*4;
        return returnNum;
    }
    returnNum = readerNow->borrowNew(bookNow->getISBN());
    if (returnNum > 0) {
        returnNum = returnNum*16;
        bookNow->returnBook(readerNow->getRid());
    }
    return returnNum;
}

int Library::returnBook(){
    int returnNum = 0;
    if (bookNow == NULL) {
        returnNum += 1;
    }
    if (readerNow == NULL) {
        returnNum += 2;
    }
    if (returnNum > 0) {
        return returnNum;
    }
    returnNum = bookNow->returnBook(readerNow->getRid());
    if (returnNum > 0) {
        returnNum = returnNum*4;
        return returnNum;
    }
    returnNum = readerNow->returnOld(bookNow->getISBN());
    returnNum = returnNum*8;
    return returnNum;
}

int Library::RenewBook(){
	return readerNow->renewBook(bookNow->getISBN());
}

void Library::AUTHOR_search() {
	string look_for;
    printf("请输入将要查找的书籍的作者: ");
	cin >> look_for;
	cout<<endl;
	BookNode * test;
	int count = 0;
    for( test = bookPool; test != NULL; test = test->nextBook ){
		if(test->book->author.find(look_for) != test->book->author.npos ){
            bookNow = test->book;
			printBookInfo();
			cout<<endl;
			count++;
        }
    }
	cout<<"总共找到 "<<count<<" 本作者姓名包含 "<<look_for<<" 字样的书籍。"<<endl<<endl;;
}


bool Library::RID_search() {
    unsigned ridS;
    cout << "请输入要查找的读者的ID： ";
    cin >> ridS;
    ReaderNode* target;
    for (target=readerPool; target!=NULL; target = target->nextReader) {
        if (target->reader->getRid() == ridS) {
            readerNow = target->reader;
            printReaderInfo();
            return 1;
        }
    }
    cout << "未找到ID为 " << ridS << " 的读者" <<endl;
    return 0;
}

bool Library::READERNAME_search(){
    string look_for;
    cout << "请输入要查找的读者姓名: ";
    cin >> look_for;
    ReaderNode* target;
    for (target=readerPool; target!=NULL; target=target->nextReader) {
        if (target->reader->name.find(look_for) != target->reader->name.npos) {
            break;
        }
    }
    if(target == NULL){
        cout<< "抱歉，未能找到姓名包含 " << look_for << " 字样的读者" <<endl;
        return 0;
    } else {
        readerNow = target->reader;
        printReaderInfo();
        return 1;
    }
}

bool Library::EMAIL_search(){
    string look_for;
    cout << "请输入要查找的读者的 E-mail: ";
    cin >> look_for;
    ReaderNode* target;
    for (target=readerPool; target!=NULL; target=target->nextReader) {
		if (target->reader->email.compare(look_for) == 0) {
            break;
        }
    }
    if(target == NULL){
        cout<< "抱歉，未能找到E-mail为 " << look_for << " 的读者" <<endl;
        return 0;
    } else {
        readerNow = target->reader;
        printReaderInfo();
        return 1;
    }
}


void Library::Time_now(){
	time_t rawtime;
    tm* now;
    time(&rawtime);
    now = localtime(&rawtime);
	cout << "现在是 " << now->tm_year + 1900 <<"年 "<< now->tm_mon + 1 <<"月 "<< now->tm_mday <<"号 ";
	cout << now->tm_hour <<"时 "<< now->tm_min <<"分 "<< now->tm_sec <<"秒 ";
	switch (now->tm_wday){
		case 0: cout << "  星期日"<<endl;break;
		case 1: cout << "  星期一"<<endl;break;
		case 2: cout << "  星期二"<<endl;break;
		case 3: cout << "  星期三"<<endl;break;
		case 4: cout << "  星期四"<<endl;break;
		case 5: cout << "  星期五"<<endl;break;
		case 6: cout << "  星期六"<<endl;break;
		default: cout << "  错误： 读取日期失败"<<endl;
	}
}
	
	