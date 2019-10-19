//AODGraphs.h
//16-04-2019

void drawAngleOfDeviationX(){
	if(!file.IsOpen()){
		cout<<"No file loaded.\n";
	} else { 
		aodHist = new TH1D(name.c_str(), "Angle of Deviation (XZ plane) in radians;Radians;Count", 250, -0.45, 0.45);
		double dwc1pos[3]={0,0,0};
		double dwc2pos[3]={0,0,0};
		double dwc3pos[3]={0,0,0};
		double mm1pos[3]={0,0,0};
		double mm2pos[3]={0,0,0};
		double init=0, final=0, angle=0;
		for(int i = 0; i < entries; i++){
	    	t->GetEntry(i);
	    	if(detectedByAll(2)){
	    		getHitPos(dwc1pos, dwc2pos, dwc3pos, mm1pos, mm2pos);
	    		init = atan((dwc2pos[x]-dwc1pos[x])/(dwc2pos[z]-dwc1pos[z]));
	        	final = atan((mm2pos[x]-mm1pos[x])/(mm2pos[z]-mm1pos[z]));
	        	angle = final-init;
	        	aodHist->Fill(angle);
	    	}
	    }
	    aodCanvas = new TCanvas("aodCanvas","aodCanvas",700,500);
	    aodCanvas->cd();
	    aodHist->Draw();
	    aodHist->Fit("gaus");
	    gStyle->SetOptStat("nem");
	    aodHist->SetDirectory(0);
	}
}

