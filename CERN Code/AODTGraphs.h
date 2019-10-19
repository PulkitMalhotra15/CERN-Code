///AODTGraphs.h
//17-04-2019

double getAxisRange(string graphType="", string axis = "X", string MinOrMax=""){

	double ampsXMin = -10;
	double ampsXMax = 270;
	double bpXMax = 0.6;
	double bpXMin = -0.1;
	double momXMin = -12000;
	double momXMax = 12000;
	double momabsXMin = -1000;
	double momabsXMax = 12000;
	double qXMin = -1.5;
	double qXMax = 1.5;

	double angleMin=-0.08;
	double angleMax=0.08;

	double returnValue=NULL;

	if(axis=="X"){
		if (graphType=="amps"){
			returnValue=(MinOrMax=="min"?ampsXMin:ampsXMax);
		}
		else if (graphType=="momentum"){
			returnValue=(MinOrMax=="min"?momXMin:momXMax);
		}		
		else if (graphType=="momentumabs"){
			returnValue=(MinOrMax=="min"?momabsXMin:momabsXMax);
		}
		else if(graphType=="charge"){
			returnValue=(MinOrMax=="min"?qXMin:qXMax);
		}
		else if (graphType=="bp"){
			returnValue=(MinOrMax=="min"?bpXMin:bpXMax);
		}
	}

	if(axis=="Y"){
		if(graphType!=""){
			returnValue=(MinOrMax=="min"?angleMin:angleMax);
		}
	}

	if(!validGraphType(graphType) || !(axis=="X" || axis=="Y") || !(MinOrMax=="min" || MinOrMax=="max")){
		//overprotective programming?
		cout<<"Chosen Options: "<<graphType<<", "<<axis<<", "<<MinOrMax<<endl;
		cout<<"No valid axis conditions specified\n";
		cout<<"Valid graphType options: amps, momentum, momentumabs, bp\n";
		cout<<"Valid axis options: X, Y\n";
		cout<<"Valid MinOrMax options: min, max\n";
	}

	return returnValue;
}

std::vector<double> getExperimentalAngles(){
	
	std::vector<double> v;

	double dwc1pos[3]={0,0,0};
	double dwc2pos[3]={0,0,0};
	double dwc3pos[3]={0,0,0};
	double mm1pos[3]={0,0,0};
	double mm2pos[3]={0,0,0};
	
	double init=0, final=0, angle=0;
	double total=0;
	
	for(int i = 0; i < entries; i++){
    	t->GetEntry(i);
    	if(detectedByAll(2)){
    		getHitPos(dwc1pos, dwc2pos, dwc3pos, mm1pos, mm2pos);
    		init = atan((dwc2pos[x]-dwc1pos[x])/(dwc2pos[z]-dwc1pos[z]));
        	final = atan((mm2pos[x]-mm1pos[x])/(mm2pos[z]-mm1pos[z]));
        	angle = final-init;
        	v.push_back(angle);
        	total++;
    	}
    }
    return v;
}

double getExperimentalAnglesMean(std::vector<double> angleVector){
	gr = new TGraph(angleVector.size(),&angleVector[0],&angleVector[0]);
	return gr->GetMean();
}
double getExperimentalAnglesStdDev(std::vector<double> angleVector, double mean){
	std::vector<double> validVector;
	for(int i=0;i<angleVector.size();i++){
		if(fabs(1.0-double(angleVector[i])/mean)>0.5){
			validVector.push_back(angleVector[i]);
		}
	}
	gr = new TGraph(validVector.size(),&validVector[0],&validVector[0]);
	cout<<validVector.size()<<endl;
	return gr->GetRMS();
}

double getExperimentalAnglesStdDev(std::vector<double> angleVector){
	gr = new TGraph(angleVector.size(),&angleVector[0],&angleVector[0]);
	return gr->GetRMS()*0.65;
}
// void drawExperimentalAnglesMulti(){
// 	std::vector<double> means, sigmas, ampvec, ampserr;
// 	for(int i=0;i<selectedRuns;i++){
// 		if(getisOK(int(runSelection[i]))){

// 			loadAllDetectors(runSelection[i]);
// 			std::vector<double> values = getExperimentalAngles();

// 			means.push_back(fabs(getExperimentalAnglesMean(values)-returnZero()));
// 			sigmas.push_back(fabs(getExperimentalAnglesStdDev(values)-returnZero()));

// 			ampvec.push_back(amps);
// 			ampserr.push_back(0);

// 		}
// 	}
// 	c1 = new TCanvas("c1","c1",400,400);
// 	gr = new TGraphErrors(int(means.size()), &ampvec[0], &means[0], &ampserr[0], &sigmas[0]);
// 	gr->SetMarkerSize(1);
	
// 	TH1F* h = gr->GetHistogram();
// 	h->SetAxisRange(-0.08,0.08,"Y");
	
// 	TAxis* XAxis = gr->GetXaxis();
//    	XAxis->SetLimits(-10.0, 270.0);
// 	//TAxis* YAxis = gr->GetYaxis();
//    	//YAxis->SetLimits(-0.08,0.08);
	
// 	gr->Draw("AP*");
// 	gr->Draw("nem");
// 	gr->Fit("pol1");
// 	drawAODTheoretical("amps");
	
// }

//###############################################IMPORTANT FUNCTIONS###############################################//

