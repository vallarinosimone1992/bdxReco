

#ifndef _MC_Full_Det_p_
#define _MC_Full_Det_p_

#include <JANA/JEventProcessor.h>
#include "TH1D.h"
class TH1D;
class TH2D;
class TTree;
class JROOTOutput;


class MC_Full_Det_p:public jana::JEventProcessor{
	public:
		MC_Full_Det_p();
		~MC_Full_Det_p();
		const char* className(void){return "MC_Full_Det_p";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		int m_isFirstCallToBrun;

      double phe1_tot, phe2_tot, E_tot, E_tot_MC, multi_cal, multi_iv, multi_ev, multi_cal_ext_layer, multi_cal_thr;
      double multi_iv_channel0;
      double totEdep,E_single_crys;
  	  int nhit_cal,nhit_ev,nhit_iv;

      double E1[800],E2[800],E[800];
      int sector_cal[800], x_cal[800],y_cal[800];
      int sector_ev[800], channel_ev[800];
      int sector_iv[800], channel_iv[800];


	  int component,eventN, Tw, sector,x,y;

	  double multi_sect0, multi_sect1, multi_sect2, multi_sect3, multi_sect4, multi_sect5, multi_sect6, multi_sect7;
	  double multi_cl,multi_cl_near_seed, Eseed, Ecluster, Tcluster;
	  int sector_seed;
	  double x_seed, y_seed, x_crysEmax, y_crysEmax;
	  double theta;

	  int block,nblock;


	  double px_gen, py_gen, pz_gen, E_gen;
	  int  pid_gen;
      double vx_gen, vy_gen, vz_gen;

        TTree *t;
        		JROOTOutput *m_ROOTOutput;

	    //	TH1D* h_wave=new TH1D("h_wave","h_wave",100,-0.5,99.5);


};




#endif // _MC_Full_Det_p_

