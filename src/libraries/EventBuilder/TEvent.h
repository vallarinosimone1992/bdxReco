// $Id$
//
//    File: TEvent.h
// Created: Thu Dec 22 12:32:10 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _TEvent_
#define _TEvent_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <string>

#include "TObject.h"
#include "TObjArray.h"


class TClass;
class TClonesArray;
class TEventHeader;

using namespace std;

class TEvent:public jana::JObject,public TObject{
public:
	JOBJECT_PUBLIC(TEvent);

	// Add data members here. For example:
	// int id;
	// double E;

	// This method is used primarily for pretty printing
	// the second argument to AddString is printf style format
	void toStrings(vector<pair<string,string> > &items)const{
		// AddString(items, "id", "%4d", id);
		// AddString(items, "E", "%f", E);
	}

private:

	TEventHeader	*m_eventHeader;
	vector<TClonesArray*> m_collections;
	TObjArray			   m_objects;
	TClonesArray* getCollection (int id) const;
	void deleteCollection (int id);
public:
	TEvent();
	virtual ~TEvent();
	virtual void Clear(Option_t* option="");
	void clearCollections(){m_collections.clear();}
	void clearObjects(){m_objects.Clear();}

	void printObjects() const;
	void addObject(TObject *obj);
	TObject* getObject(string name) const;
	int hasObject(string name) const;

	inline int  getNcollections() const{return m_collections.size();}
	void		printCollections() const;


	void		  addCollection(TClonesArray *coll,int checkAlreadyExists=1);
	TClonesArray* getCollection(TClass *theClass,string name) const;
	void          deleteCollection(TClass *theClass,string name);

	int  		 hasCollection(TClonesArray *coll) const;
	int 		 hasCollection(TClass *theClass,string name) const;


	TEventHeader* getEventHeader() const {
		return m_eventHeader;
	}

	void setEventHeader(TEventHeader* eventHeader) {
		m_eventHeader = eventHeader;
	}


#if (!defined(__APPLE__))
	ClassDef(TEvent,1);
#endif
};

#endif // _TEvent_