void drawAODGraphs(string graphType=""){
	if(selectedRuns!=0){
		// if (!AODMulti){
		// 	TCanvas *AODMulti;
		// } 
		// if(aodHist){
		// 	TH2D *aodHist;
		// }
		// if(graphType!=""){

		// }
		if(graphType=="momentum"){
			aodHist = new TH2D("Momentum vs Deviation",";Momentum;Radians",1000,-12000,12000,1000,-0.04,0.04);
		}
		if(graphType=="momentumabs"){
			aodHist = new TH2D("Momentum vs Deviation",";Momentum;Radians",1000,-1000,11000,1000,-0.01,0.04);
		}
		if(graphType=="amps"){
			aodHist = new TH2D("Amps vs Deviation",";Amps;Radians",270,-10,260,1000,-0.04,0.04);
		}
		cout<<endl;
		double AODMean[maxRuns];
		clearSelection(AODMean);
		int badMeans = 0;
		for(int i=0;i<selectedRuns;i++){
			loadAllDetectors(runSelection[i], false);
			cout<<"Current Selection Index: "<<i<<endl;
			if(!getisOK(runSelection[i])){
				badMeans++;
				cout<<"###Bad run###"<<endl;
				cout<<"Conditions: "<<name.c_str()<<endl;
				cout<<"Run:        "<<runSelection[i]<<endl<<endl;
				continue;
			} else if(getType(runSelection[i])!="deflection"){
				cout<<"###"<<getType(runSelection[i])<<"###"<<endl;
			}
			else {
				AODMean[i]=AODMean();
				if(graphType=="momentum"){
					aodHist->Fill(momentum*1000,AODMean[i]-returnZero(momentum*1000));
					aodHist->SetTitle((ampsString.str()+"Amps").c_str());
				}
				if(graphType=="momentumabs"){
					aodHist->Fill(fabs(momentum*1000),fabs(AODMean[i]-returnZero(momentum*1000)));
					aodHist->SetTitle((ampsString.str()+"Amps"+" (absolute values)").c_str());
				}
				if(graphType=="amps"){
					aodHist->Fill(amps, fabs(AODMean[i]-returnZero(momentum*1000)));
					aodHist->SetTitle((momentumString.str()+"GeV/c").c_str());
				}
			}
			cout<<"Conditions: "<<name.c_str()<<endl;
			cout<<"Run:        "<<runSelection[i]<<endl;
			cout<<"Mean:       "<<AODMean[i]<<endl<<endl;
		}
		if(graphType!=""){
			TCanvas *AODMulti=new TCanvas("AODMulti","AODMulti",400,400);
			//AODMulti.SetWindowSize(400,400);
			AODMulti->cd();
			AODMulti->Draw();
			aodHist->Draw("box");
			
			if(graphType=="amps"){	
				aodHist->Fit("pol1","c");
			}
			if(graphType=="momentum" || graphType=="momentumabs"){
				f1 = new TF1("f1","[0]/x+[1]",aodHist->GetXaxis()->GetXmin(),aodHist->GetXaxis()->GetXmax());
				aodHist->Fit("f1","R");
			}

			gStyle->SetOptStat("ne");
			aodHist->SetDirectory(0);
			drawAODTheoretical(graphType);
		}
	} else {
		cout<<"No Runs Selected"<<endl;
		cout<<"Add runs using findRuns()"<<endl;
	}
}
double AODMean(){
	double dwc1pos[3]={0,0,0};
	double dwc2pos[3]={0,0,0};
	double dwc3pos[3]={0,0,0};
	double mm1pos[3]={0,0,0};
	double mm2pos[3]={0,0,0};
	double init=0, final=0, angle=0;
	double mean=0, total=0;
	for(int i = 0; i < entries; i++){
    	t->GetEntry(i);
    	if(detectedByAll(2)){
    		getHitPos(dwc1pos, dwc2pos, dwc3pos, mm1pos, mm2pos);
    		init = atan((dwc2pos[x]-dwc1pos[x])/(dwc2pos[z]-dwc1pos[z]));
        	final = atan((mm2pos[x]-mm1pos[x])/(mm2pos[z]-mm1pos[z]));
        	angle = final-init;
        	mean+=angle;
        	total++;
    	}
    }
    mean/=total;
    return mean;
}
double AODMean(bool something){
	//This is an old version
	double init=0, final=0, value=0;
	int totalEntries=0;
  	for(int i = 0; i < entries; i++){
  		//cout<<".";
	    t->GetEntry(i);
	    //value = 0;
	    indexSum3 = 0;
	    QSum3 = 0;
	    indexSum1 = 0;
	    QSum1 = 0;
	    if (nmm3>nmm1){
	      minhits = nmm1;
	    }else{
	      minhits = nmm3;
	    }
	    for (int j=0; j<minhits; j++){
	      QSum3 += mm3StripQ[j];
	      indexSum3 += j*mm3StripQ[j];
	      QSum1 += mm1StripQ[j];
	      indexSum1 += j*mm1StripQ[j];
	    }
	    if (!(QSum3==0 || QSum1 ==0)){
	      clusterIndex3 = (indexSum3/QSum3) + mm3_strip_offset;
	      clusterIndex1 = (indexSum1/QSum1) + mm1_strip_offset;
	      clusterPosition3 = clusterIndex3 * pitch - mmWidth/2;
	      clusterPosition1 = clusterIndex1 * pitch - mmWidth/2;
	      //cout<<"Cluster 3 - Cluster 1"<<clusterPosition3-clusterPosition1<<endl;
	      if (ndwc1L == 1 && ndwc1R == 1 && ndwc2L == 1 && ndwc2R == 1){ 
	        dwc1XPos = ((dwc1XSlope * (int(dwc1L[0] - dwc1R[0]) + dwc1XOffset) * timeFactor)+dwc1XDisplacement);
	        dwc2XPos = ((dwc2XSlope * (int(dwc2L[0] - dwc2R[0]) + dwc2XOffset) * timeFactor)+dwc2XDisplacement);
	        //dwc3XPos = ((dwc3Slope * (int(dwc3L[0] - dwc3R[0]) + dwc3Offset) * timeFactor)+dwc3Displacement);  
	        init=atan((dwc2XPos-dwc1XPos)/(dwc2.distance*1000-dwc1.distance*1000));
	        final=atan((clusterPosition3-clusterPosition1)/(mm3.distance*1000-mm1.distance*1000));
	        //cout<<final;
	        //final=findAngle(dwc3XPos, clusterPosition1, clusterPosition3); //defined in crypticontics.h
	        value=value+(final-init);
	        //cout<<value<<endl;
	        totalEntries++;
	        //cout<<totalEntries<<endl;
	        }
    	}
    }
    cout<<endl;
    //cout<<totalEntries<<endl;
    return (value/double(totalEntries));
}
double AODSigma(){
	//Works, but use getExperimentalAnglesStdDev(std::vector<double> v);
	double dwc1pos[3]={0,0,0};
	double dwc2pos[3]={0,0,0};
	double dwc3pos[3]={0,0,0};
	double mm1pos[3]={0,0,0};
	double mm2pos[3]={0,0,0};

	double initial, final, angle;
	
	double mean=AODMean();
	double sigma=0, total=0;
  	
  	for(int i = 0; i < entries; i++){
  		t->GetEntry(i);
	  	if(detectedByAll(2)){
	  		getHitPos(dwc1pos, dwc2pos, dwc3pos, mm1pos, mm2pos);
	  		final = (mm2pos[x]-mm1pos[x]);
	  		initial = (dwc2pos[x]-dwc1pos[x]);
	  		//cout<<initial<<endl;
	  		initial = atan((dwc2pos[x]-dwc1pos[x])/(dwc2pos[z]-dwc1pos[z]));
        	final = atan((mm2pos[x]-mm1pos[x])/(mm2pos[z]-mm1pos[z]));
        	angle=final-initial;
	  		sigma += pow((angle-mean),2);
	  		total++;
 		}
  	}
  	cout<<total<<endl;
  	sigma/=total;
  	return sqrt(sigma);
}
void drawAODTheoretical(string graphType=""){
	//very bad code. please rewrite. I couldn't bother. -SC
	if(graphType=="amps"){
		ampHist = new TH2D("Amps vs Deviation","Theoretical Amps vs Deviation;Amps;Radians",270,-10,270,1000,-0.04,0.04);

		cout<<"\nTheoretical: \n";
		cout<<"60A:  "<<fabs(angleOfDeviation(beam.proton, beam, mdx27, 60))<<endl;
		cout<<"120A: "<<fabs(angleOfDeviation(beam.proton, beam, mdx27, 120))<<endl;
		cout<<"180A: "<<fabs(angleOfDeviation(beam.proton, beam, mdx27, 180))<<endl;
		cout<<"240A: "<<fabs(angleOfDeviation(beam.proton, beam, mdx27, 240))<<endl;

		ampHist->Fill(60, fabs(angleOfDeviation(beam.proton, beam, mdx27, 60)));
		ampHist->Fill(120, fabs(angleOfDeviation(beam.proton, beam, mdx27, 120)));
		ampHist->Fill(180, fabs(angleOfDeviation(beam.proton, beam, mdx27, 180)));
		ampHist->Fill(240, fabs(angleOfDeviation(beam.proton, beam, mdx27, 240)));

		ampHist->Draw("SAME");
		ampHist->Fit("pol1");
		ampHist->GetFunction("pol1")->SetLineColor(kGreen);
		gStyle->SetOptStat("ne");
		ampHist->SetDirectory(0);
	}
	if(graphType=="momentum"){
		momHist = new TH2D("Momentum vs Deviation","Theoretical Momentum vs Deviation;Momentum;Radians",1000,-12000,12000,1000,-0.04,0.04);
		
		cout<<"\nTheoretical: \n";
		cout<<"-10 GeV/c: "<<(angleOfDeviation(Beam(-10.0).proton, Beam(-10.0), mdx27, amps))<<endl;
		cout<<"-5 GeV/c:  "<<(angleOfDeviation(Beam(-5.0).proton, Beam(-5.0), mdx27, amps))<<endl;
		cout<<" 5 GeV/c:  "<<(angleOfDeviation(Beam(5.0).proton, Beam(5.0), mdx27, amps))<<endl;
		cout<<" 10 GeV/c: "<<(angleOfDeviation(Beam(10.0).proton, Beam(10.0), mdx27, amps))<<endl;

		momHist->Fill(-10000, (angleOfDeviation(Beam(-10.0).proton, Beam(-10.0), mdx27, amps)));
		momHist->Fill(-5000, (angleOfDeviation(Beam(-5.0).proton, Beam(-5.0), mdx27, amps)));
		momHist->Fill(5000, (angleOfDeviation(Beam(5.0).proton, Beam(5.0), mdx27, amps)));
		momHist->Fill(10000, (angleOfDeviation(Beam(10.0).proton, Beam(10.0), mdx27, amps)));

		f2 = new TF1("f2","[0]/x+[1]",momHist->GetXaxis()->GetXmin(), momHist->GetXaxis()->GetXmax());
		momHist->Draw("box SAME");
		momHist->Fit("f2","R");
		momHist->GetFunction("f2")->SetLineColor(kGreen);
		gStyle->SetOptStat("ne");
		momHist->SetDirectory(0);

	}
	if(graphType=="momentumabs"){
		momHist = new TH2D("Momentum vs Deviation (absolute)","Theoretical Momentum vs Deviation (absolute);Momentum;Radians",1000,-1000,12000,1000,-0.01,0.04);
		
		cout<<"\nTheoretical: \n";
		cout<<"-10 GeV/c: "<<fabs(angleOfDeviation(Beam(-10.0).proton, Beam(-10.0), mdx27, amps))<<endl;
		cout<<"-5 GeV/c:  "<<fabs(angleOfDeviation(Beam(-5.0).proton, Beam(-5.0), mdx27, amps))<<endl;
		cout<<" 5 GeV/c:  "<<fabs(angleOfDeviation(Beam(5.0).proton, Beam(5.0), mdx27, amps))<<endl;
		cout<<" 10 GeV/c: "<<fabs(angleOfDeviation(Beam(10.0).proton, Beam(10.0), mdx27, amps))<<endl;

		momHist->Fill(-10000, fabs(angleOfDeviation(Beam(-10.0).proton, Beam(-10.0), mdx27, amps)));
		momHist->Fill(-5000, fabs(angleOfDeviation(Beam(-5.0).proton, Beam(-5.0), mdx27, amps)));
		momHist->Fill(5000, fabs(angleOfDeviation(Beam(5.0).proton, Beam(5.0), mdx27, amps)));
		momHist->Fill(10000, fabs(angleOfDeviation(Beam(10.0).proton, Beam(10.0), mdx27, amps)));

		f2 = new TF1("f2","[0]/x+[1]",momHist->GetXaxis()->GetXmin(), momHist->GetXaxis()->GetXmax());
		momHist->Draw("box SAME");
		momHist->Fit("f2","R");
		momHist->GetFunction("f2")->SetLineColor(kGreen);
		gStyle->SetOptStat("ne");
		momHist->SetDirectory(0);
	}
	if(graphType=="bp"){
		ampHist = new TH2D("Amps vs Deviation","Theoretical Amps vs Deviation;Amps;Radians",100,getAxisRange(graphType,"X","min"),getAxisRange(graphType,"X","max"),1000,-0.04,0.04);

		cout<<"\nTheoretical: \n";
		cout<<"60A:  "<<fabs(angleOfDeviation(beam.proton, beam, mdx27, 60))<<endl;
		cout<<"120A: "<<fabs(angleOfDeviation(beam.proton, beam, mdx27, 120))<<endl;
		cout<<"180A: "<<fabs(angleOfDeviation(beam.proton, beam, mdx27, 180))<<endl;
		cout<<"240A: "<<fabs(angleOfDeviation(beam.proton, beam, mdx27, 240))<<endl;

		ampHist->Fill(Magnet(60/240.0).getBP(), fabs(angleOfDeviation(beam.proton, beam, mdx27, 60)));
		ampHist->Fill(Magnet(120/240.0).getBP(), fabs(angleOfDeviation(beam.proton, beam, mdx27, 120)));
		ampHist->Fill(Magnet(180/240.0).getBP(), fabs(angleOfDeviation(beam.proton, beam, mdx27, 180)));
		ampHist->Fill(Magnet(240/240.0).getBP(), fabs(angleOfDeviation(beam.proton, beam, mdx27, 240)));

		ampHist->Draw("SAME");
		ampHist->Fit("pol1");
		ampHist->GetFunction("pol1")->SetLineColor(kGreen);
		gStyle->SetOptStat("ne");
		ampHist->SetDirectory(0);
	}
}


