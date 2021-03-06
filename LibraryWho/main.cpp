#include <fstream>
#include <iostream>
#include "common.h"
#include "library.h"
#include <conio.h>
#define PASSWORD "9390sesw"

Library library;
bool revise = false;//检验是否修改过

void printHelp(){
    cout << " 功能表 " << endl;
    cout << " ?(H)  查看功能表" << endl;
    cout << " Q     退出" << endl;
    cout << " A     从一个文件中读入图书馆数据" << endl;
    cout << " F     将图书馆的数据导出" <<  endl;
    cout << " IS    通过ISBN查找书籍" << endl;
    cout << " B     通过书籍的书名查找" << endl;
	cout << " AR    通过作者查找书籍" << endl;
    cout << " P     显示当前书籍的数据" << endl;
    cout << " TM    显示现在的时间" << endl;
    cout << " R     添加读者信息" << endl;
	cout << " RN    查看当前读者" << endl;
    cout << " O     将图书馆数据导出到指定文件" << endl;
    cout << " I     从指定文件夹中导入图书馆数据" << endl;
    cout << " BO    借书" << endl;
    cout << " RB    还书" << endl;
	cout << " D     通过ID查找读者" << endl;
	cout << " M     通过E-mail查找读者" << endl;
	cout << " N     通过姓名查找读者"<<endl;
	cout << " DR    删除读者信息" <<endl;
	cout << " DB    减少书籍库存" <<endl;
	cout << " AQ    增加书籍库存" << endl;
	cout << " AB    添加书籍" <<endl;
	cout << " RE    续借" <<endl;
}

void cmd_A() {
    int num = 1;
    cout << "请初始化所有书籍的库存: ";
    cin >> num;
    if (num < 0) {
        cout << "无效的库存初始化，预设为0。" << endl;
        num = 0;
    }
    cout << "请输入文件路径: ";
    string path;
    cin >> path;
    int result;
    result = library.importBooks(path, num);
    switch (result) {
        case 0:
            cout << "成功。" << endl;
			revise = true;
            break;
        case -1:
            cout << "错误的路径。" << endl;
            break;
            
        default:
            cout << "未知错误。" << endl;
            break;
    }
}

void cmd_F() {
    cout << "请输入指定文件的路径: " << endl;
    string path;
    cin >> path;
    int result;
    result = library.printAllBooks(path);
    switch (result) {
        case 0:
            cout << "导出成功。" << endl;
            break;
        case -1:
            cout << "错误的路径。" << endl;
            break;
            
        default:
            cout << "未知错误。" << endl;
            break;
    }
}

void cmd_TM(){
	library.Time_now();
}
/*
void cmd_T() {
    cout << "Please input the path if the file to be test: " << endl;
    string path;
    cin >> path;
    ifstream fin(path);
    string a;
    for (int i = 0; i<20; i++) {
        fin >> a;
        cout << a << endl;
    }
    fin.close();
}
*/
void cmd_IS() {
    library.ISBN_search();
    library.printBookInfo();
}

void cmd_B() {
    library.BOOKNAME_search();
}

void cmd_P() {
    bool recent;
	recent = library.printBookInfo();
	if(recent == false){
		cout << "当前并未找到任何书籍" << endl;
	}
}

void cmd_RN(){
	bool recent;
	recent = library.printReaderInfo();
	if(recent == false){
		cout << "错误：当前并未有读者" << endl;
	}
}

void cmd_R() {
    int num = 0;
    int levelIn = 0;
    cout << "将要添加的读者个数： ";
    cin >> num;
    cout << "这些读者的最大借阅量: ";
    cin >> levelIn;
    switch (library.addReaders(num, levelIn)) {
        case 0:
            cout << "成功添加上述读者信息。" << endl;
			revise = true;
            break;
        case 1:
            cout << "错误：无效的读者数量。" << endl;
            break;
        case 2:
            cout << "错误：无效的等级。 " << endl;
            break;
        case 3:
            cout << "错误：无效的读者数量和等级。" << endl;
            break;
            
        default:
            cout << "未知错误。" << endl;
            break;
    }
}

