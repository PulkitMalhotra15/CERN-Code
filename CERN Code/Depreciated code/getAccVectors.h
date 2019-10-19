//getAccVectors.h
//22-02-2019
std::vector<double> accX;
std::vector<double> accY;
std::vector<double> accZ;
std::vector<double> accTX;
std::vector<double> accTZ;
std::vector<double> subFromMM;
std::vector<double> xAvg;

int accCount=0;

void getAccVectorsMult(bool verbose=false){
	double avgX=0,avgY=0,avgZ=0;
	int total=0;
	if(selectedRuns!=0){
		for(int i=0;i<selectedRuns;i++){
			if(runExists(runSelection[i])){
				loadAllDetectors(runSelection[i],false);
				getAccVectors(verbose);
				avgX+=getAverageAcceleration()[0];
				avgY+=getAverageAcceleration()[1];
				avgZ+=getAverageAcceleration()[2];
				total++;
			}
		}
	}
	avgX/=total;
	avgY/=total;
	avgZ/=total;
	cout<<avgX<<endl;
	cout<<avgY<<endl;
	cout<<avgZ<<endl;
}

void getAccVectors(bool verbose=true){
	//double init=0, final=0;
	//EVERYTHING IS IN METERS
	accX.clear();
	accY.clear();
	accZ.clear();
	accTX.clear();
	accTZ.clear();
	subFromMM.clear();
	accCount=0;
	int countxy=0;
	double tempInitX=0,tempInitY=0,tempInitZ=0;
	double tempFinalX=0,tempFinalY=0,tempFinalZ=0;
	double tempAngleX=0,tempAngleY=0,tempAngleZ=0;
	double tempInitDivideFactor=0;
	double tempFinalDivideFactor=0;
	double timeDifference=0, TOFSpeed=0;
	double temptemptemp12=0, temptemptemp34=0,temptemptemp35=0,temptemptemptemp34=0,temptemptemptemp35=0, temptemptemp45=0;
  	for(int i = 0; i < 500; i++){
	    t->GetEntry(i);
	    indexSum3 = 0;
	    QSum3 = 0;
	    indexSum1 = 0;
	    QSum1 = 0;
	    indexSum2 = 0;
	    QSum2 = 0;
	    indexSum4 = 0;
	    QSum4 = 0;
	    minhits = min(nmm1, nmm2, nmm3, nmm4);
	    for (int j=0; j<minhits; j++){
	      QSum3 += mm3StripQ[j];
	      indexSum3 += j*mm3StripQ[j];
	      QSum1 += mm1StripQ[j];
	      indexSum1 += j*mm1StripQ[j];
	      QSum2 += mm2StripQ[j];
	      indexSum2 += j*mm2StripQ[j];
	      QSum4 += mm4StripQ[j];
	      indexSum4 += j*mm4StripQ[j];
	    }
	    if (!(QSum1 == 0 || QSum4 == 0 || QSum3 == 0 || QSum2 == 0)){
	      clusterIndex1 = (indexSum1/QSum1) + mm1_strip_offset;
	      clusterIndex2 = (indexSum2/QSum2) + mm2_strip_offset;
	      clusterIndex3 = (indexSum3/QSum3) + mm3_strip_offset;
	      clusterIndex4 = (indexSum4/QSum4) + mm4_strip_offset;
	      clusterPosition1 =- (clusterIndex1 * pitch - mmWidth/2);
	      clusterPosition2 = (clusterIndex2 * pitch - mmWidth/2);
	      clusterPosition3 =- (clusterIndex3 * pitch - mmWidth/2);
	      clusterPosition4 = (clusterIndex4 * pitch - mmWidth/2);
		    if (ndwc1L == 1 && ndwc1R == 1 && ndwc2L == 1 && ndwc2R == 1 && ndwc1U == 1 && ndwc1D == 1 && ndwc2U == 1 && ndwc2D == 1){ 
		      
		        accCount++;
		        //cout<<accCount;
		        dwc1XPos = -((dwc1XSlope * (int(dwc1L[0] - dwc1R[0]) + dwc1XOffset) * timeFactor)+dwc1XDisplacement);
		        dwc2XPos = -((dwc2XSlope * (int(dwc2L[0] - dwc2R[0]) + dwc2XOffset) * timeFactor)+dwc2XDisplacement);
		        //dwc3XPos = ((dwc3Slope * (int(dwc3L[0] - dwc3R[0]) + dwc3Offset) * timeFactor)+dwc3Displacement);  
		        dwc1YPos = ((dwc1YSlope * (int(dwc1U[0] - dwc1D[0]) + dwc1YOffset) * timeFactor)+dwc1YDisplacement);
		        dwc2YPos = ((dwc2YSlope * (int(dwc2U[0] - dwc2D[0]) + dwc2YOffset) * timeFactor)+dwc2YDisplacement);
		        dwc3YPos = ((dwc3YSlope * (int(dwc3U[0] - dwc3D[0]) + dwc3YOffset) * timeFactor)+dwc3YDisplacement);

		        //Get |v| ie TOFDifference
		        timeDifference = (double(tof1[0]-tof0[0])*timeFactor-timeOffset)*1e-9;
				TOFSpeed = TOFDistance/timeDifference;

		        //finalY=findAngle(dwc3YPos, clusterPosition1, clusterPosition3); //defined in crypticontics.h
		        //finalX=findAngle(dwc3XPos, clusterPosition1, clusterPosition3); //defined in crypticontics.h
				//cout<<accCount<<"\t";

				//Vector components
		        tempInitX=((dwc2XPos-dwc1XPos)/1000);
		        tempInitY=((dwc2YPos-dwc1YPos)/1000);
		    	tempInitZ=(dwc2.distance-dwc1.distance);
		    	//tempInitZ=-0.755;
		        //cout<<"Pos init y "<<tempInitY<<"\t";
		        tempFinalX=((clusterPosition3-clusterPosition1)/1000);
		       	tempFinalY=((clusterPosition2-mm2YDisplacement)-(clusterPosition4-mm4YDisplacement))/1000;
		       	tempFinalZ=(mm3.distance-mm1.distance);
				//cout<<"Pos final y "<<tempFinalY<<endl;
		        temptemptemp12+=(tempInitY/tempInitZ); //DWC1, DWC2
		        temptemptemp34+=(((clusterPosition4-mm4YDisplacement)/1000-dwc3YPos/1000)/(mm4.distance-dwc3.distance)); //DWC3,MM4
				temptemptemp35+=(((clusterPosition2-mm2YDisplacement)/1000-dwc3YPos/1000)/(mm2.distance-dwc3.distance)); //DWC3, MM2
		        temptemptemptemp34+=(((clusterPosition4/1000)-(dwc3YPos/1000))/(mm4.distance-dwc3.distance)); //DWC3,MM4
				temptemptemptemp35+=(((clusterPosition2/1000)-(dwc3YPos/1000))/(mm2.distance-dwc3.distance)); //DWC3, MM2
		        temptemptemp45+=((tempFinalY)/tempFinalZ); //MM4,MM2
		        //temptemptemp45temp+=(tempFinalY/tempFinalZ); //MM4,MM2
		        subFromMM.push_back(subtractFromMM2(temptemptemp12,clusterPosition4/1000, tempFinalZ, clusterPosition2/1000));
		        xAvg.push_back(getAlignmentConstantsY(temptemptemp12,clusterPosition4/1000,tempFinalZ));
		        //tempInitY=0;
		        //tempFinalY=0;
		        //temptemptemp+=(dwc3YPos/1000-dwc2YPos/1000)/(dwc3.distance-dwc2.distance)-(dwc2YPos/1000-dwc1YPos/1000)/(dwc2.distance-dwc1.distance);
		        //dividing factor for making unit vectors
		        tempInitDivideFactor = sqrt(pow(tempInitX, 2) + pow(tempInitY, 2) + pow(tempInitZ,2));
		        tempFinalDivideFactor = sqrt(pow(tempFinalX, 2) + pow(tempFinalY, 2) + pow(tempFinalZ, 2));

		        //cout<<"tempInitDivideFactor"<<tempInitDivideFactor<<"\t";
		        //cout<<"tempFinalDivideFactor"<<tempFinalDivideFactor<<endl;
		        //getting angle values
		        tempAngleX=atan(tempFinalX/tempFinalZ) - atan(tempInitX/tempInitZ);
		        tempAngleY=atan(tempFinalY/tempFinalZ) - atan(tempInitY/tempInitZ);
		        tempAngleZ=atan(tempFinalZ/tempFinalZ) - atan(tempInitZ/tempInitZ);

		        //Make the unit vectors
		        tempInitX/=tempInitDivideFactor;
		        tempInitY/=tempInitDivideFactor;
		        tempInitZ/=tempInitDivideFactor;
		        
		        tempFinalX/=tempFinalDivideFactor;
		        tempFinalY/=tempFinalDivideFactor;
		        tempFinalZ/=tempFinalDivideFactor;
		        if(tempFinalZ-tempInitZ<0){countxy++;}
		    	//cout<<"Initial X"<<tempInitX<<"\t";
		    	// cout<<"Initial Y"<<tempInitY<<"\t";
		    	// cout<<"Initial Z"<<tempInitZ<<"\t";
		    	// // cout<<"Final Y "<<tempFinalY<<endl;
		    	// cout<<"Final X"<<tempFinalX<<"\t";
		    	// cout<<"Final Y"<<tempFinalY<<"\t";
		    	// cout<<"Final Z"<<tempFinalZ<<"\t";
		    	
		    	// cout<<"Vector X"<<tempFinalX-tempInitX<<"\t";
		        // cout<<"Vector Y"<<tempFinalY-tempInitY<<"\t";
		        // cout<<"Vector Z"<<tempFinalZ-tempInitZ<<endl;
		        //get acceleration and push to vectors
		        accX.push_back(getAcceleration(tempFinalX, tempInitX, tempAngleX, TOFSpeed));
		        accY.push_back(getAcceleration(tempFinalY, tempInitY, tempAngleX, TOFSpeed));
		        accZ.push_back(getAcceleration(tempFinalZ, tempInitZ, tempAngleX, TOFSpeed));

		        accTX.push_back(getTheoreticalAcceleration(tempInitX, tempInitZ, amps, TOFSpeed)[0]);
		        accTZ.push_back(getTheoreticalAcceleration(tempInitX, tempInitZ, amps, TOFSpeed)[1]);
			}
	    }
	}
		cout<<accCount;
  		if(verbose){
			cout<<"\nDWC1, DWC2: "<<temptemptemp12/accCount<<endl;
			cout<<"DWC3, MM4:  "<<temptemptemp34/accCount<<endl;
			cout<<"DWC3, MM2:  "<<temptemptemp35/accCount<<endl;
			cout<<"MM4,  MM2:  "<<temptemptemp45/accCount<<endl;
			cout<<"Without:    "<<endl;
			cout<<"DWC3, MM4:  "<<temptemptemptemp34/accCount<<endl;
			cout<<"DWC3, MM2:  "<<temptemptemptemp35/accCount<<endl;
		}
		cout<<"Experimental: "<<endl;
		cout<<"Average x: "<<getAverageAcceleration(accX)<<endl;
		cout<<"Average y: "<<getAverageAcceleration(accY)<<endl;
		cout<<"Average z: "<<getAverageAcceleration(accZ)<<endl;
		// cout<<"\n\nSub thingy:                     "<<getAverageAcceleration(subFromMM)<<endl;
		// cout<<"MM2 clusterPosition actual:     "<<getMeansCustom(5)[1]/1000<<endl;
		// cout<<"MM2 clusterPosition theoretical:"<<getAverageAcceleration(xAvg)<<endl;
		cout<<"\nTheoretical:"<<endl;
		cout<<"Average x: "<<getAverageAcceleration(accTX)<<endl;
		cout<<"Average z: "<<getAverageAcceleration(accTZ)<<endl;
		cout<<"\ncountxy"<<countxy<<endl;			
}

