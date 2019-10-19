  //trackerScatter.C
  //execute loadAllDetectors.C before running this
  //17-04-2019

void DWCScatter(char n[], bool newCanvas=true){
	double dwcXPos, dwcYPos;
    bool exit = false;
    TH2D* g = new TH2D(name.c_str(),"",100,-100,100,100,-100,100);
	//int validPoints = 0;
  	for(int i = 0; i < entries; i++){
    	t->GetEntry(i);
    	if(strcmp(n,"1")==0){
            g->SetTitle("DWC1");
    		if (ndwc1L == 1 && ndwc1R == 1 && ndwc1U == 1 && ndwc1D == 1){
    			dwcXPos = (dwc1XSlope * (int(dwc1L[0] - dwc1R[0]) + dwc1XOffset) * timeFactor)+dwc1XDisplacement;	
    			dwcYPos = (dwc1YSlope * (int(dwc1U[0] - dwc1D[0]) + dwc1YOffset) * timeFactor)+dwc1YDisplacement;
    		}	
    	}
    	else if(strcmp(n,"2")==0){
            g->SetTitle("DWC2");
            if (ndwc2L == 1 && ndwc2R == 1 && ndwc2U == 1 && ndwc2D == 1){
    			dwcXPos = (dwc2XSlope * (int(dwc2L[0] - dwc2R[0]) + dwc2XOffset) * timeFactor)+dwc2XDisplacement;
    	    	dwcYPos = (dwc2YSlope * (int(dwc2U[0] - dwc2D[0]) + dwc2YOffset) * timeFactor)+dwc2YDisplacement;
    		}	
    	}
    	else if(strcmp(n,"3")==0){
            g->SetTitle("DWC3");
            if (ndwc3L == 1 && ndwc3R == 1 && ndwc3U == 1 && ndwc3D == 1){
    			dwcXPos = (dwc3XSlope * (int(dwc3L[0] - dwc3R[0]) + dwc3XOffset) * timeFactor)+dwc3XDisplacement;	
    			dwcYPos = (dwc3YSlope * (int(dwc3U[0] - dwc3D[0]) + dwc3YOffset) * timeFactor)+dwc3YDisplacement;
    		}
    	} else {
            cout<<"DWC number not recognized. Only DWC 1, 2 and 3 availible.\n";
            exit = true;
            break;
        }
    	g->Fill(dwcXPos, dwcYPos);
        g->Draw("colz");
     	//validPoints++;
    }
    if(!exit){
        g->SetDirectory(0);
        if(newCanvas){
            dwcScatter=new TCanvas("dwcScatter","dwcScatter",400,400);
            dwcScatter->cd();
        }
        g->Draw("colz");
        gStyle->SetOptStat("em");
        //redundantCanvas();
    }
}

