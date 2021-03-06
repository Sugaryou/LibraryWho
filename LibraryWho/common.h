

#ifndef Library_common_h
#define Library_common_h

#include <assert.h>
#include <time.h>
#include <iostream>
#include <string>
using namespace std;

#define DEFAULT_PATH "C:\\Users\\ASUS\\Desktop\\Library\\"

struct Date {
    short year;
    short month;
    short day;
    bool operator ==(const Date& x) {
        bool equal = 1;
        equal = (this->year == x.year)&equal;
        equal = (this->month == x.month)&equal;
        equal = (this->day == x.day)&equal;
        return equal;
    }
    bool operator !=(const Date& x) {
        bool equal = 0;
        equal = (this->year != x.year)|equal;
        equal = (this->month != x.month)|equal;
        equal = (this->day != x.day)|equal;
        return equal;
    }
    bool operator <<(const string dataIn) {
    sscanf(dataIn.c_str(), "%hd-%hd-%hd", &year, &month, &day);
	time_t rawtime;
    tm* now;
    time(&rawtime);
    now = localtime(&rawtime);
	if (year < 1000 || year > (now->tm_year + 1900)){//出版日期不早于1000年，不晚于明年
		return 0;
	}
	if (month>12 || month<1){
		return 0;
	}
	if (day<1){
			return 0;
		}
    bool leap_year = 0;
    if (year%4 == 0) {
        if (year%100 == 0) {
            if (year%400 == 0) {
                leap_year = 1;
            } else {
                leap_year = 0;
            }
        } else {
            leap_year = 1;
        }
    } else {
        leap_year = 0;
    }//判断是否为闰年

    switch(month) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if (day > 31) {
                return 0;
            } else {
                return 1;
            }
            break;
        case 2:
            if (leap_year) {
                if (day > 29) {
                    return 0;
                } else {
                    return 1;
                }
            } else {
                if (day > 28) {
					return 0;
                } else {
                    return 1;
                }
            }
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            if (day > 30) {
				return 0;
            } else {
                return 1;
            }
            break;
            
        default:
			return 0;
    }
	}
};

struct ISBN {
	string group_a;
	string group_b;
	string group_c;
	string group_d;
	string group_e;
    bool operator ==(const ISBN& x) {
        bool equal = 1;
        equal = (this->group_a == x.group_a)&equal;
        equal = (this->group_b == x.group_b)&equal;
        equal = (this->group_c == x.group_c)&equal;
        equal = (this->group_d == x.group_d)&equal;
        equal = (this->group_e == x.group_e)&equal;
        return equal;
    }
    bool operator !=(const ISBN& x) {
        bool equal = 0;
        equal = (this->group_a != x.group_a)|equal;
        equal = (this->group_b != x.group_b)|equal;
        equal = (this->group_c != x.group_c)|equal;
        equal = (this->group_d != x.group_d)|equal;
        equal = (this->group_e != x.group_e)|equal;
        return equal;
    }
    bool operator <<(const string isbnIn) {
    size_t pos;
    size_t pos2;
    pos = isbnIn.find("-");
    group_a = isbnIn.substr(0,pos);
    pos2 = isbnIn.find("-", pos+1);
    group_b = isbnIn.substr(pos+1, pos2-pos-1);
    pos = pos2;
    pos2 = isbnIn.find("-", pos+1);
    group_c = isbnIn.substr(pos+1, pos2-pos-1);
    pos = pos2;
    pos2 = isbnIn.find("-", pos+1);
    group_d = isbnIn.substr(pos+1, pos2-pos-1);
    pos = pos2;
    group_e = isbnIn.substr(pos+1);
    if(group_a != "978" && group_a != "979") {
        return 0;
    }
    else {
        unsigned long count = 0;
        count = group_a.length()+group_b.length()+group_c.length()+group_d.length()+group_e.length();
        if (count != 13)
            return 0;
        int tmp = 0;
        count = 1;
        for (int i=0; i< group_a.length(); i++) {
            if (count%2) {
                tmp += group_a[i]-'0';
            } else {
                tmp += (group_a[i]-'0')*3;
            }
            count++;
        }
        for (int i=0; i<group_b.length(); i++) {
            if (count%2) {
                tmp += group_b[i]-'0';
            } else {
                tmp += (group_b[i]-'0')*3;
            }
            count++;
        }
        for (int i=0; i<group_c.length(); i++) {
            if (count%2) {
                tmp += group_c[i]-'0';
            } else {
                tmp += (group_c[i]-'0')*3;
            }
            count++;
        }
        for (int i=0; i<group_d.length(); i++) {
            if (count%2) {
                tmp += group_d[i]-'0';
            } else {
                tmp += (group_d[i]-'0')*3;
            }
            count++;
        }
        tmp = (10-tmp%10)%10;
        if (tmp != stoull(group_e))
            return 0;
        else
            return 1;
    }
}
};

#endif