void cmd_O() {
    cout << "是否使用默认路径(y/n) ?" << endl;
    char defPath;
    string path;
    int runResult = -1;
    cout << "[Zeyong]: ";
    cin >> defPath;
	while (defPath != 'y' && defPath != 'Y' && defPath != 'n' && defPath != 'N'){
		cout<<"y/n?"<<endl;
		cout << "[Zeyong]: ";
		cin >> defPath;
	}
    if (defPath == 'y' || defPath == 'Y'){
        path = DEFAULT_PATH;
    } else {
        cout << "请输入目标文件夹的路径: " << endl;
        cout << "[Zeyong]: ";
        cin >> path;
    }
    runResult = library.exportTheLibrary(path);
    switch (runResult) {
        case 0:
            cout << "导出成功。" << endl;
            break;
        case 1:
            cout << "导出到 *.lwb 时失败。" << endl;
            break;
        case 3:
            cout << "导出到 *.lwb 时失败。" << endl;
        case 2:
            cout << "导出到 *.lwr 时失败。" << endl;
            break;
            
        default:
            cout << "数据导出时出现未知错误。" << endl;
            break;
    }
}

bool cmd_I() {
    cout << "是否从指定文件夹读入已有的数据？" << endl;
    cout << "0. 不用；   1. 从默认路径读入；   2. 从其他路径读入. " << endl;
    cout << "[Zeyong]: ";
    int option = 0;
    int runResult = 0;
    string path;
    cin >> option;
    switch (option) {
        case 0:
            cout << "取消读入。" << endl;
            return 0;
            break;
        case 1:
            cout << "从默认路径读入。" << endl;
            path = DEFAULT_PATH;
            break;
        case 2:
            cout << "请输入要读取数据的路径： ";
            cin >> path;
            break;
            
        default:
            cout << "无效的命令，如果你想继续读入数据，请输入命令'I'。" << endl;
            return 0;
            break;
    }
    runResult = library.buildALibrary(path);
    switch (runResult) {
        case 0:
            cout << "导入成功。" << endl;
            return 1;
            break;
        case 1:
            cout << "导入 *.lwb 失败。" << endl;
            break;
        case 3:
            cout << "导入 *.lwb 失败。" << endl;
        case 2:
            cout << "导入 *.lwr 失败。" << endl;
            break;
            
        default:
            cout << "未知的读入错误。" << endl;
            break;
    }
    return 0;
}

void cmd_BO() {
	if(library.readerNow == NULL && library.bookNow == NULL){
		cout<<"错误：当前没有要借书的读者和书籍。"<<endl;return;
	}
	else if(library.readerNow == NULL){
		cout<<"错误：当前没有要借书的读者。"<<endl;return;
	}
	else if(library.bookNow == NULL){
		cout<<"错误：当前没有要借的书籍。"<<endl;return;
	}//是否没有当前的书籍或读者
    cout << library.readerNow->name << " 要借阅 《" << library.bookNow->name <<"》 (y/n)?" << endl;
    cout << "[Zeyong]: ";
    char judge;
    cin >> judge;
	while (judge != 'y' && judge != 'Y' && judge != 'n' && judge != 'N'){
		cout<<"y/n?"<<endl;
		cout << "[Zeyong]: ";
		cin >> judge;
	}
    if (judge == 'y' || judge == 'Y') {
        int returnNum = library.borrowBook();
        if (returnNum == 0) {
            cout << "借书成功 " << endl;
			revise = true;//借书成功，修改
            return;
        }
        returnNum = returnNum >> 1;
        returnNum = returnNum >> 1;
        if (returnNum&1)
            cout << "抱歉，这本书已被借完" << endl;
        returnNum = returnNum >> 1;
        if (returnNum&1)
            cout << "该读者已经借过这本书了" << endl;
        returnNum = returnNum >> 1;
        if (returnNum&1)
            cout << "该读者已经达到了最大的借阅量" << endl;
        returnNum = returnNum >> 1;
        if (returnNum&1)
            cout << "该读者已经借过这本书了" << endl;
    } 
}

