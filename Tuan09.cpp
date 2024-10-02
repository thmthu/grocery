#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>
#include <ctime>
#include <chrono>
#include <iomanip>

#pragma excution_character_set("utf-8")

using namespace std;
class CItem {
private: 
		string m_Code;
		 string m_Name;
		 string m_Producer;
		 int m_Price;
protected:
		 int amount;
public:
	CItem() :m_Code(""), m_Name(""),m_Producer(""),m_Price(0) , amount(0) {};
	virtual void Input(istream& in) {
		in >> m_Code;
		in.ignore(2);
		getline(in, m_Name, ',');
		in.ignore(1);
		getline(in, m_Producer, ',');
		in.ignore(1);
		in >> m_Price;
		in.ignore(1);
	};
	void out() {
		cout << 1;
		cout << m_Code << " " << m_Producer << endl;
		return;
	}
	void outFile(ostream& out) {
		out << m_Name << ", " << m_Producer << ", " << m_Price << ", " << amount << ".\n";

	}
	void setCode(string a) {
		m_Code = a;
	}
	void outputFood(ostream& out) {
		out << m_Code << ", " << m_Name << ", " << amount << ", ";


	}
	void outQuan(ostream &os) {
		os << m_Code << ", " << m_Name << endl;
	}
	string getCode() {
		return m_Code;
	}
	int getPrice() {
		return m_Price;
	};
	void setAmount(int a) {
		amount -= a;
	}
	int getAmount() {
		return amount;
	}
};
class CElectronic : public CItem {
private:
		string m_Warranty;
		string m_ElectricPower;
public:
	CElectronic():CItem(),m_Warranty(""), m_ElectricPower("") {}
	void Input(istream &in) {

		CItem::Input(in);
		getline(in, m_Warranty, ',');
		in.ignore(1);
		getline(in, m_ElectricPower, ',');
		in >> amount;
	}
	
};
class CTerracotta :public CItem {
private:
		string m_Material;
public:
	CTerracotta(): CItem(), m_Material("") {};
	void Input(istream& in) {

		CItem::Input(in);

		getline(in, m_Material, ',');
		in.ignore(1);
		in >> amount;
	}
};
class CFood : public CItem {

private:
	string m_MfgDate;
	string m_ExpDate;
public:
	CFood() :CItem(), m_MfgDate(""), m_ExpDate("") {};
	void Input(istream& in) {
		CItem::Input(in);
		getline(in, m_MfgDate, ',');
		in.ignore(1);
		getline(in, m_ExpDate, ',');
		in >> amount;
	}
	string getDate()
	{
		return m_ExpDate;
	};
	void outputFood(ostream& out) {
		CItem::outputFood(out);
		out << m_ExpDate << ". \n";
	}
};
class store {
private: 
	vector<CItem*> list;
public:
	void Input() {
		ifstream in("INPUT.TXT");
		string temp;
		int i = 0;
		while (!in.eof()) {
			//T30 (Bình cổ Huế, Hưng Phát, 1000000, đất sét, 5)
			CItem* a=new CItem;
			switch (in.peek()) {
			case 'E':		
				a = new CElectronic;
				break;
			case 'F':
				a = new CFood;
				break;

			case 'T':
				a = new CTerracotta;
				break;

			}
			list.push_back(a);
			list[i]->Input(in);
			in.ignore(1); // Bỏ qua ký tự ')'
			in.ignore();
			i++;

		}
		in.close();
	}
	void Output() {
		ofstream os("OUTPUT.TXT");
		for (int i = 0;i < list.size();i++) {
			os << i +1<< ". ";
			list[i]->outFile(os);
		}
		os.close();

	}
	void ViewExpDate() {
		time_t now = time(0);
		tm* ltm = localtime(&now);

		// Lấy ngày hiện tại
		int currentYear = 1900 + ltm->tm_year;
		int currentMonth = 1 + ltm->tm_mon;
		int currentDay = ltm->tm_mday;
		string date = to_string(currentYear) + (currentMonth < 10 ? ('0' + to_string(currentMonth)) : to_string(currentMonth))
			+ (currentMonth < 10 ? ('0' + to_string(currentDay)) : to_string(currentDay));
		CFood* a = new CFood;
		for (CItem * i : list) {
			a = dynamic_cast<CFood*>(i);
			if (a) {
				if (a->getDate() < date)
				{
					ofstream out("EXPDATE.TXT");
					a->outputFood(out);
					out.close();
				}
			}
		}
	}
	void Delete() {
		cout << "Nhap ma can xoa: ";
		string a;
		cin >> a;
		int k = 0;
		for (CItem* i : list) {
			if (i->getCode() == a) {
				cout << i->getCode();
				delete list[k];
				list.erase(list.begin() + k);
				return;
			}
			k++;

		}
	}
	std::string getIntegerPart(const std::string& str) {
		size_t dotPos = str.find('.');
		if (dotPos != std::string::npos) {
			return str.substr(0, dotPos); // Lấy phần chuỗi trước dấu chấm
		}
		return str; // Trả về chuỗi gốc nếu không có dấu chấm
	}
	void Buy() {
		ifstream in("BUY.TXT");
		ofstream out("INVOICE.TXT");
		string id;
		in >> id;
		out << id << endl;
		in.ignore();
		int i = 0;
			auto now = std::chrono::system_clock::now();
			std::time_t now_c = std::chrono::system_clock::to_time_t(now);
			std::tm* parts = std::localtime(&now_c);
			const char* days_of_week[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
			std::stringstream ss;
			ss << std::put_time(parts, "%H:%M:%S ") << days_of_week[parts->tm_wday]
				<< std::put_time(parts, " %d/%m/%Y");
			string day = days_of_week[parts->tm_wday];
			out << ss.str() << endl;
			//E102, Tay nghe Sony Xperia Z1, 2
			int count = 0;
			int stt = 1;
			float discount = 0;
			float total = 0;
			while (!in.eof()) {
				int amount;
				string title;
				string code;
				getline(in, code, ',');
				in.ignore(1);
				getline(in, title, ',');
				in.ignore(1);
				in >> amount;
				in.ignore();
				for (int i = 0;i <= list.size()-1;i++) {
					
					if (list[i]->getCode() == code) {
						list[i]->setAmount(amount);
						float dis = 0;
						float sumSingle = static_cast<float>(amount)* static_cast<float>(list[i]->getPrice());
						out << stt << ". " << code << ", " << title << ": " << amount << " x " << list[i]->getPrice() << " = " << getIntegerPart(to_string(sumSingle)) << u8" VNĐ" << endl;
						switch (code[0]) {
						case 'E':
						{
							if (day == "Tuesday" || day == "Wednesday") {
								dis = 0.15;
							}
						}
						case 'F':
						{
							if (day == "Friday") {
								dis = 0.2;
							}
						}
						case 'T':
						{
							if (day == "Saturday" || day == "Sunday") {
								dis = 0.3;
							}
						}
						}
						discount += dis*sumSingle;
						total += sumSingle;
						stt++;
						
					}
				}

			}
			out << u8"Giảm giá: " << getIntegerPart(to_string( discount)) << u8" VNĐ \n";
			out << u8"Số tiền thanh toán: " << getIntegerPart(to_string(total - discount ))<< u8" VNĐ \n";

		out.close();
		in.close();
	}
	void ViewQuantity() {
		int count = 0;
		vector<int> flag;
		for (int i = 0;i <= list.size() - 1;i++) {
			if (list[i]->getAmount() <= 0) {
				count++;
				flag.push_back(i);
		}
		}
		ofstream out("QUANLITY.TXT");
		out << count << endl;
		int cnt = 1;
		for (int i : flag) {
			out << cnt << ". "; list[i]->outQuan(out);
			cnt++;
		}	
		out.close();

	}
};
int main() {
	SetConsoleOutputCP(65001);
  
	store a;

	a.Input();
	a.Output();
	a.ViewExpDate();
	a.Buy();
	a.ViewQuantity();
	a.Delete();

    return 0;

}



