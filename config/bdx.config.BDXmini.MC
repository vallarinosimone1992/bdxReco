#Configuration file for BDX run.

#System parameters
MC		100
MC:RUN_NUMBER	0
JANA_CALIB_CONTEXT "variation=JLabFluxMC"
THREAD_TIMEOUT 120
THREAD_TIMEOUT_FIRST_EVENT 120


#IntVeto parameters
INTVETO:MIN_TOT  12    #min tot in ns to consider a pulse
INTVETO:THR	 0.5       #min amplitude of a pulse -in phe units
INTVETO:NPED 	 20        #num of samples in pedestal determination event by event
INTVETO:NSB	 20        #num of samples before the maximum
INTVETO:NSA	 30        #num of samples after  the maximum


INTVETO:HIT_THR_SINGLE    5   #thr in phe charge for a detector with single readout
INTVETO:HIT_THR_MULTI   -30   #thr in phe charge for a detector with multiple readout -> Irrelevant here
INTVETO:HIT_DT_MULTI    2000  #time-window coincidence for hits in a counter with multiple readout ->Irrelevant here
INTVETO:HIT_N_MULTI     2     #multiplicity for a detector with multi readout -> Irrelevant here

INTVETO:VERBOSE  3

