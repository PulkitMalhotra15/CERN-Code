//runDatabase.h
//16-04-2019

const int maxRuns = 150; //perhaps not needed if runSelection becomes a vector and other functions are likewise changed
//total 150 runs, 9 properties to check
double runList[150][7];
string runPOIType[150][2];

int selectedRuns = 0;

bool initiated = false;

#define ALL -1

//properties defined below
#define RUNNUMBER 0
#define AMPERES 1
#define MOMENTUM 2
#define RECORDED 3
#define GOOD 4
#define MUONFILTER 5
#define ISOK 6
#define TYPE 0
#define POI 1

int runSelection[maxRuns]; //would be much better if it is a vector and not an array

void findRuns(double momentum = ALL, double amps = ALL, int mf = ALL, string type=""){
	//writes runs to runSummary (after zeroing all indices) based on user specified run properties.
	//properties are optional, e.g., to find all 120A runs, use findRuns(ALL, 120);
	//User can search for only positive results by writing e.g. findRuns(5000, '+');
	if(!initiated){
		initiateDatabase();
	}
	clearSelection();
	int count=0;
	for(int i=0;i<maxRuns;i++){
		if(checkMomentum(i, momentum, (momentum==ALL))){
			if(checkAmps(i, amps, (amps==ALL))){
				if(checkMuonFilter(i, mf, (mf==ALL))){
					if(checkType(i,type, (type==""))){
						runSelection[count] = runList[i][RUNNUMBER];
						//cout<<int(runList[i][RUNNUMBER])<<endl;
						count++;
					}
				}
			}
		}
	}
	cout<<"Number of Runs: "<<count<<endl;
	if(count==0){
		cout<<"No runs found with these conditions in database\n";
	}
	selectedRuns = count;
	//cout<<"Call run numbers from runSelection[]"<<endl;
}
int appendRunSelection(double momentum = ALL, double amps = ALL, int mf = ALL, string type=""){
	//appends run(s) to the end of runSelection by searching based on run properties
	//quite messy and inefficient
	int originalSelectedRuns = selectedRuns;
	if(originalSelectedRuns!=0){	
		int originalRunSelection[maxRuns];
		int newRunSelection[maxRuns];
		int newSelectedRuns=0;
		copyArray(runSelection,originalRunSelection,selectedRuns);
		findRuns(momentum,amps,mf,type);
		newSelectedRuns=selectedRuns;
		copyArray(runSelection, newRunSelection, selectedRuns);
		clearSelection();
		selectedRuns=originalSelectedRuns;
		copyArray(originalRunSelection, runSelection, originalSelectedRuns);
		for(int i=0;i<newSelectedRuns;i++){
			appendRunSelection(newRunSelection[i],false);
		}
		return 0;
	} else {
		findRuns(momentum, amps, mf, type);
		return 0;
	}
}
void removeRun(int rn){
	if(selectedRuns!=0){
		int removeIndex = selectedRuns+1;
		bool removed = false;
		for(int i=0;i<selectedRuns;i++){
			if(rn==runSelection[i]){
				removeIndex=i;
				removed=true;
			}
		}
		if(removed){	
			selectedRuns--;
			for(int i=removeIndex;i<selectedRuns;i++){
				runSelection[i]=runSelection[i+1];
			}
			cout<<"Run "<<rn<<" removed from runSelection.";
		}
		if (!removed){
			cout<<"Run "<<rn<<" not found in runSelection.";
		}
	} else {
		cout<<"No runs selected.";
	}
}
int appendRunSelection(int rn, bool verbose = true){
	//appends single run to the end of runSelection based on run number
	if(runExists(rn)){
		for(int i=0;i<selectedRuns;i++){
			if(runSelection[i]==rn){
				if(verbose){
					cout<<"Run "<<rn<<" already in runSelection\n";
					cout<<"Index at: "<<i<<endl;
				}
				return 0;
			}
		}
		if(selectedRuns<maxRuns){
			runSelection[selectedRuns]=rn;
			selectedRuns++;
			if(verbose){
				cout<<"Run "<<rn<<" added to runSelection\n";
				cout<<"Number of runs (selectedRuns) = "<<selectedRuns<<endl;
			}
			return 0;
		} else {
			cout<<"Max number of selectable runs exceeded\n";
			return -1;
		}
	} else {
		if(verbose){
			cout<<"Run "<<rn<<" does not exist\n";
		}
		return -1;
	}
}
void findAndLoadRunWithMaxEntries(double momentum = ALL, double amps = ALL, int mf = ALL, bool micromega = true, string type="", double fast=true){
	findRuns(momentum, amps, mf, type);
	if(selectedRuns!=0){
		int toLoad=getMaxEntriesIndexInSelection(fast, true, micromega);
		loadAllDetectors(runSelection[toLoad],false);
		cout<<"Loaded Run "<<runSelection[toLoad]<<endl;
		getRunDetails(runSelection[toLoad], true);
	}
}
void getRunDetails(bool verbose = false){
	for(int i=0;i<selectedRuns;i++){
		cout<<"\nRun Index:     "<<i;
		cout<<"\n\tRun Number:  "<<int(runSelection[i]);
		cout<<"\n\tMomentum:    "<<getMomentum(int(runSelection[i]));
		cout<<"\n\tAmperes:     "<<getAmps(int(runSelection[i]));
		cout<<"\n\tMuon Filter: "<<getMuonFilter(int(runSelection[i]));
		if(verbose){
			cout<<"\n\tRecorded:    "<<getRecorded(int(runSelection[i]));
			cout<<"\n\tGood:        "<<getGood(int(runSelection[i]));
			cout<<"\n\tisOk         "<<getisOK(int(runSelection[i]));
			cout<<"\n\tType:        "<<getType(int(runSelection[i]));
			cout<<"\n\tPOI:         "<<getPOI(int(runSelection[i]));
		}
	}
	if(runSelection[0]==0){
		cout<<"No runs selected \n";
	}
	cout<<endl<<endl;
}
void getRunDetails(int rn, bool verbose = false){
	for(int i=0;i<maxRuns;i++){
		if(runList[i][0]==rn){
			cout<<"\n\tRun Number:  "<<int(rn);
			cout<<"\n\tMomentum:    "<<getMomentum(rn);
			cout<<"\n\tAmperes:     "<<getAmps(rn);
			cout<<"\n\tMuon Filter: "<<getMuonFilter(rn);
			if(verbose){
				cout<<"\n\tRecorded:    "<<getRecorded(rn);
				cout<<"\n\tGood:        "<<getGood(rn);
				cout<<"\n\tisOk         "<<getisOK(rn);
				cout<<"\n\tType:        "<<getType(rn);
				cout<<"\n\tPOI:         "<<getPOI(rn);
			}
			break;
		}
	}
	cout<<endl<<endl;
}
void getLoadedRunDetails(){
	if(runNumberString.str()!=""){
		getRunDetails(atoi(runNumberString.str().c_str()),true);
	} else {
		cout<<"No run loaded\n";
	}
}
bool runExists(int rn){
	for(int j=0;j<maxRuns;j++){
		if(runList[j][0]==rn){
			return true;
		}
	}
	return false;
}
double getAmps(int rn){
	double p = 0;
	for(int i=0;i<maxRuns;i++){
		if(runList[i][0]==rn){
			p = runList[i][AMPERES];
			return p;
		}
	}
	return p;
}
double getMomentum(int rn){
	double p = 0;
	for(int i=0;i<maxRuns;i++){
		if(runList[i][0]==rn){
			p = runList[i][MOMENTUM];
			return p;
		}
	}
	return p;
}
double getRecorded(int rn){
	double p = 0;
	for(int i=0;i<maxRuns;i++){
		if(runList[i][0]==rn){
			p = runList[i][RECORDED];
			return p;
		}
	}
	return p;
}
double getGood(int rn){
	double p = 0;
	for(int i=0;i<maxRuns;i++){
		if(runList[i][0]==rn){
			p = runList[i][GOOD];
			return p;
		}
	}
	return p;
}
double getMuonFilter(int rn){
	double p = 0;
	for(int i=0;i<maxRuns;i++){
		if(runList[i][0]==rn){
			p = runList[i][MUONFILTER];
			return p;
		}
	}
	return p;
}
double getisOK(int rn){
	double p = 0;
	for(int i=0;i<maxRuns;i++){
		if(runList[i][0]==rn){
			p = runList[i][ISOK];
			return p;
		}
	}
	return p;
}
string getType(int rn){
	string p = "";
	for(int i=0;i<maxRuns;i++){
		if(runList[i][0]==rn){
			p = runPOIType[i][TYPE];
			return p;
		}
	}
	return p;
}
string getPOI(int rn){
	string p = "";
	for(int i=0;i<maxRuns;i++){
		if(runList[i][0]==rn){
			p = runPOIType[i][POI];
			return p;
		}
	}
	return p;
}
bool checkMomentum(int rn, double n, bool bypass=false){
	if(n==POSITIVE){
		if(runList[rn][MOMENTUM]>0){
			return true;
		}
	}
	else if(n==NEGATIVE){
		if(runList[rn][MOMENTUM]<0){
			return true;
		}
	}
	else if(runList[rn][MOMENTUM]==n || bypass){
		return true;
	}
	else{
		return false;
	}
}
bool checkAmps(int rn, double n, bool bypass=false){
	if(n=='+'){
		if(runList[rn][AMPERES] == 0){
			return false;
		} else {
			bypass=true;
		}
	}
	if(runList[rn][AMPERES] == n || bypass){
		return true;
	} 
	else{
		return false;
	}
}
bool checkMuonFilter(int rn, bool n = true, bool bypass=false){
	if(runList[rn][MUONFILTER]==n || bypass){
		return true;
	}
	else{
		return false;
	}
}
bool checkType(int rn, string t, bool bypass=false){
	if(runPOIType[rn][TYPE]==t || bypass){
		return true;
	}
	else{
		return false;
	}
}
int getMaxEntriesIndexInSelection(bool fast=true, bool verbose=false, bool micromega){
	int max=0, maxIndex;
	int maxTemp;
	if(selectedRuns!=0){
		for(int i=0;i<selectedRuns;i++){
			if(getisOK(runSelection[i])){
				if(fast){
					maxTemp = getGood(runSelection[i]);
				} else {
					loadAllDetectors(runSelection[i],false);
					maxTemp = t->GetEntries();
				}
				if(micromega){
					loadAllDetectors(runSelection[i],false);
				}
				if(!micromega || t->GetListOfBranches()->FindObject("NBL4Smm1_stripQ")){
					if(maxTemp>max){
						max=maxTemp;
						maxIndex=i;
					}
				}
			}
		}
		if(verbose){
			cout<<"\nRun Number:            "<<runSelection[maxIndex]<<endl;
			cout<<"Entries:               "<<max<<endl;
			cout<<"Index in runSelection: "<<maxIndex<<endl<<endl;
		}
		return maxIndex;
	}else{
		if(verbose){
			cout<<"No runs to choose from"<<max<<endl;
		}
		return -1;
	}
}
void clearSelection(){
	for(int i=0;i<maxRuns;i++){
		runSelection[i]=0;
	}
	selectedRuns=0;
}
void clearSelection(double arr[]){
	for(int i=0;i<selectedRuns;i++){
		arr[i]=0;
	}
}
void copyArray(int src[], int dest[], int size){
	for(int i=0;i<size;i++){
		dest[i]=src[i];
	}
}
void addRunData(int run, int rn, int amp, double p, string t, string poi, int r, int g, bool mf, bool isok){
	runList[run][0] = rn;     //Run Number
	runList[run][1] = amp;    //amperes
	runList[run][2] = p;      //momentum
	runList[run][3] = r;      //recorded
	runList[run][4] = g;      //good
	runList[run][5] = mf;     //muon filter
	runList[run][6] = isok;   //isOK
	runPOIType[run][0] = t.c_str();   //Type
	runPOIType[run][1] = poi.c_str(); //POI
	//cout<<"Added run "<<run<<endl;
}
void initiateDatabase(){
	addRunData (0,1537523826, 0, 0, "calibration", "DWC Calibration", 20218, 20218, false, false); 
	addRunData (1,1537524073, 0, 0, "calibration", "DWC Calibration", 20135, 20135, false, false); 
	addRunData (2,1537524312, 0, 0, "calibration", "DWC Calibration", 20166, 20166, false, false); 
	addRunData (3,1537524775, 0, 0, "calibration", "DWC Calibration", 20223, 20223, false, true); 
	addRunData (4,1537524993, 0, 0, "calibration", "DWC Calibration", 20220, 20220, false, true); 
	addRunData (5,1537525209, 0, 0, "calibration", "DWC Calibration", 20176, 20176, false, true); 
	addRunData (6,1537525452, 0, 0, "calibration", "DWC Calibration", 20206, 20206, false, true); 
	addRunData (7,1537525665, 0, 0, "calibration", "DWC Calibration", 20196, 20196, false, true); 
	addRunData (8,1537525880, 0, 0, "calibration", "DWC Calibration", 20339, 20339, false, true); 
	addRunData (9,1537526116, 0, 0, "calibration", "DWC Calibration", 20191, 20191, false, true); 
	addRunData (10,1537526329, 0, 0, "calibration", "DWC Calibration", 20156, 20156, false, true); 
	addRunData (11,1537526544, 0, 0, "calibration", "DWC Calibration", 20157, 20157, false, true); 
	addRunData (12,1537605192, 0, 10000, "calibration", "TOF Offset", 7687, 7476, false, true); 
	addRunData (13,1537619367, 0, 10000, "calibration", "First Beam", 20393, 20393, false, false); 
	addRunData (14,1537622088, 0, 10000, "calibration", "TOF Offset", 17679, 17165, false, true); 
	addRunData (15,1537645668, 0, 10000, "calibration", "Scuba Calibration", 246857, 246856, false, true); 
	addRunData (16,1537691286, 0, 10000, "deflection", "0:False", 33833, 33833, false, false); 
	addRunData (17,1537694541, 0, 10000, "deflection", "0:False", 32741, 32741, false, true); 
	addRunData (18,1537696203, 0, 10000, "deflection", "0:False", 9764, 9764, false, true); 
	addRunData (19,1537696741, 0, 10000, "deflection", "0:False", 3244, 3244, false, true); 
	addRunData (20,1537696963, 0, 10000, "deflection", "0:False", 2731, 2731, false, true); 
	addRunData (21,1537697126, 0, 10000, "deflection", "0:False", 22374, 22374, false, true); 
	addRunData (22,1537698296, 0, 10000, "deflection", "0:False", 10388, 10388, false, true); 
	addRunData (23,1537700167, 0, 10000, "deflection", "0:False", 18689, 18689, false, true); 
	addRunData (24,1537703559, 0, 10000, "deflection", "0:False", 53125, 53125, false, true); 
	addRunData (25,1537711702, 0, 10000, "deflection", "0:False", 7737, 7737, false, true); 
	addRunData (26,1537712176, 0, 10000, "deflection", "0:False", 6588, 6588, false, true); 
	addRunData (27,1537712564, 0, 10000, "deflection", "0:False", 4846, 4846, false, true); 
	addRunData (28,1537712976, 0, 10000, "deflection", "0:False", 6588, 6588, false, true); 
	addRunData (29,1537715214, 0, 10000, "deflection", "0:False", 44768, 44768, false, true); 
	addRunData (30,1537719413, 0, 10000, "deflection", "0:False", 8887, 8887, false, true); 
	addRunData (31,1537720422, 0, 10000, "deflection", "0:False", 6129, 6129, false, true); 
	addRunData (32,1537720918, 0, 10000, "deflection", "0:False", 3310, 3310, false, true); 
	addRunData (33,1537721187, 0, 10000, "deflection", "0:False", 34855, 34855, false, true); 
	addRunData (34,1537723600, 0, 10000, "deflection", "0:False", 4476, 4476, false, true); 
	addRunData (35,1537723950, 0, 10000, "deflection", "0:False", 2911, 2911, false, true); 
	addRunData (36,1537724219, 0, 10000, "deflection", "0:False", 3528, 3528, false, true); 
	addRunData (37,1537781816, 0, 600, "deflection", "0:False", 9619, 9619, false, true); 
	addRunData (38,1537783395, 0, 10000, "deflection", "0:False", 15024, 15015, false, true); 
	addRunData (39,1537784652, 0, 10000, "deflection", "0:False", 13789, 13691, false, true); 
	addRunData (40,1537785478, 0, 10000, "deflection", "0:False", 38, 38, false, true); 
	addRunData (41,1537786038, 0, 10000, "deflection", "0:False", 546, 546, false, true); 
	addRunData (42,1537789573, 0, 10000, "deflection", "0:False", 175, 175, false, true); 
	addRunData (43,1537792175, 0, 10000, "deflection", "0:False", 563, 563, false, true); 
	addRunData (44,1537796537, 0, 10000, "deflection", "0:False", 1050, 1050, false, true); 
	addRunData (45,1537802100, 0, 10000, "deflection", "0:False", 7, 7, false, false); 
	addRunData (46,1537802128, 240, 10000, "deflection", "240:False", 497, 497, false, true); 
	addRunData (47,1537803772, 120, 10000, "deflection", "120:False", 501, 501, false, true); 
	addRunData (48,1537814423, 0, -10000, "deflection", "0:False", 12813, 12813, false, true); 
	addRunData (49,1537862811, 0, -10000, "deflection", "0:False", 79, 79, false, true); 
	addRunData (50,1537867107, 240, 5000, "deflection", "240:False", 1012, 1012, false, true); 
	addRunData (51,1537868335, 120, 5000, "deflection", "120:False", 1001, 1001, false, true); 
	addRunData (52,1537869886, 0, 5000, "deflection", "0:False", 996, 996, false, true); 
	addRunData (53,1537895191, 0, 0, "calibration", "DWC Calibration", 20330, 20330, false, false); 
	addRunData (54,1537895268, 0, 0, "calibration", "DWC Calibration", 20401, 20401, false, false); 
	addRunData (55,1537895345, 0, 0, "calibration", "DWC Calibration", 20551, 20551, false, false); 
	addRunData (56,1537895618, 0, 0, "calibration", "DWC Calibration", 150, 150, false, false); 
	addRunData (57,1537895764, 0, 0, "calibration", "DWC Calibration", 20546, 20546, false, true); 
	addRunData (58,1537895851, 0, 0, "calibration", "DWC Calibration", 20267, 20267, false, true); 
	addRunData (59,1537895941, 0, 0, "calibration", "DWC Calibration", 20232, 20232, false, true); 
	addRunData (60,1537896105, 0, 0, "calibration", "DWC Calibration", 20165, 20165, false, true); 
	addRunData (61,1537896220, 0, 0, "calibration", "DWC Calibration", 20203, 20203, false, true); 
	addRunData (62,1537896312, 0, 0, "calibration", "DWC Calibration", 23822, 23822, false, true); 
	addRunData (63,1537896423, 0, 0, "calibration", "DWC Calibration", 20929, 20929, false, true); 
	addRunData (64,1537896503, 0, 0, "calibration", "DWC Calibration", 22060, 22060, false, true); 
	addRunData (65,1537896576, 0, 0, "calibration", "DWC Calibration", 20995, 20995, false, true); 
	addRunData (66,1537898557, 0, -10000, "calibration", "Profile", 9432, 9432, false, true); 
	addRunData (67,1537900489, 12, 600, "deflection", "12:False", 26050, 26020, false, true); 
	addRunData (68,1537903575, 60, 10000, "deflection", "60:False", 1040, 1040, false, true); 
	addRunData (69,1537904292, 180, 10000, "deflection", "180:False", 1061, 1061, false, true); 
	addRunData (70,1537906219, 240, 10000, "deflection", "240:False", 1033, 1033, false, true); 
	addRunData (71,1537909725, 0, 5000, "deflection", "0:False", 1024, 1024, false, true); 
	addRunData (72,1537910138, 60, 5000, "deflection", "60:False", 1085, 1085, false, true); 
	addRunData (73,1537910395, 120, 5000, "deflection", "120:False", 1122, 1122, false, true); 
	addRunData (74,1537910721, 120, 5000, "deflection", "120:False", 0, 0, false, false); 
	addRunData (75,1537910732, 180, 5000, "deflection", "180:False", 1243, 1243, false, true); 
	addRunData (76,1537911018, 240, 5000, "deflection", "240:False", 1242, 1242, false, true); 
	addRunData (77,1537913470, 240, -10000, "deflection", "240:False", 27296, 27296, false, true); 
	addRunData (78,1537942404, 120, -10000, "deflection", "120:False", 5427, 5427, false, true); 
	addRunData (79,1538000916, 60, -10000, "deflection", "60:False", 186, 186, false, true); 
	addRunData (80,1538001499, 60, -10000, "deflection", "60:False", 4016, 4016, false, true); 
	addRunData (81,1538003771, 180, -10000, "deflection", "180:False", 2362, 2362, false, true); 
	addRunData (82,1538006510, 240, -10000, "deflection", "240:False", 2560, 2560, false, true); 
	addRunData (83,1538008044, 0, -10000, "deflection", "0:False", 29231, 29231, false, true); 
	addRunData (84,1538031580, 0, 5000, "deflection", "0:True", 2259, 2054, true, true); 
	addRunData (85,1538038031, 120, 5000, "deflection", "120:True", 1714, 1157, true, true); 
	addRunData (86,1538042143, 240, 5000, "deflection", "240:True", 1763, 1010, true, true); 
	addRunData (87,1538046170, 60, 5000, "deflection", "60:True", 1902, 1080, true, true); 
	addRunData (88,1538049472, 180, 5000, "deflection", "180:True", 1927, 998, true, true); 
	addRunData (89,1538052756, 240, -5000, "deflection", "240:True", 600, 360, true, true); 
	addRunData (90,1538055641, 0, -5000, "deflection", "0:True", 605, 305, true, true); 
	addRunData (91,1538058151, 240, 3500, "deflection", "240:True", 2003, 1192, true, true); 
	addRunData (92,1538063096, 240, 10000, "deflection", "240:True", 1410, 1393, true, true); 
	addRunData (93,1538064369, 120, 10000, "deflection", "120:True", 1522, 1287, true, true); 
	addRunData (94,1538066109, 60, 10000, "deflection", "60:True", 1563, 964, true, true); 
	addRunData (95,1538067822, 180, 10000, "deflection", "180:True", 1514, 964, true, true); 
	addRunData (96,1538068869, 0, 10000, "deflection", "0:True", 1694, 981, true, true); 
	addRunData (97,1538069833, 0, -10000, "deflection", "0:True", 1825, 1013, true, true); 
	addRunData (98,1538074506, 120, -10000, "deflection", "120:True", 796, 771, true, true); 
	addRunData (99,1538077069, 180, -10000, "deflection", "180:True", 769, 724, true, true); 
	addRunData (100,1538079107, 60, -10000, "deflection", "60:True", 812, 784, true, true); 
	addRunData (101,1538081315, 240, -10000, "deflection", "240:True", 766, 766, true, true); 
	addRunData (102,1538083650, 120, -5000, "deflection", "120:True", 912, 798, true, true); 
	addRunData (103,1538085941, 180, -5000, "deflection", "180:True", 911, 530, true, true); 
	addRunData (104,1538088606, 60, -5000, "deflection", "60:True", 935, 935, true, true); 
	addRunData (105,1538091404, 0, -5000, "deflection", "0:True", 5419, 5383, true, true); 
	addRunData (106,1538125518, 0, 0, "calibration", "DWC Calibration", 40896, 40896, false, true); 
	addRunData (107,1538125664, 0, 0, "calibration", "DWC Calibration", 41255, 41255, false, true); 
	addRunData (108,1538125793, 0, 0, "calibration", "DWC Calibration", 69084, 69084, false, true); 
	addRunData (109,1538126282, 0, 0, "calibration", "DWC Calibration", 10, 10, false, false); 
	addRunData (110,1538126300, 0, 0, "calibration", "DWC Calibration", 65417, 65417, false, true); 
	addRunData (111,1538126534, 0, 0, "calibration", "DWC Calibration", 42427, 42427, false, true); 
	addRunData (112,1538126651, 0, 0, "calibration", "DWC Calibration", 42428, 42428, false, true); 
	addRunData (113,1538126801, 0, 0, "calibration", "DWC Calibration", 43096, 43096, false, true); 
	addRunData (114,1538126926, 0, 0, "calibration", "DWC Calibration", 2516, 2516, false, false); 
	addRunData (115,1538126941, 0, 0, "calibration", "DWC Calibration", 42302, 42302, false, true); 
	addRunData (116,1538127056, 0, 0, "calibration", "DWC Calibration", 42430, 42430, false, true); 
	addRunData (117,1538127195, 0, 0, "calibration", "DWC Calibration", 42996, 42996, false, true); 
	addRunData (118,1538127316, 0, 0, "calibration", "DWC Calibration", 44869, 44869, false, false); 
	addRunData (119,1538127495, 0, 0, "calibration", "DWC Calibration", 41508, 41508, false, true); 
	addRunData (120,1538127617, 0, 0, "calibration", "DWC Calibration", 42203, 42203, false, true); 
	addRunData (121,1538170034, 0, -5000, "deflection", "0:False", 96210, 96210, false, true); 
	addRunData (122,1538243383, 0, 10000, "calibration", "Scuba Calibration", 1474, 1473, false, true); 
	addRunData (123,1538267157, 240, -5000, "deflection", "240:False", 45731, 45731, false, true); 
	addRunData (124,1538288884, 180, -5000, "deflection", "180:False", 9543, 9543, false, true); 
	addRunData (125,1538293096, 120, -5000, "deflection", "120:False", 3971, 3940, false, true); 
	addRunData (126,1538294874, 60, -5000, "deflection", "60:False", 4090, 4090, false, true); 
	addRunData (127,1538296444, 60, 10000, "deflection", "60:False", 4047, 4047, false, true); 
	addRunData (128,1538297136, 120, 10000, "deflection", "120:False", 5888, 5888, false, true); 
	addRunData (129,1538298064, 0, 10000, "deflection", "0:False", 5020, 5020, false, true); 
	addRunData (130,1538299585, 180, 10000, "deflection", "180:False", 5132, 5132, false, true); 
	addRunData (131,1538300360, 240, 10000, "deflection", "240:False", 5224, 5224, false, true); 
	addRunData (132,1538301037, 240, 5000, "deflection", "240:False", 5186, 5186, false, true); 
	addRunData (133,1538301703, 180, 5000, "deflection", "180:False", 4248, 4248, false, true); 
	addRunData (134,1538302342, 120, 5000, "deflection", "120:False", 4253, 4253, false, true); 
	addRunData (135,1538302979, 60, 5000, "deflection", "60:False", 4029, 4029, false, true); 
	addRunData (136,1538303555, 0, 5000, "deflection", "0:False", 4143, 4143, false, true); 
	addRunData (137,1538304212, 240, 7500, "deflection", "240:False", 4646, 4646, false, true); 
	addRunData (138,1538304657, 240, 3500, "deflection", "240:False", 3333, 3333, false, true); 
	addRunData (139,1538305082, 240, 6250, "deflection", "240:False", 4357, 4357, false, true); 
	addRunData (140,1538305461, 240, 8750, "deflection", "240:False", 5035, 5035, false, true); 
	addRunData (141,1538305980, 240, -8750, "deflection", "240:False", 5043, 5043, false, true); 
	addRunData (142,1538307208, 240, -7500, "deflection", "240:False", 5060, 5060, false, true); 
	addRunData (143,1538308094, 240, -6250, "deflection", "240:False", 5181, 5181, false, true); 
	addRunData (144,1538308890, 240, -3500, "deflection", "240:False", 5095, 5095, false, true); 
	addRunData (145,1538330421, 0, 0, "calibration", "QDC Pedestals", 2801, 2801, false, true); 
	addRunData (146,1538330741, 0, 10000, "calibration", "QDC Pedestals", 14218, 14218, false, true); 
	addRunData (147,1538332209, 0, 10000, "calibration", "TOF Offset", 0, 0, false, false); 
	addRunData (148,1538333479, 0, 10000, "calibration", "TOF Offset", 12989, 12931, false, true); 
	addRunData (149,1538335566, 240, 10000, "deflection", "240:False", 3307, 3307, false, true); 
	initiated=true;
	cout<<"Run Database Initiated\n";
}

// int appendRunSelection(double momentum = ALL, double amps = ALL, int mf = ALL, string type=""){
// 	int originalSelectedRuns = selectedRuns;
// 	if(originalSelectedRuns!=0){	
// 		int originalRunSelection[maxRuns];
// 		copyArray(runSelection,originalRunSelection,selectedRuns);
// 		findRuns(momentum,amps,mf,type);
// 		for(int i=0;i<selectedRuns;i++){
// 				runSelection[i+originalSelectedRuns]=runSelection[i];
// 		}
// 		if(selectedRuns==0){
// 			cout<<"No additionals runs found\n";
// 		}
// 		copyArray(originalRunSelection,runSelection,originalSelectedRuns);
// 		selectedRuns+=originalSelectedRuns;
// 		return 0;
// 	} else {
// 		findRuns(momentum,amps,mf,type);
// 		return 0;
// 	}
// }