void MMScatter(char n[], bool newCanvas=true){
    bool exit = false;
  if(strcmp(n,"1")==0 || strcmp(n,"first")==0){
    TH2D* g = new TH2D(name.c_str(),"MicroMega set 1 and 4",100,-100,100,100,-100,100); //change name to string
    for(int i = 0; i < entries; i++){
      t->GetEntry(i);
      indexSum1 = 0;
      indexSum4 = 0;
      QSum1 = 0;
      QSum4 = 0;
      if (nmm4>nmm1){
        minhits = nmm1;
      }else{
        minhits = nmm4;
      }
      for (int j=0; j<minhits; j++){
        QSum1 += mm1StripQ[j];
        indexSum1 += j*mm1StripQ[j];
        QSum4 += mm4StripQ[j];
        indexSum4 += j*mm4StripQ[j];
      }
      if (!(QSum1 == 0 || QSum2 == 0)){
        clusterIndex1 = (indexSum1/QSum1) + mm1_strip_offset;
        clusterIndex4 = (indexSum4/QSum4) + mm4_strip_offset;
        clusterPosition1 = clusterIndex1 * pitch - mmWidth/2;
        clusterPosition4 = clusterIndex4 * pitch - mmWidth/2;
        g->Fill(clusterPosition1, clusterPosition4-mm4YDisplacement);
      }
    }
  }
  else if(strcmp(n,"second")==0 || strcmp(n,"2")==0){
    TH2D* g = new TH2D(name.c_str(), "MicroMega set 3 and 2",100,-100,100,100,-100,100);
    for(int i = 0; i < entries; i++){
      t->GetEntry(i);
      indexSum2 = 0;
      indexSum3 = 0;
      QSum2 = 0;
      QSum3 = 0;
      if (nmm2>nmm3){
        minhits = nmm3;
      }else{
        minhits = nmm2;
      }
      for (int j=0; j<minhits; j++){
        QSum2 += mm2StripQ[j];
        indexSum2 += j*mm2StripQ[j];
        QSum3 += mm3StripQ[j];
        indexSum3 += j*mm3StripQ[j];
      }
      if (!(QSum2 == 0 || QSum3 == 0)){
        clusterIndex2 = (indexSum2/QSum2) + mm2_strip_offset;
        clusterIndex3 = (indexSum3/QSum3) + mm3_strip_offset;
        clusterPosition2 = clusterIndex2 * pitch - mmWidth/2;
        clusterPosition3 = clusterIndex3 * pitch - mmWidth/2;
        g->Fill(clusterPosition3, clusterPosition2-mm2YDisplacement); //mm
      }
    } 
  } else {
        cout<<"MM number not recognized. Only MM sets 1 and 2 availible.\n";
        exit = true;
        break;
    }
    if(!exit){
        g->SetDirectory(0);
        if(newCanvas){
            mmScatter=new TCanvas("mmScatter","mmScatter",400,400);
            mmScatter->cd();
        }
        g->Draw("colz");
        gStyle->SetOptStat("em");
        //redundantCanvas();
        //break;
    }  
}

void detector(int det, bool newCanvas=true){
    switch(det){
        case 1: DWCScatter("1", newCanvas);
            break;
        case 2: DWCScatter("2", newCanvas);
            break;
        case 3: DWCScatter("3", newCanvas);
            break;
        case 4: MMScatter("1", newCanvas);
            break;
        case 5: MMScatter("2", newCanvas);
            break;
        default: cout<<"Detector not found\n";
                 cout<<"Type help() for details \n";
            break;
    }
}

void trackerScatter(){
    c2 = new TCanvas("c2","c2",1500,300);
    c2->Divide(5,1);
    for(int i=5;i>0;i--){
        c2->cd(i);
        detector(i, false);    
    }
 }

