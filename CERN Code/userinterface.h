//userinterface.h
//17-04-2019
//allows a non-Satchit Chatterji to simply create his/her own graphs from scratch. Usability and readability is limited.
//user just needs to #include "_" and then write draw()

void draw(){
	cout<<"\nWhat would you like to draw?"<<endl<<endl;
	cout<<"1: X & Z Acceleration Graphs for Single Run"<<endl;
	cout<<"2: Angle Of Deviation (XZ Plane) Histogram for Single Run"<<endl;
	cout<<"3: AngleOfDeviation TGraphs with errors for Multiple Runs"<<endl;
	cout<<"4: Time Of Flight graphs for Single Run"<<endl;
	cout<<"5: XY Scatter Plots of Detectors for Single Run"<<endl;
	cout<<"6: Exit menu\n";
	int a = 0;
	cin>>a;
	switch (a){
		case 1: _uiDrawAcc(); break;
		case 2: _uiDrawAOD(); break;
		case 3: _uiDrawAODTGraphs(); break;
		case 4: _uiDrawTOF(); break;
		case 5: _uitrackerScatter(); break;
		case 6: break;
		default: cout<<"No valid option selected. Please try again.\n"; break;
	}
}

void _uiDrawAcc(){
	cout<<"\nYou are now drawing the X-Z acceleration graphs for a single run.\n";
	cout<<"What momentum would you like to select? (MeV/c only)\n";
	double _uimom = 0;
	cin>>_uimom;
	cout<<"What current value would you like to select? (Amps only)\n";
	double _uiamps = 0;
	cin>>_uiamps;
	cout<<"Would you like to include the muon filter in the run? (y/n)\n";
	string _uimftemp = "";
	cin>>_uimftemp;
	bool _uimf = (_uimftemp=="y");
	cout<<"Selecting run with most particles\n";
	findAndLoadRunWithMaxEntries(_uimom,_uiamps,_uimf);
	cout<<"Now drawing Acceleration Graphs for X and Z axes.\n";
	cout<<"Now drawing...\n";
	drawAccelerations();
	drawAccelerations();
}

void _uiDrawAOD(){
	cout<<"\nYou are now drawing the XZ-plane angle of deviation graphs for a single run.\n";
	cout<<"What momentum would you like to select? (MeV/c only)\n";
	double _uimom = 0;
	cin>>_uimom;
	cout<<"What current value would you like to select? (Amps only)\n";
	double _uiamps = 0;
	cin>>_uiamps;
	cout<<"Would you like to include the muon filter in the run? (y/n)\n";
	string _uimftemp = "";
	cin>>_uimftemp;
	bool _uimf = (_uimftemp=="y");
	cout<<"Selecting run with most particles\n";
	findAndLoadRunWithMaxEntries(_uimom,_uiamps,_uimf);
	cout<<"Now drawing...\n";
	drawAngleOfDeviationX();
}

