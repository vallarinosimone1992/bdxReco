#include "JROOTOutput.h"

JROOTOutput::JROOTOutput() :
		m_file(0), m_objects(0), m_cur_obj(0) {
	jout << "JRootOutput creator called" << endl;

}

JROOTOutput::~JROOTOutput() {
	if (m_file) {
		delete m_file;
	}
	if (m_objects) {
		delete m_objects;
	}
}

int JROOTOutput::OpenOutput(string fname) {
	jout << "JROOTOutput::OpenOutput is called with filename: " << fname << endl;
	if (m_isOpen == 0) {
		m_file = new TFile(fname.c_str(), "RECREATE");
		m_objects = new TList();
		m_isOpen = 1;
	} else {
		jout << "Already opened, nothing to do" << endl;
	}
}

int JROOTOutput::CloseOutput() {
	jout << "JROOTOutput::CloseOutput is called" << endl;
	if (m_isOpen) {
		this->SaveAll();
		m_file->Write();
		m_file->Close();
		m_isOpen = 0;
	} else {
		jout << "Already closed, nothing to do" << endl;
	}
	return NOERROR;
}

int JROOTOutput::SaveAll() {
	TIter *m_iter;
	if (!m_objects) {
		jerr << "no m_objects, doing nothing" << endl;
		fflush(stdout);
		return -1;
	}
	if (!m_file) {
		jerr << "no m_file, doing nothing" << endl;
		fflush(stdout);
		return -2;
	}

	m_iter = new TIter(m_objects);
	if (!m_iter) {
		jerr << "no m_iter, doing nothing" << endl;
		fflush(stdout);
		return -1;
	}
	m_file->cd();
	//Loop over the TList via a TIter
	while ((m_cur_obj = m_iter->Next())) {
		if (m_cur_obj != 0) {
			jout << "JROOTOutput::saving object " << m_cur_obj->GetName() << endl;
			fflush(stdout);
			m_cur_obj->Write();
		}
	}
	delete m_iter;
	return 0; // prevent compiler warnings
}

int JROOTOutput::AddObject(TObject *obj) {
	if (!m_objects) {
		jerr << "no m_objects, doing nothing" << endl;
		return -1;
	}
	m_objects->Add(obj);
	return 0; // prevent compiler warnings
}
