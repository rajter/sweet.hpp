#include <string>
#include <sstream>
#include <fstream>
#include <boost/regex.hpp>
#include <utility>
#include <iostream>
#include <algorithm>
#include "sweetql.hpp"
#include "conv.hpp"
#include "benchmark.hpp"

class Person {
public:
	Person() {} // dummy
	~Person() { }
	Person(const std::string& f, const std::string& l, const std::string& c, 
			const std::string& a, const std::string& ci, const std::string& s,
			const std::string& z, const std::string& pw, const std::string& pp,
			const std::string& m, const std::string& w) :
		firstname(f), lastname(l), company(c), address(a), city(ci), state(s),
		phoneWork(pw), phonePrivat(pp), mail(m), www(w) {
			setZip(z);
	}

	const std::string& getFirstname() const { return firstname; }
	const std::string& getLastname() const { return lastname; }
	const std::string& getCompany() const { return company; }
	const std::string& getAddress() const { return address; }
	const std::string& getCity() const { return city; }
	const std::string& getState() const { return state; }
	const std::string& getZip() const { return zipStr; }
	const std::string& getPhoneWork() const { return phoneWork; }
	const std::string& getPhonePrivat() const { return phonePrivat; }
	const std::string& getMail() const { return mail; }
	const std::string& getWww() const { return www; }

	void setZip(const std::string& a) {
		std::stringstream ss;
		ss<<a;
		ss>>zipStr;
	}
	void setFirstname(const std::string& f) { firstname = f; }
	void setLastname(const std::string& l) { lastname = l; }
	void setCompany(const std::string& o) { company = o; }
	void setAddress(const std::string& o) { address = o; }
	void setCity(const std::string& o) { city = o; }
	void setState(const std::string& o) { state = o; }
	void setPhoneWork(const std::string& o) { phoneWork = o; }
	void setPhonePrivat(const std::string& o) { phonePrivat = o; }
	void setMail(const std::string& o) { mail = o; }
	void setWww(const std::string& o) { www = o; }

	static SqlTable& table() {
		static SqlTable tab = SqlTable::sqlTable(
		"Person",
		SqlColumn::sqlColumn<std::string>("Firstname", 	&Person::setFirstname,	&Person::getFirstname),
		SqlColumn::sqlColumn<std::string>("Lastname", 	&Person::setLastname, 	&Person::getLastname),
		SqlColumn::sqlColumn<std::string>("Company", &Person::setCompany, &Person::getCompany),
		SqlColumn::sqlColumn<std::string>("Address", &Person::setAddress, &Person::getAddress),
		SqlColumn::sqlColumn<std::string>("City", &Person::setCity, &Person::getCity),
		SqlColumn::sqlColumn<std::string>("State", &Person::setState, &Person::getState),
		SqlColumn::sqlColumn<std::string>("Zip", &Person::setZip, &Person::getZip),
		SqlColumn::sqlColumn<std::string>("PhoneWork", &Person::setPhoneWork, &Person::getPhoneWork),
		SqlColumn::sqlColumn<std::string>("PhonePrivat", &Person::setPhonePrivat, &Person::getPhonePrivat),
		SqlColumn::sqlColumn<std::string>("Mail", &Person::setMail, &Person::getMail),
		SqlColumn::sqlColumn<std::string>("Www", &Person::setWww, &Person::getWww));
		return tab;
	}

private:
	std::string firstname;
	std::string lastname;
	std::string company;
	std::string address;
	std::string city;
	std::string state;
	unsigned zip;
	std::string zipStr;
	std::string phoneWork;
	std::string phonePrivat;
	std::string mail;
	std::string www;
};

typedef std::vector<Person> PersonVec;

PersonVec parsePersonFile(const std::string& fn) {
	PersonVec ret;
	std::ifstream infile(fn);
	std::vector<std::string> v;
	std::string line;
	boost::regex re("\"[^\"]+\"");
	while(std::getline(infile, line)) {
		//std::cout<<count++<<' '<<line<<std::endl;
		auto reBe = boost::sregex_iterator(line.begin(), line.end(), re);
    	auto reEn = boost::sregex_iterator();
		std::transform(reBe, reEn, std::back_inserter(v), [&v]
			(const boost::smatch& it) {
				return it.str();
			}
		);
		ret.push_back(Person(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7],
					v[8], v[9], v[10]));
		v.clear();
	}

	return ret;
}

int main() {
	SqliteDB db("testtable.db");

	Bench in;
	std::vector<Person> per = parsePersonFile("50000.csv");
	in.stop();
	std::cout<<"Reading the file took "<<in.milli()<<" msec"<<std::endl;

	Bench insert;
	db.insert<Person>(per.begin(), per.end());
	insert.stop();
	std::cout<<"Writting the persons to the db took "<<insert.milli()<<" msec"<<std::endl;

	/*auto sel(db.select<Person>());
	std::for_each(sel.first, sel.second, [](const Person& p) {
		std::cout<<p.getFirstname()<<std::endl;
		std::cout<<p.getLastname()<<std::endl;
		std::cout<<p.getCompany()<<std::endl;
		std::cout<<p.getAddress()<<std::endl;
		std::cout<<p.getZip()<<std::endl;
		std::cout<<p.getState()<<std::endl;
		std::cout<<p.getPhoneWork()<<std::endl;
		std::cout<<p.getPhonePrivat()<<std::endl;
		std::cout<<p.getMail()<<std::endl;
		std::cout<<p.getWww()<<std::endl;
	});*/
}