void help(){
    cout<<"\tDWCScatter(\"1\")\n";
    cout<<"\tDWCScatter(\"2\")\n";
    cout<<"\tDWCScatter(\"3\")\n";
    cout<<"\tMMScatter(\"first\")\n";
    cout<<"\tMMScatter(\"second\")\n";
    cout<<"\tMMScatter(\"1\")\n";
    cout<<"\tMMScatter(\"2\")\n\n";
    cout<<"\tdetector(1)\n";
    cout<<"\t\t\t(This calls DWCScatter(\"1\")\n";
    cout<<"\tdetector(2)\n";
    cout<<"\t\t\t(This calls DWCScatter(\"2\")\n";
    cout<<"\tdetector(3)\n";
    cout<<"\t\t\t(This calls DWCScatter(\"3\")\n";
    cout<<"\tdetector(4)\n";
    cout<<"\t\t\t(This calls MMScatter(\"1\")\n";
    cout<<"\tdetector(5)\n";
    cout<<"\t\t\t(This calls MMScatter(\"2\")\n";
}
void getDisplacementsY(int _detector){
    double totalY=0, countY=0,t;
    findRuns(10000,0,false,"deflection");
    for(int i=0;i<selectedRuns;i++){
        cout<<i<<endl;
        loadAllDetectors(runSelection[i],false);
        if(getisOK(runSelection[i])){
            t=getMeansCustom(_detector)[1];
            totalY+=t;
            countY++;
        }
    }
    cout<<totalY/countY;
}
void getDisplacementsX(int _detector){
    double totalY=0, countY=0,t;
    findRuns(10000,0,false,"deflection");
    for(int i=0;i<selectedRuns;i++){
        cout<<i<<endl;
        loadAllDetectors(runSelection[i],false);
        if(getisOK(runSelection[i])){
            t=getMeansCustom(_detector)[0];
            totalY+=t;
            countY++;
        }
    }
    cout<<totalY/countY;
}
void getMeansPlaintext(int n){
    double mean[2];
    TH1D histx = TH1D(runFile, "x hits", 400, -400, 400);
    TH1D histy = TH1D(runFile, "x hits", 400, -400, 400);
    double dwcXPos, dwcYPos;
    bool toFill = true;
    switch(n){
    case 1:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            if (ndwc1L == 1 && ndwc1R == 1 && ndwc1U == 1 && ndwc1D == 1){
                dwcXPos = (dwc1XSlope * (int(dwc1L[0] - dwc1R[0]) + dwc1XOffset) * timeFactor)+dwc1XDisplacement;   
                dwcYPos = (dwc1YSlope * (int(dwc1U[0] - dwc1D[0]) + dwc1YOffset) * timeFactor)+dwc1YDisplacement; 
                histx.Fill(dwcXPos);
                histy.Fill(dwcYPos);
            }
        }
        cout<<"DWC1\n";
        break;
    case 2:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            if (ndwc2L == 1 && ndwc2R == 1 && ndwc2U == 1 && ndwc2D == 1){
                dwcXPos = (dwc2XSlope * (int(dwc2L[0] - dwc2R[0]) + dwc2XOffset) * timeFactor)+dwc2XDisplacement;   
                dwcYPos = (dwc2YSlope * (int(dwc2U[0] - dwc2D[0]) + dwc2YOffset) * timeFactor)+dwc2YDisplacement; 
                histx.Fill(dwcXPos);
                histy.Fill(dwcYPos);
            }
        }
        cout<<"DWC2\n";
        break;
    case 3:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            if (ndwc3L == 1 && ndwc3R == 1 && ndwc3U == 1 && ndwc3D == 1){
                dwcXPos = (dwc3XSlope * (int(dwc3L[0] - dwc3R[0]) + dwc3XOffset) * timeFactor)+dwc3XDisplacement;   
                dwcYPos = (dwc3YSlope * (int(dwc3U[0] - dwc3D[0]) + dwc3YOffset) * timeFactor)+dwc3YDisplacement; 
                histx.Fill(dwcXPos);
                histy.Fill(dwcYPos);
            }
        }
        cout<<"DWC3\n";
        break;
    case 4:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            indexSum4 = 0;
            indexSum1 = 0;
            QSum4 = 0;
            QSum1 = 0;
            if (nmm4>nmm1){
                minhits = nmm1;
            }else{
                minhits = nmm4;
            }
            for (int j=0; j<minhits; j++){
                QSum4 += mm4StripQ[j];
                indexSum4 += j*mm4StripQ[j];
                QSum1 += mm1StripQ[j];
                indexSum1 += j*mm1StripQ[j];
            }
            if (!(QSum4 == 0 || QSum1 == 0)){
                clusterIndex4 = (indexSum4/QSum4) + mm4_strip_offset;
                clusterIndex1 = (indexSum1/QSum1) + mm1_strip_offset;
                clusterPosition4 = clusterIndex4 * pitch - mmWidth/2;
                clusterPosition1 = clusterIndex1 * pitch - mmWidth/2;
                histx.Fill(clusterPosition1);
                histy.Fill(clusterPosition4-mm4YDisplacement);
            }
        }
        cout<<"MM1/MM4\n";
        break;
    case 5:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            indexSum2 = 0;
            indexSum3 = 0;
            QSum2 = 0;
            QSum3 = 0;
            if (nmm2>nmm3){
                minhits = nmm3;
            }else{
                minhits = nmm2;
            }
            for (int j=0; j<minhits; j++){
                QSum2 += mm2StripQ[j];
                indexSum2 += j*mm2StripQ[j];
                QSum3 += mm3StripQ[j];
                indexSum3 += j*mm3StripQ[j];
            }
            if (!(QSum2 == 0 || QSum3 == 0)){
                clusterIndex2 = (indexSum2/QSum2) + mm2_strip_offset;
                clusterIndex3 = (indexSum3/QSum3) + mm3_strip_offset;
                clusterPosition2 = clusterIndex2 * pitch - mmWidth/2;
                clusterPosition3 = clusterIndex3 * pitch - mmWidth/2;
                histx.Fill(clusterPosition3);
                histy.Fill(clusterPosition2-mm2YDisplacement);
            }
        }
        cout<<"MM3/MM2\n";
        break;
    default:
        cout<<"Detector not found\n";
        toFill = false;
        break;
    }
    if(toFill){
        histx.Fit("gaus");
        histy.Fit("gaus");
        mean[0] = histx.GetFunction("gaus")->GetParameter(1);
        mean[1] = histy.GetFunction("gaus")->GetParameter(1);
        cout<<"\n\n\n\nMean x = "<<mean[0];
        cout<<"\nMean y = "<<mean[1]<<"\n\n";
        //detector(n);
        //return mean;
    } else {
        //return 0;
    }
}
double* getMeansValues(int n){
    double mean[2];
    TH1D histx = TH1D(runFile, "x hits", 1000, -1000, 1000);
    TH1D histy = TH1D(runFile, "y hits", 1000, -1000, 1000);
    double dwcXPos, dwcYPos;
    bool toFill = true;
    switch(n){
    case 1:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            if (ndwc1L == 1 && ndwc1R == 1 && ndwc1U == 1 && ndwc1D == 1){
                dwcXPos = (dwc1XSlope * (int(dwc1L[0] - dwc1R[0]) + dwc1XOffset) * timeFactor)+dwc1XDisplacement;   
                dwcYPos = (dwc1YSlope * (int(dwc1U[0] - dwc1D[0]) + dwc1YOffset) * timeFactor)+dwc1YDisplacement; 
                histx.Fill(dwcXPos);
                histy.Fill(dwcYPos);
            }
        }
        cout<<"DWC1\n";
        break;
    case 2:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            if (ndwc2L == 1 && ndwc2R == 1 && ndwc2U == 1 && ndwc2D == 1){
                dwcXPos = (dwc2XSlope * (int(dwc2L[0] - dwc2R[0]) + dwc2XOffset) * timeFactor)+dwc2XDisplacement;   
                dwcYPos = (dwc2YSlope * (int(dwc2U[0] - dwc2D[0]) + dwc2YOffset) * timeFactor)+dwc2YDisplacement;
                histx.Fill(dwcXPos);
                histy.Fill(dwcYPos);
            }
        }
        cout<<"DWC2\n";
        break;
    case 3:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            if (ndwc3L == 1 && ndwc3R == 1 && ndwc3U == 1 && ndwc3D == 1){
                dwcXPos = (dwc3XSlope * (int(dwc3L[0] - dwc3R[0]) + dwc3XOffset) * timeFactor)+dwc3XDisplacement;   
                dwcYPos = (dwc3YSlope * (int(dwc3U[0] - dwc3D[0]) + dwc3YOffset) * timeFactor)+dwc3YDisplacement;
                histx.Fill(dwcXPos);
                histy.Fill(dwcYPos);
            } 
        }
        cout<<"DWC3\n";
        break;
    case 4:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            indexSum4 = 0;
            indexSum1 = 0;
            QSum4 = 0;
            QSum1 = 0;
            if (nmm4>nmm1){
                minhits = nmm1;
            }else{
                minhits = nmm4;
            }
            for (int j=0; j<minhits; j++){
                QSum4 += mm4StripQ[j];
                indexSum4 += j*mm4StripQ[j];
                QSum1 += mm1StripQ[j];
                indexSum1 += j*mm1StripQ[j];
            }
            if (!(QSum4 == 0 || QSum1 == 0)){
                clusterIndex4 = (indexSum4/QSum4) + mm4_strip_offset;
                clusterIndex1 = (indexSum1/QSum1) + mm1_strip_offset;
                clusterPosition4 = clusterIndex4 * pitch - mmWidth/2;
                clusterPosition1 = clusterIndex1 * pitch - mmWidth/2;
                histx.Fill(clusterPosition1);
                histy.Fill(clusterPosition4-mm4YDisplacement);
            }
        }
        cout<<"MM1/MM4\n";
        break;
    case 5:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            indexSum2 = 0;
            indexSum3 = 0;
            QSum2 = 0;
            QSum3 = 0;
            if (nmm2>nmm3){
                minhits = nmm3;
            }else{
                minhits = nmm2;
            }
            for (int j=0; j<minhits; j++){
                QSum2 += mm2StripQ[j];
                indexSum2 += j*mm2StripQ[j];
                QSum3 += mm3StripQ[j];
                indexSum3 += j*mm3StripQ[j];
            }
            if (!(QSum2 == 0 || QSum3 == 0)){
                clusterIndex2 = (indexSum2/QSum2) + mm2_strip_offset;
                clusterIndex3 = (indexSum3/QSum3) + mm3_strip_offset;
                clusterPosition2 = clusterIndex2 * pitch - mmWidth/2;
                clusterPosition3 = clusterIndex3 * pitch - mmWidth/2;
                histx.Fill(clusterPosition3);
                histy.Fill(clusterPosition2-mm2YDisplacement);
            }
        }
        cout<<"MM3/MM2\n";
        break;
    default:
        cout<<"Detector not found\n";
        toFill = false;
        break;
    }
    if(toFill){
        histx.Fit("gaus");
        histy.Fit("gaus");
        mean[0] = histx.GetFunction("gaus")->GetParameter(1);
        mean[1] = histy.GetFunction("gaus")->GetParameter(1);
        //detector(n);
        return mean;
    } else {
        return 0;
    }
}
double* getMeansCustom(int n){
    double mean[2];

    double totalX=0;
    double totalY=0;
    double count=0;

    double dwcXPos, dwcYPos;
    bool toFill = true;
    switch(n){
    case 1:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            if (ndwc1L == 1 && ndwc1R == 1 && ndwc1U == 1 && ndwc1D == 1){
                dwcXPos = (dwc1XSlope * (int(dwc1L[0] - dwc1R[0]) + dwc1XOffset) * timeFactor)+dwc1XDisplacement;   
                dwcYPos = (dwc1YSlope * (int(dwc1U[0] - dwc1D[0]) + dwc1YOffset) * timeFactor)+dwc1YDisplacement;
                totalX+=dwcXPos;
                totalY+=dwcYPos;
                count++;
            } 
        }
        cout<<"DWC1\n";
        break;
    case 2:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            if (ndwc2L == 1 && ndwc2R == 1 && ndwc2U == 1 && ndwc2D == 1){
                dwcXPos = (dwc2XSlope * (int(dwc2L[0] - dwc2R[0]) + dwc2XOffset) * timeFactor)+dwc2XDisplacement;   
                dwcYPos = (dwc2YSlope * (int(dwc2U[0] - dwc2D[0]) + dwc2YOffset) * timeFactor)+dwc2YDisplacement;
                totalX+=dwcXPos;
                totalY+=dwcYPos;
                count++;
            }
        }
        cout<<"DWC2\n";
        break;
    case 3:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            if (ndwc3L == 1 && ndwc3R == 1 && ndwc3U == 1 && ndwc3D == 1){
                dwcXPos = (dwc3XSlope * (int(dwc3L[0] - dwc3R[0]) + dwc3XOffset) * timeFactor)+dwc3XDisplacement;   
                dwcYPos = (dwc3YSlope * (int(dwc3U[0] - dwc3D[0]) + dwc3YOffset) * timeFactor)+dwc3YDisplacement;
                totalX+=dwcXPos;
                totalY+=dwcYPos;
                count++;
            } 
        }
        cout<<"DWC3\n";
        break;
    case 4:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            indexSum4 = 0;
            indexSum1 = 0;
            QSum4 = 0;
            QSum1 = 0;
            if (nmm4>nmm1){
                minhits = nmm1;
            }else{
                minhits = nmm4;
            }
            for (int j=0; j<minhits; j++){
                QSum4 += mm4StripQ[j];
                indexSum4 += j*mm4StripQ[j];
                QSum1 += mm1StripQ[j];
                indexSum1 += j*mm1StripQ[j];
            }
            if (!(QSum4 == 0 || QSum1 == 0)){
                clusterIndex4 = (indexSum4/QSum4) + mm4_strip_offset;
                clusterIndex1 = (indexSum1/QSum1) + mm1_strip_offset;
                clusterPosition4 = clusterIndex4 * pitch - mmWidth/2;
                clusterPosition1 = clusterIndex1 * pitch - mmWidth/2;
                totalX+=clusterPosition1;
                totalY+=clusterPosition4-mm4YDisplacement;
                count++;
            }
        }
        cout<<"MM1/MM4\n";
        break;
    case 5:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            indexSum2 = 0;
            indexSum3 = 0;
            QSum2 = 0;
            QSum3 = 0;
            if (nmm2>nmm3){
                minhits = nmm3;
            }else{
                minhits = nmm2;
            }
            for (int j=0; j<minhits; j++){
                QSum2 += mm2StripQ[j];
                indexSum2 += j*mm2StripQ[j];
                QSum3 += mm3StripQ[j];
                indexSum3 += j*mm3StripQ[j];
            }
            if (!(QSum2 == 0 || QSum3 == 0)){
                clusterIndex2 = (indexSum2/QSum2) + mm2_strip_offset;
                clusterIndex3 = (indexSum3/QSum3) + mm3_strip_offset;
                clusterPosition2 = clusterIndex2 * pitch - mmWidth/2;
                clusterPosition3 = clusterIndex3 * pitch - mmWidth/2;
                totalX+=clusterPosition3;
                totalY+=clusterPosition2-mm2YDisplacement;
                count++;
            }
        }
        cout<<"MM3/MM2\n";
        break;
    default:
        cout<<"Detector not found\n";
        toFill = false;
        break;
    }
    if(toFill){
        mean[0] = totalX/count;
        mean[1] = totalY/count;
        return mean;
    } else {
        return 0;
    }
}
void getHitPos(double dwc1[],double dwc2[],double dwc3[],double mm1[],double mm2[]){
    getHitPos(1,dwc1);
    getHitPos(2,dwc2);
    getHitPos(3,dwc3);
    getHitPos(4,mm1);
    getHitPos(5,mm2);
}

