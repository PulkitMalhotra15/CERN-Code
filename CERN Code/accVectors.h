//getAccVectors.h
//16-04-2019
std::vector<double> acc[3];
std::vector<double> accT[3];

const int x=0, y=1, z=2;

const double flipX = 1;
const double flipY = 1;
const double flipZ = 1;

void getAccVectors(){
	//EVERYTHING IS IN METERS
	acc[x].clear();
	acc[y].clear();
	acc[z].clear();

	accT[x].clear();
	accT[y].clear();
	accT[z].clear();

	double dwc1pos[3]={0,0,0};
	double dwc2pos[3]={0,0,0};
	double dwc3pos[3]={0,0,0};
	double mm1pos[3]={0,0,0};
	double mm2pos[3]={0,0,0};

	double initial[3]={0,0,0};
	double final[3]={0,0,0};
	double angle[3]={0,0,0};
	double tempAvg=0;
  	for(int i = 0; i < entries; i++){
	    t->GetEntry(i);
	    if (detectedByAll()){ 
			getHitPos(dwc1pos, dwc2pos, dwc3pos, mm1pos, mm2pos);  

			//Vector components
	        initial[x] = (flipX) * (dwc2pos[x]-dwc1pos[x]);
	        initial[y] = (flipY) * (dwc2pos[y]-dwc1pos[y]);
	    	initial[z] = (flipZ) * (dwc2pos[z]-dwc1pos[z]);2
	    	
	        final[x] = (flipX) * (mm2pos[x]-mm1pos[x]);
	       	final[y] = (flipY) * (mm2pos[y]-mm1pos[y]);
	       	final[z] = (flipZ) * (mm2pos[z]-mm1pos[z]);

	        //getting angle values
	        angle[x] = (atan(final[x]/final[z]) - atan(initial[x]/initial[z]));
			angle[y] = (atan(final[y]/final[z]) - atan(initial[y]/initial[z]));
			angle[z] = (atan(final[z]/final[z]) - atan(initial[z]/initial[z]));

	        //Make unit vectors
	       	makeUnitVector(initial);
	       	makeUnitVector(final);
	     	cout<<initial[z]<<"\t";
	     	cout<<final[z]<<endl;
	        pushAccelerations(acc, final, initial, angle);
	        pushTheoreticalAccelerations(accT, initial);

		}
	}
	cout<<"Experimental: "<<endl;
		displayAverage(acc[x], "x");
		displayAverage(acc[y], "y");
		displayAverage(acc[z], "z");
	cout<<"\nTheoretical:"<<endl;
		displayAverage(accT[x], "x");
		displayAverage(accT[y], "y");
		displayAverage(accT[z], "z");
}

void makeUnitVector(double a[]){
	double d = sqrt(a[x]*a[x]+a[y]*a[y]+a[z]*a[z]);
	a[x] /= d;
	a[y] /= d;
	a[z] /= d;
}

double getAcceleration(double vectorFinal, double vectorInitial, double angle){
	 return (tofSpeed()*(vectorFinal-vectorInitial)/(mdx27.width/(tofSpeed()*cos(angle/2))));
}
void pushAccelerations(std::vector<double> a[], double f[], double i[], double angle[]){
	a[x].push_back(getAcceleration(f[x], i[x], angle[x]));
	a[y].push_back(getAcceleration(f[y], i[y], angle[y]));
	a[z].push_back(getAcceleration(f[z], i[z], angle[z]));
}
void pushTheoreticalAccelerations(std::vector<double> a[], double i[]){
	//double fieldStrength = -(flipY) * mdx27.getBP(amps) * mdx27.width;
	double fieldStrength = -(flipY) * (amps/240);
	double speed = (flipZ) * tofSpeed();
	double tempq = ::q*(beam.momentum.relativistic>0?1:-1);
	double m = beam.muon.mass.relativistic;
	//double m = ((beam.momentum.SI)/speed);

	a[x].push_back(tempq*speed*(-i[z])*fieldStrength/m);
	a[y].push_back(0);
	a[z].push_back(tempq*speed*(i[x])*fieldStrength/m);
} 
void displayAverage(std::vector<double> d, string xstr){
	double total=0;
	for(int i=0; i<d.size(); i++){
		total+=d.at(i);
	}
	cout<<"Average "<<xstr<<": "<<total/d.size()<<endl;
}