void drawAODGraphs3D(){
	findRuns(5000,'+',false,"deflection");
	appendRunSelection(-5000,'+',false,"deflection");

	appendRunSelection(-10000,'+',false,"deflection");
	appendRunSelection(10000,'+',false,"deflection");
	
	appendRunSelection(-8750,'+',false,"deflection");
	appendRunSelection(8750,'+',false,"deflection");
	
	appendRunSelection(-7500,'+',false,"deflection");
	appendRunSelection(7500,'+',false,"deflection");
	
	appendRunSelection(-6250,'+',false,"deflection");
	appendRunSelection(6250,'+',false,"deflection");
	
	appendRunSelection(-3500,'+',false,"deflection");
	appendRunSelection(3500,'+',false,"deflection");
	
	cout<<selectedRuns<<endl;
	if(selectedRuns!=0){
		aod3DHist = new TH3F();
		aod3DHist->SetName("Runs");
		aod3DHist->SetTitle("Momentum vs Amps vs Deviation;Momentum;Amps;Radians");
		aod3DHist->SetBins(1000,-12000,12000,27,-10,260,100,-0.04,0.04);
		for(int i=0;i<selectedRuns;i++){
			cout<<"Current Selection Index: "<<i<<endl;
			double mean=0;
			if(!getisOK(runSelection[i])){
				cout<<"###Bad run###"<<endl;
				cout<<"Conditions: "<<name.c_str()<<endl;
				cout<<"Run:        "<<runSelection[i]<<endl<<endl;
				continue;
			} else if(getType(runSelection[i])!="deflection"){ cout<<"###"<<getType(runSelection[i])<<"###"<<endl; }
			
			else {
				loadAllDetectors(runSelection[i], false);
				mean = (AODMean()); 
				aod3DHist->Fill(momentum*1000, amps, mean); 
			}
				cout<<"Conditions: "<<name.c_str()<<endl;
				cout<<"Run:        "<<runSelection[i]<<endl;
				cout<<"Mean:       "<<mean<<endl<<endl;
		}
			aod3DHist->Draw("cm(0, kWHITE,10,10,1)");
			gStyle->SetOptStat("ne");
			aod3DHist->SetDirectory(0);
			//f2=new TF2("f2","[0]*(y/x)+[1]",aod3DHist->GetXaxis()->GetXmin(), aod3DHist->GetXaxis()->GetXmax(),aod3DHist->GetYaxis()->GetXmin(), aod3DHist->GetYaxis()->GetXmax());
			//aod3DHist->Fit("f2","R");
	} else {
		cout<<"No Runs Selected"<<endl;
		cout<<"Add runs using findRuns()"<<endl;
	}

}

