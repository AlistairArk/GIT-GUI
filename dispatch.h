#pragma once

#include <iostream> // TODO
#include <map>
#include <QWidget>

class WidgetProtoBase{
protected:
	virtual ~WidgetProtoBase(){}
	explicit WidgetProtoBase(){};
public:
	virtual QWidget* clone_instance() const=0;
};

template<class W>
class WidgetFactory : public WidgetProtoBase{
public:
	explicit WidgetFactory(){}
	~WidgetFactory(){}
private: // override
	QWidget* clone_instance() const{ return new W; }
};

class WidgetDispatcher{
	typedef std::map<std::string, WidgetProtoBase const*> container;
public:
	typedef container::const_iterator const_iterator;
public:
	struct Installer{
		Installer(WidgetDispatcher& d, std::string const& label, WidgetProtoBase const* ptr){
			d.install(label,ptr);
		}
		~Installer(){
			// incomplete();
		}
	};
public:
	void install(std::string const& label, WidgetProtoBase const* ptr){
		if(_m[label]){
			std::cerr << "can't have two with same label " << label << "\n";
			exit(1); // actually, throw.
		}else{
			_m[label] = ptr;
		}
	}
public:
	const_iterator begin() const{
		return _m.begin();
	}
	const_iterator end() const{
		return _m.end();
	}
	
private:
	container _m;
};

// can only have one per compilation unit!
#define INSTALL_TAB(type, b)\
	WidgetFactory<type> LL; \
	WidgetDispatcher::Installer i(myTabs, b, &LL);