void cmd_RB() {
	if(library.readerNow == NULL && library.bookNow == NULL){
		cout<<"错误：当前没有要还书的读者和书籍。"<<endl;return;
	}
	else if(library.readerNow == NULL){
		cout<<"错误：当前没有要还书的读者。"<<endl;return;
	}
	else if(library.bookNow == NULL){
		cout<<"错误：当前没有要还的书籍。"<<endl;return;
	}//是否没有当前的书籍或读者
    cout << library.readerNow->name << " 要归还 《" << library.bookNow->name <<"》 (y/n)?" << endl;
    cout << "[Zeyong]: ";
    char judge;
    cin >> judge;
	while (judge != 'y' && judge != 'Y' && judge != 'n' && judge != 'N'){
		cout<<"y/n?"<<endl;
		cout << "[Zeyong]: ";
		cin >> judge;
	}
    if (judge == 'y' || judge == 'Y') {
        int returnNum = library.returnBook();
        if (returnNum == 0) {
            cout << "成功归还 " << endl;
			revise = true;//成功归还，修改
            return;
        }
        /*if(returnNum&1)
            cout << "并未选择要归还的书" << endl;*/
        returnNum = returnNum >> 1;
        /*if (returnNum&1)
            cout << "没有要还书的读者" << endl;*/
        returnNum = returnNum >> 1;
        if (returnNum&1)
            cout << "该读者未借过这本书" << endl;
        returnNum = returnNum >> 1;
        if (returnNum&1)
            cout << "该读者未借过这本书" << endl;
        returnNum = returnNum >> 1;
        if (returnNum&1){
            cout << "该读者已经逾期" << endl;
		    revise = true;//还书逾期，修改
		}
    } 
}

void cmd_AR(){
	library.AUTHOR_search();
}

void cmd_D() {
    library.RID_search();
}

void cmd_N() {
    library.READERNAME_search();
}

void cmd_M() {
    library.EMAIL_search();
}

void cmd_DR() {
	bool dr_temp;
	dr_temp = library.deleteReaders();
	if(dr_temp){revise = true;}
	cout << endl;
}

void cmd_DB() {
	bool db_temp;
	db_temp = library.deleteBooks();
	if(db_temp){revise = true;}
	cout << endl;
}

void cmd_AQ() {
	bool aq_temp;
	aq_temp = library.addquality();
	if(aq_temp){revise = true;}
	cout << endl;
}

void cmd_AB() {
	bool ab_temp;
	ab_temp = library.addBooks();
	if(ab_temp){revise = true;}
	cout << endl;
}

void cmd_RE() {
	if(library.readerNow == NULL && library.bookNow == NULL){
		cout<<"错误：当前没有要续借的读者和书籍。"<<endl;return;
	}
	else if(library.readerNow == NULL){
		cout<<"错误：当前没有要续借的读者。"<<endl;return;
	}
	else if(library.bookNow == NULL){
		cout<<"错误：当前没有要续借的书籍。"<<endl;return;
	}//是否没有当前的书籍或读者
    cout << library.readerNow->name << " 将要续借 《" << library.bookNow->name <<"》 (y/n)?" << endl;
    cout << "[Zeyong]: ";
    char judge;
    cin >> judge;
	while(judge != 'y' && judge != 'Y' && judge != 'n' && judge != 'N' ){
		cout<<"y/n ?"<<endl;
		cout<< "[Zeyong]: ";
		cin >> judge;
	}
    if (judge == 'y' || judge == 'Y') {
		int returnNum = library.RenewBook();
        switch (returnNum) {
            case 0:
                cout << "续借成功" << endl;
				revise = true;
                break;
            case 1:
                cout << "该读者没有借过这本书" << endl;
                break;
            case 2:
                cout << "该读者已经续借过这本书" << endl;
                break;
            case 4:
                cout << "该读者已逾期，不能续借" << endl;
                break;
                
            default:
                cout << "未知错误" << endl;
                break;
        }
    }
}

