	//crypticontics.h
    //16-04-2019

	#include "beamline.h"
	Beam beam(6.25, '+');
	Magnet mdx27("MDX27CO", 1.0);
	const Detector mm1("MicroMega", 8.022);
	const Detector mm2("MicroMega", 8.859);
	const Detector mm3("MicroMega", 8.777);
	const Detector mm4("MicroMega", 8.104);
	const Detector dwc0("DWC", -0.028);
	const Detector dwc1("DWC", 4.358);
	const Detector dwc2("DWC", 5.112);
	const Detector dwc3("DWC", 7.159);

	void defaultSetup(){
		beam=Beam(6.25,'+');
		mdx27=Magnet("MDX27CO",1,0);
		cout<<"Assigned: 6.25 GeV/c positive Beam beam"<<endl;
		cout<<"1T Magnet mdx27"<<endl;
		cout<<"Detectors mm1,mm2,mm3,mm4,dwc0,dwc1,dwc2,dwc3" <<endl;
		cout<<"All at predefined distances as per absolute Z value in Jorgen's document in meters"<<endl;
	}

	double findAngle(double d1, double d2, double d3){
    	int m = 3;
    	double y[3]={d1,d2,d3};
    	double x[3]={dwc3.distance*1000, mm1.distance*1000, mm3.distance*1000};
    	double theta[2]={0, 0};

    	double sumY=0.0;
    	double sumX=0.0;
    	double sumXY=0.0;
    	double sumXs=0.0;
  
    	for (int i = 0; i<m; i++) {
        	sumY += y[i];
        	sumX += x[i];
        	sumXs += (x[i] * x[i]);
        	sumXY += (x[i] * y[i]);
    	}

    	double denominator = ((sumXs) * m) - (sumX * sumX);

	    theta[0] = ((sumXs * sumY) - (sumX * sumXY)) / denominator;
	    theta[1] = ((m * sumXY) - (sumX * sumY)) / denominator;
    	//cout<<atan(theta[1])<<endl;
	    return (atan(theta[1]));
    }

	bool checkChargeAccuracy(double angle, Beam b, Magnet m){
		double check = fabs(1-findCharge(angle, b, m)/::q);
		cout<<check<<"\n";
		if(check < 0.1){
			return true;
		}
		else{
			return false;
		}
	}

	char* getRunFileNumber(){
		gROOT->Reset();
		cout<<"Input file name\n";
		char f[15];
		cin>>(f);
		return f;
	}

	double calccharge(int p, double BP, double valueX, double initY){ //Aayush likes this one but I like the next one
    	return ((sin(valueX)*p*5.36)/(BP*cos(initY)));
	}
	double calccharge(double totalX, double initialY, Beam b, double amps){
   		return ((sin(totalX)*b.momentum.SI)/(mdx27.getBP()*cos(initialY)));
	}

	void getTimeDifference(){
		cout<<"Boop boop";
	}