double returnZero(int p = -1){
	p=(p==-1?(momentum*1000):p); //automatic p based on global beam's ::momentum
	double z = 0;
	switch (p){

		case 10000: z = 9.99561033811694270e-003; break; //source: calculateZeros()
		case 8750:  z = 9.90642432735101140e-004; break; //source: comparison to theoretical
		case 7500:  z = 0.0021286385960629; break;       //source: comparison to theoretical
		case 6250:  z = -0.00126186462458; break;		 //source: comparison to theoretical 
		case 5000:  z = 6.37578863016942130e-003; break; //source: calculateZeros()
		case 3500:  z = -0.03410127312686427; break;     //source: comparison to theoretical
		case -3500: z = 0.05508914270865077; break;      //source: comparison to theoretical 
		case -5000: z = 8.31763303193165660e-003; break; //source: calculateZeros()
		case -6250: z = -0.00126186462458000; break;	 //source: comparison to theoretical
		case -7500: z = 0.01655245755467153; break;		 //source: comparison to theoretical
		case -8750: z = 0.01110943461008584; break;	  	 //source: comparison to theoretical
		case -10000: z = 9.00121968372224850e-003; break;//source: calculateZeros()

		default: cout<<"Zero error not found for "<<p<<"MeV/c Runs"<<endl;
			break;
	}
	return z;
}