double getAcceleration(double vectorFinal, double vectorInitial, double angle, double speed){
	 return (speed*(vectorFinal-vectorInitial)/(mdx27.width/(speed*cos(angle/2))));
	//return (vectorFinal-vectorInitial)*speed;
}
double* getTheoreticalAcceleration(double xvector, double zvector, double _amps, double speed){
	double accTVec[2];
	//double fieldStrength = getBP(amps)/mdx27.width;
	double fieldStrength = _amps/240.0;
	accTVec[0]=q*speed*(-zvector)*fieldStrength/beam.muon.mass.relativistic;
	accTVec[1]=q*speed*(xvector)*fieldStrength/beam.muon.mass.relativistic;
	return accTVec;
}
double getAverageAcceleration(std::vector<double> acc){
	double average;
	double total=0;
	for(int i=0;i<accCount;i++){
		total+=acc[i];
	}
	average=total/accCount;

	//cout<<totalX/accCount<<endl;
	//cout<<totalY/accCount<<endl;
	//cout<<totalZ/accCount<<endl;

	return average;
}
double* getAverageAcceleration(){
	double averages[3];
	double totalX=0,totalY=0,totalZ=0;
	for(int i=0;i<accCount;i++){
		totalX+=accX[i];
		totalY+=accY[i];
		totalZ+=accZ[i];
	}
	averages[0]=totalX/accCount;
	averages[1]=totalY/accCount;
	averages[2]=totalZ/accCount;

	//cout<<totalX/accCount<<endl;
	//cout<<totalY/accCount<<endl;
	//cout<<totalZ/accCount<<endl;

	return averages;
}
double getAlignmentConstantsY(double a, double c, double d){
	return a*d+c;
}
double subtractFromMM2(double a, double c, double d, double mm2Pos){
	return mm2Pos+getAlignmentConstantsY(a,c,d);
}