void getHitPos(int n, double pos[]){
    //double pos[3];
    switch(n){
    case 1:
        pos[0] = (dwc1XSlope * (int(dwc1L[0] - dwc1R[0]) + dwc1XOffset) * timeFactor)+dwc1XDisplacement;   
        pos[1] = (dwc1YSlope * (int(dwc1U[0] - dwc1D[0]) + dwc1YOffset) * timeFactor)+dwc1YDisplacement;
        pos[2] = dwc1.distance;
        break;
    case 2:
        pos[0] = (dwc2XSlope * (int(dwc2L[0] - dwc2R[0]) + dwc2XOffset) * timeFactor)+dwc2XDisplacement;   
        pos[1] = (dwc2YSlope * (int(dwc2U[0] - dwc2D[0]) + dwc2YOffset) * timeFactor)+dwc2YDisplacement;
        pos[2] = dwc2.distance;
        break;
    case 3:
        pos[0] = (dwc3XSlope * (int(dwc3L[0] - dwc3R[0]) + dwc3XOffset) * timeFactor)+dwc3XDisplacement;   
        pos[1] = (dwc3YSlope * (int(dwc3U[0] - dwc3D[0]) + dwc3YOffset) * timeFactor)+dwc3YDisplacement;
        pos[2] = dwc3.distance;
        break;
    case 4:
        indexSum4 = 0;
        indexSum1 = 0;
        QSum4 = 0;
        QSum1 = 0;
        if (nmm4>nmm1){
            minhits = nmm1;
        }else{
            minhits = nmm4;
        }
        for (int j=0; j<minhits; j++){
            QSum4 += mm4StripQ[j];
            indexSum4 += j*mm4StripQ[j];
            QSum1 += mm1StripQ[j];
            indexSum1 += j*mm1StripQ[j];
        }
        if (!(QSum4 == 0 || QSum1 == 0)){
            clusterIndex4 = (indexSum4/QSum4) + mm4_strip_offset;
            clusterIndex1 = (indexSum1/QSum1) + mm1_strip_offset;
            clusterPosition4 = clusterIndex4 * pitch - mmWidth/2;
            clusterPosition1 = clusterIndex1 * pitch - mmWidth/2;
            pos[0] = clusterPosition1;
            pos[1] = clusterPosition4-mm4YDisplacement;
            pos[2] = mm1.distance;
        }
        break;
    case 5:
        indexSum2 = 0;
        indexSum3 = 0;
        QSum2 = 0;
        QSum3 = 0;
        if (nmm2>nmm3){
            minhits = nmm3;
        }else{
            minhits = nmm2;
        }
        for (int j=0; j<minhits; j++){
            QSum2 += mm2StripQ[j];
            indexSum2 += j*mm2StripQ[j];
            QSum3 += mm3StripQ[j];
            indexSum3 += j*mm3StripQ[j];
        }
        if (!(QSum2 == 0 || QSum3 == 0)){
            clusterIndex2 = (indexSum2/QSum2) + mm2_strip_offset;
            clusterIndex3 = (indexSum3/QSum3) + mm3_strip_offset;
            clusterPosition2 = clusterIndex2 * pitch - mmWidth/2;
            clusterPosition3 = clusterIndex3 * pitch - mmWidth/2;
            pos[0] = clusterPosition3;
            pos[1] = clusterPosition2-mm2YDisplacement;
            pos[2] = mm3.distance;
        }
        break;
    default:
        cout<<"Detector not found\n";
        break;
    }
    //if(pos[0]!=NULL){
        pos[0]/=1000.0;
        pos[1]/=1000.0;
        //return pos;
    //} else {
      //  return NULL;
    //}
}
bool detectedByAll(int n = 3){
    bool d1=false, d2=false, d3=false;
    if(ndwc3L == 1 && ndwc3R == 1 && ndwc3U == 1 && ndwc3D == 1){
        d3=true;
    }
    if(ndwc2L == 1 && ndwc2R == 1 && ndwc2U == 1 && ndwc2D == 1){
        d2=true;
    }
    if(ndwc1L == 1 && ndwc1R == 1 && ndwc1U == 1 && ndwc1D == 1){
        d1=true;
    }
    switch (n){
        case 1:return d1; break;
        case 2:return (d1&&d2); break;
        case 3:return (d1&&d2&&d3); break;
        default: cout<<"No detector found, choose DWCs 1,2,3\n"; 
            return NULL; break;
    }
}
// void redundantCanvas(){

        //NOTE: This issue has been resolved by using hist->SetDirectory(0);

//       The following code attempts to disconnect the root file to the last plot.
//       It does this by pushing t (pointer) to a new redundant canvas.
//       When the file is closed, which is done before opening a new one,
//       the pointer associated with that file gets deleted too, thus the
//       MM3/MM2 scatter plot also gets deleted exclusively (the other 4 stay).
//       This is probably the worst way to do it. Good luck to the maintainer
//       of this code. There is probably an easier, safer, more efficient, and 
//       less stupid way of doing it.
//       It is probably mentioned in the documentation. Alas, that is for you.
//       This code works for now, but I assure you,
//       ###### SOMETHING WILL DEFINITELY BREAK ######
    
//     c3=new TCanvas(false);
//     c3->SetName("c3");
//     c3->SetTitle("c3");
//     c3->cd();
//     c3->Draw();
//     detector(1); //pushes t to new pad
//     c3->~TCanvas();
//}