// Redundant functions:

// void drawAmps(){
// 	if(selectedRuns!=0){
// 		std::vector<double> AODMeans;
// 		std::vector<double> AODSigmas;
// 		std::vector<double> xs;
// 		std::vector<double> xe;
// 		double badRuns=0;
// 		for(int i=0;i<selectedRuns;i++){
// 			loadAllDetectors(runSelection[i],false);
// 			cout<<"Current Selection Index: "<<i<<endl;
// 			if(!getisOK(runSelection[i])){
// 				badRuns++;
// 				cout<<"###Bad run###"<<endl;
// 				cout<<"Conditions: "<<name.c_str()<<endl;
// 				cout<<"Run:        "<<runSelection[i]<<endl<<endl;
// 			}
// 			else {
// 				AODMeans.push_back(AODMean());
// 				AODSigmas.push_back(AODSigma());
// 				xs.push_back(amps);
// 				xe.push_back(0);
// 			cout<<"Conditions: "<<name.c_str()<<endl;
// 			cout<<"Run:        "<<runSelection[i]<<endl;
// 			cout<<"Mean:       "<<AODMeans.at(i-badRuns)<<endl;
// 			cout<<"Sigma:      "<<AODSigmas.at(i-badRuns)<<endl<<endl;
// 		}
// 	}
// 		TCanvas *AODMulti=new TCanvas("AODMulti","AODMulti",400,400);
// 		//AODMulti.SetWindowSize(400,400);
// 		AODMulti->cd();
// 		AODMulti->Draw();
// 		int n = xs.size();
// 		ampGraph = new TGraphErrors(xs.size(),&xs[0],&AODMeans[0],&xe[0],&AODSigmas[0]);