void drawAODTGraphs(string graphType=""){
	std::vector<double> means, sigmas, xvec, xerr;
	bool flag=false; //overprotective programming but whatever
	int badRuns=0;
	if(validGraphType(graphType)){
		if(selectedRuns!=0){
			for(int i=0;i<selectedRuns;i++){
				loadAllDetectors(runSelection[i]);
				flag = false;
				cout<<endl;
				cout<<"Current Selection Index: "<<i<<endl;
				if(!getisOK(runSelection[i])){
					cout<<"###Bad run###"<<endl;
					cout<<"Conditions: "<<name.c_str()<<endl;
					cout<<"Run:        "<<runSelection[i]<<endl<<endl;
					flag=true;
					badRuns++;
					continue;
				}
				if(getType(runSelection[i])!="deflection"){
					cout<<"###"<<getType(runSelection[i])<<"###"<<endl;
					flag=true;
					badRuns++;
					continue;
				}
				if(!flag){
					std::vector<double> values = getExperimentalAngles();
					if(graphType=="amps"){
						means.push_back(fabs(getExperimentalAnglesMean(values)-returnZero()));
					} else {
						means.push_back(getExperimentalAnglesMean(values)-returnZero());
					}
					sigmas.push_back(getExperimentalAnglesStdDev(values));
					//sigmas.push_back(0);
					xerr.push_back(0);
					if(graphType=="amps"){ xvec.push_back(amps); } 
					else if (graphType=="momentum"){ xvec.push_back(momentum*1000); }
					else if (graphType=="momentumabs"){ xvec.push_back(fabs(momentum*1000)); }
					else if (graphType=="charge"){ xvec.push_back(momentum>0?1:-1); }
					else if (graphType=="bp"){ mdx27.updateData("strength",amps/240.0); xvec.push_back(mdx27.getBP()); }
				}
				cout<<"Conditions: "<<name.c_str()<<endl;
				cout<<"Run:        "<<runSelection[i]<<endl;
				cout<<"Mean:       "<<means.at(i-badRuns)<<endl;
				cout<<"Sigma:      "<<sigmas.at(i-badRuns)<<endl<<endl;
			}
			
			AODCanvas = new TCanvas((graphType+"canvas").c_str(),(graphType+"canvas").c_str(),400,400);
			AODGraph = new TGraphErrors(int(means.size()), &xvec[0], &means[0], &xerr[0], &sigmas[0]);
			auto AODHist = AODGraph->GetHistogram();
			auto XAxis = AODGraph->GetXaxis();
			auto YAxis = AODGraph->GetYaxis();
			YAxis->SetTitle("Angle");
			
			double xmin = getAxisRange(graphType,"X","min");
			double xmax = getAxisRange(graphType,"X","max");
			double ymin = getAxisRange(graphType,"Y","min");
			double ymax = getAxisRange(graphType,"Y","max");
			
			XAxis->SetLimits(xmin, xmax); //change x-axis range
			
			AODHist->SetAxisRange(ymin, ymax, "Y"); //change y-axis range
					
			AODGraph->Draw("AP*");
			//AODHist->SetName((";"+graphType+";Count").c_str());

			if(graphType=="amps"){
				AODGraph->SetTitle(("#theta vs Amperes, at "+momentumString.str()+"GeV/c").c_str());
				XAxis->SetTitle("Amperes");
				AODGraph->Fit("pol1");
			} else if (graphType=="momentum"){
				AODGraph->SetTitle(("#theta vs Momentum, at "+ampsString.str()+"Amps").c_str());
				XAxis->SetTitle("MeV/c");
				f1 = new TF1("f1","[0]/x+[1]", xmin, xmax);
				AODGraph->Fit("f1","R");
			} else if (graphType=="momentumabs"){
				AODGraph->SetTitle(("#theta vs Absolute Momentum, at "+ampsString.str()+"Amps"+" (absolute values)").c_str());
				XAxis->SetTitle("MeV/c");
				f1 = new TF1("f1","[0]/x+[1]", xmin, xmax);
				AODGraph->Fit("f1","R");
			} else if (graphType=="charge"){
				AODGraph->SetTitle(("#theta vs Charge, at "+ampsString.str()+"Amps").c_str());
				XAxis->SetTitle("Charge/Elemental charge");
				AODGraph->Fit("pol1");
			} else if (graphType=="bp"){
				AODGraph->SetTitle(("#theta vs Bending Power, at "+momentumString.str()+"GeV/c").c_str());
				XAxis->SetTitle("Tm");
				AODGraph->Fit("pol1");
			}
			
			XAxis->CenterTitle();
			YAxis->CenterTitle();
			//gStyle->SetOptStat("ne");
			drawAODTheoretical(graphType);

		} else {
			cout<<"No Runs Selected"<<endl;
			cout<<"Add runs using findRuns()"<<endl;
		}
	} else {
		cout<<"Valid graphType options: amps, momentum, momentumabs, bp\n";
	}
	// string basedir = "C:/Users/Satchit Chatterji/root_v5.34.36/Analysis Images/Theta vs Momentum(Abs)/";
	// string muonfilter = "";
	// if (getMuonFilter(runSelection[0])){
	// 	muonfilter = "Muon Filter ";
	// }
	// string imgname = (basedir+muonfilter+ampsString.str()+"A.png");
	// AODCanvas->SaveAs(imgname.c_str());
}

bool validGraphType(string gt){
	return (gt=="momentum" || gt=="momentumabs" || gt=="amps" || gt=="bp" || gt=="charge");
}

void saveCanvas(TCanvas *c, string gtype){
	c->SetWindowSize(800,800);
	string basedir = "C:/Users/Satchit Chatterji/root_v5.34.36/Analysis Images/Fresh/";
	string muonfilter = "", imgname = "";
	if (getMuonFilter(runSelection[0])){
		muonfilter = "Muon Filter ";
	}
	if(gtype == "amps" || gtype == "bp"){
		imgname = (basedir+muonfilter+gtype+momentumString.str()+"GeV.png");
	} else {
		imgname = (basedir+muonfilter+gtype+ampsString.str()+"A.png");
	}
	c->SaveAs(imgname.c_str());
	c->SetWindowSize(400,400);
}