void drawAccelerations(){
			//EVERYTHING IS IN METERS
	acc[x].clear();
	acc[y].clear();
	acc[z].clear();

	accT[x].clear();
	accT[y].clear();
	accT[z].clear();

	double dwc1pos[3]={0,0,0};
	double dwc2pos[3]={0,0,0};
	double dwc3pos[3]={0,0,0};
	double mm1pos[3]={0,0,0};
	double mm2pos[3]={0,0,0};

	double initial[3]={0,0,0};
	double final[3]={0,0,0};
	double angle[3]={0,0,0};
	double tempAvg=0;


	AccCanvas = new TCanvas("AccCanvas","AccCanvas",800,800);
	AccCanvas->Divide(2,2);

	AccXHist = new TH1D("Experimental Values", "Accelerations in X (Measured);m/s^2;Count", 100, -7e16, 7e16);
	AccXTHist = new TH1D("Theoretical Values", "Accelerations in X (Theoretical);m/s^2;Count", 100, -7e16, 7e16);
	AccZHist = new TH1D("Experimental Values", "Accelerations in Z (Measured);m/s^2;Count", 100, -7e14, 7e14);
	AccZTHist = new TH1D("Theoretical Values", "Accelerations in Z (Theoretical);m/s^2;Count", 100, -7e14, 7e14);

	int currentPos=0;
  	
  	for(int i = 0; i < entries; i++){
	    t->GetEntry(i);
	    if (detectedByAll()){ 
	    	currentPos++;
			getHitPos(dwc1pos, dwc2pos, dwc3pos, mm1pos, mm2pos);  

			//Vector components
	        initial[x] = (flipX) * (dwc2pos[x]-dwc1pos[x]);
	        initial[y] = (flipY) * (dwc2pos[y]-dwc1pos[y]);
	    	initial[z] = (flipZ) * (dwc2pos[z]-dwc1pos[z]);
	    	
	        final[x] = (flipX) * (mm2pos[x]-mm1pos[x]);
	       	final[y] = (flipY) * (mm2pos[y]-mm1pos[y]);
	       	final[z] = (flipZ) * (mm2pos[z]-mm1pos[z]);

	        //getting angle values
	        angle[x] = (atan(final[x]/final[z]) - atan(initial[x]/initial[z]));
			angle[y] = (atan(final[y]/final[z]) - atan(initial[y]/initial[z]));
			angle[z] = (atan(final[z]/final[z]) - atan(initial[z]/initial[z]));

	        //Make unit vectors
	       	makeUnitVector(initial);
	       	makeUnitVector(final);

	        pushAccelerations(acc, final, initial, angle);
	        pushTheoreticalAccelerations(accT, initial);

	        AccXHist->Fill(acc[x][currentPos]);
	        AccXTHist->Fill(accT[x][currentPos]);
	        AccZHist->Fill(acc[z][currentPos]);
	        AccZTHist->Fill(accT[z][currentPos]);
		}
	}
	cout<<"Experimental: "<<endl;
		displayAverage(acc[x], "x");
		displayAverage(acc[z], "z");
	cout<<"\nTheoretical:"<<endl;
		displayAverage(accT[x], "x");
		displayAverage(accT[z], "z");

	AccCanvas->cd(1);
	AccXHist->Draw();
	AccXHist->Fit("gaus");
	AccXHist->SetDirectory(0);
	AccCanvas->cd(2);
	AccXTHist->Draw();
	AccXTHist->Fit("gaus");
	AccXTHist->SetDirectory(0);
	AccCanvas->cd(3);
	AccZHist->Draw();
	AccZHist->Fit("gaus");
	AccZHist->SetDirectory(0);
	AccCanvas->cd(4);
	AccZTHist->Draw();
	AccZTHist->Fit("gaus");
	AccZTHist->SetDirectory(0);
}