// 			gStyle->SetOptStat("ne");
// 			ampGraph->SetMarkerStyle(21);
// 			ampGraph->Draw("AP");
// 			ampGraph->Fit("pol1");
// 			//ampGraph->SetDirectory(0);
		
// 	} else {
// 		cout<<"No Runs Selected"<<endl;
// 		cout<<"Add runs using findRuns()"<<endl;
// 	}
// }

// double calculateZero(int p, bool both=false){
	
// 	findRuns(p,0,false,"deflection");
// 	if(both)
// 		appendRunSelection(-p,0,false,"deflection");

// 	// appendRunSelection(-10000,0,false,"deflection");
// 	// appendRunSelection(10000,0,false,"deflection");
	
// 	// appendRunSelection(-8750,0,false,"deflection");
// 	// appendRunSelection(8750,0,false,"deflection");
	
// 	// appendRunSelection(-7500,0,false,"deflection");
// 	// appendRunSelection(7500,0,false,"deflection");
	
// 	// appendRunSelection(-6250,0,false,"deflection");
// 	// appendRunSelection(6250,0,false,"deflection");
	
// 	// appendRunSelection(-3500,0,false,"deflection");
// 	// appendRunSelection(3500,0,false,"deflection");

// 	cout<<"Total Runs: "<<selectedRuns<<endl;
// 	double total=0, amean;
// 	for(int i=0;i<selectedRuns;i++){
// 		if(!getisOK(runSelection[i])){continue;}
// 		else{
// 			loadAllDetectors(runSelection[i], true);
// 			amean=AODMean();
// 			if(fabs(amean)>0) //this conditional is added to ensure only defined values
// 				total+=amean;
// 			cout<<amean<<endl;
// 		}
// 	}
// 	total/=double(selectedRuns);
// 	cout<<total;
// 	return total;
// }