int main(int argc, const char * argv[]) {
    cout << "============ Library =============" << endl;
    string option;
	cout << "::∴★∵**☆．∴★∵**☆．   "<<endl;
	cout << "  █████．☆．∵★∵∴☆． "<<endl;
	cout << "  █田█田█．*☆．∴★∵． "<<endl;
	cout << "  █田█田█．★∵∴☆．★∵∴． "<<endl;
	cout << "  █田█田█．★∵∴☆．★..**． "<<endl;
	cout << "  █田█田█．★★∵∴☆．★*☆． "<<endl;
	cout << "  █████． "<<endl;
	cout << "◢██□██◣．~~~~~  NanJing University Library "<<endl; 
	cout << "                       Version 1.0"<<endl;
	cout << "                       版权所有 ◎ 2014 Zeyong保留所有权利"<<endl;
	cout << "                       ";
	cmd_TM();
	cout <<endl;
	cout <<"欢迎回来，请输入登录密码(8位）："<<endl;
	cout << "[Zeyong]: ";
	char key[8];
	strncpy(key, PASSWORD, 8);
	bool sign_in = true;
	int signcount = 3;
	char password;
	while(signcount!=0){
	for(int keylen = 8; keylen>0; keylen--){
		password = getch();
		cout<<"*";
		if(password != key[8-keylen]){
			sign_in = false;
		}
	}
	if(!sign_in){
		cout<<endl;
		cout<<"密码错误！"<<endl;
		signcount --;
		sign_in = true;
		if(signcount!=0){
		cout<<"还有"<<signcount<<"次机会。"<<endl;
		cout<<endl;
		cout<<"请重新输入密码"<<endl;
		cout << "[Zeyong]: ";
	}
	}
	else {
		cout<<endl;
		cout<<"成功登入！"<<endl;
		cout<<endl;
		break;}
	}
	if(signcount == 0){
		cout<<"退出ing"<<endl;
		return -1;
	}
    bool readBackup = 0;
    readBackup = cmd_I();
    while (1) {
        cout << endl;
        cout << "请输入命令 (键入 ? 或 H/h 查看功能表): " << endl;
        cout << "[Zeyong]: ";
        cin >> option;
		if(option.compare("Q") == 0 || option.compare("q") == 0) {
				if(revise == true){
					cout<<"你已修改了一些数据，但尚未保存，请问是否保存再退出？y/n"<<endl;
					char revise_save;
					cin>>revise_save;
					while (revise_save != 'y' && revise_save != 'Y' && revise_save != 'n' && revise_save != 'N'){
						cout<<"y/n?"<<endl;
						cout << "[Zeyong]: ";
						cin>>revise_save;
					}
					if(revise_save == 'y' || revise_save == 'Y'){
						cmd_O();
					}
				}
				cout<<"再见！"<<endl<<endl;
                return 0;
		}
		else if(option.compare("?") == 0 || option.compare("H") == 0 || option.compare("h") == 0) {
                printHelp();
                continue;
		}
		else if(option.compare("A") == 0 || option.compare("a") == 0) {
                cmd_A();
                continue;
		}
        else if(option.compare("F") == 0 || option.compare("f") == 0) {    
                cmd_F();
                continue;
		}
        else if(option.compare("TM") == 0 || option.compare("tm") == 0) {    
                cmd_TM();
                continue;
		}
        else if(option.compare("IS") == 0 || option.compare("is") == 0) {    
                cmd_IS();
				cout<<"您想继续查找吗？ y/n"<<endl;
				char tempIS;
				cout << "[Zeyong]: ";
				cin >> tempIS;
				while (tempIS != 'n' && tempIS != 'N'){
					if(tempIS == 'y' || tempIS == 'Y'){
						cmd_IS();
					}
					else {cout << "请输入 y/n"<<endl;}
					cout<<"您想继续查找吗？ y/n"<<endl;
					cout << "[Zeyong]: ";
					cin >> tempIS;
				}
                continue;
		}
         else if(option.compare("P") == 0 || option.compare("p") == 0) {   
                cmd_P();
                continue;
		 }
		 else if(option.compare("B") == 0 || option.compare("b") == 0) {
                cmd_B();
				cout<<"您想继续查找吗？ y/n"<<endl;
				char tempB;
				cout << "[Zeyong]: ";
				cin >> tempB;
				while (tempB != 'n' && tempB != 'N'){
					if(tempB == 'y' || tempB == 'Y'){
						cmd_B();
					}
					else {cout << "请输入 y/n"<<endl;}
					cout<<"您想继续查找吗？ y/n"<<endl;
					cout << "[Zeyong]: ";
					cin >> tempB;
				}
                continue;
		 }
		 else if(option.compare("R") == 0 || option.compare("r") == 0) {
                cmd_R();
                continue;
		 }
		 else if(option.compare("O") == 0 || option.compare("o") == 0) {
                cmd_O();
				revise = false;
                continue;
		 }
		 else if(option.compare("I") == 0 || option.compare("i") == 0) {
                if (readBackup) {
                    cout << "警告:你已经导入过外部数据了。" << endl;
                } else {
                    readBackup = cmd_I();
                }
                continue;
		 }
		 else if(option.compare("RB") == 0 || option.compare("rb") == 0) {
                cmd_RB();
                continue;
		 }
		 else if(option.compare("BO") == 0 || option.compare("bo") == 0) {
                cmd_BO();
                continue;
		 }
		 else if(option.compare("AR") == 0 || option.compare("ar") == 0) {
				cmd_AR();
				cout<<"您想继续查找吗？ y/n"<<endl;
				char tempAR;
				cout << "[Zeyong]: ";
				cin >> tempAR;
				while (tempAR != 'n' && tempAR != 'N'){
					if(tempAR == 'y' || tempAR == 'Y'){
						cmd_AR();
					}
					else {cout << "请输入 y/n"<<endl;}
					cout<<"您想继续查找吗？ y/n"<<endl;
					cout << "[Zeyong]: ";
					cin >> tempAR;
				}
				continue;  
		 }
		 else if(option.compare("D") == 0 || option.compare("d") == 0) {
                cmd_D();
				continue;
		 }
		 else if(option.compare("M") == 0 || option.compare("m") == 0) {
                cmd_M();
				continue;
		 }
		 else if(option.compare("N") == 0 || option.compare("n") == 0) {
                cmd_N();
                continue;
		 }
		 else if(option.compare("RN") == 0 || option.compare("rn") == 0){
			    cmd_RN();
				continue;
		 }
		 else if(option.compare("DR") == 0 || option.compare("dr") == 0){
			    cmd_DR();
				continue;
		 }
		 else if(option.compare("DB") == 0 || option.compare("db") == 0){
			    cmd_DB();
				continue;
		 }
		 else if(option.compare("AQ") == 0 || option.compare("aq") == 0){
			    cmd_AQ();
				continue;
		 }
		 else if(option.compare("AB") == 0 || option.compare("ab") == 0){
			    cmd_AB();
				continue;
		 }
		 else if(option.compare("RE") == 0 || option.compare("re") == 0){
			    cmd_RE();
				continue;
		 }
		 else{
                cout << "未知命令：" << option << endl;
                continue;
		 }
        }
	return 0;    
}