void _uiDrawAODTGraphs(){
	cout<<"\nYou are now drawing the AOD TGraphs for a set of runs.\n";
	cout<<"What graph will it be? Theta vs _______:\n";
	cout<<"1: Momentum\n";
	cout<<"2: Momentum (Absolute Values)\n";
	cout<<"3: Amperes\n";
	cout<<"4: Bending Power\n";
	cout<<"5: Charge\n";
	int gtint = 0;
	string gtype = "";
	cin>>gtint;
	double _uimom, _uiamps, _uimf;
	switch (gtint){
		case 1: gtype = "momentum";
				_uimom = ALL;
				cout<<"Now selecting for Theta vs Momentum\n";
				cout<<"What amperage would you like to select? (Amps only)\n";
				_uiamps = 0;
				cin>>_uiamps;
				cout<<"Would you like to include all runs (reply with '1'), no muon filter ('2'), or just muon filter runs ('3')?\n";
				int _uimftemp = 0;
				cin>>_uimftemp;
				_uimf = false;
				if (_uimftemp == 1){
					_uimf = ALL;
				}
				else if (_uimftemp == 2){
					_uimf == false;
				} 
				else if (_uimftemp == 3){
					_uimf == true;
				} else {
					cout<<"Not a valid option, try again";
				}
				break;
		case 2: gtype = "momentumabs";
		 		_uimom = ALL;
				cout<<"Now selecting for Theta vs Momentum (Abs)\n";
				cout<<"What amperage would you like to select? (Amps only)\n";
				_uiamps = 0;
				cin>>_uiamps;
				cout<<"Would you like to include all runs (reply with '1'), no muon filter ('2'), or just muon filter runs ('3')?\n";
				int _uimftemp = 0;
				cin>>_uimftemp;
				_uimf = false;
				if (_uimftemp == 1){
					_uimf = ALL;
				}
				else if (_uimftemp == 2){
					_uimf == false;
				} 
				else if (_uimftemp == 3){
					_uimf == true;
				} else {
					cout<<"Not a valid option, try again";
				}
				break;
		case 3: gtype = "amps";
				_uiamps = ALL;
				cout<<"Now selecting for Theta vs Amps\n";
				cout<<"What momentum would you like to select? (MeV/c only)\n";
				_uimom = 0;
				cin>>_uimom;
				cout<<"Would you like to include all runs (reply with '1'), no muon filter ('2'), or just muon filter runs ('3')?\n";
				int _uimftemp = 0;
				cin>>_uimftemp;
				_uimf = false;
				if (_uimftemp == 1){
					_uimf = ALL;
				}
				else if (_uimftemp == 2){
					_uimf == false;
				} 
				else if (_uimftemp == 3){
					_uimf == true;
				} else {
					cout<<"Not a valid option, try again";
				}
				break;
		case 4: gtype = "bp";
				_uiamps = ALL;
				cout<<"Now selecting for Theta vs Bending Power\n";
				cout<<"What momentum would you like to select? (MeV/c only)\n";
				_uimom = 0;
				cin>>_uimom;
				cout<<"Would you like to include all runs (reply with '1'), no muon filter ('2'), or just muon filter runs ('3')?\n";
				int _uimftemp = 0;
				cin>>_uimftemp;
				_uimf = false;
				if (_uimftemp == 1){
					_uimf = ALL;
				}
				else if (_uimftemp == 2){
					_uimf == false;
				} 
				else if (_uimftemp == 3){
					_uimf == true;
				} else {
					cout<<"Not a valid option, try again";
				}
				break;
		case 5: gtype = "charge";
		 		_uimom = ALL;
				cout<<"Now selecting for Theta vs Charge\n";
				cout<<"What amperage would you like to select? (Amps only)\n";
				_uiamps = 0;
				cin>>_uiamps;
				cout<<"Would you like to include all runs (reply with '1'), no muon filter ('2'), or just muon filter runs ('3')?\n";
				int _uimftemp = 0;
				cin>>_uimftemp;
				_uimf = false;
				if (_uimftemp == 1){
					_uimf = ALL;
				}
				else if (_uimftemp == 2){
					_uimf == false;
				} 
				else if (_uimftemp == 3){
					_uimf == true;
				} else {
					cout<<"Not a valid option, try again";
				}
				break;
		default: cout<<"No valid option chosen. Try again."; break;
	}
	findRuns(_uimom,_uiamps,_uimf);
	cout<<"Now drawing...\n";
	drawAODTGraphs(gtype);
}

void _uiDrawTOF(){
	cout<<"\nYou are now drawing the TOF graphs for a single run.\n";
	cout<<"Select which of the following you would like to draw: \n";
	cout<<"1: Time-Of-Flight Graph\n";
	cout<<"2: Speed of Particles Graph\n";
	int a = 0;
	cin>>a;
	if(!(a==1||a==2)){
		cout<<"Not a valid option. Try again.\n\n";
		_uiDrawTOF();
	}
	cout<<"What momentum would you like to select? (MeV/c only)\n";
	double _uimom = 0;
	cin>>_uimom;
	cout<<"Selecting run with most particles\n";
	findAndLoadRunWithMaxEntries(_uimom);
	if(a==1){
		drawTOF();
	} 
	else if(a==2) {
		drawTOFSpeed();
	}
}

void _uitrackerScatter(){
	cout<<"\nYou are now drawing the trackerScatter graphs for a single run.\n";
	cout<<"Select which of the following you would like to draw: \n";
	cout<<"1: All trackers (DWCs and MMs)\n";
	cout<<"2: DWC1\n";
	cout<<"3: DWC2\n";
	cout<<"4: DWC3\n";
	cout<<"5: MM set 1 and 4\n";
	cout<<"6: MM set 2 and 3\n";
	int opt = 0;
	cin>>(opt);

	cout<<"What momentum would you like to select? (MeV/c only)\n";
	double _uimom = 0;
	cin>>_uimom;
	cout<<"What current value would you like to select? (Amps only)\n";
	double _uiamps = 0;
	cin>>_uiamps;
	cout<<"Would you like to include the muon filter in the run? (y/n)\n";
	string _uimftemp = "";
	cin>>_uimftemp;
	bool _uimf = (_uimftemp=="y");

	cout<<"Selecting run with most particles\n";
	findAndLoadRunWithMaxEntries(_uimom,_uiamps,_uimf);
	cout<<"Now drawing...\n";
	switch (opt){
		case 1: trackerScatter(); break;
		case 2: DWCScatter("1"); break;
		case 3: DWCScatter("2"); break;
		case 4: DWCScatter("3"); break;
		case 5: MMScatter("2"); MMScatter("1"); break;
		case 6: MMScatter("2"); break;
		default : cout<<"No valid option selected. Please try again.\n"; break;
	}
}
